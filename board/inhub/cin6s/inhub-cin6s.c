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

#ifdef CONFIG_FEC_MXC
static int setup_fec(void)
{
	struct iomuxc *iomuxc_regs = (struct iomuxc *)IOMUXC_BASE_ADDR;
	int ret;

	/* provide the PHY clock from the i.MX 6 */
	ret = enable_fec_anatop_clock(1, ENET_50MHZ);
	if (ret)
		return ret;

	/* Use 50M anatop REF_CLK and output it on ENET2_TX_CLK */
	clrsetbits_le32(&iomuxc_regs->gpr[1],
			IOMUX_GPR1_FEC2_CLOCK_MUX2_SEL_MASK,
			IOMUX_GPR1_FEC2_CLOCK_MUX1_SEL_MASK);

	/* give new Ethernet PHY power save mode circuitry time to settle */
	mdelay(300);

	return 0;
}

int board_phy_config(struct phy_device *phydev)
{
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

static struct mxc_serial_plat mxc_serial_plat = {
	.reg = (struct mxc_uart *)UART1_BASE,
	.use_dte = 1,
};

U_BOOT_DRVINFO(mxc_serial) = {
	.name = "serial_mxc",
	.plat = &mxc_serial_plat,
};
