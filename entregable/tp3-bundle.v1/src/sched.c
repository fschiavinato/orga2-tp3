/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

typedef struct tarea_t {
    unsigned short ts_idx;
    unsigned char viva;
} tarea;

typedef struct queue_t {
    unsigned int cant;
    unsigned int tarea_actual;
    tarea* tareas;
} queue;


queue run_queues[NUM_QUEUES] = {
    {
        MAX_NUM_TAREAS_SANAS,
        0,
        (tarea[MAX_NUM_TAREAS_SANAS]){}
    },
    {
        MAX_NUM_TAREAS_JUGA,
        0,
        (tarea[MAX_NUM_TAREAS_JUGA]){}
    },
    {
        MAX_NUM_TAREAS_JUGB,
        0,
        (tarea[MAX_NUM_TAREAS_JUGB]){}
    }
};

unsigned int current_task_ticks = 0;
unsigned int current_queue = 0;
unsigned char en_idle = 0;
unsigned char parado = 0;


unsigned short sched_proxima_tarea() {
    unsigned short res = TS_IDX_IDLE;
    current_queue = (current_queue+1) % NUM_QUEUES;
    unsigned int iT = 0, iQ = 0;
    unsigned char incQ;

    if(!parado) {
        while(iQ < NUM_QUEUES && run_queues[current_queue+iQ].
                tareas[(run_queues[current_queue].tarea_actual+1+iT) % run_queues[current_queue].cant].
                viva == FALSE) {
            incQ = ++iT / run_queues[current_queue + iQ].cant;
            iT %= run_queues[current_queue + iQ].cant;
            iQ += incQ;
        }
        if(iQ < NUM_QUEUES) {
            current_queue += iQ;
            run_queues[current_queue].tarea_actual += iT+1;
            res = run_queues[current_queue].tareas[run_queues[current_queue].tarea_actual].ts_idx;
        }
    }
    else {
        res = sched_tarea_actual();
    }
    return res;
} 

unsigned short sched_tarea_actual() {
    unsigned short res = run_queues[current_queue].tareas[run_queues[current_queue].tarea_actual].ts_idx;
    if(en_idle || run_queues[current_queue].tareas[run_queues[current_queue].tarea_actual].viva == FALSE) 
        res = TS_IDX_IDLE;
    return res;
} 

unsigned short sched_idle() {
    en_idle = TRUE;
    return TS_IDX_IDLE;
}

void sched_parar() {
    parado = TRUE;
}

void sched_reanudar() {
    parado = FALSE;
}
