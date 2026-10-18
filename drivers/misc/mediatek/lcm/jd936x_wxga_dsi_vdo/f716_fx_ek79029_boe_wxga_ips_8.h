// SPDX-License-Identifier: GPL-2.0
/*
 * f716_fx_ek79029_boe_wxga_ips_8.h
 * Reconstructed from Casper VIA S48 stock kernel Image (0xffffff800873ffbc)
 * Display: 8.0" WXGA (800x1280) IPS, BOE panel with EK79029 driver IC
 */

#ifndef _F716_FX_EK79029_BOE_WXGA_IPS_8_H_
#define _F716_FX_EK79029_BOE_WXGA_IPS_8_H_

static void init_f716_ek79029_lcm(void)
{
	unsigned int data_array[16];

	pr_info("Kernel/LCM: %s() enter\n", "init_lcm_registers");

	/* Enable register access password */
	data_array[0] = 0x00023902;
	data_array[1] = 0x0000aacd;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000030;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000133;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000032;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00004136;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000003a;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008267;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002769;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000016d;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001668;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00093902;
	data_array[1] = 0x000f0055;
	data_array[2] = 0x000f000f;
	data_array[3] = 0x0000000f;
	dsi_set_cmdq(data_array, 4, 1);
	MDELAY(2);
	MDELAY(2);

	data_array[0] = 0x00113902;
	data_array[1] = 0x000f0056;
	data_array[2] = 0x000f000f;
	data_array[3] = 0x000f000f;
	data_array[4] = 0x000f000f;
	data_array[5] = 0x0000000f;
	dsi_set_cmdq(data_array, 6, 1);
	MDELAY(2);
	MDELAY(2);

	data_array[0] = 0x00033902;
	data_array[1] = 0x00800072;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00033902;
	data_array[1] = 0x00102073;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000035e;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00005e41;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000a461;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000387e;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001074;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000203f;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001447;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00006648;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000504f;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00004f4e;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001139;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001060;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000d050;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00003476;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000807c;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000042e;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	/* Positive gamma curve (register 0x53, 19 parameters) */
	data_array[0] = 0x00143902;
	data_array[1] = 0x181a1f53;
	data_array[2] = 0x16141414;
	data_array[3] = 0x10131718;
	data_array[4] = 0x0b0e0e0f;
	data_array[5] = 0x01030609;
	dsi_set_cmdq(data_array, 6, 1);
	MDELAY(2);

	/* Negative gamma curve (register 0x54, 19 parameters) */
	data_array[0] = 0x00143902;
	data_array[1] = 0x181a1f54;
	data_array[2] = 0x16141414;
	data_array[3] = 0x12151918;
	data_array[4] = 0x0b0e0e0f;
	data_array[5] = 0x01030609;
	dsi_set_cmdq(data_array, 6, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002a5f;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000463;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00003128;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000029;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000fc34;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000312d;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00006778;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000c41d;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000f41d;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(2);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000004d;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(20);
	MDELAY(50);
}

#endif /* _F716_FX_EK79029_BOE_WXGA_IPS_8_H_ */
