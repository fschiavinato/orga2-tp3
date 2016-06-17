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
    cursor* cur_j;
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
    cur_j = &screen_obtener_visual_jugador(jugador)->cursor;
    screen_quitar_cursor(cur_j);
    screen_ubicar_cursor(cur_j, cur_j->posicion.x + desp_x, cur_j->posicion.y + desp_y);
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
    tarea* actual = sched_info_tarea_actual();
    actual->virus = yoSoy;
    if(es_jugador(yoSoy)) {
        if(es_jugador(sched_info_queue_actual()->jug)) {
            screen_infectar_infectadora(yoSoy, actual->pos_x, actual->pos_y);
        }
        else {
            screen_infectar_sana(yoSoy, actual->pos_x, actual->pos_y);
        }
        screen_actualizar_puntajes();
    }
}

void game_donde(unsigned int* pos) {
    pos[0] = sched_info_tarea_actual()->pos_x;
    pos[1] = sched_info_tarea_actual()->pos_y;
}

void game_mapear(int x, int y) {
    jugador_visual* v_jug = screen_obtener_visual_jugador(sched_info_tarea_actual()->virus);
    cursor* cur_pag = screen_cursor_pagina_tarea_actual();

    cur_pag->visible.c = v_jug->cursor_pagina_visible.c;

    if(cur_pag->en_mapa == TRUE) {
        screen_quitar_cursor(cur_pag);
    }
    else {
        cur_pag->en_mapa = TRUE;
    }
    mmu_unmapear_pagina(DIR_LOG_PAGINA_TAREA, rcr3());
    mmu_mapear_pagina(DIR_LOG_PAGINA_TAREA, rcr3(), (unsigned int) mmu_dir_mapa(x, y), PG_USER | PG_WRITE);
    screen_ubicar_cursor(cur_pag, x, y);
}
