/* GPLv2 (c) Airbus */
#include <debug.h>
#include <intr.h>

void bp_handler() {
   // TODO
}

void bp_trigger() {
	// TODO	
}

void print_idt_content(idt_reg_t idtr_ptr) {
    int_desc_t* idt_ptr;
    idt_ptr = (int_desc_t*)(idtr_ptr.addr);
    int i=0;
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
}

void tp() {
	// TODO print idtr
	idt_reg_t idtr;
	get_idtr(idtr);
	print_idt_content(idtr);
	// TODO call bp_trigger
   //bp_trigger();
}
