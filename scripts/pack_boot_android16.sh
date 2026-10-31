#!/usr/bin/env bash
#
# pack_boot_android16.sh - Package Android 16-Compatible Boot Image for Casper VIA S48
#

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="${REPO_ROOT}/out"
mkdir -p "${OUT_DIR}"

KERNEL_BIN="${OUT_DIR}/arch/arm64/boot/Image.gz"
DTB_BIN="${OUT_DIR}/arch/arm64/boot/dts/mediatek/mt8168.dtb"
RAMDISK_GZ="${OUT_DIR}/ramdisk_dfe.cpio.gz"
OUTPUT_BOOT="${OUT_DIR}/boot_oc_2.1ghz_android16.img"

# Fallback to reference objects if out/ is not yet built
if [[ ! -f "${KERNEL_BIN}" ]]; then
    if [[ -f "${REPO_ROOT}/../oc_boot/kernel" ]]; then
        KERNEL_BIN="${REPO_ROOT}/../oc_boot/kernel"
    fi
fi

if [[ ! -f "${DTB_BIN}" ]]; then
    if [[ -f "${REPO_ROOT}/../oc_boot/dtb" ]]; then
        DTB_BIN="${REPO_ROOT}/../oc_boot/dtb"
    fi
fi

if [[ ! -f "${RAMDISK_GZ}" ]]; then
    if [[ -f "${REPO_ROOT}/../gsi_boot/ramdisk.cpio.gz" ]]; then
        RAMDISK_GZ="${REPO_ROOT}/../gsi_boot/ramdisk.cpio.gz"
    fi
fi

if [[ ! -f "${KERNEL_BIN}" ]] || [[ ! -f "${DTB_BIN}" ]] || [[ ! -f "${RAMDISK_GZ}" ]]; then
    echo "ERROR: Missing required components to package boot image." >&2
    echo "Kernel:  ${KERNEL_BIN}" >&2
    echo "DTB:     ${DTB_BIN}" >&2
    echo "Ramdisk: ${RAMDISK_GZ}" >&2
    exit 1
fi

echo "=========================================================="
echo " Packaging Android 16 Boot Image (Header v2 + DFE)"
echo " Kernel:  ${KERNEL_BIN}"
echo " DTB:     ${DTB_BIN}"
echo " Ramdisk: ${RAMDISK_GZ}"
echo " Output:  ${OUTPUT_BOOT}"
echo "=========================================================="

mkbootimg \
    --header_version 2 \
    --os_version 10.0.0 \
    --os_patch_level 2020-12 \
    --kernel "${KERNEL_BIN}" \
    --ramdisk "${RAMDISK_GZ}" \
    --dtb "${DTB_BIN}" \
    --pagesize 2048 \
    --base 0x00000000 \
    --kernel_offset 0x40080000 \
    --ramdisk_offset 0x55000000 \
    --second_offset 0x00000000 \
    --tags_offset 0x54000000 \
    --dtb_offset 0x0000000054000000 \
    --cmdline "bootopt=64S3,32N2,64N2 buildvariant=user androidboot.selinux=permissive" \
    -o "${OUTPUT_BOOT}"

echo "SUCCESS: Boot image created."
ls -lh "${OUTPUT_BOOT}"
sha256sum "${OUTPUT_BOOT}"
