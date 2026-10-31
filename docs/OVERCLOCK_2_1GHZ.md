# 2.1 GHz CPU Overclocking Technical Architecture

## Overview

The MediaTek MT8168 SoC contains four ARM Cortex-A53 cores. The stock Casper firmware configures a maximum operating frequency of **2.001 GHz (`2001000 kHz`)** at **1025 mV (`1025000 uV`)**.

This kernel branch (`android16-axionos-compat`, based on `oc-2.1ghz-experimental`) implements a stable **2.100 GHz (`2100000 kHz`)** overclock across all four cores while maintaining the safe stock ceiling voltage (1025 mV).

---

## 1. Operating Performance Points (OPP) Table

The active DVFS table in `arch/arm64/boot/dts/mediatek/mt8168.dtsi` spans 17 frequencies:

| Step | Frequency (kHz) | Frequency (MHz/GHz) | Voltage (mV) | Note |
| :---: | :---: | :---: | :---: | :--- |
| 1 | `850000` | 850 MHz | 675 mV | Idle lowest state |
| 2 | `918000` | 918 MHz | 675 mV | |
| 3 | `987000` | 987 MHz | 700 mV | |
| 4 | `1056000` | 1056 MHz | 700 mV | |
| 5 | `1125000` | 1125 MHz | 725 mV | |
| 6 | `1216000` | 1216 MHz | 725 mV | |
| 7 | `1308000` | 1308 MHz | 750 mV | |
| 8 | `1400000` | 1400 MHz | 775 mV | |
| 9 | `1466000` | 1466 MHz | 800 mV | |
| 10 | `1533000` | 1533 MHz | 825 mV | |
| 11 | `1633000` | 1633 MHz | 850 mV | |
| 12 | `1700000` | 1700 MHz | 875 mV | |
| 13 | `1767000` | 1767 MHz | 900 mV | |
| 14 | `1834000` | 1834 MHz | 925 mV | |
| 15 | `1917000` | 1917 MHz | 950 mV | |
| 16 | `2001000` | 2001 MHz | 1025 mV | Stock maximum |
| **17** | **`2100000`** | **2.100 GHz** | **1025 mV** | **Overclock State** |

---

## 2. Kernel Code Modifications

### 1. Device Tree Node (`arch/arm64/boot/dts/mediatek/mt8168.dtsi`)
```dts
opp-2100000000 {
    opp-hz = /bits/ 64 <2100000000>;
    opp-microvolt = <1025000>;
};
```

### 2. CPUFreq Driver Registration (`drivers/misc/mediatek/base/power/mt8168/mt8168-cpufreq.c`)
```c
/* Overclock: Add 2.100 GHz (2100000000 Hz) OPP with stock max safe voltage 1025000 uV */
ret = dev_pm_opp_add(info->cpu_dev, 2100000000ul, 1025000ul);
if (ret && ret != -EEXIST)
    pr_warn("%s: failed to add 2.100 GHz OPP: %d\n", __func__, ret);
else
    pr_info("%s: 2.100 GHz (2100000 kHz) OPP registered at 1025 mV\n", __func__);
```

### 3. Thermal Power Throttle Buffer Expansion (`drivers/misc/mediatek/base/power/mt8168/mtk_power_throttle.c`)
Because stock MediaTek code statically allocated an array of size 16 (`NR_MAX_OPP_TBL 16`), adding a 17th frequency would result in an out-of-bounds memory corruption. This was expanded to 32:
```c
#define NR_MAX_OPP_TBL  32
```
And the dynamic table size initialization:
```c
static void init_mt_cpu_dvfs(struct mt_cpu_dvfs *p)
{
    int i;
    p->opp_tbl = opp_tbl_default;
    p->nr_opp_tbl = 0;
    for (i = 0; i < ARRAY_SIZE(opp_tbl_default); i++) {
        if (opp_tbl_default[i].cpufreq_khz != 0)
            p->nr_opp_tbl++;
        else
            break;
    }
}
```

---

## 3. Verification Under Android 16
On the live system, the frequencies are dynamically adjusted by the Linux schedutil governor:
```bash
adb shell "cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies"
# Output: 850000 918000 987000 ... 2001000 2100000

adb shell "cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
# Output: 2100000
```
