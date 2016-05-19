/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [GDT_IDX_KERNEL_CODE_DESC] = (gdt_entry) {
        (unsigned short)    0x6E00,             /* limit[0:15]  */
        (unsigned short)    0x0000,             /* base[0:15]   */
        (unsigned char)     0x00,               /* base[23:16]  */
        (unsigned char)     SEG_CODE_EXRD,      /* type         */ // BANANA!
        (unsigned char)     SEG_TYPE_CODEDATA,  /* s            */
        (unsigned char)     SEG_PRIV0,          /* dpl          */
        (unsigned char)     SEG_PRES,           /* p            */
        (unsigned char)     0x03,               /* limit[16:19] */
        (unsigned char)     SEG_AVL,            /* avl          */
        (unsigned char)     !SEG_IA32E,         /* l            */
        (unsigned char)     SEG_MOD_32b,        /* db           */
        (unsigned char)     SEG_GRAN_4K,        /* g            */
        (unsigned char)     0x00,               /* base[31:24]  */
    },
    [GDT_IDX_KERNEL_DATA_DESC] = (gdt_entry) {
        (unsigned short)    0x6E00,             /* limit[0:15]  */
        (unsigned short)    0x0000,             /* base[0:15]   */
        (unsigned char)     0x00,               /* base[23:16]  */
        (unsigned char)     SEG_DATA_RDWR,      /* type         */ // BANANA!
        (unsigned char)     SEG_TYPE_CODEDATA,  /* s            */
        (unsigned char)     SEG_PRIV0,          /* dpl          */
        (unsigned char)     SEG_PRES,           /* p            */
        (unsigned char)     0x03,               /* limit[16:19] */
        (unsigned char)     SEG_AVL,            /* avl          */
        (unsigned char)     !SEG_IA32E,         /* l            */
        (unsigned char)     SEG_MOD_32b,        /* db           */
        (unsigned char)     SEG_GRAN_4K,        /* g            */
        (unsigned char)     0x00,               /* base[31:24]  */
    },
    [GDT_IDX_USER_CODE_DESC] = (gdt_entry) {
        (unsigned short)    0x6E00,             /* limit[0:15]  */
        (unsigned short)    0x0000,             /* base[0:15]   */
        (unsigned char)     0x00,               /* base[23:16]  */
        (unsigned char)     SEG_CODE_EXRD,      /* type         */ // BANANA!
        (unsigned char)     SEG_TYPE_CODEDATA,  /* s            */
        (unsigned char)     SEG_PRIV3,          /* dpl          */
        (unsigned char)     SEG_PRES,           /* p            */
        (unsigned char)     0x03,               /* limit[16:19] */
        (unsigned char)     SEG_AVL,            /* avl          */
        (unsigned char)     !SEG_IA32E,         /* l            */
        (unsigned char)     SEG_MOD_32b,        /* db           */
        (unsigned char)     SEG_GRAN_4K,        /* g            */
        (unsigned char)     0x00,               /* base[31:24]  */
    },
    [GDT_IDX_USER_DATA_DESC] = (gdt_entry) {
        (unsigned short)    0x6E00,             /* limit[0:15]  */
        (unsigned short)    0x0000,             /* base[0:15]   */
        (unsigned char)     0x00,               /* base[23:16]  */
        (unsigned char)     SEG_DATA_RDWR,      /* type         */ // BANANA!
        (unsigned char)     SEG_TYPE_CODEDATA,  /* s            */
        (unsigned char)     SEG_PRIV3,          /* dpl          */
        (unsigned char)     SEG_PRES,           /* p            */
        (unsigned char)     0x03,               /* limit[16:19] */
        (unsigned char)     SEG_AVL,            /* avl          */
        (unsigned char)     !SEG_IA32E,         /* l            */
        (unsigned char)     SEG_MOD_32b,        /* db           */
        (unsigned char)     SEG_GRAN_4K,        /* g            */
        (unsigned char)     0x00,               /* base[31:24]  */
    },
    [GDT_IDX_VIDEO_DESC] = (gdt_entry) {
        (unsigned short)    0x1F3F,             /* limit[0:15]  */ // 80*25*2 = 0x1F40
        (unsigned short)    0x8000,             /* base[0:15]   */
        (unsigned char)     0x0B,               /* base[23:16]  */
        (unsigned char)     SEG_DATA_RDWR,      /* type         */ 
        (unsigned char)     SEG_TYPE_CODEDATA,  /* s            */
        (unsigned char)     SEG_PRIV0,          /* dpl          */
        (unsigned char)     SEG_PRES,           /* p            */
        (unsigned char)     0x00,               /* limit[16:19] */
        (unsigned char)     SEG_AVL,            /* avl          */
        (unsigned char)     !SEG_IA32E,         /* l            */
        (unsigned char)     SEG_MOD_32b,        /* db           */
        (unsigned char)     SEG_GRAN_1B,        /* g            */
        (unsigned char)     0x00,               /* base[31:24]  */
    }
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};



