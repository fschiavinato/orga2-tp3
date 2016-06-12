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

struct str_ts;

unsigned char* sched_proximo_indice();
struct str_ts* sched_ts_tarea_actual();
void sched_correr_siguiente_tarea();
void sched_iniciar_tarea(unsigned short gdt_idx, unsigned int queue_idx);
void sched_detener_tarea(unsigned short gdt_idx);

extern void crear_contexto_usr(struct str_ts* task, unsigned char* codigo, unsigned char* dirmapa);


#endif	/* !__SCHED_H__ */
