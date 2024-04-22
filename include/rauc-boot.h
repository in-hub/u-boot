/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2019-2021 in.hub GmbH
 */
#ifndef _RAUC_BOOT_H
#define _RAUC_BOOT_H

#define RAUC_BOOTENV \
	"altbootcmd=env default bootcmd; saveenv; reset;\0" \
	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"raucboot="                                                                         \
	"test -n ${BOOT_ORDER} || setenv BOOT_ORDER A B; " \
	"test -n ${BOOT_A_LEFT} || setenv BOOT_A_LEFT 3; " \
	"test -n ${BOOT_B_LEFT} || setenv BOOT_B_LEFT 3; " \
	"setenv bootargs; " \
	"for BOOT_SLOT in ${BOOT_ORDER}; do " \
	"  if test x${bootargs} != x; then " \
	"    ; " \
	"  elif test x${BOOT_SLOT} = xA; then " \
	"    if test ${BOOT_A_LEFT} -gt 0; then " \
	"      echo Found valid slot A, ${BOOT_A_LEFT} attempts remaining; " \
	"      setexpr BOOT_A_LEFT ${BOOT_A_LEFT} - 1; " \
	"      setenv bootargs ${default_bootargs} rauc.slot=A; " \
	"      setenv raucslot A; " \
	"      setenv ubivol 0; " \
	"      setenv mmcbootpart 2; " \
	"      setenv mmcrootpart 3; " \
	"    fi; " \
	"  elif test x${BOOT_SLOT} = xB; then " \
	"    if test ${BOOT_B_LEFT} -gt 0; then " \
	"      echo Found valid slot B, ${BOOT_B_LEFT} attempts remaining; " \
	"      setexpr BOOT_B_LEFT ${BOOT_B_LEFT} - 1; " \
	"      setenv bootargs ${default_bootargs} rauc.slot=B; " \
	"      setenv raucslot B; " \
	"      setenv ubivol 1; " \
	"      setenv mmcbootpart 4; " \
	"      setenv mmcrootpart 5; " \
	"    fi; " \
	"  fi; " \
	"done; " \
	"if test -n ${bootargs}; then " \
	"  saveenv; " \
	"  if test -n ${mmcdev}; then " \
	"    setenv bootargs ${bootargs} boot=/dev/mmcblk${mmcdev}p${mmcbootpart} root=/dev/mmcblk${mmcdev}p${mmcrootpart}; " \
	"    sysboot mmc ${mmcdev}:${mmcbootpart} fat ${kernel_addr_r} /extlinux/extlinux-${systype}.conf; " \
	"  fi; " \
	"  if test -n ${ubipart}; then " \
	"    ubi part ${ubipart}; " \
	"    ubi block SNS_BOOT_${raucslot}; " \
	"    setenv bootargs ${bootargs} verbose ubi.mtd=${ubipart} ubi.block=0,SNS_BOOT_${raucslot} ubi.mtd=ubi ubi.block=1,SNS_ROOT_${raucslot} boot=/dev/ubiblock0_${ubivol} root=/dev/ubiblock1_${ubivol}; " \
	"    sysboot ubi 0 fat ${kernel_addr_r} /extlinux/extlinux-${systype}.conf; " \
	"  fi; " \
	"else " \
	"  echo No valid slot found, resetting tries to 3; " \
	"  setenv BOOT_A_LEFT 3; " \
	"  setenv BOOT_B_LEFT 3; " \
	"  saveenv; " \
	"  reset; " \
	"fi; " \
	"usb start; " \
	"setenv bootargs boot=/dev/sda2 root=/dev/sda3 rauc.slot=A; " \
	"sysboot usb 0:2 fat ${kernel_addr_r} /extlinux/extlinux-${systype}.conf; " \
	"reset;\0"

#endif /* _RAUC_BOOT_H */
