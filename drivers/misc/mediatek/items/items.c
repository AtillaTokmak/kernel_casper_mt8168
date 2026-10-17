// SPDX-License-Identifier: GPL-2.0
/*
 * items.c - InfoTM / Casper hardware configuration items parser
 *
 * Reconstructed from Casper stock kernel binary strings and disassembly
 * at addresses 0x7f1e00-0x7f2900, 0x11d4157, 0x131b3b0-0x131b520.
 *
 * Exposes:
 *   int item_exist(const char *key)
 *   int item_integer(const char *key, int default_val)
 *   int item_string(char *val, const char *key, int max_len)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <mt-plat/items.h>

#define MAX_ITEMS 128
#define MAX_KEY_LEN 64
#define MAX_VAL_LEN 128

struct item_entry {
	char key[MAX_KEY_LEN];
	char val[MAX_VAL_LEN];
};

static struct item_entry items_table[MAX_ITEMS];
static int items_count = 0;
static DEFINE_MUTEX(items_lock);

/* Built-in fallback table matching Casper VIA S48 / F716 stock items.ini */
static const char * const default_items[] = {
	"device.model", "F716",
	"LCM", "f716_fx_ek79029_boe_wxga_ips_8",
	"sensor.accelerometer.bma.orientation", "5",
	"sensor.accelerometer.mpu.orientation", "6",
	"sensor.gyroscope.mpu.orientation", "6",
	"sensor.magnetometer.akm.orientation", "5",
	"touchpanel.gsl.modle", "F716_GSL1680_WXGA",
	"touchpanel.x.reverse", "1",
	"touchpanel.y.reverse", "1",
	"ro.vendor.camera.main.orientation", "90",
	"ro.vendor.camera.sub.orientation", "270",
	"mtk.camera.app.orientation.enable", "0",
	"ro.sf.lcd_density", "213",
	"uart.printk", "1",
	"ro.sf.hwrotation", "0",
	"ro.vendor.logo.hwrotation", "0",
	"ro.surface_flinger.primary_display_orientation", "0",
	"ro.vendor.audio.useExternAmp", "1",
	"ro.vendor.audio.two.speaker", "1",
	NULL, NULL
};

static void items_add_entry(const char *key, const char *val)
{
	int i;

	if (!key || !val || !*key)
		return;

	for (i = 0; i < items_count; i++) {
		if (!strcmp(items_table[i].key, key)) {
			strncpy(items_table[i].val, val, MAX_VAL_LEN - 1);
			items_table[i].val[MAX_VAL_LEN - 1] = '\0';
			return;
		}
	}

	if (items_count < MAX_ITEMS) {
		strncpy(items_table[items_count].key, key, MAX_KEY_LEN - 1);
		items_table[items_count].key[MAX_KEY_LEN - 1] = '\0';
		strncpy(items_table[items_count].val, val, MAX_VAL_LEN - 1);
		items_table[items_count].val[MAX_VAL_LEN - 1] = '\0';
		items_count++;
	}
}

static void items_parse_buffer(char *buf, size_t len)
{
	char *p, *line, *saveptr = NULL;
	bool in_items = false;

	p = buf;
	while ((line = strsep(&p, "\r\n")) != NULL) {
		char *k, *v;

		/* trim leading whitespace */
		while (*line == ' ' || *line == '\t')
			line++;

		/* comment or empty line */
		if (!*line || *line == '#')
			continue;

		if (!strcmp(line, "items.start")) {
			in_items = true;
			continue;
		}
		if (!strcmp(line, "items.end")) {
			in_items = false;
			break;
		}

		if (!in_items)
			continue;

		/* split key and value by whitespace */
		k = line;
		v = line;
		while (*v && *v != ' ' && *v != '\t')
			v++;
		if (!*v)
			continue;
		*v++ = '\0';
		while (*v == ' ' || *v == '\t')
			v++;

		/* remove trailing comments or whitespace */
		{
			char *end = v;
			while (*end && *end != '#')
				end++;
			*end = '\0';
			while (end > v && (*(end - 1) == ' ' || *(end - 1) == '\t')) {
				end--;
				*end = '\0';
			}
		}

		items_add_entry(k, v);
	}
}

