
void irq_init() {
    // Install vector handler for all the hardware interrupts (0 - 15 - but i
    // am sure we will remap them to a diffrent range.
    // Speaking of which, we need to remap the PIC as well.
    // PIC is mapped through pic_i8259->init()
    //
    // Linux appears to do this at line 72 of arch/x86/kernel/irqinit.c
    // chip->init();
    // for(i = 0; i < irqs; i++)
    //   irq_set_chip_and_handler(i, chip, handler_level_irq);
    //
    // This handle_lavel_irq call is defined in /kernel/irq/chip.c
    // that dispatches a call in handler.c that gets a installed driver's
    // handler and executes it.
}
