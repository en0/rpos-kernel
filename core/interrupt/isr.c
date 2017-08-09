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

#include <string.h>
#include <stddef.h>
#include <rpos/cpu.h>
#include <rpos/syscall.h>
#include <rpos/dbglog.h>


#include "isr.h"
#include "idt.h"

void __isr_exit(regs_t* regs);
void __isr_fork(regs_t* regs);
void __isr_read(regs_t* regs);
void __isr_write(regs_t* regs);
void __isr_open(regs_t* regs);
void __isr_close(regs_t* regs);
void __isr_wait(regs_t* regs);
void __isr_link(regs_t* regs);
void __isr_unlink(regs_t* regs);
void __isr_execve(regs_t* regs);
void __isr_lseek(regs_t* regs);
void __isr_getpid(regs_t* regs);
void __isr_kill(regs_t* regs);
void __isr_times(regs_t* regs);
void __isr_sbrk(regs_t* regs);
void __isr_gettimeofday(regs_t* regs);
void __isr_stat(regs_t* regs);
void __isr_fstat(regs_t* regs);
void __isr_isatty(regs_t* regs);
void __isr_unhandled(regs_t* regs);


void(*g_isr_vectors[])(regs_t*) = {
    &__isr_unhandled,       // Noexist
    &__isr_exit,            // 0x80:1
    &__isr_fork,            // 0x80:2
    &__isr_read,            // 0x80:3
    &__isr_write,           // 0x80:4
    &__isr_open,            // 0x80:5
    &__isr_close,           // 0x80:6
    &__isr_wait,            // 0x80:7
    &__isr_link,            // 0x80:8
    &__isr_unlink,          // 0x80:9
    &__isr_execve,          // 0x80:10
    &__isr_lseek,           // 0x80:11
    &__isr_getpid,          // 0x80:12
    &__isr_kill,            // 0x80:13
    &__isr_times,           // 0x80:14
    &__isr_sbrk,            // 0x80:15
    &__isr_gettimeofday,    // 0x80:16
    &__isr_stat,            // 0x80:17
    &__isr_fstat,           // 0x80:18
    &__isr_isatty           // 0x80:19
};

uint16_t max_vector;

void isr_initialize_api() {
    max_vector = sizeof(g_isr_vectors) / sizeof(unsigned int);
    idt_setGate(0x80, &isr_stub128, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
}

void __isr_handler(regs_t *r) {

    if(r->eax < max_vector)
        g_isr_vectors[r->eax](r);
    else 
        __isr_unhandled(r);
}

void __isr_exit(regs_t* regs) {
    regs->eax = syscall_exit(regs->ebx);
}

void __isr_fork(regs_t* regs) {
    regs->eax = syscall_fork();
}

void __isr_read(regs_t* regs) {
    regs->eax = syscall_read(regs->ebx, (char*)regs->ecx, regs->edx);
}

void __isr_write(regs_t* regs) {
    regs->eax = syscall_write(regs->ebx, (char*)regs->ecx, regs->edx);
}

void __isr_open(regs_t* regs) {
    regs->eax = syscall_open((const char*)regs->ebx, regs->ecx, regs->edx);
}

void __isr_close(regs_t* regs) {
    regs->eax = syscall_close(regs->ebx);
}

void __isr_wait(regs_t* regs) {
    regs->eax = syscall_wait((int*)regs->ebx);
}

void __isr_link(regs_t* regs) {
    regs->eax = syscall_link((char*)regs->ebx, (char*)regs->ecx);
}

void __isr_unlink(regs_t* regs) {
    regs->eax = syscall_unlink((char*)regs->ebx);
}

void __isr_execve(regs_t* regs) {
    regs->eax = syscall_execve((char*)regs->ebx, (char**)regs->ecx, (char**)regs->edx);
}

void __isr_lseek(regs_t* regs) {
    regs->eax = syscall_lseek(regs->ebx, regs->ecx, regs->edx);
}

void __isr_getpid(regs_t* regs) {
    regs->eax = syscall_getpid();
}

void __isr_kill(regs_t* regs) {
    regs->eax = syscall_kill(regs->ebx, regs->ecx);
}

void __isr_times(regs_t* regs) {
    regs->eax = syscall_times((struct tms*)regs->ebx);
}

void __isr_sbrk(regs_t* regs) {
    regs->eax = syscall_sbrk(regs->ebx);
}

void __isr_gettimeofday(regs_t* regs) {
    regs->eax = syscall_gettimeofday((struct timeval*)regs->ebx, (void*)regs->ecx);
}

void __isr_stat(regs_t* regs) {
    regs->eax = syscall_stat((const char*)regs->ebx, (struct stat*)regs->ecx);
}

void __isr_fstat(regs_t* regs) {
    regs->eax = syscall_fstat(regs->ebx, (struct stat*)regs->ecx);
}

void __isr_isatty(regs_t* regs) {
    regs->eax = syscall_isatty(regs->ebx);
}

void __isr_unhandled(regs_t* regs) {
    regs->eax = syscall_unknown(regs);
}
