/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2011  Black Sphere Technologies Ltd.
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TARGET_JTAG_SCAN_H
#define TARGET_JTAG_SCAN_H

#include <stddef.h>
#include "jtagtap.h"

#define JTAG_MAX_DEVS	32
#define JTAG_MAX_IR_LEN	16

typedef struct jtag_dev_s {
	const char *jd_descr;
	uint32_t jd_idcode;
	uint32_t current_ir;

	union {
		uint8_t jd_dev;
		uint8_t dr_prescan;
	};
	uint8_t dr_postscan;

	uint8_t ir_len;
	uint8_t ir_prescan;
	uint8_t ir_postscan;
} jtag_dev_t;

extern struct jtag_dev_s jtag_devs[JTAG_MAX_DEVS + 1];
extern uint32_t jtag_dev_count;

void jtag_dev_write_ir(jtag_proc_t *jp, uint8_t jd_index, uint32_t ir);
void jtag_dev_shift_dr(jtag_proc_t *jp, uint8_t jd_index, uint8_t *dout, const uint8_t *din, size_t ticks);
void jtag_add_device(uint32_t dev_index, const jtag_dev_t *jtag_dev);

#endif /* TARGET_JTAG_SCAN_H */
