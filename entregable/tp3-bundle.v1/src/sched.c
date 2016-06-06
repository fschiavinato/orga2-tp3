/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

typedef struct tarea_t {
    unsigned short gdt_idx;
    struct tarea_t* sig;
} tarea;

// Tenemos una run_queue por cada jugador y sanas. Cada una es una lista ciclica con un puntero al primero y uno al ultimo.
struct queues {
    tarea* primera;
    tarea* ultima;
    unsigned int cant;
} run_queues[NUM_QUEUES];

unsigned int current_task_ticks = 0;
unsigned int current_queue = 0;

unsigned short sched_proximo_indice() { 
    unsigned short next_task_gdt_idx;
    unsigned short next_queue = (current_queue+1) % NUM_QUEUES;
    current_task_ticks++;

        while(next_queue != current_queue && run_queues[next_queue].cant == 0) 
            next_queue = (next_queue+1) % NUM_QUEUES;
        
    // Si no hay tareas quedamos en idle.
    if(run_queues[next_queue].cant == 0)
        next_task_gdt_idx = GDT_IDX_IDLE_TASK;
    else if(next_queue == current_queue)
        next_task_gdt_idx = run_queues[next_queue].primera->sig->gdt_idx;
    else
        next_task_gdt_idx = run_queues[next_queue].primera->gdt_idx;

    return next_task_gdt_idx * GDT_DESC_SIZE;
}

unsigned short sched_cambia_tarea() {
    return (current_task_ticks + 1) >= QUANTUM && sched_proximo_indice() != rtr();
}

void sched_agregar_tarea(unsigned short gdt_idx, unsigned int queue_idx) {
    tarea nueva_tarea = { gdt_idx, run_queues[queue_idx].primera};
    if(run_queues[queue_idx].cant > 0) {
        run_queues[queue_idx].primera = &nueva_tarea;
        run_queues[queue_idx].ultima = &nueva_tarea;
        run_queues[queue_idx].primera->sig = run_queues[queue_idx].primera;
    }
    else {
        nueva_tarea.sig = &nueva_tarea;
        run_queues[queue_idx].primera = &nueva_tarea;
        run_queues[queue_idx].ultima = &nueva_tarea;
    }
    run_queues[queue_idx].cant++;
}

void sched_correr_siguiente_tarea() {
    unsigned short next_queue = (current_queue+1) % NUM_QUEUES;
    current_task_ticks++;
    if(current_task_ticks >= QUANTUM) {
        if(run_queues[current_queue].cant > 0) {
            run_queues[current_queue].ultima = run_queues[current_queue].primera;
            run_queues[current_queue].primera = run_queues[current_queue].primera->sig;
        }
        while(next_queue != current_queue && run_queues[next_queue].cant == 0)
            next_queue = (next_queue+1) % NUM_QUEUES;

        current_task_ticks = 0;
        current_queue = next_queue;
    }
}

void sched_sacar_tarea(unsigned short gdt_idx) {
    int queue_idx = 0;
    int task_idx = 0;
    tarea* task = NULL;
    tarea* prev_task = NULL;
    for(; queue_idx < NUM_QUEUES; queue_idx++) {
        task = run_queues[queue_idx].primera;
        prev_task = run_queues[queue_idx].ultima;
        for(task_idx = 0; task_idx < run_queues[queue_idx].cant; task_idx++) {
            if(task->gdt_idx) {
                if(run_queues[queue_idx].cant > 1) {
                    prev_task->sig = task->sig;
                    if(task == run_queues[queue_idx].primera)
                       run_queues[queue_idx].primera = task->sig; 

                    if(task == run_queues[queue_idx].ultima)
                       run_queues[queue_idx].ultima = task->sig; 

                }
                else {
                    run_queues[queue_idx].primera = NULL;
                    run_queues[queue_idx].ultima = NULL;
                }
                run_queues[queue_idx].cant--;
                task_idx--;
            }
            task = task->sig;
        }
    }
}
