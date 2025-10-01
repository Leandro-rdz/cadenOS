/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("kernel mem [0x%p - 0x%p]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   multiboot_memory_map_t* entry = (multiboot_memory_map_t*)info->mbi->mmap_addr;
   while((uint32_t)entry < (info->mbi->mmap_addr + info->mbi->mmap_length)) {
      // TODO print "[start - end] type" for each entry
      switch (entry->type)
      {
      case 1:
         debug("[0x%llx - 0x%llx] MULTIBOOT_MEMORY_AVAILABLE\n",entry->addr,entry->addr+entry->len);
         break;
      case 2: 
         debug("[0x%llx - 0x%llx] MULTIBOOT_MEMORY_RESERVED\n",entry->addr,entry->addr+entry->len);
         break;
      case 3:
         debug("[0x%llx - 0x%llx] MULTIBOOT_MEMORY_ACPI_RECLAIMABLE\n",entry->addr,entry->addr+entry->len);
         break;
      case 4:
         debug("[0x%llx - 0x%llx] MULTIBOOT_MEMORY_NVS\n",entry->addr,entry->addr+entry->len);
         break;
      default:
         break;
      }
      entry++;
   }

}
