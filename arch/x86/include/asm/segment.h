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

#ifndef _x86_ASM_SEGMENT_H
#define _x86_ASM_SEGMENT_H 1

#define GDT_ENTRY_CS  1
#define GDT_ENTRY_DS  2
#define GDT_ENTRY_TSS 3
#define __CS  (GDT_ENTRY_CS*8)
#define __DS  (GDT_ENTRY_DS*8)
#define __TSS (GDT_ENTRY_TSS*8)

#define GDT_ENTRY(flags, base, limit) \
    ((((base)  & _AC(0xff000000,ULL)) << (56-24)) | \
     (((flags) & _AC(0x0000f0ff,ULL)) << 40) | \
     (((limit) & _AC(0x000f0000,ULL)) << (48-16)) | \
     (((base)  & _AC(0x00ffffff,ULL)) << 16) | \
     (((limit) & _AC(0x0000ffff,ULL))))

#define GDT_FLAGS(Pr, Privl, Ex, DC, RW, Gr, Sz) \
    (((Gr    & 1) << 15)| \
     ((Sz    & 1) << 14)| \
     ((Pr    & 1) << 7) | \
     ((Privl & 3) << 5) | \
     ((Ex    & 1) << 3) | \
     ((DC    & 1) << 2) | \
     ((RW    & 1) << 1) | \
     (1 << 4))

#ifndef __ASSEMBLER__

struct Gdt {
    uint16_t length;
    uint32_t table;
} __attribute__((packed));

#endif /* ! __ASSEMBLER__ */
#endif /* !_x86_ASM_SEGMENT_H */
