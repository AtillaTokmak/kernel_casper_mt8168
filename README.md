# Casper VIA S48 (F716 / MT8168) Android Kernel Source

Clean, standalone Linux 4.14.141 kernel source for the **Casper VIA S48** tablet (Motherboard: **F716**, SoC: **MediaTek MT8168**).

In a custom ROM tree (e.g. LineageOS / AOSP / Treble), place this repository at:
```
kernel/casper/mt8168/
```

---

## Supported Hardware & Features
- **Display / Panel:** `jd9365da_wxga_dsi_vdo` (800x1280) with fully verified, seamless LCD suspend/resume power sequencing.
- **Touchscreen:** Silead GSLX68X (1280x800) with in-kernel firmware initialization and ESD recovery.
- **Wi-Fi Subsystem:** MediaTek MT6631 / MT8168 Gen4M (802.11 a/b/g/n/ac) with in-kernel driver preloader (`wmt_drv.ko`, `wmt_chrdev_wifi.ko`, `wlan_drv_gen4m.ko`).
- **Bluetooth:** MT6631 Bluetooth 5.0 with in-kernel autoloader (`bt_drv.ko`).
- **Audio & Battery:** MT6357 PMIC audio codec and battery gauge. Charger watchdog reset loop fixed.

---

## Branches
- `main` (**Production Default**): Standard 2.001 GHz kernel with full stability, power management, and thermal protections.
- `oc-2.1ghz-experimental`: Experimental 2.100 GHz CPU overclock branch with expanded power throttle tables (verified working under load; see technical notes below).

---

## Requirements & Toolchain

### 1. Cross Compiler
This kernel requires the Google Android AArch64 GCC 4.9 toolchain:
- **Download:** [Android Prebuilts GCC 4.9](https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9)
- **Path:** Export `CROSS_COMPILE` pointing to the toolchain prefix, e.g.:
  ```bash
  export CROSS_COMPILE=/path/to/aarch64-linux-android-4.9/bin/aarch64-linux-android-
  ```

### 2. Build Configuration
- **Defconfig:** `tb8168p1_64_bsp_f716_defconfig`
- **SoC Platform:** `mt8168`
- **Architecture:** `arm64`

---

## How to Build

### Full Build (Kernel + DTBs + Connectivity Modules)
Run the root build script:
```bash
./build.sh all
```

Output files will be generated in `out/`:
- `out/Image`
- `out/Image.gz`
- `out/Image.gz-dtb`
- `out/tb8168p1_64_bsp_f716.dtb`
- `out/modules/wmt_drv.ko`
- `out/modules/bt_drv.ko`
- `out/modules/wmt_chrdev_wifi.ko`
- `out/modules/wlan_drv_gen4m.ko`

### Packaging 32MB Boot Image (Header v2 + AVB)
```bash
chmod +x scripts/pack_boot.sh
./scripts/pack_boot.sh
```
Produces `out/boot_via_s48_32m.img` ready for `fastboot flash boot`.

---

## Connectivity Module Dependency Order
When initializing or loading modules manually, load strictly in this sequence:
1. `wmt_drv.ko` (creates `/dev/wmtdetect`, asserts `vendor.connsys.driver.ready=yes`)
2. `bt_drv.ko` (creates `/dev/stpbt`)
3. `wmt_chrdev_wifi.ko` (creates `/dev/wmtWifi`)
4. `wlan_drv_gen4m.ko` (creates `wlan0` interface)

*Note:* In this kernel, an in-kernel preload driver (`drivers/misc/mediatek/connectivity/via_s48_preload/`) automatically mounts and preloads these drivers into RAM during late init, so manual `insmod` is not required.

---

## GSI / Treble Compatibility Notes
When running modern Generic System Images (Android 10/11/12+ GSI) on Casper VIA S48, set the following system property to prevent the Bluetooth stack from aborting on MT6631 unsupported HCI commands:
```bash
setprop persist.sys.bt.unsupported.commands "182,174"
```

---

## License
Kernel sources, device trees, and drivers are licensed under the **GNU General Public License v2.0 (GPL-2.0)** matching the upstream Linux kernel. See `COPYING` for details.
