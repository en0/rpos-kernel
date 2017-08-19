#include <stdint.h>
#include <rpos/log.h>
#include <rpos/init.h>
#include <rpos/irq.h>
#include <arch/io.h>
#include <arch/core.h>
#include <asm/i8259.h>

#define PIC0            0x20
#define PIC0_DATA       0x21
#define PIC1            0xA0
#define PIC1_DATA       0xA1

#define ICW1_ICW4       0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE     0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08		/* Level triggered (edge) mode */
#define ICW1_INIT       0x10		/* Initialization - required! */
 
#define ICW4_8086       0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C		/* Buffered mode/master */
#define ICW4_SFNM       0x10		/* Special fully nested (not) */

#define PIC_EOI         0x20

static int i8259_init();

static void remap(uint32_t offset_pic0, uint32_t offset_pic1) {

    outb(ICW1_INIT | ICW1_ICW4, PIC0); io_wait(); // Initialize pic0 with cascade mode
    outb(ICW1_INIT | ICW1_ICW4, PIC1); io_wait(); // Initialize pic1 with cascade mode

	outb(offset_pic0, PIC0_DATA); io_wait();      // ICW2: Master PIC vector offset
	outb(offset_pic1, PIC1_DATA); io_wait();      // ICW2: Slave PIC vector offset
	outb(4, PIC0_DATA); io_wait();                // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(2, PIC1_DATA); io_wait();                // ICW3: tell Slave PIC its cascade identity (0000 0010)
 
	outb(ICW4_8086, PIC0_DATA); io_wait();
	outb(ICW4_8086, PIC1_DATA); io_wait();
 
	outb(0xFF, PIC0_DATA); io_wait();             // Maskoff All Interrupts
	outb(0xFF, PIC1_DATA); io_wait();

}

static void ack_irq(uint32_t num) {
    if(num >= 8) 
        outb(PIC_EOI, PIC1);
    outb(PIC_EOI, PIC0);
}

static void mask_irq(uint32_t n) {
    uint16_t port;
    uint8_t val;

    if(n < 8) {
        port = PIC0_DATA;
    } else {
        port = PIC1_DATA;
        n -= 8;
    }

    val = inb(port) | (1<<n);
    outb(val, port);
}

static void unmask_irq(uint32_t n) {
    uint16_t port;
    uint8_t val;

    if(n < 8) {
        port = PIC0_DATA;
    } else {
        port = PIC1_DATA;
        n -= 8;
    }

    val = inb(port) & ~(1<<n);
    outb(val, port);
}

IrqChip_t i8259_chip = {
    .name = "Intel 8259A PIC",
    .irq_ack = ack_irq,
    .irq_disable = mask_irq,
    .irq_enable = unmask_irq,
    .irq_suspend = cli,
    .irq_resume = sti
};

PicI8259_t pic_i8259 = {
    .init = i8259_init,
    .chip = &i8259_chip
};

static int i8259_init() {
    // Remap PIC0 to 0x20-0x27 and PIC1 to 0x28-0x2F
    log.printf("=> x86_kernel :: Initializing HW => [%s]\n", i8259_chip.name);
    remap(0x20, 0x28);
    return 0;
}

device_initcall(i8259_init);
