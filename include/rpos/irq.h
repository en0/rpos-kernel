#ifndef _RPOS_IRQ_H
#define _RPOS_IRQ_H 1

typedef struct IrqChip {
    // The name of the chip.
    const char *name;

    // Send ack to hardware
    void(*irq_ack)(unsigned int num);

    // Disable an interrupt (masking)
    void(*irq_disable)(unsigned int num);

    // Enable an interrupt (unmasking)
    void(*irq_enable)(unsigned int num);

    // Stop all interrupts (cli)
    void(*irq_suspend)();

    // Enable all interrupts (sti)
    void(*irq_resume)();
} IrqChip_t;

#endif /* ! _RPOS_IRQ_H */
