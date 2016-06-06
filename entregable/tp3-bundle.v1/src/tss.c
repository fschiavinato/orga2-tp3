/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_idle;

unsigned int tss_obtener( unsigned short gdt_idx ) {
    gdt_entry gdt_seg = gdt[gdt_idx];
    return SEG_GET_BASE(gdt_seg.base_31_24, gdt_seg.base_23_16, gdt_seg.base_0_15);
}
