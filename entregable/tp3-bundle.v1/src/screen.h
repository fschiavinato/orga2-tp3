/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS                      50
#define VIDEO_COLS                      80

#define BORDE_SUPERIOR_ANCHO            1
#define BORDE_SUPERIOR_COLOR            (ca) {0x20, 0x0F}
#define BORDE_INFERIOR_ANCHO            5
#define BORDE_INFERIOR_COLOR            (ca) {0x20, 0x0F}
#define MARCADORA_BOX_OFFSETX           48
#define MARCADORA_BOX_ANCHO             6
#define MARCADORA_BOX_COLOR             (ca) {0x20, C_BG_BLUE | C_FG_WHITE} // fondo azul, letras blancas.
#define MARCADORB_BOX_OFFSETX           (48+MARCADORA_BOX_ANCHO)
#define MARCADORB_BOX_ANCHO             6
#define MARCADORB_BOX_COLOR             (ca) {0x20, C_BG_RED | C_FG_WHITE} // fondo rojo, letras blancas.

#define RELOJESA_OFFSETX                3
#define RELOJESA_OFFSETY                DIST_ABAJO(3)
#define RELOJESB_OFFSETY                DIST_ABAJO(3)
#define RELOJESB_OFFSETX                23
#define RELOJESS_OFFSETX                3
#define RELOJESS_OFFSETY                DIST_ABAJO(1)
#define ESPACIO_ENTRE_RELOJES           1
#define CANT_RELOJES_JUG                5
#define CANT_RELOJES_SAN                15

#define VIDASA_OFFSETX                  41
#define VIDASA_OFFSETY                  DIST_ABAJO(3)
#define VIDASA_SCORE_OFFSETX            (VIDASA_OFFSETX+1)
#define VIDASA_SCORE_OFFSETY            (VIDASA_OFFSETY+2)
#define VIDASB_OFFSETX                  62
#define VIDASB_OFFSETY                  DIST_ABAJO(3)
#define VIDASB_SCORE_OFFSETX            (VIDASB_OFFSETX+1)
#define VIDASB_SCORE_OFFSETY            (VIDASB_OFFSETY+2)

#define DIST_ABAJO(y)                   (VIDEO_FILS - 1 - y)

#define CURSOR_IDX_JUGA                 0
#define CURSOR_IDX_JUGB                 1

#define CURSOR_POS_DEF_JUGA            ((pos){19,22})
#define CURSOR_CA_JUGA                 (ca){'*', C_FG_BLACK}
#define CURSOR_POS_DEF_JUGB            ((pos){59,22})
#define CURSOR_CA_JUGB                 (ca){'*', C_FG_BLACK}


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

pos* screen_obtener_pos_cursor(int j);

unsigned int screen_obtener_idx_cursor(int jug);

#endif  /* !__SCREEN_H__ */
