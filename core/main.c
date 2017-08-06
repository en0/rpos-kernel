#include <stdio.h>
#include <core/cpu.h>
#include <klib/dbglog.h>
#include <klib/bitmap.h>

#include "memory/vmem.h"

int main(int argc, char** argv) {

    /*
     * Kernel main
     */

    dbglogf("System started!\n");

    uint32_t* x = vmem_allocate_kernel_heap_page();
    *x = 123;

    return 0;
}

