#!/usr/bin/env bash
#
# Casper VIA S48 (F716 / MT8168) Standalone Kernel & Module Build Script
# Usage: ./build.sh [kernel|modules|boot|all]
#

set -e

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT_DIR="${REPO_ROOT}/out"
MODULES_OUT="${OUT_DIR}/modules"

# Toolchain configuration
if [ -z "${CROSS_COMPILE}" ]; then
    TC_CANDIDATE="/home/atilla/arastirma/donor_audit_mt8365_ayla/MT8365_Q0_Ayla/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-"
    if [ -f "${TC_CANDIDATE}gcc" ]; then
        CROSS_COMPILE="${TC_CANDIDATE}"
    elif command -v aarch64-linux-android-gcc >/dev/null 2>&1; then
        CROSS_COMPILE="aarch64-linux-android-"
    elif command -v aarch64-linux-gnu-gcc >/dev/null 2>&1; then
        CROSS_COMPILE="aarch64-linux-gnu-"
    else
        echo "ERROR: aarch64 cross-compiler not found!"
        echo "Please export CROSS_COMPILE=/path/to/aarch64-linux-android-"
        exit 1
    fi
fi

STRIP_BIN="${CROSS_COMPILE}strip"
if [ ! -f "${STRIP_BIN}" ] && ! command -v "${STRIP_BIN}" >/dev/null 2>&1; then
    STRIP_BIN="aarch64-linux-android-strip"
fi

export ARCH=arm64
export CROSS_COMPILE
export HOSTCFLAGS="-fcommon"

DEFCONFIG="tb8168p1_64_bsp_f716_defconfig"
MTK_PLATFORM="mt8168"
JOBS="$(nproc)"

mkdir -p "${OUT_DIR}" "${MODULES_OUT}"

build_kernel() {
    echo "=========================================================="
    echo " Building Casper VIA S48 Kernel (${DEFCONFIG})"
    echo " Architecture: ${ARCH}"
    echo " Cross Compile: ${CROSS_COMPILE}gcc"
    echo "=========================================================="

    if [ ! -f "${REPO_ROOT}/.config" ]; then
        echo "Applying defconfig ${DEFCONFIG}..."
        make -C "${REPO_ROOT}" "${DEFCONFIG}"
    fi

    echo "Compiling Kernel Image, Image.gz, and DTBs (-j${JOBS})..."
    make -C "${REPO_ROOT}" -j"${JOBS}" Image Image.gz mediatek/tb8168p1_64_bsp_f716.dtb mediatek/mt8168_dtbo.dtb modules

    cp "${REPO_ROOT}/arch/arm64/boot/Image" "${OUT_DIR}/"
    cp "${REPO_ROOT}/arch/arm64/boot/Image.gz" "${OUT_DIR}/"
    cp "${REPO_ROOT}/arch/arm64/boot/dts/mediatek/tb8168p1_64_bsp_f716.dtb" "${OUT_DIR}/"
    cp "${REPO_ROOT}/arch/arm64/boot/dts/mediatek/mt8168_dtbo.dtb" "${OUT_DIR}/"

    # Produce Image.gz-stockdtb if base DTB exists
    BASE_DTB="${REPO_ROOT}/arch/arm64/boot/dts/mediatek/tb8168p1_64_bsp_f716.dtb"
    if [ -f "/home/atilla/arastirma/s48-donor-audit/analysis/boot.img-8b757c.dtb" ]; then
        BASE_DTB="/home/atilla/arastirma/s48-donor-audit/analysis/boot.img-8b757c.dtb"
    fi
    cat "${OUT_DIR}/Image.gz" "${BASE_DTB}" > "${OUT_DIR}/Image.gz-dtb"

    echo "Kernel compiled successfully -> ${OUT_DIR}/Image"
}

