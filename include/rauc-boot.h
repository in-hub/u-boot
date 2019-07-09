/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2019 in.hub GmbH
 */
#ifndef _RAUC_BOOT_H
#define _RAUC_BOOT_H

#define RAUC_BOOTENV \
	"altbootcmd=env default bootcmd; saveenv; reset;\0" \
	"findbootslot="                                                                         \
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
	"      setenv bootargs ${default_bootargs} boot=/dev/mmcblk0p2 root=/dev/mmcblk0p3 rauc.slot=A; " \
	"      setenv mmcpart 2; " \
	"    fi; " \
	"  elif test x${BOOT_SLOT} = xB; then " \
	"    if test ${BOOT_B_LEFT} -gt 0; then " \
	"      echo Found valid slot B, ${BOOT_B_LEFT} attempts remaining; " \
	"      setexpr BOOT_B_LEFT ${BOOT_B_LEFT} - 1; " \
	"      setenv bootargs ${default_bootargs} boot=/dev/mmcblk0p4 root=/dev/mmcblk0p5 rauc.slot=B; " \
	"      setenv mmcpart 4; " \
	"    fi; " \
	"  fi; " \
	"done; " \
	"if test -n ${bootargs}; then " \
	"  saveenv; " \
	"else " \
	"  echo No valid slot found, resetting tries to 3; " \
	"  setenv BOOT_A_LEFT 3; " \
	"  setenv BOOT_B_LEFT 3; " \
	"  saveenv; " \
	"  reset; " \
	"fi\0"

#endif /* _RAUC_BOOT_H */
