# Notes TP3

**Il n'y a pas vraiment de question et je suis pas sûr d'avoir tout compris donc je mets mes notes ici**

Déjà on nous demande a fait faire une gdt avec un séparation entre le kernel side et le user side:

*gdt[0] = NULL*
*gdt[1] = kernel code*
*gdt[2] = kernel data*
*gdt[3] = TSS*
*gdt[4] = user code*
*gdt[5] = user data*

deja pour mettre le bon cs on prend le bon segment exemple
 
 ```c
//Ici on prend le cs
uint16_t sel_user_code = (uint16_t)gdt_usr_seg_sel(4); 

 ```


On doit reconstruire un tss avec le bon esp et ss pour garder quand on va switch de ring 0 à ring 3
puis on fait ce que demande le tp cad: 

```c
  asm volatile("push %0\n\t" //SS  = user data
                  "push %%ebp\n\t" // EBP = ESP ici car flat 
                  "pushf \n\t"  // push les flags
                  "push %1\n\t" // CS = user code
                  "push %2\n\t" // EIP le code userland
                  "iret"
                  :
                  :"r"(sel_user_data),"r"(sel_user_code),"r"(userland)
               );
```