int item_exist(const char *key)
{
	int i;

	if (!key)
		return 0;

	mutex_lock(&items_lock);
	for (i = 0; i < items_count; i++) {
		if (!strcmp(items_table[i].key, key)) {
			mutex_unlock(&items_lock);
			return 1;
		}
	}
	mutex_unlock(&items_lock);
	return 0;
}
EXPORT_SYMBOL(item_exist);

int item_integer(const char *key, int default_val)
{
	int i, val;

	if (!key)
		return default_val;

	mutex_lock(&items_lock);
	for (i = 0; i < items_count; i++) {
		if (!strcmp(items_table[i].key, key)) {
			if (kstrtoint(items_table[i].val, 0, &val) == 0) {
				mutex_unlock(&items_lock);
				return val;
			}
			break;
		}
	}
	mutex_unlock(&items_lock);
	return default_val;
}
EXPORT_SYMBOL(item_integer);

int item_string(char *val, const char *key, int max_len)
{
	int i;

	if (!val || !key || max_len <= 0)
		return -EINVAL;

	mutex_lock(&items_lock);
	for (i = 0; i < items_count; i++) {
		if (!strcmp(items_table[i].key, key)) {
			strncpy(val, items_table[i].val, max_len - 1);
			val[max_len - 1] = '\0';
			mutex_unlock(&items_lock);
			return 0;
		}
	}
	mutex_unlock(&items_lock);
	return -ENOENT;
}
EXPORT_SYMBOL(item_string);

static int items_proc_show(struct seq_file *m, void *v)
{
	int i;

	seq_puts(m, "items.start\n");
	mutex_lock(&items_lock);
	for (i = 0; i < items_count; i++) {
		seq_printf(m, "%-32s %s\n", items_table[i].key, items_table[i].val);
	}
	mutex_unlock(&items_lock);
	seq_puts(m, "items.end\n");
	return 0;
}

static int items_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, items_proc_show, NULL);
}

static const struct file_operations items_proc_fops = {
	.open = items_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init items_init(void)
{
	struct device_node *np;
	const __be32 *reg;
	int len;
	bool loaded = false;

	pr_info("items driver (c) 2009, 2014 InfoTM (Casper VIA S48 / F716)\n");

	/* Load default Casper F716 table first */
	{
		int i;
		for (i = 0; default_items[i] != NULL; i += 2) {
			items_add_entry(default_items[i], default_items[i + 1]);
		}
	}

	/* Look for items_reserved_mem in Device Tree */
	np = of_find_node_by_name(NULL, "items_reserved_mem");
	if (np) {
		reg = of_get_property(np, "reg", &len);
		if (reg) {
			int na = of_n_addr_cells(np);
			int ns = of_n_size_cells(np);

			if (len >= (na + ns) * (int)sizeof(u32)) {
				phys_addr_t base = of_read_number(reg, na);
				size_t size = of_read_number(reg + na, ns);
				void *va;

			pr_info("[PHY layout]items_reserved_mem   :   0x%08llx - 0x%08llx (0x%llx)\n",
				(u64)base, (u64)(base + size), (u64)size);

			if (size > 0 && size < 0x100000) {
				va = memremap(base, size, MEMREMAP_WB);
				if (va) {
					char *kbuf = kmalloc(size + 1, GFP_KERNEL);
					if (kbuf) {
						memcpy(kbuf, va, size);
						kbuf[size] = '\0';
						items_parse_buffer(kbuf, size);
						kfree(kbuf);
						loaded = true;
					}
					memunmap(va);
				}
			}
		}
		}
		of_node_put(np);
	}

	proc_create("items", 0444, NULL, &items_proc_fops);

	pr_info("items: initialized %d board properties (LK buffer %s)\n",
		items_count, loaded ? "loaded" : "not present, using F716 defaults");

	return 0;
}

core_initcall(items_init);

MODULE_DESCRIPTION("InfoTM / Casper Items Driver");
MODULE_LICENSE("GPL v2");
