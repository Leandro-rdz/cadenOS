/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>
void userland() {
   asm volatile ("mov %eax, %cr0");
}

void tp() {
   gdt_reg_t gdtr;
   seg_desc_t my_gdt[6] ;
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
   my_gdt[2].type = SEG_DESC_DATA_RWA ;
   my_gdt[2].s = 1;
   my_gdt[2].dpl = 0;
   my_gdt[2].p = 1;
   my_gdt[2].avl = 0;
   my_gdt[2].l = 0;
   my_gdt[2].d = 1;
   my_gdt[2].g = 1;

   my_gdt[3].limit_1 = 0x1f;
   my_gdt[3].limit_2 = 0X0;
   my_gdt[3].base_1 = 0x0000;
   my_gdt[3].base_2 = 0x60;
   my_gdt[3].base_3 = 0;
   my_gdt[3].type =  SEG_DESC_DATA_RWA ;
   my_gdt[3].s = 1;
   my_gdt[3].dpl = 0;
   my_gdt[3].p = 1;
   my_gdt[3].avl = 1;
   my_gdt[3].l = 0;
   my_gdt[3].d = 1;
   my_gdt[3].g = 0;

   my_gdt[4].limit_1 = 0xffff;
   my_gdt[4].limit_2 = 0Xf;
   my_gdt[4].base_1 = 0x0000;
   my_gdt[4].base_2 = 0x00;
   my_gdt[4].base_3 = 0;
   my_gdt[4].type =  SEG_DESC_CODE_XRA ;
   my_gdt[4].s = 1;
   my_gdt[4].dpl = 3;
   my_gdt[4].p = 1;
   my_gdt[4].avl = 0;
   my_gdt[4].l = 0;
   my_gdt[4].d = 1;
   my_gdt[4].g = 1;


   my_gdt[5].limit_1 = 0xffff;
   my_gdt[5].limit_2 = 0Xf;
   my_gdt[5].base_1 = 0x0000;
   my_gdt[5].base_2 = 0x00;
   my_gdt[5].base_3 = 0;
   my_gdt[5].type =  SEG_DESC_DATA_RWA ;
   my_gdt[5].s = 1;
   my_gdt[5].dpl = 3;
   my_gdt[5].p = 1;
   my_gdt[5].avl = 0; 
   my_gdt[5].l = 0;
   my_gdt[5].d = 1;
   my_gdt[5].g = 1;

   gdtr.limit = sizeof(my_gdt) - 1;
   gdtr.desc  = my_gdt;
   set_gdtr(gdtr);
   tss_t tss;
   
   uint16_t sel_user_code = (uint16_t)gdt_usr_seg_sel(4); 
   uint16_t sel_user_data = (uint16_t)gdt_usr_seg_sel(5); 

   tss.s0.esp = get_ebp();
   tss.s0.ss = gdt_krn_seg_sel(3);

   my_gdt[3].base_1 = ((uint32_t)&tss) & 0xFFFF;
   my_gdt[3].base_2 = (((uint32_t)&tss) >> 16) & 0xFF;
   my_gdt[3].base_3 = (((uint32_t)&tss) >> 24) & 0xFF;
   my_gdt[3].type   = SEG_DESC_SYS_TSS_AVL_32;
   my_gdt[3].s      = 0;   // système
   my_gdt[3].dpl    = 0;   // ring 0
   my_gdt[3].p      = 1;

   
   set_tr(gdt_krn_seg_sel(3));
    asm volatile("push %0\n\t"
                  "push %%ebp\n\t"
                  "pushf \n\t"
                  "push %1\n\t"
                  "push %2\n\t"
                  "iret"
                  :
                  :"r"(sel_user_data),"r"(sel_user_code),"r"(userland)
               );

}
