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

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "23k256.h"

#define SPI_RAM32_READ	0x03
#define SPI_RAM32_WRITE 0x02

#define READ_STAT_REG 	0x05
#define WRITE_STAT_REG 	0x01

#define MODE_BYTE 		0x00
#define MODE_SEQ 		0x40
#define MODE_PAGE 		0x80

/****** user define space *********/
#define RAM_CS_LOW
#define RAM_CS_HIGH

uint8_t ram_spi_write(uint8_t data)
{

    return 0;
}

uint8_t ram_spi_write16(uint16_t data)
{
    ram_spi_write(data > 8);
    return ram_spi_write(data | 0x0F);
}

/**** end of user define space ****/

void ram_mode_set(int mode)
{
    RAM_CS_LOW;
    ram_spi_write(WRITE_STAT_REG);
    ram_spi_write(mode);
    RAM_CS_HIGH;
}

void ram_write(uint16_t addr, uint8_t data)
{
    RAM_CS_LOW;
    ram_spi_write(WRITE_STAT_REG);
    ram_spi_write16(addr);
    ram_spi_write(data);
    RAM_CS_HIGH;
}

uint8_t ram_read(uint16_t addr)
{
    uint8_t data = 0;

    RAM_CS_LOW;
    ram_spi_write(WRITE_STAT_REG);
    ram_spi_write16(addr);
    data = ram_spi_write(0);
    RAM_CS_HIGH;

    return data;
}

void ram_write_loop(uint16_t addr, uint8_t data, uint16_t length)
{
    int i = 0;

    ram_mode_set(MODE_SEQ);
    RAM_CS_LOW;
    ram_spi_write(SPI_RAM32_WRITE);
    ram_spi_write16(addr);
    for (i=0;i<length;i++) {
        ram_spi_write(data);
    }
    RAM_CS_HIGH;
}

void ram_write_array(uint16_t addr, uint8_t* data, uint16_t length)
{
    int i = 0;

    ram_mode_set(MODE_SEQ);
    RAM_CS_LOW;
    ram_spi_write(SPI_RAM32_WRITE);
    ram_spi_write16(addr);
    for (i=0;i<length;i++) {
        ram_spi_write(data[i]);
    }
    RAM_CS_HIGH;

}

void ram_read_array(uint16_t addr, uint8_t* data, uint16_t length)
{
    int i = 0;

    ram_mode_set(MODE_SEQ);
    RAM_CS_LOW;
    ram_spi_write(SPI_RAM32_READ);
    ram_spi_write16(addr);
    for (i=0;i<length;i++) {
        data[i] = ram_spi_write(0);
    }
    RAM_CS_HIGH;
}

void ram_write16(uint16_t addr, uint16_t data)
{
    ram_write_array(addr, (uint8_t*)&data, 2);
}

void ram_write32(uint16_t addr, uint32_t data)
{
    ram_write_array(addr, (uint8_t*)&data, 4);
}

uint16_t ram_read16(uint16_t addr)
{
    uint16_t data = 0;

    ram_read_array(addr, (uint8_t*)&data, 2);

    return data;
}

uint32_t ram_read32(uint16_t addr)
{
    uint32_t data = 0;

    ram_read_array(addr, (uint8_t*)&data, addr);

    return data;
}

