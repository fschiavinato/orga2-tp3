; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"
%include "defines.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice
extern sched_cambia_tarea
extern sched_correr_siguiente_tarea

;; Keyboard
%define KEYBOARD_PORT 0x60
%define KEY_W 0x11
%define KEY_A 0x1E
%define KEY_S 0x1F
%define KEY_D 0x20
%define KEY_LSHIFT 0x2A
%define KEY_I 0x17
%define KEY_K 0x25
%define KEY_L 0x26
%define KEY_J 0x24
%define KEY_RSHIFT 0x36
%define KEY_Y 0x15

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    mov eax, %1
    imprimir_texto_mp isr%1_msg, isr%1_len, 0x07, 0, 0
    call sched_matar_tarea_actual
    mov ebx, eax
    call sched_ts_tarea_actual
    cambio_contexto eax, ebx

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

isr0_msg:   db 'Divide Error'
isr0_len:   equ $ - isr0_msg

isr1_msg:   db 'Debug Exception'
isr1_len:   equ $ - isr1_msg

isr2_msg:   db 'NMI Interrupt'
isr2_len:   equ $ - isr2_msg

isr3_msg:   db 'Breakpoint'
isr3_len:   equ $ - isr3_msg

isr4_msg:   db 'Overflow'
isr4_len:   equ $ - isr4_msg

isr5_msg:   db 'BOUND Range Exceeded'
isr5_len:   equ $ - isr5_msg

isr6_msg:   db 'Invalid Opcode'
isr6_len:   equ $ - isr6_msg

isr7_msg:   db 'Device Not Available'
isr7_len:   equ $ - isr7_msg

isr8_msg:   db 'Double Fault'
isr8_len:   equ $ - isr8_msg

isr9_msg:   db 'Coprocessor Segment Overrun'
isr9_len:   equ $ - isr9_msg

isr10_msg:   db 'Invalid TSS'
isr10_len:   equ $ - isr10_msg

isr11_msg:   db 'Segment Not Present'
isr11_len:   equ $ - isr11_msg

isr12_msg:   db 'Stack-Segment Fault'
isr12_len:  equ $ - isr12_msg

isr13_msg:   db 'General Protection'
isr13_len:  equ $ - isr13_msg

isr14_msg:   db 'Page Fault'
isr14_len:  equ $ - isr14_msg

isr16_msg:   db 'Floating-Point error'
isr16_len:  equ $ - isr16_msg

isr17_msg:   db 'Alignment Check'
isr17_len:  equ $ - isr17_msg

isr18_msg:   db 'Machine Check'
isr18_len:  equ $ - isr18_msg

isr19_msg:   db 'SIMD Floating-Point Exception'
isr19_len:  equ $ - isr19_msg

isr20_msg:   db 'Virtualization Error'
isr20_len:  equ $ - isr20_msg

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20

global _isr32
global _isr33
global _isr102

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

_isr32:
    pushad
    call proximo_reloj
    call fin_intr_pic1
    call sched_ts_tarea_actual
    mov ebx, eax
    call sched_proxima_tarea
    cmp eax, [ebx+TS_ESP0_OFFSET]
    je .fin_isr32
    cambio_contexto ebx, eax

.fin_isr32:
    popad
    iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

_isr33:
    pushad
    in al, KEYBOARD_PORT

    mov ebx, ENUM_DIR_ARB
    mov ecx, ENUM_JUG_JUGA
    cmp al, KEY_W
    je mover_cursor

    mov ebx, ENUM_DIR_IZQ
    cmp al, KEY_A
    je mover_cursor

    mov ebx, ENUM_DIR_ABA
    cmp al, KEY_S
    je mover_cursor

    mov ebx, ENUM_DIR_DER
    cmp al, KEY_D
    je mover_cursor

    cmp al, KEY_LSHIFT
    je lanzar_tarea

    mov ebx, ENUM_DIR_ARB
    mov ecx, ENUM_JUG_JUGB
    cmp al, KEY_I
    je mover_cursor

    mov ebx, ENUM_DIR_ABA
    cmp al, KEY_K
    je mover_cursor

    mov ebx, ENUM_DIR_IZQ
    cmp al, KEY_J
    je mover_cursor

    mov ebx, ENUM_DIR_DER
    cmp al, KEY_L
    je mover_cursor

    cmp al, KEY_Y
    je start_end_modo_debug

    cmp al, KEY_RSHIFT
    je lanzar_tarea

fin_isr33:
    call fin_intr_pic1
    popad
    iret

mover_cursor:
    push ebx
    push ecx
    call game_mover_cursor
    add esp, 8
    jmp fin_isr33

lanzar_tarea:
    push ecx
    call game_lanzar
    add esp, 4
    jmp fin_isr33


start_end_modo_debug:
jmp fin_isr33


;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
_isr102:
    pushad
    cmp eax, INT_DONDE
    je int_donde
    cmp eax, INT_SOY
    je int_soy
    cmp eax, INT_MAPEAR
    je int_mapear

fin_isr102:
    call sched_idle
    mov ebx, eax
    call sched_ts_tarea_actual
    cambio_contexto eax, ebx
    popad
    iret

int_donde:
    push ebx
    call game_donde
    add esp, 4
    jmp fin_isr102

int_soy:
    push ebx
    call game_soy
    add esp, 4
    jmp fin_isr102

int_mapear:
    push ecx
    push ebx
    call game_mapear
    add esp, 8
    jmp fin_isr102




;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
    pushad
    inc DWORD [isrnumero]
    mov ebx, [isrnumero]
    cmp ebx, 0x4
    jl .ok
        mov DWORD [isrnumero], 0x0
        mov ebx, 0
    .ok:
        add ebx, isrClock
        imprimir_texto_mp ebx, 1, 0x0f, 49, 79
        popad
    ret
