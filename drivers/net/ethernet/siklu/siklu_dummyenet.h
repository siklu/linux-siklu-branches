/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Definitions for Siklu Dummy MAC Ethernet device driver.
 *
 */

#ifndef SIKLU_DUMMYENET_H
#define SIKLU_DUMMYENET_H

#include <linux/netdevice.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/if_vlan.h>
#include <linux/phylink.h>


/*
 * struct sikluenet_local - sikluenet is a dummy struct
 */
struct sikluenet_local {
	struct net_device *ndev;
	struct device *dev;

	/* Connection to PHY device */
	struct device_node *phy_node;

	struct phylink *phylink;
	struct phylink_config phylink_config;

	/* Clock for bus */
	struct clk *clk;

	/* MDIO bus data */
	struct mii_bus *mii_bus;	/* MII bus reference */

	/* IO registers, dma functions and IRQs */
	resource_size_t regs_start;
	void __iomem *regs;
	void __iomem *dma_regs;

	struct work_struct dma_err_task;

	int tx_irq;
	int rx_irq;
	int eth_irq;
	phy_interface_t phy_mode;

	u32 options;			/* Current options word */
	u32 features;

	/* Buffer descriptors */
	dma_addr_t tx_bd_p;
	u32 tx_bd_num;
	dma_addr_t rx_bd_p;
	u32 rx_bd_num;
	u32 tx_bd_ci;
	u32 tx_bd_tail;
	u32 rx_bd_ci;

	u32 max_frm_size;
	u32 rxmem;

	int csum_offload_on_tx_path;
	int csum_offload_on_rx_path;

	u32 coalesce_count_rx;
	u32 coalesce_count_tx;
};



#endif /* SIKLU_DUMMYENET_H */
