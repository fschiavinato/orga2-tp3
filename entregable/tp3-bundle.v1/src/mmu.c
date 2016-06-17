/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

unsigned int proxima_pagina_libre;

void mmu_inicializar() {
 	proxima_pagina_libre = INICIO_PAGINAS_LIBRES;
}

unsigned int mmu_proxima_pagina_fisica_libre() {
	unsigned int pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return pagina_libre;
}

unsigned int mmu_nueva_pila_kernel() {
    return mmu_proxima_pagina_fisica_libre() + PAGE_SIZE;
}

void mmu_mapear_pagina(unsigned int virtual,
unsigned int cr3,
unsigned int fisica,
unsigned char attr) {
	unsigned int *pdirectorio = (unsigned int*) CR3_BASE_ADDR(cr3);
	unsigned int *ptabla;
	int i = 0;
	if(!PDE_PRESENT(pdirectorio[PDE_INDEX(virtual)])) {
		ptabla = (unsigned int*) mmu_proxima_pagina_fisica_libre();
		pdirectorio[PDE_INDEX(virtual)] = (unsigned int) ptabla | (unsigned int) PG_USER | (unsigned int) PG_WRITE | (unsigned int) PG_PRESENT;
		for(; i < ENTRIES_TABLE; i++)
			ptabla[i] = 0x0;
	}
	else
		ptabla = (unsigned int*) PDE_DIRECCION(pdirectorio[PDE_INDEX(virtual)]);
	if(!PTE_PRESENT(ptabla[PTE_INDEX(virtual)]))
		ptabla[PTE_INDEX(virtual)] = fisica | attr | PG_PRESENT;
	tlbflush();
}

void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3) {
	unsigned int *pdirectorio = (unsigned int*) cr3;
	unsigned int *ptabla;
	int i = 0;
	if(/*!*/PDE_PRESENT(pdirectorio[PDE_INDEX(virtual)])) {//??
		ptabla = (unsigned int*) PDE_DIRECCION(pdirectorio[PDE_INDEX(virtual)]);
		ptabla[PTE_INDEX(virtual)] = 0x0;
		for(; i < ENTRIES_TABLE && !PTE_PRESENT(ptabla[i]); i++);
		if(i == ENTRIES_TABLE) {
			pdirectorio[PDE_INDEX(virtual)] = 0x0;
		}
	}
	tlbflush();
}

unsigned int mmu_inicializar_dir_tarea( unsigned char* code, unsigned int dirmapa) {
	unsigned int* pdirectorio = (unsigned int*) mmu_proxima_pagina_fisica_libre();
	int i, j;
	unsigned char* pcodigo_destino = (unsigned char*) DIR_LOG_AFUERA_MEMORIA;
	for (i = 0; i < NUM_TABLES_IDENTITY_MAPPING; i++) {
		unsigned int*  ptabla = (unsigned int*) mmu_proxima_pagina_fisica_libre();
		pdirectorio[i] = (unsigned int) ptabla | (unsigned int) PG_PRESENT |  (unsigned int) PG_WRITE | (unsigned int) PG_KERNEL;

		for(j = 0; j < PAGE_SIZE; j++) {
			ptabla[j] = (i * (ENTRIES_TABLE) + j) * PAGE_SIZE + PAGE_PRESRW;
		}
		ptabla += ENTRIES_TABLE;
	}
	for (; i < ENTRIES_TABLE; i++) {
		pdirectorio[i] = 0;
	}

	mmu_mapear_pagina(DIR_LOG_CODIGO_TAREA, (unsigned int) pdirectorio, dirmapa, PG_USER | PG_WRITE);
	mmu_mapear_pagina(DIR_LOG_PAGINA_TAREA, (unsigned int) pdirectorio, dirmapa, PG_USER | PG_WRITE);

	mmu_mapear_pagina(DIR_LOG_AFUERA_MEMORIA, rcr3(), dirmapa, PG_KERNEL | PG_WRITE);

	for(i = 0; i < PAGE_SIZE; i++) {
		pcodigo_destino[i] = code[i];
	}

	mmu_unmapear_pagina(DIR_LOG_AFUERA_MEMORIA, rcr3());

	tlbflush();
	return (unsigned int) pdirectorio;
}

void mmu_inicializar_dir_kernel() {
  unsigned int*  pdirectorio = (unsigned int*) ADDR_PAGE_DIR;
  unsigned int*  ptabla = (unsigned int*) ADDR_PAGE_TABLE;
  int i, j;
  for (i = 0; i < NUM_TABLES_IDENTITY_MAPPING; i++)	{
    pdirectorio[i] = ((unsigned int) ptabla) + PAGE_PRESRW;

    for(j = 0; j < PAGE_SIZE; j++) {
    	ptabla[j] = (i * (ENTRIES_TABLE) + j) * PAGE_SIZE + PAGE_PRESRW;
    }
    ptabla += ENTRIES_TABLE;
  }
  for (; i < ENTRIES_TABLE; i++) {
  	pdirectorio[i] = 0;
  }
  tlbflush();
}

unsigned char* mmu_dir_mapa(unsigned int x, unsigned int y) {
    x %= ANCHO_MAPA;
    y %= ALTO_MAPA;
    return (unsigned char*) (DIR_PHY_MAPA + x*PAGE_SIZE + y*PAGE_SIZE*ANCHO_MAPA);
}
