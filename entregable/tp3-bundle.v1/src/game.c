/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

void game_mover_cursor(int jugador, direccion dir) {
    unsigned int desp_x = 0;
    unsigned int desp_y = 0;
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
    switch(jugador) {
        case ID_CODE_JUGA:
        case ID_CODE_JUGB:
            pos* pos_cursor = screen_obtener_cursor(jugador);
            screen_quitar_cursor(jugador);
            screen_ubicar_cursor(jugador, pos->x + desp_x , pos->y + desp_y);
    }
}

void game_lanzar(unsigned int jugador) {
    switch(jugador) {
        case ID_CODE_JUGA:
            break;
        case ID_CODE_JUGB:
            break;
    }
}

void game_soy(unsigned int yoSoy) {
}

void game_donde(unsigned int* pos) {
}

void game_mapear(int x, int y) {
}

