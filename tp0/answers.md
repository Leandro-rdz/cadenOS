**Q1 : A l'aide du [linker.lds](../utils/linker.lds) et de [entry.s](../kernel/core/entry.s), 
déterminer la taille de la section `.mbh`  et `.stack` et en déduire d'où
provient la valeur de point d'entrée à `0x302010` au lieu de `0x300000`.**

En lisant [linker.lds](../utils/linker.lds), on voit ce mapage d'adresse : 

```
   . = 0x300000;
   .mbh      : { KEEP(*(.mbh)) . = ALIGN(4);     } : phboot
   .stack    : { KEEP(*(.stack))                 } : phstack

   __kernel_start__ = .;
```

On voit donc que on commence à adresser à 0x300000 mais avant le start on voit qu'il y a un `.mbh` (multiboot header)  qui est aligné sur et le `.stack` (pour la pile). Si on regarde ces sections en details : 


Dans [start.c](../kernel/core/start.c), on voit que le tableau de `.mbh` contient 3 entrées, chacun étant sur 4 octets donc on a besoin de 12 octets. Après cette section, nous sommes à l'adresse `0x30000c`

```c
volatile const uint32_t __mbh__ mbh[] = {
   MBH_MAGIC,
   MBH_FLAGS,
   (uint32_t)-(MBH_MAGIC+MBH_FLAGS),
};
```

Enfin dans [entry.s](../kernel/core/entry.s) on voit que l'on a les lignes suivantes pour la pile.

```
.section .stack, "aw", @nobits
.align 16
.space 0x2000
```

On voit donc que l'on alloue de l'espace pour une pile avec une taille de 0x2000 donc 8192 octets ou 8 Kio . De plus on s'aligne sur 16 bits donc on finit à `0x302010`.
