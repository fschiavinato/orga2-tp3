/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

void mmu_inicializar() {
 

}


void mmu_inicializar_dir_kernel(){
  unsigned int*  pdirectorio = (unsigned int*) ADDR_PAGE_DIR;
  unsigned int*  ptabla = (unsigned int*) ADDR_PAGE_TABLE;
  int i, j;
  for (i = 0; i < NUM_TABLES_IDENTITY_MAPPING; i++)	{
    pdirectorio[i] = ((unsigned int) ptabla) + PAGE_PRESRW;

    for(j = 0; j < PAGE_SIZE_HEX; j++) {
    	ptabla[j] = (i * (ENTRIES_TABLE) + j) * PAGE_SIZE_HEX + PAGE_PRESRW;
    }
    ptabla += ENTRIES_TABLE; // PAGE_SIZE_HEX está en bytes. Estamos sumandolo a un puntero uint, con lo cual C lo multiplica por 4.
  }
  for (; i < ENTRIES_TABLE; i++) {
  	pdirectorio[i] = 0;
  }
}
