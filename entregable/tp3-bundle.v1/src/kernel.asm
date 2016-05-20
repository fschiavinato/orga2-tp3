; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

%define GDT_DESC_SIZE 0x08
%define GDT_IDX_KERNEL_CODE_DESC 0x03*GDT_DESC_SIZE
%define GDT_IDX_KERNEL_DATA_DESC 0x04*GDT_DESC_SIZE
%define GDT_IDX_USER_CODE_DESC 0x05*GDT_DESC_SIZE
%define GDT_IDX_USER_DATA_DESC 0x06*GDT_DESC_SIZE
%define GDT_IDX_VIDEO_DESC 0x07*GDT_DESC_SIZE

global start
extern GDT_DESC
extern idt_inicializar
extern IDT_DESC
extern deshabilitar_pic

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0 
    

    ; Habilitar A20
    call habilitar_A20
    
    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax
    
    ; Saltar a modo protegido
    jmp GDT_IDX_KERNEL_CODE_DESC:modo_protegido

BITS 32
    modo_protegido:

    ; Establecer selectores de segmentos
    mov eax, GDT_IDX_KERNEL_DATA_DESC
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax

    ; Establecer la base de la pila
    mov ebp, 0x27000
    mov esp, 0x27000
    
    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar pantalla
    call inicializar_pantalla
    
    ; Inicializar el manejador de memoria
 
    ; Inicializar el directorio de paginas
    
    ; Cargar directorio de paginas

    ; Habilitar paginacion
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar
    call deshabilitar_pic
    
    ; Cargar IDT

    lidt [IDT_DESC]
    
 
    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones
    sti
    
    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    int 0x07
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"

BITS 32
inicializar_pantalla:
    push ds
    mov eax, GDT_IDX_VIDEO_DESC
    mov ds, eax
    mov ecx, 80*50
    mov eax, 0x00
.loop:
    mov byte [eax], ' ' ; Guardamos un espacio.
    inc eax
    mov byte [eax], 01110000b ; 0111 = grey sin bright, 0000 = black sin bright
    inc eax
    loop .loop
    pop ds
    ret
