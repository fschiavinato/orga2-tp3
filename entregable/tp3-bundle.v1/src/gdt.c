/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
unsigned int gdt_count = GDT_COUNT_INIT;

gdt_entry gdt[GDT_COUNT_INIT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,                     /* limit[0:15]  */
        (unsigned short)    0x0000,                     /* base[0:15]   */
        (unsigned char)     0x00,                       /* base[23:16]  */
        (unsigned char)     0x00,                       /* type         */
        (unsigned char)     0x00,                       /* s            */
        (unsigned char)     0x00,                       /* dpl          */
        (unsigned char)     0x00,                       /* p            */
        (unsigned char)     0x00,                       /* limit[16:19] */
        (unsigned char)     0x00,                       /* avl          */
        (unsigned char)     0x00,                       /* l            */
        (unsigned char)     0x00,                       /* db           */
        (unsigned char)     0x00,                       /* g            */
        (unsigned char)     0x00,                       /* base[31:24]  */
    },
    [GDT_IDX_KERNEL_CODE_DESC] = (gdt_entry) {
        (unsigned short)    SEG_LIMIT_0_15(0x36E00),    /* limit[0:15]  */
        (unsigned short)    SEG_BASE_0_15(0x0),         /* base[0:15]   */
        (unsigned char)     SEG_BASE_16_23(0x0),        /* base[23:16]  */
        (unsigned char)     SEG_CODE_EXRD,              /* type         */ // BANANA!
        (unsigned char)     SEG_TYPE_CODEDATA,          /* s            */
        (unsigned char)     SEG_PRIV0,                  /* dpl          */
        (unsigned char)     SEG_PRES,                   /* p            */
        (unsigned char)     SEG_LIMIT_16_19(0x36E00),    /* limit[16:19] */
        (unsigned char)     SEG_AVL,                    /* avl          */
        (unsigned char)     !SEG_IA32E,                 /* l            */
        (unsigned char)     SEG_MOD_32b,                /* db           */
        (unsigned char)     SEG_GRAN_4K,                /* g            */
        (unsigned char)     SEG_BASE_24_31(0x0),        /* base[31:24]  */
    },
    [GDT_IDX_KERNEL_DATA_DESC] = (gdt_entry) {
        (unsigned short)    SEG_LIMIT_0_15(0x36E00),    /* limit[0:15]  */
        (unsigned short)    SEG_BASE_0_15(0x0),         /* base[0:15]   */
        (unsigned char)     SEG_BASE_16_23(0x0),        /* base[23:16]  */
        (unsigned char)     SEG_DATA_RDWR,              /* type         */ // BANANA!
        (unsigned char)     SEG_TYPE_CODEDATA,          /* s            */
        (unsigned char)     SEG_PRIV0,                  /* dpl          */
        (unsigned char)     SEG_PRES,                   /* p            */
        (unsigned char)     SEG_LIMIT_16_19(0x36E00),   /* limit[16:19] */
        (unsigned char)     SEG_AVL,                    /* avl          */
        (unsigned char)     !SEG_IA32E,                 /* l            */
        (unsigned char)     SEG_MOD_32b,                /* db           */
        (unsigned char)     SEG_GRAN_4K,                /* g            */
        (unsigned char)     SEG_BASE_24_31(0x0),        /* base[31:24]  */
    },
    [GDT_IDX_USER_CODE_DESC] = (gdt_entry) {
        (unsigned short)    SEG_LIMIT_0_15(0x36E00),    /* limit[0:15]  */
        (unsigned short)    SEG_BASE_0_15(0x0),         /* base[0:15]   */
        (unsigned char)     SEG_BASE_16_23(0x0),        /* base[23:16]  */
        (unsigned char)     SEG_CODE_EXRD,              /* type         */ // BANANA!
        (unsigned char)     SEG_TYPE_CODEDATA,          /* s            */
        (unsigned char)     SEG_PRIV3,                  /* dpl          */
        (unsigned char)     SEG_PRES,                   /* p            */
        (unsigned char)     SEG_LIMIT_16_19(0x36E00),   /* limit[16:19] */
        (unsigned char)     SEG_AVL,                    /* avl          */
        (unsigned char)     !SEG_IA32E,                 /* l            */
        (unsigned char)     SEG_MOD_32b,                /* db           */
        (unsigned char)     SEG_GRAN_4K,                /* g            */
        (unsigned char)     SEG_BASE_24_31(0x0),        /* base[31:24]  */
    },
    [GDT_IDX_USER_DATA_DESC] = (gdt_entry) {
        (unsigned short)    SEG_LIMIT_0_15(0x36E00),    /* limit[0:15]  */
        (unsigned short)    SEG_BASE_0_15(0x0),         /* base[0:15]   */
        (unsigned char)     SEG_BASE_16_23(0x0),        /* base[23:16]  */
        (unsigned char)     SEG_DATA_RDWR,              /* type         */ // BANANA!
        (unsigned char)     SEG_TYPE_CODEDATA,          /* s            */
        (unsigned char)     SEG_PRIV3,                  /* dpl          */
        (unsigned char)     SEG_PRES,                   /* p            */
        (unsigned char)     SEG_LIMIT_16_19(0x36E00),   /* limit[16:19] */
        (unsigned char)     SEG_AVL,                    /* avl          */
        (unsigned char)     !SEG_IA32E,                 /* l            */
        (unsigned char)     SEG_MOD_32b,                /* db           */
        (unsigned char)     SEG_GRAN_4K,                /* g            */
        (unsigned char)     SEG_BASE_24_31(0x0),        /* base[31:24]  */
    },
    [GDT_IDX_VIDEO_DESC] = (gdt_entry) {
        (unsigned short)    SEG_LIMIT_0_15(0x1F3F),     /* limit[0:15]  */ // 80*25*2 = 0x1F40
        (unsigned short)    SEG_BASE_0_15(0xB8000),      /* base[0:15]   */
        (unsigned char)     SEG_BASE_16_23(0xB8000),       /* base[23:16]  */
        (unsigned char)     SEG_DATA_RDWR,              /* type         */ 
        (unsigned char)     SEG_TYPE_CODEDATA,          /* s            */
        (unsigned char)     SEG_PRIV0,                  /* dpl          */
        (unsigned char)     SEG_PRES,                   /* p            */
        (unsigned char)     SEG_LIMIT_16_19(0x1F3F),      /* limit[16:19] */
        (unsigned char)     SEG_AVL,                    /* avl          */
        (unsigned char)     !SEG_IA32E,                 /* l            */
        (unsigned char)     SEG_MOD_32b,                /* db           */
        (unsigned char)     SEG_GRAN_1B,                /* g            */
        (unsigned char)     SEG_BASE_24_31(0xB8000),       /* base[31:24]  */
    },
    [GDT_IDX_IDLE_TASK] = (gdt_entry) {
        (unsigned short)    SEG_LIMIT_0_15(sizeof(struct str_tss)),     /* limit[0:15]  */ 
        (unsigned short)    0x0,                                        /* base[0:15]   */ 
        (unsigned char)     0x0,                                        /* base[23:16]  */  
        (unsigned char)     SEG_TASK_NONBUSY,                           /* type         */
        (unsigned char)     SEG_TYPE_SYSTEM,                            /* s            */
        (unsigned char)     SEG_PRIV0,                                  /* dpl          */
        (unsigned char)     SEG_PRES,                                   /* p            */
        (unsigned char)     SEG_LIMIT_16_19(sizeof(struct str_tss)),    /* limit[16:19] */
        (unsigned char)     SEG_AVL,                                    /* avl          */
        (unsigned char)     !SEG_IA32E,                                 /* l            */
        (unsigned char)     SEG_MOD_32b,                                /* db           */
        (unsigned char)     SEG_GRAN_1B,                                /* g            */
        (unsigned char)     0x0,                                        /* base[31:24]  */ 
    }
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};


//Lo usamos para completar valores que no podemos completar con la inicializacion estática.
void gdt_inicializar() {
    // GDT_IDLE_TASK
    gdt_entry2d* gdt_idle = (gdt_entry2d*) &gdt[GDT_IDX_IDLE_TASK];
    gdt_idle->lo = gdt_idle->lo | (SEG_BASE_0_15(&tss_idle) << 16);
    gdt_idle->hi = gdt_idle->hi | SEG_BASE_16_23(&tss_idle) | (SEG_BASE_24_31(&tss_idle) << 24) ;

}

unsigned int gdt_nueva_entrada() {
    GDT_DESC.gdt_length += sizeof(gdt_entry);
    gdt_count++;
    return gdt_count - 1;
}

