/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;

void tss_inicializar( tss tss_idle) {
	

}

/* Cambios realizados...no mucho, se aunmento el valor define de gdt_count a 68(era 8), para tener espacio pra las entradas de tareas. Se definio la gdt iddle e inicial.
(la gdt esta comentada por faltas o no haberls probado, pero con gran posibilidad de falta, la gdt iddle esta sin exportar el tss.c )sin definir las variables a usar en base=..pudo haberse puesto con defines....)