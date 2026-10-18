// SPDX-License-Identifier: GPL-2.0
/*
 * Casper VIA S48 (MT8168) connsys module preload.
 *
 * The stock vendor wmt_drv/bt_drv/wmt_chrdev_wifi/wlan_drv_gen4m modules are
 * rejected by this kernel (modversions CRC mismatch for printk, vsnprintf and
 * __stack_chk_fail) and /vendor is dm-verity protected, so they cannot be
 * replaced on the device. Modules rebuilt against this exact kernel are
 * embedded in the kernel image and loaded in the same order the vendor init
 * chain would load them:
 *
 *   init.wmt_drv.rc  : wmt_drv
 *   init.bt_drv.rc   : bt_drv
 *   init.wlan_drv.rc : wmt_chrdev_wifi, wlan_drv_gen4m
 *
 * Loading happens before userspace starts so that /dev/wmtdetect exists when
 * the vendor wmt_loader service runs (it is oneshot and does not retry).
 * Later "insmod" requests for vendor modules from init fail harmlessly
 * (name already loaded / CRC mismatch), so stock and rebuilt modules can never
 * be loaded together.
 *
 * Loading goes through the normal load_module() path: modversions and vermagic
 * checks are NOT bypassed.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>

extern int via_s48_load_module_blob(const void *blob, unsigned long len);

#define VIA_S48_BLOB_DECL(n) \
	extern const u8 via_s48_##n##_start[], via_s48_##n##_end[]

VIA_S48_BLOB_DECL(wmt_drv);
VIA_S48_BLOB_DECL(bt_drv);
VIA_S48_BLOB_DECL(wmt_chrdev_wifi);
VIA_S48_BLOB_DECL(wlan_drv_gen4m);

struct via_s48_blob {
	const char *name;
	const u8 *start;
	const u8 *end;
};

#define VIA_S48_BLOB(n) { #n, via_s48_##n##_start, via_s48_##n##_end }

static const struct via_s48_blob via_s48_blobs[] = {
	VIA_S48_BLOB(wmt_drv),
	VIA_S48_BLOB(bt_drv),
	VIA_S48_BLOB(wmt_chrdev_wifi),
	VIA_S48_BLOB(wlan_drv_gen4m),
};

static bool via_s48_preload_enable = true;
module_param_named(enable, via_s48_preload_enable, bool, 0444);

static int __init via_s48_connsys_preload_init(void)
{
	size_t i;
	int err;

	if (!via_s48_preload_enable) {
		pr_info("via_s48_connsys: preload disabled\n");
		return 0;
	}

	for (i = 0; i < ARRAY_SIZE(via_s48_blobs); i++) {
		const struct via_s48_blob *b = &via_s48_blobs[i];

		err = via_s48_load_module_blob(b->start, b->end - b->start);
		if (err) {
			/* Do not stop boot; later modules depend on earlier ones
			 * and will fail on their own with a clear error.
			 */
			pr_err("via_s48_connsys: %s.ko (%zu bytes) load failed: %d\n",
			       b->name, (size_t)(b->end - b->start), err);
			continue;
		}
		pr_info("via_s48_connsys: %s.ko (%zu bytes) loaded\n",
			b->name, (size_t)(b->end - b->start));
	}

	return 0;
}
late_initcall_sync(via_s48_connsys_preload_init);
