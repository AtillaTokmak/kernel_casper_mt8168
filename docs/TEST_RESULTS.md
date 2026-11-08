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

---

## 3. Hardware Fixes & Android 16 Hardware Validation (November 8, 2026)

**Verification Status:** Verified on physical hardware by the user (`AtillaTokmak`).

| Subsystem / Feature | Implementation / Driver | Hardware Test Result | Validation Notes |
| :--- | :--- | :---: | :--- |
| **Front Camera** | `gc2385sub_mipi_raw` | **PASS** | Camera HAL identifies facing front camera; preview and capture operational. |
| **Flashlight / Torch** | `flashlights_led191` | **PASS** | Quick Settings flashlight toggle and camera capture flash operational. |
| **Capacitive Touch Keys** | `mtk_gslX680` (Back, Home, App Switch) | **PASS** | Physical capacitive touch keys under display emit correct events and trigger navigation actions. On-screen navigation bar maintained in parallel. |
| **DTB Integrity & Packaging** | `scripts/verify_dtb_integrity.py` | **PASS** | Complete 69,134-byte DT table verified; prevented DTB truncation defect from hwfix_v1. |

---

## 4. Boot Image Build History & Verification

| Image Name | SHA-256 Hash | Physical Device Status | Notes |
| :--- | :--- | :---: | :--- |
| `boot_oc_2.1ghz_android16.img` | `879da6cfc5ce2e84cc5d54a21c69bbee28bf14278743dfe2ae1a711418fcbef9` | **PASS** | Baseline working Android 16 + 2.1 GHz OC kernel (hardware fixes not included). |
| `boot_oc_2.1ghz_android16_hwfix_v1.img` | `c1d5136402ce81ec75a571d9892454eb5b0813ad3a0bdabc88bec15ee62ff6df` | **FAILED** | DTB truncated to 69,074 bytes (60 bytes cut off; `FDT_ERR_TRUNCATED`). Device reset to preloader. |
| `boot_oc_2.1ghz_android16_hwfix_v1_dtbfix.img` | `a92375eebd011e3e51788273a5c6446e72570394aee357e38491ecc8e95695a5` | **PASS** | **Final Working Release Image:** Kernel with front camera, LED191 flash, and capacitive touchkeys + complete 69,134-byte DTB. Verified working on physical device. |

