/**
 ** Copyright (c) 2017 "Ian Laird"
 ** Research Project Operating System (rpos) - https://github.com/en0/rpos
 ** 
 ** This file is part of rpos
 ** 
 ** rpos is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 ** 
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 ** 
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef _x86_CPU_IO_H
#define _x86_CPU_IO_H

#include <stdint.h>

static inline void outb(uint8_t val, uint16_t port) {
    asm volatile ( "out %0, %1;"
                 : /* no return */
                 : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    asm volatile ( "in %1, %0"
                 : "=a"(val)
                 : "Nd"(port));
    return val;
}

static inline void io_wait() {
	asm volatile ( "outb %%al,%0" : : "dN"(0x80));
}

#endif /* ! _x86_CPU_IO_H */
