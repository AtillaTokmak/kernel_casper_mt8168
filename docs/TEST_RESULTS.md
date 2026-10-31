# Live Test Results & Hardware Validation Report

## Environment
- **Device:** Casper VIA S48 (`F716`)
- **OS Under Test:** AxionOS 2.9 (Android 16 QPR2 / Baklava, API 36, Build `BP4A.251205.006`)
- **Kernel:** Linux 4.14.141 `#10 SMP PREEMPT Sun Oct 18 17:44:57 +03 2026` (`atilla@sandbox`)
- **Test Date:** October 31, 2026

---

## 1. Hardware Verification Matrix

| Hardware Feature | Driver / Subsystem | Test Result | Diagnostic Observation |
| :--- | :--- | :---: | :--- |
| **Boot Sequence** | Little Kernel -> Linux 4.14 | **PASS** | Bootloader `orange` status; clean handover |
| **CPU Overclock** | MT8168 DVFS / Schedutil | **PASS** | Clocks scale up to 2,100,000 kHz across all 4 cores |
| **Display Panel** | `jd9365da` MIPI DSI | **PASS** | 800 x 1280 @ 56.1 FPS hardware composition |
| **GPU Acceleration** | ARM Mali-G52 Bifrost | **PASS** | SurfaceFlinger and HWUI hardware accelerated |
| **Touchscreen** | Silead GSLX68X / MTK TPD | **PASS** | Direct multi-touch mapper operational |
| **Storage Subsystem** | eMMC 32 GB (`mmcblk0`) | **PASS** | 22 GB `/data` unencrypted EXT4 direct mount |
| **ADB Connectivity** | Android Debug Bridge | **PASS** | Fully authenticated shell, port 5037 active |
| **Battery & Power** | MT6357 PMIC Gauge | **PASS** | Battery percentage, voltage, and charging reporting |
| **System UI** | AxionOS Launcher / Settings | **PASS** | `FallbackHome` / `WelcomeActivity` rendered cleanly |

---

## 2. Boot Performance & Stability
- **Cold Boot Time:** ~35 seconds from Preloader handshake to launcher.
- **Thermal Behavior:** Idles at ~38°C; no thermal runaway observed at 2.1 GHz during initial bring-up tests.
- **SELinux Status:** System is running with Enforcing mode active.
