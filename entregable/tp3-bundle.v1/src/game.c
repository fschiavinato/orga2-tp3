/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

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
    if(sched_correr_tarea(queue_idx, dir_phy_codigo, pos_cursor->x, pos_cursor->y)) 
            screen_mapa_imprimir_tarea_infectadora(jugador, pos_cursor->x, pos_cursor->y);

}

void game_soy(unsigned int yoSoy) {
}

void game_donde(unsigned int* pos) {
}

void game_mapear(int x, int y) {
}

