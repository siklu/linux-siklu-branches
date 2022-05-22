#include <linux/init.h>
#include <linux/io.h>
#include <linux/printk.h>
#include <linux/mdio.h>
#include <linux/sysctl.h>

#define IMX6ULL_MUX_PHYS_ADDR   0x020E0000
#define IMX6ULL_PAD_PHYS_ADDR   0x020E0300
#define IMX6ULL_DAISY_PHYS_ADDR 0x020E0500
#define IMX6ULL_MUX_PHYS_SIZE   0x200
#define IMX6ULL_PAD_PHYS_SIZE   0x100
#define IMX6ULL_DAISY_PHYS_SIZE 0x200

#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO06        0x74
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO07        0x78

#define IOMUXC_SW_MUX_CTL_PAD_ENET2_RX_DATA0    0xE4
#define IOMUXC_SW_MUX_CTL_PAD_ENET2_RX_DATA1    0xE8

struct mii_bus *siklu_get_mii(void);

static char sysctl_buffer[32];

static int ti_read_reg(struct ctl_table *ctl, int write, void *buffer,
		size_t *lenp, loff_t *ppos)
{
	static unsigned int dev_id, reg;
	int ret;

	if (write) {
		ret = proc_dostring(ctl, write, buffer, lenp, ppos);
		if (ret < 0)
			return ret;
		sscanf(buffer, "%x,%x", &dev_id, &reg);
	} else {
		struct mii_bus *bus;

		bus = siklu_get_mii();
		if (bus == NULL)
			return -ENODEV;

		ret = mdiobus_read(bus, 4, MII_ADDR_C45 |
				(dev_id <<  MII_DEVADDR_C45_SHIFT) | reg);
		if (ret < 0)
			return ret;
		sprintf(sysctl_buffer, "%04x\n", ret);
		ret = proc_dostring(ctl, write, buffer, lenp, ppos);
	}

	return ret;
}

static struct ctl_table tir_sysctl_table[] = {
	{
		.procname = "tir",
		.mode = 0644,
		.proc_handler = ti_read_reg,
		.data = &sysctl_buffer,
		.maxlen = sizeof(sysctl_buffer),
	},
	{ },
};

static struct ctl_table tir_phy_general[] = {
	{
		.procname = "phy_general",
		.mode = 0555,
		.child = tir_sysctl_table,
	},
	{ },
};

static struct ctl_table tir_reg_root[] = {
	{
		.procname = "dev",
		.mode = 0555,
		.child = tir_phy_general,
	},
	{ },
};

static int tir_init_module(void)
{
	void __iomem *mux_vptr, *pad_vptr, *daisy_vptr;
	struct mii_bus *bus;

	mux_vptr = ioremap(IMX6ULL_MUX_PHYS_ADDR, IMX6ULL_MUX_PHYS_SIZE);
	if (mux_vptr == NULL)
		goto fail;

	pad_vptr = ioremap(IMX6ULL_PAD_PHYS_ADDR, IMX6ULL_PAD_PHYS_SIZE);
	if (pad_vptr == NULL)
		goto fail;

	daisy_vptr = ioremap(IMX6ULL_DAISY_PHYS_ADDR, IMX6ULL_DAISY_PHYS_SIZE);
	if (daisy_vptr == NULL)
		goto fail;

	/* Once */
	writel(0, mux_vptr + IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO07);
	writel(4, mux_vptr + IOMUXC_SW_MUX_CTL_PAD_ENET2_RX_DATA1);

	/* Bus1 */
	writel(8, mux_vptr + IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO06);
	writel(4, mux_vptr + IOMUXC_SW_MUX_CTL_PAD_ENET2_RX_DATA0);
	writel(0xb029, pad_vptr + 4);
	writel(0xb029, pad_vptr + 0x74);
	writel(1, daisy_vptr + 0x78);
	writel(0xb829, pad_vptr + 0x70);
	writel(0, daisy_vptr + 0x120);

	bus = siklu_get_mii();
	if (bus) {
		int val;

		val = mdiobus_read(bus, 4, MII_ADDR_C45 |
				(1 <<  MII_DEVADDR_C45_SHIFT) | 3);
		printk("%s: val: %x\n", __func__, val);
	}

	if (register_sysctl_table(tir_reg_root) == NULL)
		goto fail;

	printk("%s: done\n", __func__);
	return 0;

fail:
	printk("%s: failed\n", __func__);
	return -ENODEV;
}

late_initcall(tir_init_module);

