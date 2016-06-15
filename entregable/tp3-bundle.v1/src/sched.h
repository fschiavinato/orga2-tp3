/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"
#include "gdt.h"
#include "mmu.h"

struct str_ts;

typedef struct tarea_t {
    unsigned short ts_idx;
    unsigned char viva;
    unsigned int pos_x;
    unsigned int pos_y;
} tarea;

typedef struct queue_t {
    unsigned int cant;
    unsigned int tarea_actual;
    unsigned int ts_start_idx;
    tarea* tareas;
} queue;

unsigned char* sched_proximo_indice();
struct str_ts* sched_ts_tarea_actual();
tarea* sched_info_tarea_actual();
void sched_correr_siguiente_tarea();
unsigned int sched_correr_tarea(unsigned int idx_queue, unsigned char* dir_phy_codigo,unsigned int x, unsigned int y);
void sched_matar_tarea_actual();

extern void crear_contexto_usr(struct str_ts* task, unsigned char* codigo, unsigned char* dirmapa);
int rand();
void srand(unsigned int seed);

#endif	/* !__SCHED_H__ */
