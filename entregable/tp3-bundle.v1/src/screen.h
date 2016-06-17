/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS                          50
#define VIDEO_COLS                          80

#define BORDE_SUPERIOR_ANCHO                1
#define BORDE_SUPERIOR_COLOR                (ca) {0x20, 0x0F}
#define BORDE_INFERIOR_ANCHO                5
#define BORDE_INFERIOR_COLOR                (ca) {0x20, 0x0F}
#define MARCADORA_BOX_OFFSETX               48
#define MARCADORA_BOX_ANCHO                 6
#define MARCADORA_BOX_COLOR                 (ca) {0x20, C_BG_BLUE | C_FG_WHITE} // fondo azul, letras blancas.
#define MARCADORA_OFFSETX                   MARCADORA_BOX_OFFSETX+2
#define MARCADORA_OFFSETY                   DIST_ABAJO(2)
#define POS_MARCADOR_JUGA                   (pos){MARCADORA_OFFSETX, MARCADORA_OFFSETY}
#define MARCADORB_BOX_OFFSETX               (48+MARCADORA_BOX_ANCHO)
#define MARCADORB_BOX_ANCHO                 6
#define MARCADORB_BOX_COLOR                 (ca) {0x20, C_BG_RED | C_FG_WHITE} // fondo rojo, letras blancas.
#define MARCADORB_OFFSETX                   MARCADORB_BOX_OFFSETX+2
#define MARCADORB_OFFSETY                   DIST_ABAJO(2)
#define POS_MARCADOR_JUGB                   (pos){MARCADORB_OFFSETX, MARCADORB_OFFSETY}

#define RELOJESA_OFFSETX                    3
#define RELOJESA_OFFSETY                    DIST_ABAJO(3)
#define RELOJESB_OFFSETY                    DIST_ABAJO(3)
#define RELOJESB_OFFSETX                    23
#define RELOJESS_OFFSETX                    3
#define RELOJESS_OFFSETY                    DIST_ABAJO(1)
#define ESPACIO_ENTRE_RELOJES               1
#define CANT_RELOJES_JUG                    5
#define CANT_RELOJES_SAN                    15

#define VIDASA_OFFSETX                      41
#define VIDASA_OFFSETY                      DIST_ABAJO(3)
#define VIDASA_SCORE_OFFSETX                (VIDASA_OFFSETX+1)
#define VIDASA_SCORE_OFFSETY                (VIDASA_OFFSETY+2)
#define VIDASB_OFFSETX                      62
#define VIDASB_OFFSETY                      DIST_ABAJO(3)
#define VIDASB_SCORE_OFFSETX                (VIDASB_OFFSETX+1)
#define VIDASB_SCORE_OFFSETY                (VIDASB_OFFSETY+2)

#define DIST_ABAJO(y)                       (VIDEO_FILS - 1 - y)

#define SCREEN_IDX_JUGA                     0
#define SCREEN_IDX_JUGB                     1

#define CURSOR_POS_DEF_JUGA                 ((pos){19,22})
#define CURSOR_CA_JUGA                      (ca){'*', C_FG_BLACK}
#define CURSOR_POS_DEF_JUGB                 ((pos){59,22})
#define CURSOR_CA_JUGB                      (ca){'*', C_FG_BLACK}

#define POS_MARCADOR_VIDAS_JUGA             (pos){VIDASA_SCORE_OFFSETX, VIDASA_SCORE_OFFSETY}
#define POS_MARCADOR_VIDAS_JUGB             (pos){VIDASB_SCORE_OFFSETX, VIDASB_SCORE_OFFSETY}

#define CA_TAREA_INFECTADORA_JUGA           (ca){'*', C_BG_LIGHT_GREY | C_FG_BLUE}
#define CA_TAREA_INFECTADORA_JUGB           (ca){'*', C_BG_LIGHT_GREY | C_FG_RED}

