/* Copyright (C) www.lightembedded.com
 * 2019 - xianlee.wu xianleewu@163.com
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef __23K256_H__
#define __23K256_H__

#include "stdint.h"

void ram_mode_set(int mode);

void ram_write(uint16_t addr, uint8_t data);

uint8_t ram_read(uint16_t addr);

void ram_write_loop(uint16_t addr, uint8_t data, uint16_t length);

void ram_write_array(uint16_t addr, uint8_t* data, uint16_t length);

void ram_read_array(uint16_t addr, uint8_t* data, uint16_t length);

void ram_write16(uint16_t addr, uint16_t data);

void ram_write32(uint16_t addr, uint32_t data);

uint16_t ram_read16(uint16_t addr);

uint32_t ram_read32(uint16_t addr);

#endif  /*__23K256_H__*/
