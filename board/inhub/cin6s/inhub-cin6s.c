// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2025 in.hub GmbH
 */
#include <init.h>
#include <linux/delay.h>
#include <asm/arch-mx6/clock.h>
#include <asm/arch/sys_proto.h>
#include <dm.h>
#include <dm/platform_data/serial_mxc.h>
#include <miiphy.h>
#include <netdev.h>

DECLARE_GLOBAL_DATA_PTR;

#define MX6_PAD_SNVS_PMIC_STBY_REQ_ADDR 0x2290040

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)PHYS_SDRAM, PHYS_SDRAM_SIZE);

	return 0;
}

int board_mmc_get_env_dev(int devno)
{
	return 0;
}

#ifdef CONFIG_FEC_MXC
static int setup_fec(void)
{
	int ret;

	ret = enable_fec_anatop_clock(1, ENET_25MHZ);
	if (ret)
		return ret;

	enable_enet_clk(1);

	return 0;
}

int board_phy_config(struct phy_device *phydev)
{
	phydev->advertising = ADVERTISED_100baseT_Full | ADVERTISED_100baseT_Half;

	if (phydev->drv->config)
		phydev->drv->config(phydev);
	return 0;
}
#endif /* CONFIG_FEC_MXC */

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

#ifdef CONFIG_FEC_MXC
	setup_fec();
#endif

	return 0;
}

int board_late_init(void)
{
	/*
	 * Disable output driver of PAD CCM_PMIC_STBY_REQ. This prevents the
	 * SOC to request for a lower voltage during sleep. This is necessary
	 * because the voltage is changing too slow for the SOC to wake up
	 * properly.
	 */
	__raw_writel(0x8080, MX6_PAD_SNVS_PMIC_STBY_REQ_ADDR);

	return 0;
}

#if defined(CONFIG_OF_LIBFDT) && defined(CONFIG_OF_BOARD_SETUP)
int ft_board_setup(void *blob, struct bd_info *bd)
{
	return 0;
}
#endif
static struct mxc_serial_plat mxc_serial_plat = {
	.reg = (struct mxc_uart *)UART1_BASE,
	.use_dte = 1,
};

U_BOOT_DRVINFO(mxc_serial) = {
	.name = "serial_mxc",
	.plat = &mxc_serial_plat,
};
