subdir-ccflags-y += -Werror
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include

# gslX680 need project separate
ifneq ($(wildcard $(srctree)/drivers/input/touchscreen/mediatek/gslX680/$(MTK_PLATFORM)/),)
obj-y   +=  $(MTK_PLATFORM)/
else
obj-y   +=  mt8167/
endif
