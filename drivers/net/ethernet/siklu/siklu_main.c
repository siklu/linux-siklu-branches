/*
 * Siklu Dummy MAC Ethernet device driver
 *
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/etherdevice.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/of_mdio.h>
#include <linux/of_net.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/phy.h>
#include <linux/mii.h>
#include <linux/ethtool.h>

#include "siklu_dummyenet.h"


/**
 * sikluenet_open - Driver open routine.
 * @ndev:	Pointer to net_device structure
 *
 * Return: 0, on success.
 *	    non-zero error value on failure
 *
 * This is the driver open routine. It calls phylink_start to start the
 * PHY device.
 */
static int sikluenet_open(struct net_device *ndev)
{
	return 0;
}

/**
 * sikluenet_stop - Driver stop routine.
 * @ndev:	Pointer to net_device structure
 *
 * Return: 0, on success.
 *
 * This is the driver stop routine. It calls phylink_disconnect to stop the PHY
 * device.
 */
static int sikluenet_stop(struct net_device *ndev)
{
	return 0;
}


/**
 * sikluenet_change_mtu - Driver change mtu routine.
 * @ndev:	Pointer to net_device structure
 * @new_mtu:	New mtu value to be applied
 *
 * Return: Always returns 0 (success).
 */
static int sikluenet_change_mtu(struct net_device *ndev, int new_mtu)
{
	return 0;
}



static int sikluenet_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	return 0;
}

/**
 * sikluenet_start_xmit - Starts the transmission.
 * @skb:	sk_buff pointer that contains data to be Txed.
 * @ndev:	Pointer to net_device structure.
 *
 * Return: NETDEV_TX_OK, on success
 *	    NETDEV_TX_BUSY, if any of the descriptors are not free
 */
static netdev_tx_t
sikluenet_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	return 0;
}

/**
 * netdev_set_mac_address - Write the MAC address (from outside the driver)
 * @ndev:	Pointer to the net_device structure
 * @p:		6 byte Address to be written as MAC address
 *
 * Return: 0 for all conditions. Presently, there is no failure case.
 */
static int netdev_set_mac_address(struct net_device *ndev, void *p)
{
	return 0;
}

/**
 * sikluenet_set_multicast_list - Prepare the multicast table
 * @ndev:	Pointer to the net_device structure
 *
 * This function is called to initialize the multicast table during
 * initialization. The Ethernet basic multicast support has a four-entry
 * multicast table which is initialized here. Additionally this function
 * goes into the net_device_ops structure entry ndo_set_multicast_list. This
 * means whenever the multicast table entries need to be updated this
 * function gets called.
 */
static void sikluenet_set_multicast_list(struct net_device *ndev)
{
}

static const struct net_device_ops sikluenet_netdev_ops = {
	.ndo_open = sikluenet_open,
	.ndo_stop = sikluenet_stop,
	.ndo_start_xmit = sikluenet_start_xmit,
	.ndo_change_mtu	= sikluenet_change_mtu,
	.ndo_set_mac_address = netdev_set_mac_address,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_do_ioctl = sikluenet_ioctl,
	.ndo_set_rx_mode = sikluenet_set_multicast_list,
};
/* Must be shorter than length of ethtool_drvinfo.driver field to fit */
#define DRIVER_NAME		"siklunet"
#define DRIVER_DESCRIPTION	"Siklu Dummy MAC Ethernet driver"
#define DRIVER_VERSION		"1.00a"


/* Match table for of_platform binding */
static const struct of_device_id siklu_of_match[] = {
	{ .compatible = "siklu,dummy-ethernet-1.00.a", },
	{},
};

MODULE_DEVICE_TABLE(of, siklu_of_match);

/**
 * sikluenet_ethtools_get_drvinfo - Get various Dummy Siklu Ethernet driver information.
 * @ndev:	Pointer to net_device structure
 * @ed:		Pointer to ethtool_drvinfo structure
 *
 * This implements ethtool command for getting the driver information.
 * Issue "ethtool -i ethX" under linux prompt to execute this function.
 */
static void sikluenet_ethtools_get_drvinfo(struct net_device *ndev,
					 struct ethtool_drvinfo *ed)
{
	strlcpy(ed->driver, DRIVER_NAME, sizeof(ed->driver));
	strlcpy(ed->version, DRIVER_VERSION, sizeof(ed->version));
}

/**
 * sikluenet_ethtools_get_regs_len - Get the total regs length present in the
 *				   Ethernet core.
 * @ndev:	Pointer to net_device structure
 *
 * This implements ethtool command for getting the total register length
 * information.
 *
 * Return: the total regs length
 */
static int sikluenet_ethtools_get_regs_len(struct net_device *ndev)
{
	return sizeof(u32) * 40;
}

