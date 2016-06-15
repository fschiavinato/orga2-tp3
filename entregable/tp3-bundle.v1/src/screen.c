/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "colors.h"  

typedef struct cursor_t {
    pos posicion;
    ca visible;
    ca abajo; // guardamos lo que está debajo del cursor.
} cursor;

cursor cursores[CANT_JUGADORES] = {
    [CURSOR_IDX_JUGA] = {
        CURSOR_POS_DEF_JUGA,
        CURSOR_CA_JUGA,
        CURSOR_CA_JUGA
    },
    [CURSOR_IDX_JUGB] = {
        CURSOR_POS_DEF_JUGB,
        CURSOR_CA_JUGB,
        CURSOR_CA_JUGB
    }
};

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_char(const ca* ch , unsigned int x, unsigned int y) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    p[y][x].c = ch->c;
    p[y][x].a = ch->a;
}

ca* screen_mapa_obtener(unsigned int x, unsigned int y) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    return &p[y][x];
}


void screen_mapa_imprimir(const ca* ch, unsigned int x, unsigned int y) {
    print_char(ch, x, y + BORDE_SUPERIOR_ANCHO);
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {   
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}

void print_int_sinattr(unsigned int n, unsigned int x, unsigned int y) {   
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int_sinattr(a,x-1,y);
    }
    p[y][x].c = '0'+n;
}

void imprimir_pantalla(){
    int i, j;
    // Dibujamos los bordes.
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;

    for(i = 0; i < BORDE_SUPERIOR_ANCHO; i++)
        for(j = 0; j < VIDEO_COLS; j++)
            p[i][j] = BORDE_SUPERIOR_COLOR;

    for(i = 0; i < BORDE_INFERIOR_ANCHO; i++)
        for(j = 0; j < VIDEO_COLS; j++)
            p[VIDEO_FILS - 1 - i][j] = BORDE_INFERIOR_COLOR;

    // Dibujamos los marcadores.
    for(i = 0; i < BORDE_INFERIOR_ANCHO; i++) {
        for(j = 0; j < MARCADORA_BOX_ANCHO; j++)
            p[VIDEO_FILS - 1 - i][MARCADORA_BOX_OFFSETX + j] = MARCADORA_BOX_COLOR;

        for(j = 0; j < MARCADORB_BOX_ANCHO; j++)
            p[VIDEO_FILS - 1 - i][MARCADORB_BOX_OFFSETX + j] = MARCADORB_BOX_COLOR;
    }
    
    // Dibujamos los relojes.

    for(i = 0; i < CANT_RELOJES_JUG; i++)
        p[RELOJESA_OFFSETY][RELOJESA_OFFSETX + i*(ESPACIO_ENTRE_RELOJES + 1)].c = 'X';

    print("<A", RELOJESA_OFFSETX + i*(ESPACIO_ENTRE_RELOJES + 1), RELOJESA_OFFSETY, (BORDE_SUPERIOR_COLOR).a);
    
    for(i = 0; i < CANT_RELOJES_JUG; i++)
        p[RELOJESB_OFFSETY][RELOJESB_OFFSETX + i*(ESPACIO_ENTRE_RELOJES + 1)].c = 'X';

    print("B>", RELOJESB_OFFSETX - (ESPACIO_ENTRE_RELOJES + 1) - 1, RELOJESB_OFFSETY, (BORDE_SUPERIOR_COLOR).a);

    for(i = 0; i < CANT_RELOJES_SAN; i++)
        p[RELOJESS_OFFSETY][RELOJESS_OFFSETX + i*(ESPACIO_ENTRE_RELOJES + 1)].c = 'X';

    // Imprimimos las vidas
    

    print("vidas", VIDASA_OFFSETX, VIDASA_OFFSETY, (BORDE_SUPERIOR_COLOR).a);
    print("vidas", VIDASB_OFFSETX, VIDASB_OFFSETY, (BORDE_SUPERIOR_COLOR).a);
    actualizar_vidas(20, JUGA);
    actualizar_vidas(20, JUGB);

    screen_ubicar_cursor(JUGA, CURSOR_POS_DEF_JUGA.x, CURSOR_POS_DEF_JUGA.y);
    screen_ubicar_cursor(JUGB, CURSOR_POS_DEF_JUGB.x, CURSOR_POS_DEF_JUGB.y);
}

void actualizar_vidas(int vidas, int j) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    unsigned register int offset_x = 0;
    unsigned register int offset_y = 0;
    switch(j) {
        case JUGA:
            offset_x = VIDASA_SCORE_OFFSETX;
            offset_y = VIDASA_SCORE_OFFSETY;

            break;
        case JUGB:
            offset_x = VIDASB_SCORE_OFFSETX;
            offset_y = VIDASB_SCORE_OFFSETY;
            break;
    }
    p[offset_y][offset_x].c = ' ';
    p[offset_y][offset_x+1].c = ' ';
    print_int_sinattr(vidas, offset_x+1, offset_y);
}

void screen_quitar_cursor(int jug) {
    unsigned int idx = screen_obtener_idx_cursor(jug);
    unsigned int i = 0;
    for(; i < CANT_JUGADORES && (cursores[i].posicion.x != cursores[idx].posicion.x || cursores[i].posicion.y != cursores[idx].posicion.y || i == idx); i++);
    if(i == CANT_JUGADORES) {
        screen_mapa_imprimir(&cursores[idx].abajo, cursores[idx].posicion.x, cursores[idx].posicion.y);
    }
    else {
        screen_mapa_imprimir(&cursores[i].visible, cursores[idx].posicion.x, cursores[idx].posicion.y);
    }
}

void screen_ubicar_cursor(int jug, unsigned int x, unsigned int y) {

    unsigned int idx = screen_obtener_idx_cursor(jug);
    int i = 0;
    for(; i < CANT_JUGADORES && (cursores[i].posicion.x != x || cursores[i].posicion.y != y || i == idx); i++);
    if(i == CANT_JUGADORES) {
        cursores[idx].abajo.c = screen_mapa_obtener(x, y)->c;
        cursores[idx].abajo.a = screen_mapa_obtener(x, y)->a;
        cursores[idx].visible.a = screen_mapa_obtener(x, y)->a;
    }
    else {
        cursores[idx].abajo.c = cursores[i].abajo.c;
        cursores[idx].abajo.a = cursores[i].abajo.a;
        cursores[idx].visible.a = cursores[i].visible.a;
    }
    cursores[idx].posicion.x = x;
    cursores[idx].posicion.y = y;
    screen_mapa_imprimir(&cursores[idx].visible, x, y);
}

pos* screen_obtener_pos_cursor(int jug) {
    pos* res = NULL;
    switch(jug) {
        case JUGA:
            res = &cursores[CURSOR_IDX_JUGA].posicion;
            break;
        case JUGB:
            res = &cursores[CURSOR_IDX_JUGB].posicion;
            break;
    }
    return res;
}

unsigned int screen_obtener_idx_cursor(int jug) {
    unsigned int res = 0;
    switch(jug) {
        case JUGA:
            res = CURSOR_IDX_JUGA;
            break;
        case JUGB:
            res = CURSOR_IDX_JUGB;
            break;
    }
    return res;
}

