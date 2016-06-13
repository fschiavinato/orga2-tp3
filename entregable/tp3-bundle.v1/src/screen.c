/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "colors.h"  

pos cursores[CANT_JUGADORES];
ca debajo_cursor[CANT_JUGADORES]; 

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

void print_char(ca* c , unsigned int x, unsigned int y) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    p[y][x].c = c->c ;
    p[y][x].a = c->a;
}

ca* screen_mapa_obtener(unsigned int x, unsigned int y) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    return &p[x][y];
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
        for(j = 0; j < MARCADOR1_BOX_ANCHO; j++)
            p[VIDEO_FILS - 1 - i][MARCADOR1_BOX_OFFSETX + j] = MARCADOR1_BOX_COLOR;

        for(j = 0; j < MARCADOR2_BOX_ANCHO; j++)
            p[VIDEO_FILS - 1 - i][MARCADOR2_BOX_OFFSETX + j] = MARCADOR2_BOX_COLOR;
    }
    
    // Dibujamos los relojes.

    for(i = 0; i < CANT_RELOJES_JUG; i++)
        p[RELOJES1_OFFSETY][RELOJES1_OFFSETX + i*(ESPACIO_ENTRE_RELOJES + 1)].c = 'X';

    print("<A", RELOJES1_OFFSETX + i*(ESPACIO_ENTRE_RELOJES + 1), RELOJES1_OFFSETY, (BORDE_SUPERIOR_COLOR).a);
    
    for(i = 0; i < CANT_RELOJES_JUG; i++)
        p[RELOJES2_OFFSETY][RELOJES2_OFFSETX + i*(ESPACIO_ENTRE_RELOJES + 1)].c = 'X';

    print("B>", RELOJES2_OFFSETX - (ESPACIO_ENTRE_RELOJES + 1) - 1, RELOJES2_OFFSETY, (BORDE_SUPERIOR_COLOR).a);

    for(i = 0; i < CANT_RELOJES_SAN; i++)
        p[RELOJESS_OFFSETY][RELOJESS_OFFSETX + i*(ESPACIO_ENTRE_RELOJES + 1)].c = 'X';

    // Imprimimos las vidas
    

    print("vidas", VIDAS1_OFFSETX, VIDAS1_OFFSETY, (BORDE_SUPERIOR_COLOR).a);
    print("vidas", VIDAS2_OFFSETX, VIDAS2_OFFSETY, (BORDE_SUPERIOR_COLOR).a);
    actualizar_vidas(20, JUGA);
    actualizar_vidas(20, JUGB);

    screen_ubicar_cursor(JUGA, POS_X_DEF_JUGA, POS_X_DEF_JUGA);
    screen_ubicar_cursor(JUGB, POS_X_DEF_JUGB, POS_X_DEF_JUGB);

}

void actualizar_vidas(int vidas, int j) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    switch(j) {
        case JUGA:
            p[VIDAS1_SCORE_OFFSETY][VIDAS1_SCORE_OFFSETX].c = ' ';
            p[VIDAS1_SCORE_OFFSETY][VIDAS1_SCORE_OFFSETX+1].c = ' ';
            print_int_sinattr(vidas, VIDAS1_SCORE_OFFSETX+1, VIDAS1_SCORE_OFFSETY);

            break;
        case JUGB:
            p[VIDAS2_SCORE_OFFSETY][VIDAS2_SCORE_OFFSETX].c = ' ';
            p[VIDAS2_SCORE_OFFSETY][VIDAS2_SCORE_OFFSETX+1].c = ' ';
            print_int_sinattr(vidas, VIDAS2_SCORE_OFFSETX+1, VIDAS2_SCORE_OFFSETY);

            break;
    }
}

void screen_quitar_cursor(int j) {
    switch(j) {
        case JUGA:
            print_char(&debajo_cursor[CURSOR_IDX_JUGA], cursores[CURSOR_IDX_JUGA].x, cursores[CURSOR_IDX_JUGA].y);
            break;
        case JUGB:
            print_char(&debajo_cursor[CURSOR_IDX_JUGB], cursores[CURSOR_IDX_JUGB].x, cursores[CURSOR_IDX_JUGB].y);
            break;
    }

}

void screen_ubicar_cursor(int j, unsigned int x, unsigned int y) {

    unsigned int idx = 0;
    switch(j) {
        case JUGA:
            idx = CURSOR_IDX_JUGA;
            break;
        case JUGB:
            idx = CURSOR_IDX_JUGB;
            break;
    }
    debajo_cursor[idx].c = screen_mapa_obtener(x, y)->c;
    debajo_cursor[idx].a = screen_mapa_obtener(x, y)->a;
    cursores[idx].x = x;
    cursores[idx].y = y;
}

pos* screen_obtener_cursor(int j) {
    pos* res = NULL;
    switch(j) {
        case JUGA:
            res = &cursores[CURSOR_IDX_JUGA];
            break;
        case JUGB:
            res = &cursores[CURSOR_IDX_JUGA];
            break;
    }

    return res;
}

