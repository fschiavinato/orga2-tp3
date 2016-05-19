/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_H                 15
#define CANT                    5
#define SIZE_W                  80
#define SIZE_H                  44


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 8

#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_KERNEL_CODE_DESC 3
#define GDT_IDX_KERNEL_DATA_DESC 4
#define GDT_IDX_USER_CODE_DESC 5
#define GDT_IDX_USER_DATA_DESC 6
#define GDT_IDX_VIDEO_DESC 7

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)


/* Parametros del segmento en la gdt */
/* -------------------------------------------------------------------------- */
#define SEG_TYPE_SYSTEM  0x00 
#define SEG_TYPE_CODEDATA 0x01
#define SEG_PRES 0x01 
#define SEG_AVL 0x01
#define SEG_MOD_16b 0x00
#define SEG_MOD_32b 0x01
#define SEG_IA32E 0x01
#define SEG_GRAN_1B 0x00 
#define SEG_GRAN_4K 0x01
#define SEG_PRIV0  0x00 
#define SEG_PRIV1 0x01 
#define SEG_PRIV2 0x02 
#define SEG_PRIV3 0x03
 
// CODE/DATA
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed

//SYSTEM

#define SEG_TASK_BUSY      0x09
#define SEG_TASK_NONBUSY   0x0B
 
/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO_SCREEN            0x000B8000 /* direccion fisica del buffer de video */

#endif  /* !__DEFINES_H__ */
