#!/usr/bin/env bash
#
# Casper VIA S48 (F716 / MT8168) Boot Image Packaging Script
# Uses Header v2 and AVB 2.0 RSA2048 Hash Footer (Partition: 33,554,432 bytes)
#

set -e

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="${REPO_ROOT}/out"
ANALYSIS_DIR="/home/atilla/arastirma/s48-donor-audit/analysis"
AVB_KEY="/home/atilla/arastirma/donor_audit_mt8365_ayla/MT8365_Q0_Ayla/external/avb/test/data/testkey_rsa2048.pem"

RAMDISK="${OUT_DIR}/stock_ramdisk.cpio.gz"
if [ ! -f "${RAMDISK}" ]; then
    if [ -f "/home/atilla/arastirma/build_out_via_s48/stock_ramdisk.cpio.gz" ]; then
        cp "/home/atilla/arastirma/build_out_via_s48/stock_ramdisk.cpio.gz" "${RAMDISK}"
    elif [ -f "${ANALYSIS_DIR}/stock-ramdisk.bin" ]; then
        cp "${ANALYSIS_DIR}/stock-ramdisk.bin" "${RAMDISK}"
    fi
fi

# Ensure 69074-byte DT table container exists
DT_TABLE="${OUT_DIR}/boot_dt_table.img"
if [ ! -f "${DT_TABLE}" ]; then
    if [ -f "/home/atilla/arastirma/build_out_via_s48/boot_dt_table.img" ]; then
        cp "/home/atilla/arastirma/build_out_via_s48/boot_dt_table.img" "${DT_TABLE}"
    elif [ -f "${ANALYSIS_DIR}/boot.img-8b757c.dtb" ]; then
        python3 -c "
import struct
with open('${ANALYSIS_DIR}/boot.img-8b757c.dtb', 'rb') as f: dtb = f.read()
hdr = struct.pack('>8I', 0xd7b7ab1e, 64 + len(dtb), 32, 32, 1, 32, 2048, 0)
entry = struct.pack('>8I', len(dtb), 64, 0, 0, 0, 0, 0, 0)
with open('${DT_TABLE}', 'wb') as f: f.write(hdr + entry + dtb)
"
    fi
fi

KERNEL_PAYLOAD="${OUT_DIR}/Image.gz-dtb"
RAW_BOOT="${OUT_DIR}/boot_raw.img"
FINAL_BOOT="${OUT_DIR}/boot_via_s48_32m.img"

echo "=== Packaging Boot Image (Header v2) ==="
echo "Kernel:  ${KERNEL_PAYLOAD}"
echo "Ramdisk: ${RAMDISK}"
echo "DTB:     ${DT_TABLE}"

mkbootimg \
    --header_version 2 \
    --os_version 10.0.0 \
    --os_patch_level 2020-12 \
    --kernel "${KERNEL_PAYLOAD}" \
    --ramdisk "${RAMDISK}" \
    --dtb "${DT_TABLE}" \
    --pagesize 2048 \
    --base 0x00000000 \
    --kernel_offset 0x40080000 \
    --ramdisk_offset 0x55000000 \
    --second_offset 0x40f00000 \
    --tags_offset 0x54000000 \
    --dtb_offset 0x0000000054000000 \
    --board "" \
    --cmdline "bootopt=64S3,32N2,64N2 buildvariant=user" \
    -o "${RAW_BOOT}"

echo "Adding AVB Hash Footer (Partition size: 33,554,432 bytes)..."
cp "${RAW_BOOT}" "${FINAL_BOOT}"

avbtool add_hash_footer \
    --image "${FINAL_BOOT}" \
    --partition_size 33554432 \
    --partition_name boot \
    --algorithm SHA256_RSA2048 \
    --key "${AVB_KEY}" \
    --prop com.android.build.boot.os_version:10 \
    --prop com.android.build.boot.security_patch:2019-06-06

echo "=== Boot Image Packaged Successfully ==="
ls -lh "${FINAL_BOOT}"
sha256sum "${FINAL_BOOT}"
