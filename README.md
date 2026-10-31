# Casper VIA S48 (F716 / MT8168) Android Kernel Source

Clean, standalone Linux 4.14.141 kernel source for the **Casper VIA S48** tablet (Motherboard: **F716**, SoC: **MediaTek MT8168**).

In a custom ROM tree (e.g. LineageOS / AxionOS / AOSP / Treble), place this repository at:
```
kernel/casper/mt8168/
```

---

## Supported Hardware & Features
- **Android Compatibility:** Verified fully functional on **Android 16 QPR2 / Baklava** (AxionOS 2.9, API 36) and stock Android 10.
- **CPU Overclock:** Stable **2.100 GHz** DVFS overclocking across all 4 Cortex-A53 cores (expanded power throttle table, stock safe 1025 mV voltage ceiling).
- **Display / Panel:** `jd9365da_wxga_dsi_vdo` (800x1280 @ 56.1 FPS) with seamless LCD suspend/resume power sequencing.
- **Touchscreen:** Silead GSLX68X / MTK TPD with in-kernel firmware initialization and direct multi-touch mapping.
- **GPU Acceleration:** ARM Mali-G52 (Bifrost) with full hardware composition in SurfaceFlinger.
- **Wi-Fi Subsystem:** MediaTek MT6631 / MT8168 Gen4M (802.11 a/b/g/n/ac) with in-kernel driver preloader (`wmt_drv.ko`, `wmt_chrdev_wifi.ko`, `wlan_drv_gen4m.ko`).
- **Bluetooth:** MT6631 Bluetooth 5.0 with in-kernel autoloader (`bt_drv.ko`).
- **Audio & Battery:** MT6357 PMIC audio codec and battery gauge. Charger watchdog reset loop fixed.

---

## Branches
- `android16-axionos-compat` (**Active Android 16 Bring-up**): Verified working on Android 16 (Baklava) with 2.1 GHz overclock, DFE ramdisk packaging tooling, and complete documentation.
- `oc-2.1ghz-experimental`: 2.100 GHz CPU overclock branch.
- `android-10` / `main`: Baseline 2.001 GHz stock-frequency kernel.

---

## Documentation
- [Android 16 Compatibility Guide](docs/ANDROID16_COMPATIBILITY.md)
- [Boot Image Forensic Comparison](docs/BOOT_IMAGE_COMPARISON.md)
- [Kernel Configuration Audit](docs/KERNEL_CONFIGURATION.md)
- [2.1 GHz Overclock Architecture](docs/OVERCLOCK_2_1GHZ.md)
- [Build & Packaging Instructions](docs/BUILD_INSTRUCTIONS.md)
- [Live Hardware Test Results](docs/TEST_RESULTS.md)

---

## Quick Build & Packaging

### 1. Compile Kernel & Modules
```bash
export CROSS_COMPILE=/path/to/aarch64-linux-android-
./build.sh all
```

### 2. Package Android 16 Boot Image
```bash
./scripts/pack_boot_android16.sh
```
Produces `out/boot_oc_2.1ghz_android16.img` ready for flashing via `fastboot flash boot`.

---

## GSI / Treble Compatibility Notes
When running modern Generic System Images (Android 15 / 16 GSI) on Casper VIA S48:
1. **Disable FUSE-BPF:** `setprop persist.sys.fuse.bpf.override false`
2. **Universal BPF Bypass:** `setprop persist.sys.phh.bpf false`
3. **USB Gadget Binding:** `setprop sys.usb.controller 11201000.usb`
4. **Bluetooth HCI Patch:** `setprop persist.sys.bt.unsupported.commands "182,174"`
5. **MediaCodec UBSAN Fix:** Filter out legacy `OMX.MTK.AUDIO.DECODER.MP2` from vendor XML.

---

## License
Kernel sources, device trees, and drivers are licensed under the **GNU General Public License v2.0 (GPL-2.0)** matching the upstream Linux kernel. See `COPYING` for details.

Based on the original kernel work by Zhiqin Chen.
