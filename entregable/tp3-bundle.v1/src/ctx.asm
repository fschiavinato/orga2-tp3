; Si es una tarea la pila tiene que estar como sigue:

; esp0 -> | CR3    |
;         | DS     |
;---------------------------
;         | EDI    |
;         | ESI    |
;         | EBP0   |
;         | ESP0   |
;         | EBX    |    Lo de esta seccion es para el pushad/popad
;         | EDX    |
;         | ECX    |
;         | EAX    |
;----------------------------
;         | EIP    |
;         | CS     |
;         | EFLAGS |
;Si es de usuario necesita lo siguiente:
;         | ESP    |
;         | SS     |


; 1: ts_idx de la tarea actual
; 2: ts_idx de la tarea nueva
; Suponemos que se llama desde una interrupcion y que se hizo un pushad
%macro cambio_contexto 1
    mov eax, ds
    push eax
    mov eax, cr3
    push eax
    mov esp, [ts_tareas + %1*TS_SIZE + TS_ESP0_OFFSET]
    mov [tss_sistema + TSS_ESP0_OFFSET], esp
    pop eax
    mov cr3, eax
    pop eax
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    popad
    iret
%endmacro    

%include "defines.mac"
%define CTX_CR3_OFFSET          0
%define CTX_DS_OFFSET           4
%define CTX_EDI_OFFSET          8
%define CTX_ESI_OFFSET          12
%define CTX_EBP0_OFFSET         16
%define CTX_ESP0_OFFSET         20
%define CTX_EBX_OFFSET          24
%define CTX_EDX_OFFSET          28
%define CTX_ECX_OFFSET          32
%define CTX_EAX_OFFSET          36
%define CTX_EIP_OFFSET          40
%define CTX_CS_OFFSET           44
%define CTX_EFLAGS_OFFSET       48
%define CTX_ESP_OFFSET          52
%define CTX_SS_OFFSET           56


; void crear_contexto(ts* task, uint* codigo, uint* dirmapa)
%define task ebp+8
%define codigo ebp+12
%define dirmapa ebp+16
crear_contexto_usr:
    push ebp
    mov ebp, esp
    push ebx
    mov eax, [dirmapa]
    push eax
    mov eax, [codigo]
    push eax
    call mmu_inicializar_dir_tarea
    mov ebx, eax
    call mmu_nueva_pila_kernel
    mov edx, eax
    sub eax, 60
    mov dword [eax+CTX_SS_OFFSET], GDT_IDX_USER_DATA_DESC
    mov dword [eax+CTX_ESP_OFFSET], DIR_LOG_PILA_TAREA
    mov dword [eax+CTX_EFLAGS_OFFSET], CTX_EFLAGS
    mov dword [eax+CTX_CS_OFFSET], GDT_IDX_USER_CODE_DESC
    mov dword [eax+CTX_EIP_OFFSET], DIR_LOG_CODIGO_TAREA
    mov dword [eax+CTX_EAX_OFFSET], 0x0
    mov dword [eax+CTX_ECX_OFFSET], 0x0
    mov dword [eax+CTX_EDX_OFFSET], 0x0
    mov dword [eax+CTX_EBX_OFFSET], 0x0
    mov [eax+CTX_ESP0_OFFSET], eax
    mov [eax+CTX_EBP0_OFFSET], edx
    mov dword [eax+CTX_ESI_OFFSET], 0x0
    mov dword [eax+CTX_EDI_OFFSET], 0x0
    mov dword [eax+CTX_DS_OFFSET], GDT_IDX_USER_DATA_DESC
    mov [eax+CTX_CR3_OFFSET], ebx
    mov [task+TS_ESP0_OFFSET], eax
    pop ebx
    pop ebp
    ret
