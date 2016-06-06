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

unsigned short sched_proximo_indice();
unsigned short sched_cambia_tarea();
void sched_correr_siguiente_tarea();
void sched_iniciar_tarea(unsigned short gdt_idx, unsigned int queue_idx);
void sched_detener_tarea(unsigned short gdt_idx);


#endif	/* !__SCHED_H__ */
