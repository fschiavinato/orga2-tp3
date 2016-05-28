/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "colors.h"  

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
    actualizar_vidas(20, jugador1);
    actualizar_vidas(20, jugador2);

}

void actualizar_vidas(int vidas, jugador j) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    switch(j) {
        case jugador1:
            p[VIDAS1_SCORE_OFFSETY][VIDAS1_SCORE_OFFSETX].c = ' ';
            p[VIDAS1_SCORE_OFFSETY][VIDAS1_SCORE_OFFSETX+1].c = ' ';
            print_int_sinattr(vidas, VIDAS1_SCORE_OFFSETX+1, VIDAS1_SCORE_OFFSETY);

            break;
        case jugador2:
            p[VIDAS2_SCORE_OFFSETY][VIDAS2_SCORE_OFFSETX].c = ' ';
            p[VIDAS2_SCORE_OFFSETY][VIDAS2_SCORE_OFFSETX+1].c = ' ';
            print_int_sinattr(vidas, VIDAS2_SCORE_OFFSETX+1, VIDAS2_SCORE_OFFSETY);

            break;
    }
}





