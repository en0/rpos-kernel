## Copyright (c) 2017 "Ian Laird"
## Research Project Operating System (rpos) - https://github.com/en0/rpos
## 
## This file is part of rpos
## 
## rpos is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

OUT=rpos-kernel
#EM=qemu-system-i386 -serial stdio -append "initfs=initfs.cpio init=/bin/sh" -initrd initfs.cpio
EM=qemu-system-i386 -serial stdio -append "initfs=initfs.cpio init=/bin/sh"
TARGET_ARCH=x86

ifeq ($(PROFILE), DEBUG)
K_CFLAGS:=$(K_CFLAGS) -ggdb
K_ASFLAGS:=$(K_ASFLAGS) -ggdb
endif

CONF=$(patsubst %.h.in, include/rpos/%.h, kernel.h.in)

OBJS=$(patsubst %, %.o, $(shell find . -type f -iname '*.c' -not -path './arch/*' -or -iname '*.S' -not -path './arch/*')) \
	 $(patsubst %, %.o, $(shell find ./arch/$(TARGET_ARCH)/ -type f -iname '*.c' -o -iname '*.S'))

CLEAN_OBJS=$(OBJS:%=clean-%) $(CONF:%=clean-%) $(OUT:%=clean-%)

.PHONEY : all run debug clean $(CLEAN_OBJS)

all : $(OUT)

include/rpos/%.h : %.h.in
	@echo \ - ENV $@
	@$(shell ./Config $< > $@)

$(OUT) : $(OBJS)
	@echo \ - LD $@
	@$(K_LD) $(K_LDFLAGS) -Tlinker.ld -o $@ $^

%.c.o : %.c
	@echo \ - CC $@
	@$(K_CC) $(K_CFLAGS) -Iinclude -o $@ -c $<

%.S.o : %.S
	@echo \ - AS $@
	@$(K_CC) $(K_CFLAGS) -Iinclude -o $@ -c $<

./arch/%.c.o : ./arch/%.c
	@echo \ - CC $@
	@$(K_CC) $(K_CFLAGS) -Iarch/$(TARGET_ARCH)/include -Iinclude -o $@ -c $<

./arch/%.S.o : ./arch/%.S
	@echo \ - AS $@
	@$(K_CC) $(K_CFLAGS) -Iarch/$(TARGET_ARCH)/include -Iinclude -o $@ -c $<

$(OBJS) : $(CONF)

clean : $(CLEAN_OBJS)

$(CLEAN_OBJS) :
	@echo \ - RM $(@:clean-%=%)
	@$(RM) $(@:clean-%=%)

run : $(OUT)
	$(EM) $(EM_FLAGS) -kernel $<

debug : $(OUT)
	$(EM) $(EM_FLAGS) -s -S -kernel $<
