

**Q1 : Rappeler comment fonctionne l'instruction `SGDT` (rôle, paramètres, etc.), puis trouver dans [`kernel/include/segmem.h`](https://file+.vscode-resource.vscode-cdn.net/home/leandro/Documents/TLS-SEC/TLS-SEC/OS/cadenOS/kernel/include/segmem.h) la macro qui l'utilise pour afficher l'adresse de base de la GDT en cours d'utilisation ainsi que sa "limite" (type utile : `gdt_reg_t`).**


La commande `SGDT x` => store la GDTR (Global description table register) à l'adresse x. Cela permet donc de recupérer le GDTR dans X  et de pouvoir le lire plus tard

Dans  [`kernel/include/segmem.h`](https://file+.vscode-resource.vscode-cdn.net/home/leandro/Documents/TLS-SEC/TLS-SEC/OS/cadenOS/kernel/include/segmem.h) on a : 

```c
#define get_gdtr(aLocation) \
asm volatile ("sgdt %0"::"m"(aLocation):"memory")
```

On  a ici `get_gdtr` qui fait la commande sgdt

de plus on a 
```c
typedef struct global_descriptor_table_register
{
   uint16_t            limit;           /* dt limit = size - 1 */ 
   union                                /* base address */       
   {
      offset_t         addr;                                     
      seg_desc_t       *desc;                                    
   };
} __attribute__((packed)) gdt_reg_t;
```

![[Pasted image 20251008115536.png]]
On a limit qui est  sur 16 bits ,  donc 64kb

**Q2* : Dans [`tp.c`](https://file+.vscode-resource.vscode-cdn.net/home/leandro/Documents/TLS-SEC/TLS-SEC/OS/cadenOS/tp1/tp.c), un exemple d'implémentation d'affichage du contenu de table de type GDT est fournie (fonction `print_gdt_content`). L'utiliser pour afficher le contenu de la GDT courante.**

On a juste fait: 

```c
void tp() {
    // TODO
    gdt_reg_t test;
    get_gdtr(test);
    print_gdt_content(test);
}
```


En l'executant on a 

```bash
0 [0x0 - 0xfff0] seg_t: 0x0 desc_t: 0 priv: 0 present: 0 avl: 0 longmode: 0 default: 0 gran: 0 
1 [0x0 - 0xffffffff] seg_t: 0xb desc_t: 1 priv: 0 present: 1 avl: 0 longmode: 0 default: 1 gran: 1 
2 [0x0 - 0xffffffff] seg_t: 0x3 desc_t: 1 priv: 0 present: 1 avl: 0 longmode: 0 default: 1 gran: 1 
3 [0x0 - 0xffff] seg_t: 0xf desc_t: 1 priv: 0 present: 1 avl: 0 longmode: 0 default: 0 gran: 0 
4 [0x0 - 0xffff] seg_t: 0x3 desc_t: 1 priv: 0 present: 1 avl: 0 longmode: 0 default: 0 gran: 0 
```





**Q3 : Lire les valeurs des sélecteurs de segment à l'aide des macros prévues à cet effet dans [`kernel/include/segmem.h`](https://file+.vscode-resource.vscode-cdn.net/home/leandro/Documents/TLS-SEC/TLS-SEC/OS/cadenOS/kernel/include/segmem.h), et en déduire quels descripteurs de cette GDT sont en cours d'utilisation pour :**

- Le segment de code (sélecteur cs)
- Le segment de données (sélecteur ds)
- Le segment de pile (sélecteur ss)
- D'autres segments (sélecteurs autres : es, fs, gs, etc.)

**Q4 : Que constate-t-on ? Que dire de la ségrégation mémoire mise en place par défaut par GRUB avec une telle configuration ?**