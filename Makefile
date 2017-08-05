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
EM=qemu-system-i386 -serial stdio -append "shell=/bin/sh"

.PHONEY : all run debug clean $(CLEAN_OBJS)

CONF=$(patsubst %.h.in, include/%.h, $(wildcard *.in))

OBJS=$(patsubst %.c, %.c.o, $(shell find . -type f -iname '*.c')) \
	 $(patsubst %.s, %.s.o, $(shell find . -type f -iname '*.s'))

CLEAN_OBJS=$(OBJS:%=clean-%) $(CONF:%=clean-%) $(OUT:%=clean-%)

ifeq ($(PROFILE), DEBUG)
K_CFLAGS:=$(K_CFLAGS) -ggdb
K_ASFLAGS:=$(K_ASFLAGS) -ggdb
endif

all : $(OUT)

include/%.h : %.h.in
	@echo \ - ENV $@
	@envsubst > $@ < $<

$(OUT) : $(OBJS)
	@echo \ - LD $@
	@$(K_LD) $(K_LDFLAGS) -Tlinker.ld -o $@ $^ -lc

%.c.o : %.c
	@echo \ - CC $@
	@$(K_CC) $(K_CFLAGS) -Iinclude -o $@ -c $<

%.s.o : %.s
	@echo \ - AS $@
	@$(K_AS) $(K_ASFLAGS) -o $@ $<

$(OBJS) : $(CONF)

clean : $(CLEAN_OBJS)

$(CLEAN_OBJS) :
	@echo \ - RM $(@:clean-%=%)
	@$(RM) $(@:clean-%=%)

run : $(OUT)
	$(EM) $(EM_FLAGS) -kernel $<

debug : $(OUT)
	$(EM) $(EM_FLAGS) -s -S -kernel $<
