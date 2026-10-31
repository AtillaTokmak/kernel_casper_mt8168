# Kernel Configuration Audit: MediaTek MT8168 for Android 16

## Target Board: Casper VIA S48 (`F716`)
- **Defconfig Path:** `arch/arm64/configs/tb8168p1_64_bsp_f716_defconfig`
- **Baseline Architecture:** ARM64 (aarch64)
- **Linux Kernel Release:** 4.14.141

---

## 1. Verified Key Configuration Directives

### Networking & Berkeley Packet Filter (BPF)
```config
CONFIG_BPF=y
CONFIG_BPF_SYSCALL=y
CONFIG_BPF_EVENTS=y
CONFIG_CGROUP_BPF=y
CONFIG_NETFILTER_XT_MATCH_BPF=y
CONFIG_NET_CLS_BPF=y
CONFIG_HAVE_EBPF_JIT=y
```
*Purpose:* Meets AOSP networking requirements for socket monitoring and eBPF syscall operations.

### Storage & Filesystem
```config
CONFIG_EXT4_FS=y
CONFIG_EXT4_FS_POSIX_ACL=y
CONFIG_EXT4_FS_SECURITY=y
CONFIG_FUSE_FS=y
CONFIG_DM_VERITY=y
CONFIG_BLK_DEV_DM=y
```
*Purpose:* Provides robust EXT4 support with POSIX ACLs. FUSE supports userspace storage emulation. FUSE-BPF (`CONFIG_FUSE_BPF`) is absent in 4.14; userspace override `persist.sys.fuse.bpf.override=false` is utilized instead of backporting.

### SELinux & Security
```config
CONFIG_SECURITY_SELINUX=y
CONFIG_SECURITY_SELINUX_BOOTPARAM=y
CONFIG_SECURITY_SELINUX_BOOTPARAM_VALUE=1
CONFIG_SECURITY_SELINUX_DEVELOP=y
CONFIG_SECURITY_SELINUX_AVC_STATS=y
CONFIG_DEFAULT_SECURITY_SELINUX=y
```
*Purpose:* Allows development control via cmdline (`androidboot.selinux=permissive`). Tested functional in both Enforcing and Permissive states on Android 16.

### Power & CPU Frequency
```config
CONFIG_CPU_FREQ=y
CONFIG_CPU_FREQ_GOV_PERFORMANCE=y
CONFIG_CPU_FREQ_GOV_POWERSAVE=y
CONFIG_CPU_FREQ_GOV_USERSPACE=y
CONFIG_CPU_FREQ_GOV_ONDEMAND=y
CONFIG_CPU_FREQ_GOV_CONSERVATIVE=y
CONFIG_CPU_FREQ_GOV_SCHEDUTIL=y
```
*Purpose:* Schedutil governor active with DVFS operating across 17 operating performance points (850 MHz to 2.100 GHz).
