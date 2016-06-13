/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#define BORDE_SUPERIOR_ANCHO    1
#define BORDE_SUPERIOR_COLOR    (ca) {0x20, 0x0F}
#define BORDE_INFERIOR_ANCHO    5
#define BORDE_INFERIOR_COLOR    (ca) {0x20, 0x0F}
#define MARCADOR1_BOX_OFFSETX    48
#define MARCADOR1_BOX_ANCHO      6
#define MARCADOR1_BOX_COLOR     (ca) {0x20, C_BG_BLUE | C_FG_WHITE} // fondo azul, letras blancas.
#define MARCADOR2_BOX_OFFSETX    (48+MARCADOR1_BOX_ANCHO)
#define MARCADOR2_BOX_ANCHO      6
#define MARCADOR2_BOX_COLOR     (ca) {0x20, C_BG_RED | C_FG_WHITE} // fondo rojo, letras blancas.

#define RELOJES1_OFFSETX        3
#define RELOJES1_OFFSETY        DIST_ABAJO(3)
#define RELOJES2_OFFSETY        DIST_ABAJO(3)
#define RELOJES2_OFFSETX        23
#define RELOJESS_OFFSETX        3
#define RELOJESS_OFFSETY        DIST_ABAJO(1)
#define ESPACIO_ENTRE_RELOJES   1
#define CANT_RELOJES_JUG        5
#define CANT_RELOJES_SAN        15

#define VIDAS1_OFFSETX          41
#define VIDAS1_OFFSETY          DIST_ABAJO(3)
#define VIDAS1_SCORE_OFFSETX    (VIDAS1_OFFSETX+1)
#define VIDAS1_SCORE_OFFSETY    (VIDAS1_OFFSETY+2)
#define VIDAS2_OFFSETX          62
#define VIDAS2_OFFSETY          DIST_ABAJO(3)
#define VIDAS2_SCORE_OFFSETX    (VIDAS2_OFFSETX+1)
#define VIDAS2_SCORE_OFFSETY    (VIDAS2_OFFSETY+2)

#define DIST_ABAJO(y)   (VIDEO_FILS - 1 - y)

#define CURSOR_IDX_JUGA         0
#define CURSOR_IDX_JUGB         1


#include "colors.h"
#include "defines.h"
#include "game.h"
/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

typedef struct pos_t {
    unsigned int x;
    unsigned int y;
} pos;


void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);

void print_int_sinattr(unsigned int n, unsigned int x, unsigned int y);

void actualizar_vidas(int vidas, int j); 

void screen_quitar_cursor(int j);

void screen_ubicar_cursor(int j, unsigned int x, unsigned int y);

pos* screen_obtener_cursor(int j);

#endif  /* !__SCREEN_H__ */
