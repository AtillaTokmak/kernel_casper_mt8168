/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _MT_PLAT_ITEMS_H_
#define _MT_PLAT_ITEMS_H_

#ifdef __cplusplus
extern "C" {
#endif

int item_exist(const char *key);
int item_integer(const char *key, int default_val);
int item_string(char *val, const char *key, int max_len);

#ifdef __cplusplus
}
#endif

#endif /* _MT_PLAT_ITEMS_H_ */