/**
 * sikluenet_ethtools_get_regs - Dump the contents of all registers present
 *			       in Ethernet core.
 * @ndev:	Pointer to net_device structure
 * @regs:	Pointer to ethtool_regs structure
 * @ret:	Void pointer used to return the contents of the registers.
 *
 * This implements ethtool command for getting the Ethernet register dump.
 * Issue "ethtool -d ethX" to execute this function.
 */
static void sikluenet_ethtools_get_regs(struct net_device *ndev,
				      struct ethtool_regs *regs, void *ret)
{
}

static void sikluenet_ethtools_get_ringparam(struct net_device *ndev,
					   struct ethtool_ringparam *ering)
{
}

static int sikluenet_ethtools_set_ringparam(struct net_device *ndev,
					  struct ethtool_ringparam *ering)
{
	return 0;
}

/**
 * sikluenet_ethtools_get_pauseparam - Get the pause parameter setting for
 *				     Tx and Rx paths.
 * @ndev:	Pointer to net_device structure
 * @epauseparm:	Pointer to ethtool_pauseparam structure.
 *
 * This implements ethtool command for getting ethernet pause frame
 * setting. Issue "ethtool -a ethX" to execute this function.
 */
static void
sikluenet_ethtools_get_pauseparam(struct net_device *ndev,
				struct ethtool_pauseparam *epauseparm)
{
}

/**
 * sikluenet_ethtools_set_pauseparam - Set device pause parameter(flow control)
 *				     settings.
 * @ndev:	Pointer to net_device structure
 * @epauseparm:Pointer to ethtool_pauseparam structure
 *
 * This implements ethtool command for enabling flow control on Rx and Tx
 * paths. Issue "ethtool -A ethX tx on|off" under linux prompt to execute this
 * function.
 *
 * Return: 0 on success, -EFAULT if device is running
 */
static int
sikluenet_ethtools_set_pauseparam(struct net_device *ndev,
				struct ethtool_pauseparam *epauseparm)
{
	return 0;
}

/**
 * sikluenet_ethtools_get_coalesce - Get DMA interrupt coalescing count.
 * @ndev:	Pointer to net_device structure
 * @ecoalesce:	Pointer to ethtool_coalesce structure
 *
 * This implements ethtool command for getting the DMA interrupt coalescing
 * count on Tx and Rx paths. Issue "ethtool -c ethX" under linux prompt to
 * execute this function.
 *
 * Return: 0 always
 */
static int sikluenet_ethtools_get_coalesce(struct net_device *ndev,
					 struct ethtool_coalesce *ecoalesce)
{
	return 0;
}

/**
 * sikluenet_ethtools_set_coalesce - Set DMA interrupt coalescing count.
 * @ndev:	Pointer to net_device structure
 * @ecoalesce:	Pointer to ethtool_coalesce structure
 *
 * This implements ethtool command for setting the DMA interrupt coalescing
 * count on Tx and Rx paths. Issue "ethtool -C ethX rx-frames 5" under linux
 * prompt to execute this function.
 *
 * Return: 0, on success, Non-zero error value on failure.
 */
static int sikluenet_ethtools_set_coalesce(struct net_device *ndev,
					 struct ethtool_coalesce *ecoalesce)
{
	return 0;
}

static int
sikluenet_ethtools_get_link_ksettings(struct net_device *ndev,
				    struct ethtool_link_ksettings *cmd)
{
	return 0;
}

static int
sikluenet_ethtools_set_link_ksettings(struct net_device *ndev,
				    const struct ethtool_link_ksettings *cmd)
{
	return 0;
}

static const struct ethtool_ops sikluenet_ethtool_ops = {
	.supported_coalesce_params = ETHTOOL_COALESCE_MAX_FRAMES,
	.get_drvinfo    = sikluenet_ethtools_get_drvinfo,
	.get_regs_len   = sikluenet_ethtools_get_regs_len,
	.get_regs       = sikluenet_ethtools_get_regs,
	.get_link       = ethtool_op_get_link,
	.get_ringparam	= sikluenet_ethtools_get_ringparam,
	.set_ringparam	= sikluenet_ethtools_set_ringparam,
	.get_pauseparam = sikluenet_ethtools_get_pauseparam,
	.set_pauseparam = sikluenet_ethtools_set_pauseparam,
	.get_coalesce   = sikluenet_ethtools_get_coalesce,
	.set_coalesce   = sikluenet_ethtools_set_coalesce,
	.get_link_ksettings = sikluenet_ethtools_get_link_ksettings,
	.set_link_ksettings = sikluenet_ethtools_set_link_ksettings,
};

static void sikluenet_validate(struct phylink_config *config,
			     unsigned long *supported,
			     struct phylink_link_state *state)
{
	/* nothing meaningful to do */
}

