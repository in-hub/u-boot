/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2025 in.hub GmbH
 *
 * Configuration settings for the in.hub CIN6S module.
 *
 * based on colibri-imx6ull.h
 */

#ifndef __INHUB_CIN6S_CONFIG_H
#define __INHUB_CIN6S_CONFIG_H

#include "mx6_common.h"
#define CONFIG_IOMUX_LPSR

#define PHYS_SDRAM_SIZE			SZ_1G

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(32 * SZ_1M)

/* ENET1 */
#define IMX_FEC_BASE			ENET2_BASE_ADDR

/* MMC Config */
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_USDHC_NUM	2

/* I2C configs */
#define CONFIG_SYS_I2C_SPEED		100000

#define MEM_LAYOUT_ENV_SETTINGS \
	"bootm_size=0x10000000\0" \
	"fdt_addr_r=0x82100000\0" \
	"kernel_addr_r=0x81000000\0" \
	"pxefile_addr_r=0x87100000\0" \
	"ramdisk_addr_r=0x82200000\0" \
	"scriptaddr=0x87000000\0"

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(DHCP, dhcp, na)
#include <rauc-boot.h>
#define BOOTENV RAUC_BOOTENV

#define CONFIG_EXTRA_ENV_SETTINGS \
	BOOTENV \
	MEM_LAYOUT_ENV_SETTINGS \
	"fdtfile=imx6ull-inhub-hub-en200.dtb\0" \
	"console=ttymxc0\0"

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/* Physical Memory Map */
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* USB Configs */
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET

#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2

#define CONFIG_USBD_HS

#endif /* __INHUB_CIN6S_CONFIG_H */
