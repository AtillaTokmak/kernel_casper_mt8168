#
# Copyright (C) 2015 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))

subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/video/include/ \
                    -I$(srctree)/drivers/misc/mediatek/video/common/ \
                    -I$(srctree)/drivers/misc/mediatek/video/common/rdma20/ \
                    -I$(srctree)/drivers/misc/mediatek/video/common/wdma20/ \
                    -I$(srctree)/drivers/misc/mediatek/video/common/layering_rule_base/v1.1/ \
                    -I$(srctree)/drivers/misc/mediatek/video/$(MTK_PLATFORM)/dispsys/ \
                    -I$(srctree)/drivers/misc/mediatek/video/$(MTK_PLATFORM)/videox/ \
                    -I$(srctree)/drivers/misc/mediatek/mmp/

ifneq ($(CONFIG_MTK_LCM), y)
	obj-y += mtdummy/
else
	######################################
	obj-$(CONFIG_MTK_FB) += $(subst ",,$(CONFIG_MTK_PLATFORM))/
	obj-$(CONFIG_MACH_MT6771) += common/
	obj-$(CONFIG_MACH_MT6768) += common/
	obj-$(CONFIG_MACH_MT8167) += common/
	obj-$(CONFIG_MACH_MT8168) += common/
	obj-$(CONFIG_MACH_MT6739) += common/
	obj-$(CONFIG_MACH_MT6785) += common/
	obj-$(CONFIG_MACH_MT6885) += common/
	obj-$(CONFIG_MACH_MT6765) += common/
endif

