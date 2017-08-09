#include <unistd.h>
#include <stdio.h>
#include <rpos/cpu.h>
#include <rpos/task.h>
#include <rpos/dbglog.h>
#include <rpos/bitmap.h>

int main(int argc, char** argv) {

    /*
     * Kernel main
     */

    task_initialize_kernel_task(argv[0], VIRT_ADDR_HEAP, VIRT_ADDR_EHEAP);

    dbglogf("System started: %s\n", task_active->name);
    printf("MSG: %s\n", "Hello, World!");

    return 0;
}

