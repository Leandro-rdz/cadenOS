/* GPLv2 (c) Airbus */
#include <debug.h>
#include <intr.h>

// Q2
void bp_handler() {
    asm volatile ("pusha");         //Q8
    uint32_t val;
    asm volatile ("mov 4(%%ebp), %0":"=r"(val));
    debug("[Q7] ---------- EIP = %x\n", val);
    debug("TIE LE KERNEL OU QUOI\n");

    //Q9
    asm volatile ("popa");
    asm volatile ("leave; iret");
}

void bp_trigger() {
    // Q4
	asm volatile("int3");
	debug("en fait tié un tigre\n");
}

void print_idt_content(idt_reg_t idtr_ptr) {
    int_desc_t* idt_ptr;
    idt_ptr = (int_desc_t*)(idtr_ptr.addr);
    int i=0;
    debug("\n\n-------- PRINT IDT CONTENT\n");
    while ((uint32_t)idt_ptr < ((idtr_ptr.addr) + idtr_ptr.limit)) {
        uint32_t start = idtr_ptr.addr;
        uint32_t end = start + (idt_ptr ->offset_1 | idt_ptr->offset_2 <<16);
        debug("%d ", i);
        debug("[0x%x ", start);
        debug("- 0x%x] ", end);
        debug("idt_t: 0x%x ", idt_ptr->type);
        debug("sel: 0x%x ", idt_ptr->selector);
		debug("ist: 0x%x ", idt_ptr->ist);
		debug("priv: %d ", idt_ptr->dpl);
        debug("present: %d ", idt_ptr->p);
        debug("\n");
        idt_ptr++;
        i++;
    }
    debug("--------\n");
}

void tp() {
    // Q1
    idt_reg_t idtr;
    get_idtr(idtr);
    //print_idt_content(idtr);
    debug("IDT @ 0x%x\n", (unsigned int) idtr.addr);

    // Q3
    idtr.desc[3].offset_1 = (uint16_t)((uint32_t) bp_handler);
    idtr.desc[3].offset_2 = (uint16_t)((uint32_t) bp_handler >>16);

    // Q4
    bp_trigger();
    debug("en fait tié un crayon à papier\n");
}
