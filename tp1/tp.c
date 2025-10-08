/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>

void userland() {
   asm volatile ("mov %eax, %cr0");
}

void print_gdt_content(gdt_reg_t gdtr_ptr) {
    seg_desc_t* gdt_ptr;
    gdt_ptr = (seg_desc_t*)(gdtr_ptr.addr);
    int i=0;
    while ((uint32_t)gdt_ptr < ((gdtr_ptr.addr) + gdtr_ptr.limit)) {
        uint32_t start = gdt_ptr->base_3<<24 | gdt_ptr->base_2<<16 | gdt_ptr->base_1;
        uint32_t end;
        if (gdt_ptr->g) {
            end = start + ( (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1) <<12) + 4095;
        } else {
            end = start + (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1);
        }
        debug("%d ", i);
        debug("[0x%x ", start);
        debug("- 0x%x] ", end);
        debug("seg_t: 0x%x ", gdt_ptr->type);
        debug("desc_t: %d ", gdt_ptr->s);
        debug("priv: %d ", gdt_ptr->dpl);
        debug("present: %d ", gdt_ptr->p);
        debug("avl: %d ", gdt_ptr->avl);
        debug("longmode: %d ", gdt_ptr->l);
        debug("default: %d ", gdt_ptr->d);
        debug("gran: %d ", gdt_ptr->g);
        debug("\n");
        gdt_ptr++;
        i++;
    }
}


void tp() {
	// TODO
    gdt_reg_t gdtr;
    get_gdtr(gdtr);
    print_gdt_content(gdtr);

    debug("Displaying segments :\n");
    debug(" CS :  0x%x\n", get_cs());
    debug(" DS :  0x%x\n", get_ds());
    debug(" SS :  0x%x\n", get_ss());
    debug(" ES :  0x%x\n", get_es());
    debug(" FS :  0x%x\n", get_fs());
    debug(" GS :  0x%x\n", get_gs());
   
    
    seg_desc_t my_gdt[7] ;
    my_gdt[0] = (seg_desc_t){0};
    my_gdt[1].limit_1 = 0xffff;
    my_gdt[1].limit_2 = 0Xf;
    my_gdt[1].base_1 = 0;
    my_gdt[1].base_2 = 0;
    my_gdt[1].base_3 = 0;
    my_gdt[1].type =  SEG_DESC_CODE_XRA ;
    my_gdt[1].s = 1;
    my_gdt[1].dpl = 0;
    my_gdt[1].p = 1;
    my_gdt[1].avl = 0;
    my_gdt[1].l = 0;
    my_gdt[1].d = 1;
    my_gdt[1].g = 1; 


    my_gdt[2].limit_1 = 0xffff;
    my_gdt[2].limit_2 = 0Xf;
    my_gdt[2].base_1 = 0;
    my_gdt[2].base_2 = 0;
    my_gdt[2].base_3 = 0;
    my_gdt[2].type =  SEG_DESC_DATA_RWA ;
    my_gdt[2].s = 1;
    my_gdt[2].dpl = 0;
    my_gdt[2].p = 1;
    my_gdt[2].avl = 0;
    my_gdt[2].l = 0;
    my_gdt[2].d = 1;
    my_gdt[2].g = 1;

    gdt_reg_t my_gdtr ;
    my_gdtr.addr = (long unsigned int)my_gdt;
    my_gdtr.limit = sizeof(my_gdt)-1;
    set_seg_sel(1,cs);
    set_ds(2);
    set_gdtr(my_gdtr);
    get_gdtr(my_gdtr);
    print_gdt_content(my_gdtr);




}
