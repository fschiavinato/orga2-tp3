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
    [SCHED_QUEUE_IDX_SANAS] = {
        MAX_NUM_TAREAS_SANAS,
        0,
        (tarea[MAX_NUM_TAREAS_SANAS]){}
    },
    [SCHED_QUEUE_IDX_JUGA] = {
        MAX_NUM_TAREAS_JUGA,
        0,
        (tarea[MAX_NUM_TAREAS_JUGA]){}
    },
    [SCHED_QUEUE_IDX_JUGB] = {
        MAX_NUM_TAREAS_JUGB,
        0,
        (tarea[MAX_NUM_TAREAS_JUGB]){}
    }
};

unsigned int current_task_ticks = 0;
unsigned int current_queue = 0;
unsigned char en_idle = 0;
unsigned char parado = 0;

void sched_inicializar() {
	int i = 0;
	for(; i < MAX_NUM_TAREAS_SANAS; i++) {
		unsigned int ts_idx = run_queues[SCHED_QUEUE_IDX_SANAS].tareas[i].ts_idx;
		crear_contexto_usr(&ts_tareas[ts_idx], (unsigned char*) DIR_PHY_CODIGO_SANA, (unsigned char*) DIR_PHY_MAPA);
		run_queues[SCHED_QUEUE_IDX_SANAS].tareas[i].viva = TRUE;
	}

}

unsigned short sched_proxima_tarea() {
    unsigned short res = TS_IDX_IDLE;
    current_queue = (current_queue+1) % NUM_QUEUES;
    unsigned int cT = 0, cQ = 0;
    unsigned int iT = run_queues[current_queue].tarea_actual+1, iQ = current_queue;

    if(!parado) {

        while(cQ < NUM_QUEUES && run_queues[iQ].tareas[iT].viva == FALSE) {
            
            cQ += (++cT) / run_queues[iQ].cant;
            cT %= run_queues[iQ].cant;
            iQ = (current_queue+cQ) % NUM_QUEUES;
            iT = (run_queues[iQ].tarea_actual+cT) % run_queues[iQ].cant;

        }
        if(cQ < NUM_QUEUES) {
            current_queue = iQ;
            run_queues[current_queue].tarea_actual = iT;
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