static void sikluenet_mac_pcs_get_state(struct phylink_config *config,
				      struct phylink_link_state *state)
{
	/* nothing meaningful to do */
}

static void sikluenet_mac_an_restart(struct phylink_config *config)
{
	/* Unsupported, do nothing */
}

static void sikluenet_mac_config(struct phylink_config *config, unsigned int mode,
			       const struct phylink_link_state *state)
{
	/* nothing meaningful to do */
}

static void sikluenet_mac_link_down(struct phylink_config *config,
				  unsigned int mode,
				  phy_interface_t interface)
{
	/* nothing meaningful to do */
}

static void sikluenet_mac_link_up(struct phylink_config *config,
				struct phy_device *phy,
				unsigned int mode, phy_interface_t interface,
				int speed, int duplex,
				bool tx_pause, bool rx_pause)
{
	/* nothing meaningful to do */
}

static const struct phylink_mac_ops sikluenet_phylink_ops = {
	.validate = sikluenet_validate,
	.mac_pcs_get_state = sikluenet_mac_pcs_get_state,
	.mac_an_restart = sikluenet_mac_an_restart,
	.mac_config = sikluenet_mac_config,
	.mac_link_down = sikluenet_mac_link_down,
	.mac_link_up = sikluenet_mac_link_up,
};

/**
 * sikluenet_probe - Siklu dummy MAC Ethernet probe function.
 * @pdev:	Pointer to platform device structure.
 *
 * Return: 0, on success
 *	    Non-zero error value on failure.
 *
 * This is the probe routine for Siklu dummy MAC Ethernet driver. This is called before
 * any other driver routines are invoked. It allocates and sets up the Ethernet
 * device. Parses through device tree and populates fields of
 * sikluenet_local. It registers the Ethernet device.
 */
static int sikluenet_probe(struct platform_device *pdev)
{
	int ret;
	struct sikluenet_local *lp;
	struct net_device *ndev;

	ndev = alloc_etherdev(sizeof(*lp));
	if (!ndev)
		return -ENOMEM;

	platform_set_drvdata(pdev, ndev);

	SET_NETDEV_DEV(ndev, &pdev->dev);
	ndev->flags &= ~IFF_MULTICAST;  /* clear multicast */
	ndev->features = NETIF_F_SG;
	ndev->netdev_ops = &sikluenet_netdev_ops;
	ndev->ethtool_ops = &sikluenet_ethtool_ops;

	/* MTU range: 64 - 9000 */
	ndev->min_mtu = 64;
	ndev->max_mtu = 9000;

	lp = netdev_priv(ndev);
	lp->ndev = ndev;
	lp->dev = &pdev->dev;
	lp->options = 0;
	lp->rx_bd_num = 1024;
	lp->tx_bd_num = 64;

	lp->phylink_config.dev = &ndev->dev;
	lp->phylink_config.type = PHYLINK_NETDEV;

	lp->phylink = phylink_create(&lp->phylink_config, pdev->dev.fwnode,
				     lp->phy_mode,
				     &sikluenet_phylink_ops);
	if (IS_ERR(lp->phylink)) {
		ret = PTR_ERR(lp->phylink);
		dev_err(&pdev->dev, "phylink_create error (%i)\n", ret);
		printk(KERN_ERR "============  sikluenet_probe  call phylink_create() Error =========== \n");
		goto free_netdev;
	}

	ret = register_netdev(lp->ndev);
	if (ret) {
		dev_err(lp->dev, "register_netdev() error (%i)\n", ret);
		printk(KERN_ERR "===========  sikluenet_probe  call register_netdev() Error =========== \n");
		goto free_netdev;
	}

	return 0;

free_netdev:
	free_netdev(ndev);

	return ret;
}

static int sikluenet_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct sikluenet_local *lp = netdev_priv(ndev);

	unregister_netdev(ndev);

	if (lp->phylink)
		phylink_destroy(lp->phylink);

	if (lp->clk)
		clk_disable_unprepare(lp->clk);

	of_node_put(lp->phy_node);
	lp->phy_node = NULL;

	free_netdev(ndev);

	return 0;
}

static void sikluenet_shutdown(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);

	rtnl_lock();
	netif_device_detach(ndev);

	if (netif_running(ndev))
		dev_close(ndev);

	rtnl_unlock();
}

static struct platform_driver siklu_driver = {
	.probe = sikluenet_probe,
	.remove = sikluenet_remove,
	.shutdown = sikluenet_shutdown,
	.driver = {
		 .name = "siklu_dummyenet",
		 .of_match_table = siklu_of_match,
	},
};

module_platform_driver(siklu_driver);

MODULE_DESCRIPTION("Siklu Dummy MAC Ethernet driver");
MODULE_AUTHOR("Siklu");
MODULE_LICENSE("GPL");
