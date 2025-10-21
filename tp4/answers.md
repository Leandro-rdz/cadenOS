**Q3\* : Modifier le registre CR0 de sorte à activer la pagination dans `tp.c`.
  Que se passe-t-il ? Pourquoi ?**

  Cela faute à l'infini car notre pgd est vide donc comme la pagination est activée mais CR3 vaut 0 => le process génère une **Page Fault** + pas d'idtr donc triple fault into reboot

  | Étape | Action                                                    | Conséquence                        |
| ----- | --------------------------------------------------------- | ---------------------------------- |
| 1     | Activation du bit PG de CR0                               | Passage en mode paginé             |
| 2     | Accès mémoire suivant → traduction via PGD pointé par CR3 | La MMU tente de traduire l’adresse |
| 3     | Pas de mapping valide                                     | Page Fault (#PF)                   |
| 4     | Pas de gestionnaire d’interruption valide                 | Double puis triple faute           |
| 5     | Redémarrage CPU                                           | RIP                                 |


**Q5\* : Le but va être maintenant d'initialiser la mémoire virtuelle en "identity mapping" : les adresses virtuelles doivent être identiques aux adresses physiques. Pour cela :
Bien étudier les plages d'adresses physiques occupées par le noyau (readelf -e kernel.elf, regarder les program headers).
Préparer au moins une entrée dans le PGD pour la PTB.
Préparer plusieurs entrées dans la PTB.**


Resultat du `readelf -e kernel.elf`

```bash
❯ readelf -e kernel.elf
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Intel 80386
  Version:                           0x1
  Entry point address:               0x303010
  Start of program headers:          52 (bytes into file)
  Start of section headers:          35136 (bytes into file)
  Flags:                             0x0
  Size of this header:               52 (bytes)
  Size of program headers:           32 (bytes)
  Number of program headers:         3
  Size of section headers:           40 (bytes)
  Number of section headers:         19
  Section header string table index: 18

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .mbh              PROGBITS        00300000 000094 00000c 00   A  0   0  4
  [ 2] .stack            NOBITS          00300010 0000a0 002000 00  WA  0   0 16
  [ 3] .idt_jmp          PROGBITS        00302010 0000b0 000ffc 00  AX  0   0 16
  [ 4] .text             PROGBITS        00303010 0010b0 001527 00  AX  0   0 16
  [ 5] .rodata           PROGBITS        00304538 0025d8 00038c 00   A  0   0  4
  [ 6] .data             PROGBITS        003048e0 002980 000070 00  WA  0   0 32
  [ 7] .bss              NOBITS          00304960 0029f0 000c20 00  WA  0   0 32
  [ 8] .gnu.build.a[...] NOTE            00000000 0029f0 000070 00      0   0  4
  [ 9] .debug_line       PROGBITS        00000000 002a60 000b9f 00      0   0  1
  [10] .debug_line_str   PROGBITS        00000000 0035ff 00016a 01  MS  0   0  1
  [11] .debug_info       PROGBITS        00000000 003769 002ffc 00      0   0  1
  [12] .debug_abbrev     PROGBITS        00000000 006765 000cc3 00      0   0  1
  [13] .debug_aranges    PROGBITS        00000000 007428 000148 00      0   0  8
  [14] .debug_str        PROGBITS        00000000 007570 000cb9 01  MS  0   0  1
  [15] .debug_rnglists   PROGBITS        00000000 008229 000031 00      0   0  1
  [16] .symtab           SYMTAB          00000000 00825c 0003d0 10     17  33  4
  [17] .strtab           STRTAB          00000000 00862c 000252 00      0   0  1
  [18] .shstrtab         STRTAB          00000000 00887e 0000bf 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), p (processor specific)

Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  LOAD           0x000094 0x00300000 0x00300000 0x0000c 0x0000c RWE 0x4
  LOAD           0x000000 0x00300010 0x00300010 0x00000 0x02000 RW  0x10
  LOAD           0x0000b0 0x00302010 0x00302010 0x02940 0x03570 RWE 0x20

 Section to Segment mapping:
  Segment Sections...
   00     .mbh 
   01     .stack 
   02     .idt_jmp .text .rodata .data .bss 
```


**Q6 : Une fois la pagination activée, essayer d'afficher le contenu d'une entrée de votre PTB. Que se passe-t-il ? Trouver la solution pour être capable de modifier les entrées de votre PTB une fois la pagination activée.**


On a

```bash
Exception: Page fault
#PF details: p:0 wr:0 us:0 id:0 addr 0x601004
cr0 = 0x80000011
cr4 = 0x0

```

Donc une page fault. C'est logique car en fait on a pas mappé l'adresse de `ptr[4]` vu que on a mappé que sur `pgd[0] ` ce qui fait 4Mo Hors on veut acceder à `0x601000` qui est hors du mapping
=> on doit utiliser `pgd[1]`


**Q7* : Avant d'activer la pagination, on souhaiterait faire en sorte que l'adresse virtuelle `0xc0000000` permette de modifier votre PGD après activation de la pagination. Comment le réaliser ?**


Pour modifier la pgd après activation de la pagination on mappe la pgd elle meme.


**Q9 : Effacer la première entrée du PGD. Que constatez-vous ? Expliquez pourquoi ?**


Lorsque l'on efface la premiere entrée du PGD => Le PGD n’indique plus de PTB pour la première plage de 4 Mo (0x00000000 → 0x003FFFFF).
-> toutes les traductions d’adresses dans cette plage vont échouer → page fault dès qu’on y accède.