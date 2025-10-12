**Dans ce TP, il pourra être utile de se référer à la documentation Intel pour
comprendre en détail ce que font les instructions suivantes et quel est leur
rôle : INT3, RET, IRET. Il pourra être également utile de savoir à quoi
correspond le numéro d'exception #6, ou savoir se servir des instructions
PUSH/PUSHA, POP/POPA.**

*INT3* : place un breakpoint
*RET* : permet de faire un return ça va jumper à l'adresse de retour en haut de la pile et la dépiler
*IRET* : interruption avec code de retour suite à une exception

*numéro d'exception 6* : invalid opcode exception

*PUSH x* : empile x et decremente le stack pointer pour accepter ce x
*POP x* : depile et met dans x et incremente le stack pointer pour retirer le dernier element 

*POP* -> dépile 1 registre
*POPA* -> dépile tout les registres


**Q5 : Cette implémentation pousse le CPU à générer une faute. Pour comprendre
  pourquoi, à l'aide d'un outil de désassemblage comme `objdump -D`, lister
  les instructions, générées à la compilation, de la fonction `bp_handler()`. 
  Quelle est la dernière instruction de cette fonction ? Quel est son
  impact sur la pile ? Est-ce cohérent avec ce qui était sur la pile au
  moment de l'arrivée d'une interruption ?**

 -> Il faudrait utiliser IRET et pas RET car on a déclenché une interruption et non une fonction, la définition dans la pile correspond donc a une frame d'interruption et non une frame de fonction.


**Q7 : Quelle signification cette valeur a-t-elle ? S'aider à nouveau de `objdump -D`
pour comparer cette valeur à une adresse de votre noyau.**
En lisant la valeur à 4(%ebp) dans bp_handler, on obtient l'adresse : `303f99` qui correspond au `sub` dans `bp_trigger`  (donc l'adresse de retour de l'interruption) :
```asm
00303f92 <bp_trigger>:
  303f92:       55                      push   %ebp
  303f93:       89 e5                   mov    %esp,%ebp
  303f95:       83 ec 08                sub    $0x8,%esp
  303f98:       cc                      int3
  303f99:       83 ec 0c                sub    $0xc,%esp
  303f9c:       68 7b 4a 30 00          push   $0x304a7b
  303fa1:       e8 2a f1 ff ff          call   3030d0 <printf>
  303fa6:       83 c4 10                add    $0x10,%esp
  303fa9:       90                      nop
  303faa:       c9                      leave
  303fab:       c3                      ret
```

**Q8\* : Qu'est-ce qui n'est pas stocké par le CPU à l'arrivée d'une
  interruption et qu'il est impératif de sauvegarder avant tout traitement de
  l'interruption ? L'implémenter en assembleur inline dans  `bp_handler`.**

  EIP, CS, EFLAGS sont sauvegardés automatiquement en revanche pas les segments, registres généraux et registres de controles -> faut faire un pusha


**Q9\* : Par quelle instruction doit se terminer la routine pour que le noyau
  rende la main à la fonction tp() ? L'implémenter en assembleur inline dans
  `bp_handler`.**
  pour une fonction il faudrait faire un ret mais ici un iret (pour interruption return)

**Q11 : Quelles conclusions peut-on tirer du développement en C d'un
  gestionnaire d'interruption ? Pourquoi l'assembleur semble-t-il plus
  approprié ?**
l'assembleur permet de contrôler précisemment le comportement attendu là où le C peut faire des comportements non attendues.