#define CA_TAREA_INFECTADA_JUGA             (ca){' ', C_BG_BLUE | C_FG_BLACK}
#define CA_TAREA_INFECTADA_JUGB             (ca){' ', C_BG_RED | C_FG_BLACK}
#define CA_TAREA_SANA                       (ca){' ', C_BG_GREEN | C_FG_BLACK}

#define CA_PAGINA_MAPEADA_JUGA              (ca){'A', C_BG_GREEN | C_FG_BLACK}
#define CA_PAGINA_MAPEADA_JUGB              (ca){'B', C_BG_GREEN | C_FG_BLACK}

#define CA_MAPA                             (ca){' ', C_BG_LIGHT_GREY | C_FG_BLACK}

//defines debug

#define DEBUG_ANCHO                         (54 - DEBUG_OFFSETX)
#define DEBUG_ALTO                          (42 - DEBUG_OFFSETY)
#define DEBUG_OFFSETX                       24
#define DEBUG_OFFSETY                       6
#define DEBUG_MARCO_INF_SIZE                1
#define DEBUG_MARCO_IZQ_SIZE                1
#define DEBUG_MARCO_SUP_SIZE                1
#define DEBUG_MARCO_DER_SIZE                1
#define DEBUG_MARCO_COLOR                   (ca) {' ', C_BG_BLACK | C_FG_WHITE}
#define DEBUG_TITULO_SIZE                   1
#define DEBUG_TITULO_COLOR                  (ca) {' ', C_BG_RED | C_FG_WHITE}
#define DEBUG_REGISTER_COLOR                (ca) {' ', C_BG_LIGHT_GREY | C_FG_WHITE}
#define DEBUG_REGISTER_TITLE_COLOR          (ca) {' ', C_BG_LIGHT_GREY | C_FG_BLACK}
#define DEBUG_COL1_OFFSETX                  26
#define DEBUG_COL1_OFFSETY                  DEBUG_OFFSETY+3
#define DEBUG_COL2_OFFSETX                  40
#define DEBUG_COL2_OFFSETY                  DEBUG_OFFSETY+3
#define DEBUG_ESPACIO_ENTRADAS              2


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

typedef struct cursor_t {
    pos posicion;
    ca visible;
    ca abajo; // guardamos lo que está debajo del cursor.
    unsigned char en_mapa;
} cursor;

typedef struct jugador_visual_t {
    cursor cursor;
    ca cursor_pagina_visible;
    ca tarea_infectadora;
    ca tarea_infectada;
    pos marcador_vidas;
    pos marcador;
    unsigned int jug;
} jugador_visual;

typedef struct  store_screen{
    ca sto_screen[VIDEO_FILS][VIDEO_COLS];
} store_screen;


void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);

void print_int_sinattr(unsigned int n, unsigned int x, unsigned int y);

void screen_actualizar_vidas(int j, unsigned int vidas);

void screen_actualizar_puntajes();

void screen_actualizar_reloj(unsigned int queue_idx, unsigned int tarea_idx, unsigned int numero_reloj);

void screen_quitar_cursor(cursor* cur);

void screen_ubicar_cursor(cursor* cur, unsigned int x, unsigned int y);

pos* screen_obtener_pos_cursor(int j);

cursor* screen_cursor_pagina_tarea_actual();

jugador_visual* screen_obtener_visual_jugador(int jug);

void screen_mapa_imprimir_pagina(int jug, unsigned int x, unsigned int y);

void screen_mapa_imprimir_tarea_infectadora(int jug, unsigned int x, unsigned int y);

void screen_infectar_sana(int jug, unsigned int x, unsigned int y);

void screen_infectar_infectadora(int jug, unsigned int x, unsigned int y);

void screen_matar(unsigned int x, unsigned int y);

void screen_mapa_imprimir_tarea_sana(unsigned int x, unsigned int y);

void screen_mapa_ubicar_pagina(unsigned int x, unsigned int y);

void screen_mapa_quitar_pagina();

void screen_mostrar_consola_debug(unsigned int* info);

void screen_ocultar_consola_debug();

#endif  /* !__SCREEN_H__ */
