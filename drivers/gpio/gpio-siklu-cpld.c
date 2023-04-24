// SPDX-License-Identifier: GPL-2.0-only

#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/gpio/driver.h>
#include <linux/firmware/siklu-cpld.h>

#define DRV_NAME "siklu-cpld-gpio"

static DEFINE_MUTEX(siklu_cpld_gpio_mutex);

/* offset values: value/bits 0-7 for reg 0x7, value/bits 8-15 for reg 0xf, value/bits 16-23 for reg 0x14 */
static int siklu_cpld_gpio_get(struct gpio_chip *gc, unsigned offset)
{
	struct device *cpld_dev = gpiochip_get_data(gc);
	u8 reg_val, cpld_reg = R_CPLD_LOGIC_SFP_MODE;
	unsigned adjust_offset = 0;

	if (offset <= 7) {
		cpld_reg = R_CPLD_LOGIC_SFP_MODE;
		adjust_offset = offset;
	} else if ((offset >= 8) && (offset <= 15)) {
		cpld_reg = R_CPLD_LOGIC_INT_HNDLR_1;
		adjust_offset = offset - 8;
	} else {
		cpld_reg = R_CPLD_LOGIC_MISC_1;
		adjust_offset = offset - 16;
	}

	siklu_cpld_reg_read(cpld_dev, cpld_reg, &reg_val);

/*	printk(KERN_ERR
	       "siklu_cpld_gpio_get, offset = 0x%x, cpld_reg = 0x%x, reg_val = 0x%x \n",
	       offset, cpld_reg, reg_val);*/

	return !!(reg_val & (1 << adjust_offset));
#if 0	
	struct device *cpld_dev = gpiochip_get_data(gc);
	u8 reg_val;

	siklu_cpld_reg_read(cpld_dev, R_CPLD_LOGIC_GPIO, &reg_val);

	printk(KERN_ERR
	       "siklu_cpld_gpio_get, offset = 0x%x, reg_val = 0x%x \n",
	       offset, reg_val);

	return !!(reg_val & (1 << offset));
#endif
}

/* offset values: value/bits 0-7 for reg 0x7, value/bits 8-15 for reg 0xf, value/bits 16-23 for reg 0x14 */
static void siklu_cpld_gpio_set(struct gpio_chip *gc, unsigned offset, int value)
{
	struct device *cpld_dev = gpiochip_get_data(gc);
	u8 reg_val, cpld_reg = R_CPLD_LOGIC_SFP_MODE;
	unsigned adjust_offset = 0;

	if (offset <= 7)
	{
		cpld_reg = R_CPLD_LOGIC_SFP_MODE;
		adjust_offset = offset;
	}
	else if ((offset >= 8) && (offset <= 15))
	{
		cpld_reg = R_CPLD_LOGIC_INT_HNDLR_1;
		adjust_offset = offset - 8;
	}
	else
	{
		cpld_reg = R_CPLD_LOGIC_MISC_1;
		adjust_offset = offset - 16;
	}

	mutex_lock(&siklu_cpld_gpio_mutex);
	siklu_cpld_reg_read(cpld_dev, cpld_reg, &reg_val);
	if (value)
		reg_val |= (1 << adjust_offset);
	else
		reg_val &= ~(1 << adjust_offset);

/*	printk(KERN_ERR
	       "siklu_cpld_gpio_set, offset = 0x%x, cpld_reg = 0x%x, value = 0x%x, reg_val = 0x%x \n",
	       offset, cpld_reg, value, reg_val);*/

	siklu_cpld_reg_write(cpld_dev, cpld_reg, reg_val);
	mutex_unlock(&siklu_cpld_gpio_mutex);
#if 0	
	struct device *cpld_dev = gpiochip_get_data(gc);
	u8 reg_val;

	mutex_lock(&siklu_cpld_gpio_mutex);
	siklu_cpld_reg_read(cpld_dev, R_CPLD_LOGIC_GPIO, &reg_val);
	if (value)
		reg_val |= (1 << offset);
	else
		reg_val &= ~(1 << offset);

	printk(KERN_ERR
	       "siklu_cpld_gpio_set, offset = 0x%x, value = 0x%x, reg_val = 0x%x \n",
	       offset, value, reg_val);

	siklu_cpld_reg_write(cpld_dev, R_CPLD_LOGIC_GPIO, reg_val);
	mutex_unlock(&siklu_cpld_gpio_mutex);
#endif	
}

/* offset values: value/bits 0-7 for reg 0x7, value/bits 8-15 for reg 0xf, value/bits 16-23 for reg 0x14 */
static int siklu_cpld_gpio_get_dir(struct gpio_chip *gc, unsigned int offset)
{
//	printk(KERN_ERR "siklu_cpld_gpio_get_dir, offset = 0x%x \n",offset);

	if (offset < 16)
		return GPIO_LINE_DIRECTION_IN;
	else
		return GPIO_LINE_DIRECTION_OUT;
}

static int siklu_cpld_gpio_direction_input(struct gpio_chip *chip,
				     unsigned int offset)
{
//	printk(KERN_ERR "siklu_cpld_gpio_direction_input, offset = 0x%x \n",offset);

	return 0;
}

static int siklu_cpld_gpio_direction_output(struct gpio_chip *chip,
				      unsigned int offset, int value)
{
//	printk(KERN_ERR "siklu_cpld_gpio_direction_output, offset = 0x%x \n",offset);

	return 0;
}

static int siklu_cpld_gpio_request(struct gpio_chip *chip, unsigned int offset)
{
	return 0;
}

static int siklu_cpld_gpio_probe(struct platform_device *pdev)
{
	struct device *cpld_dev = pdev->dev.parent;
	struct gpio_chip *gc;

	gc = devm_kzalloc(&pdev->dev, sizeof(*gc), GFP_KERNEL);
	if (gc == NULL)
		return -ENOMEM;

	gc->ngpio = 8*3;
	gc->base = 504/*-1*/;
	gc->label = DRV_NAME;
	gc->owner = THIS_MODULE;
	gc->direction_input = siklu_cpld_gpio_direction_input;
	gc->direction_output = siklu_cpld_gpio_direction_output;
	gc->get_direction = siklu_cpld_gpio_get_dir;
	gc->get = siklu_cpld_gpio_get;
	gc->set = siklu_cpld_gpio_set;
	gc->request = siklu_cpld_gpio_request;
	gc->parent = &pdev->dev;
	gc->of_node	= pdev->dev.of_node;

	platform_set_drvdata(pdev, gc);

/*	printk(KERN_ERR
	       "*********************************siklu_cpld_gpio_probe************************************************\n");
	printk(KERN_ERR
	       "*********************************siklu_cpld_gpio_probe************************************************\n");
	printk(KERN_ERR
	       "*********************************siklu_cpld_gpio_probe************************************************\n");*/

	return devm_gpiochip_add_data(&pdev->dev, gc, cpld_dev);
}

static const struct of_device_id siklu_cpld_gpio_of_match[] = {
	{ .compatible = "siklu,eh8010-cpld-gpio", },
	{},
};
MODULE_DEVICE_TABLE(of, siklu_cpld_gpio_of_match);

static struct platform_driver siklu_cpld_gpio_driver = {
	.probe	= siklu_cpld_gpio_probe,
	.driver	= {
		.name		= DRV_NAME,
		.of_match_table	= siklu_cpld_gpio_of_match,
	},
};
module_platform_driver(siklu_cpld_gpio_driver);
