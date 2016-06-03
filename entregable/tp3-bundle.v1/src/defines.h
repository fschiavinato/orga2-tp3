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
#define GDT_COUNT 68

#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_KERNEL_CODE_DESC 3
#define GDT_IDX_KERNEL_DATA_DESC 4
#define GDT_IDX_USER_CODE_DESC 5
#define GDT_IDX_USER_DATA_DESC 6
#define GDT_IDX_VIDEO_DESC 7
#define GDT_IDX_INICIAL_DESC 8   //agregado
#define GDT_IDX_IDDLE_DESC 9	 //agregado


/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)


/* Parametros del gate en la gdt */

#define IDT_INTERRUPT	(0x6 << 8) /*es un puerta de interrupcion*/
#define IDT_TRAP		(0x7 << 8) /*es una puerta de trampa*/
#define IDT_TASK 		(0x5 << 8) /*es una puerta de tareas*/
#define IDT_32BITS		(0x1 << 11)/*d*/
#define IDT_16BITS		(0x0 << 11)/*d*/
#define IDT_USR0		(0x0 << 13)/*dpl*/
#define IDT_USR1		(0x1 << 13)/*dpl*/
#define IDT_USR2		(0x2 << 13)/*dpl*/
#define IDT_USR3		(0x3 << 13)/*dpl*/
#define IDT_PRESENT 	(0x1 << 15)/*esta presente*/

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
 
// CODE/DATA/

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
#define SEG_TASK_NONBUSY   0x0B //esto ya estaba, el nombre del tag me hace pensar que es una segmento de tarea. pero el 0x0b deberia ser descriptor de TSS..como dice busy, asumo que era eso, y lo uso.


//MMU
#define PAGE_SIZE 				0x1000
#define ENTRIES_TABLE			1024
#define PAGE_PRESRW             0x03

#define INICIO_PAGINAS_LIBRES	0x100000 	
#define DIRECCION_VIRTUAL_CODIGO_TAREA	0x08000000


#define PDE_INDEX(virtual) virtual >> 22
#define PTE_INDEX(virtual) (virtual << 10) >> 22
#define PDE_PRESENT(entry)	(entry & 0x1)
#define PTE_PRESENT(entry)	(entry & 0x1)
#define PDE_DIRECCION(entry)	(entry & 0xFFFFF000)
#define ALIGN(dir)			dir
#define PG_READ 			0x0
#define PG_WRITE      		0x2
#define PG_USER 			0x4
#define PG_KERNEL      		0x0
#define PG_PRESENT         	0x1

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO_SCREEN            0x000B8000 /* direccion fisica del buffer de video */
#define ADDR_PAGE_DIR			0x00027000 
#define ADDR_PAGE_TABLE			0x00028000
#define NUM_TABLES_IDENTITY_MAPPING  0x01
 #endif  /* !__DEFINES_H__ */
