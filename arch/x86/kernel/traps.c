
void trap_init() {
    /*
     * This code will:
     *
     * Install all the fault handlers. -> handler in entry_32::fault\num
     * install the X86_INT80 handler -> handler in entry_32::isr_entry128
     *
     *

    set_system_intr_gate(X86_TRAP_PF, page_fault);
    set_system_intr_gate(0x80, entry_int80_32);
    load_idt(&idt_descr);
    */
}
