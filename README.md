# RPOS Kernel

The Research Project Operating System.

## Synopsis

RPOS is my attempt at a i686 kernel. It is being designed to work with Red
Hat's [Newlib]() and support basic, single execution program from a cpio file
system.  Multitasking might be added at some point but it is currently not my
focus.

## Features

- [X] Higher half loading
- [X] 4Kb physical frame allocation
- [X] 4Kb pageing
- [X] 4Kb virtual frame allocation
- [X] RTC implemented using the 8259 PIC
- [X] COM1 Serial logging (Debug output)
- [ ] Virtual File System using CPIO initfs
- [ ] Basic newlib system request handling
- [ ] Application loaded by kernel after system is up.
- [ ] Ability to execute another program using the shell from the VirtFS
