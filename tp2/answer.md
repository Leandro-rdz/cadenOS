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


**Q1\* : Dans tp.c, localiser l'IDT et afficher son adresse de chargement**
