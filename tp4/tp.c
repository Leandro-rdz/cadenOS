/* GPLv2 (c) Airbus */
#include <debug.h>
#include <cr.h>
#include <pagemem.h>

void init_ptb(pde32_t * pgd ,pte32_t * ptb, int index){
	for (int i = 0; i < 1024; i++){
    	pg_set_entry(&ptb[i],PG_P| PG_KRN |PG_RW , i+1024*index);
	}
	pg_set_entry(&pgd[index],PG_P | PG_KRN | PG_RW, page_get_nr(ptb) );

}

void tp() {
	debug("====================================TP====================================\n");
	
	//Q1
	cr3_reg_t cr3;
	cr3.raw = get_cr3();
	debug("CR3 addr : %d\n",(uint32_t) cr3.raw);


	//Q2
	pde32_t *pgd = (pde32_t *)0x600000;
	set_cr3((uint32_t)pgd);
	cr3.raw = get_cr3();
	debug("CR3 addr : %d\n",(uint32_t) cr3.raw);

	//Q3
	uint32_t cr0 = get_cr0();
	//set_cr0(cr0|CR0_PG);
	//debug("CR0 pagination = %d\n",cr0);

	//Q4
	pte32_t *ptb = (pte32_t *)0x601000;
	
	
	//Q5
	memset((void*)pgd, 0, PAGE_SIZE);
	for (int i = 0; i < 1024; i++){
    	pg_set_entry(&ptb[i],PG_P| PG_KRN |PG_RW , i);
	}
	pg_set_entry(&pgd[0],PG_P | PG_KRN | PG_RW, page_get_nr(ptb) );

	//Q6
	
	debug("CR0 pagination = %b\n",cr0);
	//debug("Affichage d'une entrée de la ptb : %b\n", ptb[1].raw);
	

	//sol
	pte32_t *ptb2 = (pte32_t *)0x602000;
	
	
	for (int i = 0; i < 1024; i++){
    	pg_set_entry(&ptb2[i],PG_P| PG_KRN |PG_RW , i+1024);
	}
	pg_set_entry(&pgd[1],PG_P | PG_KRN | PG_RW, page_get_nr(ptb2) );
	set_cr0(cr0|CR0_PG);
	debug("Affichage d'une entrée de la ptb : %b\n", ptb2[1].raw);


	//Q7
	//macro créé qu'on pourrait mettre dans pagemem 
	//Permet de savoir quelle ptb gere l'adresse virtuelle
	#define PGD_INDEX(addr) ((addr) >> 22 & 0x3FF)

	pg_set_entry(&pgd[PGD_INDEX(0xc0000000)], PG_P | PG_RW | PG_KRN,page_get_nr(pgd ));
	debug("Affichage de l'endroit ou on peut modifier la pgd : %x\n", pgd[PGD_INDEX(0xC0000000)].raw);



	//Q8
	//macro pour la ptb savoir quelle page de la ptb
	#define PTB_INDEX(addr) ((addr >> 12) & 0x3FF)
	pte32_t *ptb3 = (pte32_t * ) 0x603000 ;
	pg_set_zero(ptb3);
	pg_set_entry(&pgd[PGD_INDEX(0x700000)], PG_P | PG_RW | PG_KRN, page_get_nr(ptb3));
	pg_set_entry(&pgd[PGD_INDEX(0x7FF000)], PG_P | PG_RW | PG_KRN, page_get_nr(ptb3));

	char *v1 = (char*)0x700000; 
	char *v2 = (char*)0x7ff000;
	memset((void*)ptb3, 0, PAGE_SIZE);
	pg_set_entry(&ptb3[PTB_INDEX(0x700000)], PG_P | PG_RW | PG_KRN,page_get_nr(0x2000) );
	pg_set_entry(&ptb3[PTB_INDEX(0x7ff000)], PG_P | PG_RW | PG_KRN,page_get_nr(0x2000) );
	debug("%p = %s | %p = %s\n", v1, v1, v2, v2);

	//Q9
	pg_set_zero(&pgd[0]);
	//invalidate(&pgd[0]); 
	//char *ptr = (char*)0x0;   
	//debug("Accès à 0x0 : %c\n", *ptr); 
	debug("====================================TP====================================\n");
}
