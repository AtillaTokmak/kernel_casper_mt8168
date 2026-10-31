# Android 16 (Baklava / API 36) Kernel Compatibility Guide

## Overview

This document describes the architectural requirements, compatibility considerations, and verified boot parameters for running **Android 16 QPR2 / Baklava** (tested on AxionOS 2.9, Build `BP4A.251205.006`) with the Linux 4.14.141 kernel on the **Casper VIA S48** tablet (MediaTek MT8168 / F716).

---

## 1. Kernel Architecture Prerequisites

Android 16 introduces strict userspace requirements compared to earlier releases. When running on a Linux 4.14 baseline:

| Subsystem | Android 16 Requirement | Linux 4.14 Status | Implementation / Resolution |
| :--- | :--- | :--- | :--- |
| **Storage (FUSE)** | Expects FUSE-BPF (`CONFIG_FUSE_BPF`) | Not present in 4.14 | Set userspace property `persist.sys.fuse.bpf.override=false` |
| **Network BPF** | `netbpfload` requires eBPF | `CONFIG_BPF=y`, `CONFIG_BPF_SYSCALL=y` | TrebleDroid bypass active; `persist.sys.phh.bpf=false` |
| **File Encryption** | KeyMint AIDL mandatory for FBE | Only Keymaster 4.0 HIDL available | Remove `fileencryption=...` from ramdisk fstab (DFE) |
| **SELinux** | Enforcing mode with strict policies | Tested Enforcing & Permissive | `androidboot.selinux=permissive` recommended for bring-up |
| **Media Codecs** | UBSAN-sanitized `libmedia_codeclist` | MTK MP2 decoder declares `audio/mpeg-L2` | Filter `OMX.MTK.AUDIO.DECODER.MP2` in vendor XML |
| **VINTF** | Expects FCM matrices | Vendor requires FCM Level 4 (Q) | Inject `compatibility_matrix.4.xml` into `/system/etc/vintf/` |

---

## 2. Boot Image Assembly for Android 16

The boot image must be packaged with Android Boot Image Header Version 2:

```bash
mkbootimg \
    --header_version 2 \
    --os_version 10.0.0 \
    --os_patch_level 2020-12 \
    --kernel out/Image.gz \
    --ramdisk out/ramdisk_dfe.cpio.gz \
    --dtb out/tb8168p1_64_bsp_f716.dtb \
    --pagesize 2048 \
    --base 0x00000000 \
    --kernel_offset 0x40080000 \
    --ramdisk_offset 0x55000000 \
    --second_offset 0x00000000 \
    --tags_offset 0x54000000 \
    --dtb_offset 0x0000000054000000 \
    --cmdline "bootopt=64S3,32N2,64N2 buildvariant=user androidboot.selinux=permissive" \
    -o out/boot_oc_2.1ghz_android16.img
```

### Key Parameters:
- **`--cmdline`:** Appending `androidboot.selinux=permissive` allows unhindered binder IPC communication between Android 16 system services and Android 10 vendor HAL daemons.
- **`--ramdisk`:** Must contain an unencrypted `fstab.mt8168` where `fileencryption=aes-256-xts` and `checkpoint=block` are removed.
- **`--dtb`:** Matches the 2.100 GHz operating-performance-point (OPP) frequency table.
