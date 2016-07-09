/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

//Arreglo de queues
queue run_queues[NUM_QUEUES] = {
    [SCHED_QUEUE_IDX_SANAS] = {
        MAX_NUM_TAREAS_SANAS,//15
        0,                    // viva = 0 
        TS_START_IDX_SANAS,//0
        NULL,
        (tarea[MAX_NUM_TAREAS_SANAS]){} // MAX_NUM_TAREAS_SANAS =15
    },
    [SCHED_QUEUE_IDX_JUGA] = {
        MAX_NUM_TAREAS_JUGA,//5
        0,
        TS_START_IDX_JUGA,//15
        JUGA,
        (tarea[MAX_NUM_TAREAS_JUGA]){}// MAX_NUM_TAREAS_JUGA =5
    },
    [SCHED_QUEUE_IDX_JUGB] = {
        MAX_NUM_TAREAS_JUGB, //5
        0,
        TS_START_IDX_JUGB,//20
        JUGB,
        (tarea[MAX_NUM_TAREAS_JUGB]){} //MAX_NUM_TAREAS_JUGB =5
    }
};

/*
unsigned int cant;
    unsigned int tarea_actual;
    unsigned int ts_start_idx;
    unsigned int jug;
    tarea* tareas;
*/

unsigned int current_task_ticks = 0;
unsigned int current_queue = 0;
unsigned char en_idle = 0;
unsigned char parado = 0;
unsigned char debug = 0;

void sched_inicializar() {
    int i = 0;
    unsigned int ts_idx = TS_START_IDX_SANAS;
    unsigned int x;
    unsigned int y;
    for(; i < MAX_NUM_TAREAS_SANAS; i++, ts_idx++) {
        do {
            y = rand() % ALTO_MAPA; //genera poscision al azar para posicionar las tareas
            x = rand() % ANCHO_MAPA; //genera poscision con un random para posicionar las tareas
        } while(!sched_correr_tarea(SCHED_QUEUE_IDX_SANAS, (unsigned char*) DIR_PHY_CODIGO_SANA, x, y));
        screen_mapa_imprimir_tarea_sana(x, y);
    }
}

//se encarga de que las tareas sanas aparezcan en la pantalla


unsigned char* sched_proxima_tarea() {
    //unsigned char* res;
    unsigned char* res = ts_tareas[TS_IDX_IDLE].esp0; //TS_IDX_IDLE =25 no sirve para nada, pude haberle puesto naa
    unsigned int cT = 0, cQ = 0;
    unsigned int iQ;
    unsigned int iT;

    tarea* actual = sched_info_tarea_actual();// te devuelve el struct tarea de la que se tarea que se venia ejecutando 
    if(actual->viva == TRUE) {
        actual->estado_reloj++;
        actual->estado_reloj %= CANT_ESTADOS_RELOJ;
        screen_actualizar_reloj(current_queue, run_queues[current_queue].tarea_actual, actual->estado_reloj);
    }

    if(parado == FALSE || debug == FALSE) {
        do {
            iQ = (current_queue + cQ + 1) % NUM_QUEUES;
            iT = (run_queues[iQ].tarea_actual + cT + 1) % run_queues[iQ].cant;
            cQ += (++cT) / (run_queues[iQ].cant + 1);
            cT %= run_queues[iQ].cant + 1;

        } while((cQ <= NUM_QUEUES) && (run_queues[iQ].tareas[iT].viva == FALSE));

        if(cQ <= NUM_QUEUES) {
            current_queue = iQ;
            run_queues[current_queue].tarea_actual = iT;
            res = ts_tareas[run_queues[iQ].tareas[iT].ts_idx].esp0;
            en_idle = 0;
        }
        else {
            en_idle = 1;
        }

        if(parado == TRUE) {
          parado = FALSE;
          screen_ocultar_consola_debug();
        }

    }
    else {
        res = sched_ts_tarea_actual()->esp0;
    }
    return res;
}


//Nos dice la tarea proxima a ejecutar actualizando las colas (y nos devuelve un puntero a su esp0)

struct str_ts* sched_ts_tarea_actual() {
    tarea* t =  sched_info_tarea_actual();
    return en_idle? &ts_tareas[TS_IDX_IDLE]: &ts_tareas[t->ts_idx];
}

tarea* sched_info_tarea_actual() {
    tarea* res = &run_queues[current_queue].tareas[run_queues[current_queue].tarea_actual];
    return res;
}

queue* sched_info_queue_actual() {
    queue* res = &run_queues[current_queue];
    return res;
}

unsigned char* sched_idle() {
    en_idle = TRUE;
    return ts_tareas[TS_IDX_IDLE].esp0;
}

void sched_toggle_debug() {
    debug = !debug;
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
        crear_contexto_usr(&ts_tareas[q->tareas[iT].ts_idx], dir_phy_codigo, mmu_dir_mapa(x, y));
        q->tareas[iT].estado_reloj = 0;
        q->tareas[iT].virus = q->jug;
        q->tareas[iT].viva = TRUE;

    }
    return iT < q->cant && !lugar_ocupado;
}


unsigned char* sched_matar_tarea_actual(unsigned int* info) {
    unsigned char* res;
    tarea* actual = sched_info_tarea_actual();
    if(actual != NULL) {
        actual->viva = FALSE;
    }
    res = sched_idle();
    screen_matar(actual->pos_x, actual->pos_y);
    screen_actualizar_puntajes();
    screen_actualizar_reloj(current_queue, run_queues[current_queue].tarea_actual, CANT_ESTADOS_RELOJ);
    if(debug) {
      parado = TRUE;
      screen_mostrar_consola_debug(info);
    }
    return res;
}

unsigned int sched_infectados(unsigned int jug) {
    int i, j;
    unsigned int cant = 0;
    for(i = 0; i < NUM_QUEUES; i++) {
        for(j = 0; j < run_queues[i].cant; j++) {
            if(run_queues[i].tareas[j].viva == TRUE && run_queues[i].tareas[j].virus == jug) {
                cant++;
            }
        }
    }

    return cant;
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

