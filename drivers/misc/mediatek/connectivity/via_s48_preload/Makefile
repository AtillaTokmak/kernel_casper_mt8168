# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_VIA_S48_CONNSYS_PRELOAD) += via_s48_connsys_preload_drv.o
via_s48_connsys_preload_drv-y := via_s48_connsys_preload.o via_s48_connsys_blobs.o

blobs := wmt_drv.ko bt_drv.ko wmt_chrdev_wifi.ko wlan_drv_gen4m.ko

# The .incbin'ed images must exist (and match SHA256SUMS); fail early with a
# clear message instead of an assembler error.
ifeq ($(CONFIG_VIA_S48_CONNSYS_PRELOAD),y)
$(foreach b,$(blobs),$(if $(wildcard $(srctree)/$(src)/blobs/$(b)),,\
	$(error $(src)/blobs/$(b) missing: copy the rebuilt modules there, see blobs/SHA256SUMS)))
endif

$(obj)/via_s48_connsys_blobs.o: $(addprefix $(srctree)/$(src)/blobs/,$(blobs))
