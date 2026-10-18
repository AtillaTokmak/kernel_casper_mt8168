/* SPDX-License-Identifier: GPL-2.0 */
/* Module images embedded into the kernel for via_s48_connsys_preload.c */
	.section .rodata, "a"
	.balign 8

	.globl via_s48_wmt_drv_start
via_s48_wmt_drv_start:
	.incbin "drivers/misc/mediatek/connectivity/via_s48_preload/blobs/wmt_drv.ko"
	.globl via_s48_wmt_drv_end
via_s48_wmt_drv_end:
	.balign 8

	.globl via_s48_bt_drv_start
via_s48_bt_drv_start:
	.incbin "drivers/misc/mediatek/connectivity/via_s48_preload/blobs/bt_drv.ko"
	.globl via_s48_bt_drv_end
via_s48_bt_drv_end:
	.balign 8

	.globl via_s48_wmt_chrdev_wifi_start
via_s48_wmt_chrdev_wifi_start:
	.incbin "drivers/misc/mediatek/connectivity/via_s48_preload/blobs/wmt_chrdev_wifi.ko"
	.globl via_s48_wmt_chrdev_wifi_end
via_s48_wmt_chrdev_wifi_end:
	.balign 8

	.globl via_s48_wlan_drv_gen4m_start
via_s48_wlan_drv_gen4m_start:
	.incbin "drivers/misc/mediatek/connectivity/via_s48_preload/blobs/wlan_drv_gen4m.ko"
	.globl via_s48_wlan_drv_gen4m_end
via_s48_wlan_drv_gen4m_end:
	.balign 8
