# Kernel Build & Packaging Instructions

## 1. Prerequisites & Toolchain

### Recommended Toolchain
Google Android GCC 4.9 AArch64 prebuilt toolchain:
```bash
git clone https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9 toolchain
export CROSS_COMPILE="$(pwd)/toolchain/bin/aarch64-linux-android-"
export ARCH=arm64
```

---

## 2. Compilation Commands

### Step 1: Configure
```bash
make O=out tb8168p1_64_bsp_f716_defconfig
```

### Step 2: Compile Kernel & Device Tree
```bash
make O=out -j"$(nproc)"
```
This produces:
- `out/arch/arm64/boot/Image`
- `out/arch/arm64/boot/Image.gz`
- `out/arch/arm64/boot/dts/mediatek/mt8168.dtb`

### Step 3: Compile MediaTek Out-of-tree Connectivity Modules
Run the module build function via `build.sh`:
```bash
./build.sh modules
```
Staged in `out/modules/`:
- `wmt_drv.ko`
- `bt_drv.ko`
- `wmt_chrdev_wifi.ko`
- `wlan_drv_gen4m.ko`

---

## 3. Packaging Android 16 Boot Image

To package an Android 16-ready boot image (Header v2 with DFE ramdisk):
```bash
./scripts/pack_boot_android16.sh
```
This script assembles `out/boot_oc_2.1ghz_android16.img` ready for flashing via fastboot:
```bash
fastboot flash boot out/boot_oc_2.1ghz_android16.img
```
