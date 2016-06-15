/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_sistema = {
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned int) ADDR_PAGE_DIR,
    (unsigned short) GDT_IDX_KERNEL_DATA_DESC * GDT_DESC_SIZE,
    (unsigned short) NULL,
    (unsigned int) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned int) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned int) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL,
    (unsigned short) NULL
};
ts ts_tareas[MAX_NUM_TAREAS];


