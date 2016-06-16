/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

unsigned int vidasA = CANT_VIDAS;
unsigned int vidasB = CANT_VIDAS;

// Funciones Auxiliares.
unsigned int game_obtener_queue_idx_jugador(unsigned int jugador) {
    unsigned int res = 0;
    switch(jugador) {
        case JUGA:
            res = SCHED_QUEUE_IDX_JUGA;
            break;
        case JUGB:
            res = SCHED_QUEUE_IDX_JUGB;
    }
    return res;
}

unsigned char* game_obtener_dir_phy_codigo_jugador(unsigned int jugador) {
    unsigned char* res = 0;
    switch(jugador) {
        case JUGA:
            res = (unsigned char*) DIR_PHY_CODIGO_JUGA;
            break;
        case JUGB:
            res = (unsigned char*) DIR_PHY_CODIGO_JUGB;
    }
    return res;
}

unsigned int* game_vidas(int jug) {
    unsigned int* res = NULL;
    switch(jug) {
        case JUGA:
            res = &vidasA;
            break;
        case JUGB:
            res = &vidasB;
    }
    return res;
}

unsigned int es_jugador(int jug) {
    unsigned int res = FALSE;
    switch(jug) {
        case JUGA:
        case JUGB:
            res = TRUE;
    }
    return res;
}

void game_mover_cursor(int jugador, direccion dir) {
    int desp_x = 0;
    int desp_y = 0;
    pos* pos_cursor;
    switch(dir) {
        case IZQ:
            desp_x = -1;
            break;
        case DER:
            desp_x = 1;
            break;
        case ARB:
            desp_y = -1;
            break;
        case ABA:
            desp_y = 1;
            break;
    }
    pos_cursor = screen_obtener_pos_cursor(jugador);
    screen_quitar_cursor(jugador);
    screen_ubicar_cursor(jugador, pos_cursor->x + desp_x, pos_cursor->y + desp_y);
}

void game_lanzar(unsigned int jugador) {
    unsigned int queue_idx = game_obtener_queue_idx_jugador(jugador);
    unsigned char* dir_phy_codigo = game_obtener_dir_phy_codigo_jugador(jugador);
    pos* pos_cursor = screen_obtener_pos_cursor(jugador);
    if(*game_vidas(jugador) > 0) {
        if(sched_correr_tarea(queue_idx, dir_phy_codigo, pos_cursor->x, pos_cursor->y)) 
        {
            screen_mapa_imprimir_tarea_infectadora(jugador, pos_cursor->x, pos_cursor->y);
            (*game_vidas(jugador))--;
            screen_actualizar_vidas(jugador, *game_vidas(jugador));
            screen_actualizar_puntajes();
        }
    }
}

void game_soy(unsigned int yoSoy) {
    if(es_jugador(yoSoy)) {
        screen_infectar(yoSoy);
        sched_infectar(yoSoy);
        screen_actualizar_puntajes();
    }
}

void game_donde(unsigned int* pos) {
    pos[0] = sched_info_tarea_actual()->pos_x;
    pos[1] = sched_info_tarea_actual()->pos_y;
}

void game_mapear(int x, int y) {
    screen_mapa_quitar_pagina();
    mmu_mapear_pagina(DIR_LOG_PAGINA_TAREA, rcr3(), (unsigned int) mmu_dir_mapa(x,y), PG_USER | PG_WRITE);
    screen_mapa_ubicar_pagina(x, y);
}