build_modules() {
    echo "=========================================================="
    echo " Building Connectivity Kernel Modules (WMT, BT, Wi-Fi)"
    echo "=========================================================="

    CONN_ROOT="${REPO_ROOT}/modules/connectivity"
    WMT_DIR="${CONN_ROOT}/common"
    BT_DIR="${CONN_ROOT}/bt/mt66xx/legacy"
    WIFI_ADAPTOR_DIR="${CONN_ROOT}/wlan/adaptor"
    WIFI_GEN4M_DIR="${CONN_ROOT}/wlan/core/gen4m"

    # 1. wmt_drv.ko
    echo "--> [1/4] Building wmt_drv.ko..."
    make -C "${REPO_ROOT}" M="${WMT_DIR}" \
        KERNEL_OUT="${REPO_ROOT}" TOP="${REPO_ROOT}" \
        MTK_PLATFORM="${MTK_PLATFORM}" \
        CONFIG_MTK_COMBO=m CONFIG_MTK_COMBO_CHIP=CONNAC \
        AUTOCONF_H="${REPO_ROOT}/include/generated/autoconf.h" \
        modules -j"${JOBS}"

    # 2. bt_drv.ko
    echo "--> [2/4] Building bt_drv.ko..."
    make -C "${REPO_ROOT}" M="${BT_DIR}" \
        KERNEL_OUT="${REPO_ROOT}" TOP="${REPO_ROOT}" \
        MTK_PLATFORM="${MTK_PLATFORM}" \
        WMT_SRC_FOLDER="${WMT_DIR}" \
        AUTOCONF_H="${REPO_ROOT}/include/generated/autoconf.h" \
        KBUILD_EXTRA_SYMBOLS="${WMT_DIR}/Module.symvers" \
        modules -j"${JOBS}"

    # 3. wmt_chrdev_wifi.ko
    echo "--> [3/4] Building wmt_chrdev_wifi.ko..."
    make -C "${REPO_ROOT}" M="${WIFI_ADAPTOR_DIR}" \
        KERNEL_OUT="${REPO_ROOT}" TOP="${REPO_ROOT}" \
        MTK_PLATFORM="${MTK_PLATFORM}" \
        WMT_SRC_FOLDER="${WMT_DIR}" \
        KBUILD_EXTRA_SYMBOLS="${WMT_DIR}/Module.symvers" \
        modules -j"${JOBS}"

    # 4. wlan_drv_gen4m.ko
    echo "--> [4/4] Building wlan_drv_gen4m.ko..."
    make -C "${REPO_ROOT}" M="${WIFI_GEN4M_DIR}" \
        KERNEL_OUT="${REPO_ROOT}" TOP="${REPO_ROOT}" \
        MTK_PLATFORM="${MTK_PLATFORM}" \
        CONFIG_MTK_COMBO_WIFI_HIF=axi \
        MTK_COMBO_CHIP=CONNAC \
        WLAN_CHIP_ID=8168 \
        MTK_ANDROID_WMT=y \
        MODULE_NAME=wlan_drv_gen4m \
        KBUILD_EXTRA_SYMBOLS="${WMT_DIR}/Module.symvers ${WIFI_ADAPTOR_DIR}/Module.symvers" \
        modules -j"${JOBS}"

    # Strip and copy to output
    echo "Stripping and staging modules..."
    "${STRIP_BIN}" --strip-unneeded "${WMT_DIR}/wmt_drv.ko" -o "${MODULES_OUT}/wmt_drv.ko"
    "${STRIP_BIN}" --strip-unneeded "${BT_DIR}/bt_drv.ko" -o "${MODULES_OUT}/bt_drv.ko"
    "${STRIP_BIN}" --strip-unneeded "${WIFI_ADAPTOR_DIR}/wmt_chrdev_wifi.ko" -o "${MODULES_OUT}/wmt_chrdev_wifi.ko"
    "${STRIP_BIN}" --strip-unneeded "${WIFI_GEN4M_DIR}/wlan_drv_gen4m.ko" -o "${MODULES_OUT}/wlan_drv_gen4m.ko"

    echo "=== Connectivity Modules Built Successfully ==="
    ls -lh "${MODULES_OUT}"
}

ACTION="${1:-all}"

case "${ACTION}" in
    kernel)
        build_kernel
        ;;
    modules)
        build_modules
        ;;
    all)
        build_kernel
        build_modules
        ;;
    *)
        echo "Usage: $0 [kernel|modules|all]"
        exit 1
        ;;
esac
