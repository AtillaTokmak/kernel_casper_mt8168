# Forensic Boot Image Comparison Report

This document records the exact SHA-256 hashes, component breakdowns, and differences between the reference boot images analyzed during the Android 16 bring-up project on Casper VIA S48.

---

## 1. Artifact SHA-256 Checksums

| Artifact File | Size (Bytes) | SHA-256 Checksum | Purpose / Description |
| :--- | :--- | :--- | :--- |
| **Stock `boot.img`** | 33,554,432 | `bc31d5e1ca609631502d5ecc53ce9d9db45838d8ad7346d6a74f2dc436e3c311` | Official Casper C1.03 firmware release |
| **`boot_via_s48_32m_oc_2.1ghz_standard.img`** | 33,554,432 | `70584943aa85f80fb0b8aa707fdd35fcf99541f6ce6dc3893f07096963c9d3c9` | Reference 2.1 GHz kernel + stock ramdisk |
| **`boot_gsi.img`** | 10,131,456 | `1365822690a638c1947183e18a9ba38b03c2d7cd3c9342f14055ddd72bb2decb` | Stock kernel + DFE ramdisk + permissive cmdline |
| **`boot_oc_2.1ghz_android16.img`** | 11,079,680 | `879da6cfc5ce2e84cc5d54a21c69bbee28bf14278743dfe2ae1a711418fcbef9` | Baseline Working Image: 2.1 GHz OC kernel + DFE ramdisk |
| **`boot_oc_2.1ghz_android16_hwfix_v1.img`** | 10,582,016 | `c1d5136402ce81ec75a571d9892454eb5b0813ad3a0bdabc88bec15ee62ff6df` | **FAILED Image:** Hardware fixes included, but DTB truncated to 69,074 bytes (60 bytes cut off). Bootloader reset loop. |
| **`boot_oc_2.1ghz_android16_hwfix_v1_dtbfix.img`** | 10,582,016 | `a92375eebd011e3e51788273a5c6446e72570394aee357e38491ecc8e95695a5` | **Final Working Release Image:** Hardware fixes (front camera, LED191 flash, touch navigation keys) + complete 69,134-byte DTB. Verified working on physical device. |

---

## 2. Component Hash Comparison

### Kernel Binary (`kernel` payload inside boot image):
- **Stock Kernel (C1.03):** `588d40706ae486acb25874ee7487382377c585290901821bb5770ef64f94eb37` (9,206,542 bytes)
  - Banner: `Linux version 4.14.141 #1 SMP PREEMPT Mon Jun 6 10:55:37 CST 2022 aarch64`
- **Overclocked 2.1 GHz Kernel:** `3726c8507cc13f971991ea92b8fa3e17badcfbe2b5718496e76953b99bd941d2` (10,213,985 bytes)
  - Banner: `Linux version 4.14.141 (atilla@sandbox) (gcc version 4.9.x) #10 SMP PREEMPT Sun Oct 18 17:44:57 +03 2026 aarch64`
  - Note: Used in both `boot_via_s48_32m_oc_2.1ghz_standard.img` and `boot_oc_2.1ghz_android16.img`.

### Device Tree Blob (`dtb` payload):
- **Stock DTB:** `fef4c430dd70035a496b407c948cd2f5d69eb8d9095c8abbfcc792e946b1aaa9` (69,074 bytes)
  - Frequencies: 850 MHz to 2001 MHz (16 OPP entries)
- **Overclocked DTB:** `6fb094551a67d69ddc61828e6be705f6941a73f5de1403f5f658e39a6421eafd` (69,134 bytes)
  - Frequencies: 850 MHz to 2100 MHz (17 OPP entries; contains `opp-2100000000`)

### Ramdisk Archive (`ramdisk` payload):
- **Stock / OC Standard Ramdisk:** `85fe6d31416fc435f2f1cfc15a61d6124cd51c4565aecb086db2e4d25633e5c8` (794,962 bytes)
  - Fstab contains: `fileencryption=aes-256-xts,checkpoint=block`
- **DFE Ramdisk (Android 16 Compatible):** `959247f12e8736ac24a73752e259e844ee48d56a64e1069f05d535ca44cf19db` (791,581 bytes)
  - Fstab contains: Direct unencrypted EXT4 flags (`wait,check,formattable,quota,resize,reservedsize=128m`)

---

## 3. Findings Summary
1. The 2.1 GHz kernel in `boot_via_s48_32m_oc_2.1ghz_standard.img` originally failed to boot Android 16 purely due to the ramdisk's `fileencryption` setting.
2. The kernel binary and DTB were 100% sound.
3. Combining the 2.1 GHz kernel + 2.1 GHz DTB with the DFE ramdisk resulted in immediate, successful boot into the Android 16 launcher.
4. `hwfix_v1` encountered an early boot reset because the packaging step truncated the DTB from 69,134 to 69,074 bytes (stock size reference error), causing `libfdt` to fail with `FDT_ERR_TRUNCATED`.
5. Restoring the complete 69,134-byte DT table and validating with `scripts/verify_dtb_integrity.py` produced `boot_oc_2.1ghz_android16_hwfix_v1_dtbfix.img`, successfully verifying front camera, flashlight, and capacitive keys on the physical device.

