/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"


queue run_queues[NUM_QUEUES] = {
    [SCHED_QUEUE_IDX_SANAS] = {
        MAX_NUM_TAREAS_SANAS,
        0,
        TS_START_IDX_SANAS,
        (tarea[MAX_NUM_TAREAS_SANAS]){}
    },
    [SCHED_QUEUE_IDX_JUGA] = {
        MAX_NUM_TAREAS_JUGA,
        0,
        TS_START_IDX_JUGA,
        (tarea[MAX_NUM_TAREAS_JUGA]){}
    },
    [SCHED_QUEUE_IDX_JUGB] = {
        MAX_NUM_TAREAS_JUGB,
        0,
        TS_START_IDX_JUGB,
        (tarea[MAX_NUM_TAREAS_JUGB]){} //no deberia ser tareas[TS_START_IDX_JUGB]
    }
};

unsigned int current_task_ticks = 0;
unsigned int current_queue = 0;
unsigned char en_idle = 0;
unsigned char parado = 0;

void sched_inicializar() {
    int i = 0;
    unsigned int ts_idx = TS_START_IDX_SANAS;
    unsigned int x;
    unsigned int y;
    for(; i < MAX_NUM_TAREAS_SANAS; i++, ts_idx++) {
        do {
            y = rand() % ALTO_MAPA;
            x = rand() % ANCHO_MAPA;
        } while(!sched_correr_tarea(SCHED_QUEUE_IDX_SANAS, (unsigned char*) DIR_PHY_CODIGO_SANA, x, y));
        screen_mapa_imprimir_tarea_sana(x, y);
    }
}

unsigned char* sched_proxima_tarea() {
    unsigned char* res = ts_tareas[TS_IDX_IDLE].esp0;
    current_queue = (current_queue+1) % NUM_QUEUES;
    unsigned int cT = 0, cQ = 0;
    unsigned int iT = run_queues[current_queue].tarea_actual+1, iQ = current_queue;

    if(!parado) {
        while(cQ <= NUM_QUEUES && run_queues[iQ].tareas[iT].viva == FALSE) {
            cQ += (++cT) / run_queues[iQ].cant;
            cT %= run_queues[iQ].cant;
            iQ = (current_queue+cQ) % NUM_QUEUES;
            iT = (run_queues[iQ].tarea_actual+1+cT) % run_queues[iQ].cant;

        }
        if(cQ <= NUM_QUEUES) {
            current_queue = iQ;
            run_queues[current_queue].tarea_actual = iT;
            res = ts_tareas[run_queues[current_queue].tareas[run_queues[current_queue].tarea_actual].ts_idx].esp0;
        }
    }
    else {
        res = sched_ts_tarea_actual()->esp0;
    }
    return res;
} 

struct str_ts* sched_ts_tarea_actual() {
    tarea* t =  sched_info_tarea_actual();
    return t != NULL? &ts_tareas[t->ts_idx]: &ts_tareas[TS_IDX_IDLE];
} 

tarea* sched_info_tarea_actual() {
    tarea* res = &run_queues[current_queue].tareas[run_queues[current_queue].tarea_actual];
    if(en_idle || res->viva == FALSE) 
        res = NULL;
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

unsigned int sched_correr_tarea(unsigned int idx_queue, unsigned char* dir_phy_codigo,unsigned int x, unsigned int y) {
    unsigned int iT = 0;
    unsigned int iQ = 0;
    unsigned int lugar_ocupado = 0;
    queue* q = &run_queues[idx_queue];
    for(; iQ < NUM_QUEUES ; iQ++) 
        for(iT = 0; iT < run_queues[iQ].cant; iT++) {
            lugar_ocupado = lugar_ocupado || (run_queues[iQ].tareas[iT].pos_x == x && run_queues[iQ].tareas[iT].pos_y == y && run_queues[iQ].tareas[iT].viva == TRUE);
        }

    for(iT = 0; iT < q->cant && q->tareas[iT].viva == TRUE; iT++);

    if(iT < q->cant && !lugar_ocupado)
    {
        q->tareas[iT].ts_idx = q->ts_start_idx + iT;
        q->tareas[iT].pos_x = x;
        q->tareas[iT].pos_y = y;
        crear_contexto_usr(&ts_tareas[q->ts_start_idx + iT], dir_phy_codigo, mmu_dir_mapa(x, y));
        q->tareas[iT].viva = TRUE;
    }
    return iT < q->cant && !lugar_ocupado;
}


void sched_matar_tarea_actual() {
    tarea* actual = sched_info_tarea_actual();
    if(actual != NULL) {
        actual->viva = FALSE;
    }
}


// Funciones auxiliares.

 
static unsigned long int next = 1;
 
int rand( void ) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}
 
void srand( unsigned int seed )
{
    next = seed;
}
