#!/usr/bin/env python3
import sys
import struct

def verify_boot_or_dtb(path):
    print(f"[*] Verifying integrity of: {path}")
    with open(path, 'rb') as f:
        data = f.read()

    # Check if this is an Android boot image
    if data[:8] == b'ANDROID!':
        if len(data) < 1660:
            print("[-] FAIL: Boot image header smaller than 1660 bytes.")
            return False
        page_sz = struct.unpack('<I', data[36:40])[0]
        hdr_ver = struct.unpack('<I', data[40:44])[0]
        if hdr_ver < 2:
            print(f"[-] Boot header version {hdr_ver} does not have separate DTB field.")
            return True
        kernel_sz = struct.unpack('<I', data[8:12])[0]
        ramdisk_sz = struct.unpack('<I', data[16:20])[0]
        dtb_sz = struct.unpack('<I', data[1648:1652])[0]

        def page_align(x, p): return (x + p - 1) & ~(p - 1)
        k_pages = page_align(kernel_sz, page_sz)
        r_pages = page_align(ramdisk_sz, page_sz)
        dtb_offset = page_sz + k_pages + r_pages

        if dtb_offset + dtb_sz > len(data):
            print(f"[-] FAIL: Boot image truncated before dtb section ended. File: {len(data)}, needed: {dtb_offset + dtb_sz}")
            return False
        dtb_data = data[dtb_offset:dtb_offset + dtb_sz]
        print(f"    [+] Boot Header: kernel_size={kernel_sz}, ramdisk_size={ramdisk_sz}, dtb_size={dtb_sz} at offset {dtb_offset}")
    else:
        dtb_data = data

    if len(dtb_data) < 32:
        print(f"[-] FAIL: DTB data is too short ({len(dtb_data)} bytes).")
        return False

    # Check DT Table Magic
    dt_magic_be = struct.unpack('>I', dtb_data[:4])[0]
    dt_magic_le = struct.unpack('<I', dtb_data[:4])[0]

    if dt_magic_be == 0xd7b7ab1e:
        table_total_sz, tbl_hdr_sz, entry_sz, entry_count, entries_off = struct.unpack('>IIIII', dtb_data[4:24])
        endian = '>'
    elif dt_magic_le == 0x1eabb7d7:
        table_total_sz, tbl_hdr_sz, entry_sz, entry_count, entries_off = struct.unpack('<IIIII', dtb_data[4:24])
        endian = '<'
    else:
        # Check raw FDT
        fdt_magic = struct.unpack('>I', dtb_data[:4])[0]
        if fdt_magic == 0xd00dfeed:
            fdt_totalsize = struct.unpack('>I', dtb_data[4:8])[0]
            if len(dtb_data) < fdt_totalsize:
                print(f"[-] FAIL: Raw FDT truncated! Available: {len(dtb_data)}, Header totalsize: {fdt_totalsize} (deficit: {fdt_totalsize - len(dtb_data)} bytes)")
                return False
            print(f"    [+] Raw FDT valid: {len(dtb_data)} bytes matches header totalsize {fdt_totalsize}.")
            return True
        print(f"[-] FAIL: Unknown DTB magic: 0x{dtb_data[:4].hex()}")
        return False

    print(f"    [+] DT Table Header: total_size={table_total_sz}, header_size={tbl_hdr_sz}, entry_size={entry_sz}, entry_count={entry_count}, entries_offset={entries_off}")

    if len(dtb_data) < table_total_sz:
        print(f"[-] FAIL: DT Table truncated! Available: {len(dtb_data)} bytes, Header total_size: {table_total_sz} bytes (deficit: {table_total_sz - len(dtb_data)} bytes).")
        return False

    # Check entries
    for i in range(entry_count):
        off = entries_off + (i * entry_sz)
        if off + entry_sz > len(dtb_data):
            print(f"[-] FAIL: DT entry {i} offset beyond table bounds.")
            return False
        e_dt_sz, e_dt_off = struct.unpack(f'{endian}II', dtb_data[off:off+8])
        if e_dt_off + e_dt_sz > len(dtb_data):
            print(f"[-] FAIL: DT entry {i} data (offset {e_dt_off}, size {e_dt_sz}) exceeds available DTB size {len(dtb_data)}.")
            return False
        entry_fdt = dtb_data[e_dt_off:e_dt_off+e_dt_sz]
        if len(entry_fdt) < 8:
            print(f"[-] FAIL: DT entry {i} FDT too small ({len(entry_fdt)} bytes).")
            return False
        fdt_magic = struct.unpack('>I', entry_fdt[:4])[0]
        if fdt_magic != 0xd00dfeed:
            print(f"[-] FAIL: DT entry {i} invalid FDT magic: 0x{fdt_magic:08x}")
            return False
        fdt_totalsize = struct.unpack('>I', entry_fdt[4:8])[0]
        if len(entry_fdt) < fdt_totalsize:
            print(f"[-] FAIL: DT entry {i} FDT truncated! Available: {len(entry_fdt)}, FDT header totalsize: {fdt_totalsize} (deficit: {fdt_totalsize - len(entry_fdt)} bytes).")
            return False
        print(f"    [+] Entry {i}: dt_offset={e_dt_off}, dt_size={e_dt_sz}, FDT totalsize={fdt_totalsize} -> OK")

    print("[+] PASS: DTB integrity fully verified!")
    return True

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: verify_dtb_integrity.py <boot.img or dtb file>")
        sys.exit(1)
    success = verify_boot_or_dtb(sys.argv[1])
    sys.exit(0 if success else 1)
