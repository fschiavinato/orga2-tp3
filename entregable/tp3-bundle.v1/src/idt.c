/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero)    /*es un descriptor de interrupcion de nivel kernel*/                                                                                    \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) /*& (unsigned int) 0xFFFF*/);        \
    idt[numero].segsel = (unsigned short) 0x18;      /*porque hay un 18 aca??*, para decir que es el tecero de la gdt? */                                                           \
    idt[numero].attr = (unsigned short) IDT_PRESENT | IDT_USR0 | IDT_INTERRUPT | IDT_32BITS;                                                                 \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 /*& (unsigned int) 0xFFFF*/);

/*comente el *& (unsigned int) 0xFFFF porque me arecio que no servia para nada, lo dej por si me equivoco*/

#define IDT_ENTRYUSR(numero)  /*es un descriptor de  interrupcion de nivel usuario*/                                                                                      \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) /*& (unsigned int) 0xFFFF*/);        \
    idt[numero].segsel = (unsigned short) 0x18;      /*porque hay un 18 aca?? para decir que es el tecero de la gdt? */                                                            \
    idt[numero].attr = (unsigned short) IDT_PRESENT | IDT_USR3 | IDT_INTERRUPT | IDT_32BITS;                                                                \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 /*& (unsigned int) 0xFFFF*/);


void idt_inicializar() {
    // Excepciones
    IDT_ENTRY(0)
    IDT_ENTRY(1)
    IDT_ENTRY(2)
    IDT_ENTRY(3)
    IDT_ENTRY(4)
    IDT_ENTRY(5)
    IDT_ENTRY(6)
    IDT_ENTRY(7)
    IDT_ENTRY(8)
    IDT_ENTRY(9)
    IDT_ENTRY(10)
    IDT_ENTRY(11)
    IDT_ENTRY(12)
    IDT_ENTRY(13)
    IDT_ENTRY(14)
    IDT_ENTRY(16)
    IDT_ENTRY(17)
    IDT_ENTRY(18)
    IDT_ENTRY(19)
    IDT_ENTRY(20)
    IDT_ENTRY(32) // 0x20: timer
    IDT_ENTRY(33) // 0x21: keyboard
    IDT_ENTRYUSR(102) // 0x66: software
}
