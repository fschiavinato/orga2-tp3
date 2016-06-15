/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "colors.h"  

jugador_visual visual_jugadores[CANT_JUGADORES] = {
    [SCREEN_IDX_JUGA] = {
        {
            CURSOR_POS_DEF_JUGA,
            CURSOR_CA_JUGA,
            CURSOR_CA_JUGA
        },
        CA_PAGINA_MAPEADA_JUGA,
        CA_TAREA_INFECTADORA_JUGA,
        CA_TAREA_INFECTADA_JUGA,
        POS_MARCADOR_VIDAS_JUGA

    },
    [SCREEN_IDX_JUGB] = {
        {
            CURSOR_POS_DEF_JUGB,
            CURSOR_CA_JUGB,
            CURSOR_CA_JUGB
        },
        CA_PAGINA_MAPEADA_JUGB,
        CA_TAREA_INFECTADORA_JUGB,
        CA_TAREA_INFECTADA_JUGB,
        POS_MARCADOR_VIDAS_JUGB
    }
};

cursor paginas_mapeadas_visual[MAX_NUM_TAREAS]; // Usamos el mismo indice que en el arreglo de ts.

//-----------------------------------------------------------------------------
// Funciones Auxiliares
//-----------------------------------------------------------------------------

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

void print_char_sinattr(const ca* ch , unsigned int x, unsigned int y) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    p[y][x].c = ch->c;
}

void print_char_sincar(const ca* ch , unsigned int x, unsigned int y) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    p[y][x].a = ch->a;
}

ca* screen_mapa_obtener(unsigned int x, unsigned int y) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    return &p[y + BORDE_SUPERIOR_ANCHO][x];
}

void screen_mapa_imprimir(const ca* ch, unsigned int x, unsigned int y) {
    print_char(ch, x, y + BORDE_SUPERIOR_ANCHO);
}

void screen_mapa_imprimir_sinattr(const ca* ch, unsigned int x, unsigned int y) {
    print_char_sinattr(ch, x, y + BORDE_SUPERIOR_ANCHO);
}

void screen_mapa_imprimir_sincar(const ca* ch, unsigned int x, unsigned int y) {
    print_char_sincar(ch, x, y + BORDE_SUPERIOR_ANCHO);
}

void screen_ubicar_debajo_cursores(const ca* ch, unsigned int x, unsigned int y) {
    int i = 0;
    for(; i < CANT_JUGADORES; i++) {
        if(visual_jugadores[i].cursor.posicion.x == x && visual_jugadores[i].cursor.posicion.y == y) {
            visual_jugadores[i].cursor.abajo.c = ch->c; 
            visual_jugadores[i].cursor.abajo.a = ch->a;
        }  
    }
}

void screen_ubicar_debajo_paginas_mapeadas(const ca* ch, unsigned int x, unsigned int y) {
    int i = 0;
    for(; i < MAX_NUM_TAREAS; i++) {
        if(paginas_mapeadas_visual[i].posicion.x == x && paginas_mapeadas_visual[i].posicion.y == y) {
            paginas_mapeadas_visual[i].abajo.c = ch->c; 
            paginas_mapeadas_visual[i].abajo.a = ch->a;
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

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
    screen_actualizar_vidas(20, JUGA);
    screen_actualizar_vidas(20, JUGB);

    screen_ubicar_cursor(JUGA, CURSOR_POS_DEF_JUGA.x, CURSOR_POS_DEF_JUGA.y);
    screen_ubicar_cursor(JUGB, CURSOR_POS_DEF_JUGB.x, CURSOR_POS_DEF_JUGB.y);
}

void screen_actualizar_vidas(int jug, int vidas) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    pos* offset = &screen_obtener_visual_jugador(jug)->marcador_vidas;
    p[offset->y][offset->x].c = ' ';
    p[offset->y][offset->x+1].c = ' ';
    print_int_sinattr(vidas, offset->x+1, offset->y);
}

void screen_actualizar_puntajes() {

}

void screen_quitar_cursor(int jug) {
    jugador_visual* v_jug  = screen_obtener_visual_jugador(jug);
    unsigned int i = 0;
    for(; i < CANT_JUGADORES && (visual_jugadores[i].cursor.posicion.x != v_jug->cursor.posicion.x || visual_jugadores[i].cursor.posicion.y != v_jug->cursor.posicion.y || &visual_jugadores[i] == v_jug); i++);
    if(i == CANT_JUGADORES) {
        screen_mapa_imprimir(&v_jug->cursor.abajo, v_jug->cursor.posicion.x, v_jug->cursor.posicion.y);
    }
    else {
        screen_mapa_imprimir(&visual_jugadores[i].cursor.visible, v_jug->cursor.posicion.x, v_jug->cursor.posicion.y);
    }
}

void screen_ubicar_cursor(int jug, unsigned int x, unsigned int y) {
    jugador_visual* v_jug = screen_obtener_visual_jugador(jug);
    int i = 0;
    for(; i < CANT_JUGADORES && (visual_jugadores[i].cursor.posicion.x != x || visual_jugadores[i].cursor.posicion.y != y || &visual_jugadores[i] == v_jug); i++);
    if(i == CANT_JUGADORES) {
        v_jug->cursor.abajo.c = screen_mapa_obtener(x, y)->c;
        v_jug->cursor.abajo.a = screen_mapa_obtener(x, y)->a;
        v_jug->cursor.visible.a = screen_mapa_obtener(x, y)->a;
    }
    else {
        v_jug->cursor.abajo.c = visual_jugadores[i].cursor.abajo.c;
        v_jug->cursor.abajo.a = visual_jugadores[i].cursor.abajo.a;
        v_jug->cursor.visible.a = visual_jugadores[i].cursor.visible.a;
    }
    v_jug->cursor.posicion.x = x % ANCHO_MAPA;
    v_jug->cursor.posicion.y = y % ALTO_MAPA;
    screen_mapa_imprimir(&v_jug->cursor.visible, v_jug->cursor.posicion.x, v_jug->cursor.posicion.y);
}

pos* screen_obtener_pos_cursor(int jug) {

    return &screen_obtener_visual_jugador(jug)->cursor.posicion;
}

jugador_visual* screen_obtener_visual_jugador(int jug) {
    unsigned int idx = 0;
    switch(jug) {
        case JUGA:
            idx = SCREEN_IDX_JUGA;
            break;
        case JUGB:
            idx = SCREEN_IDX_JUGB;
            break;
    }
    return &visual_jugadores[idx];
}

void screen_mapa_ubicar_pagina(unsigned int x, unsigned int y) {
    cursor* pmv = &paginas_mapeadas_visual[sched_info_tarea_actual()->ts_idx];
    unsigned int i = 0;
    for(; i < MAX_NUM_TAREAS && (paginas_mapeadas_visual[i].posicion.x != pmv->posicion.x || paginas_mapeadas_visual[i].posicion.y != pmv->posicion.y || &paginas_mapeadas_visual[i] == pmv); i++);
    if(i < MAX_NUM_TAREAS) {
        pmv->abajo.c = paginas_mapeadas_visual[i].abajo.c;
        pmv->abajo.a = paginas_mapeadas_visual[i].abajo.a;
    }
    else {
        pmv->abajo.c = screen_mapa_obtener(x, y)->c; 
        pmv->abajo.a = screen_mapa_obtener(x, y)->a;
    }
    screen_ubicar_debajo_cursores(&pmv->visible, x, y);

    for(; i < CANT_JUGADORES && (visual_jugadores[i].cursor.posicion.x != pmv->posicion.x || visual_jugadores[i].cursor.posicion.y != pmv->posicion.y); i++);
    if(i == CANT_JUGADORES) {
        screen_mapa_imprimir_sinattr(&pmv->visible, pmv->posicion.x, pmv->posicion.y);
    }
}

void screen_mapa_quitar_pagina() {
    cursor* pmv = &paginas_mapeadas_visual[sched_info_tarea_actual()->ts_idx];
    ca* nuevo_ca = &pmv->abajo;
    unsigned int i = 0;
    for(; i < MAX_NUM_TAREAS && (paginas_mapeadas_visual[i].posicion.x != pmv->posicion.x || paginas_mapeadas_visual[i].posicion.y != pmv->posicion.y || &paginas_mapeadas_visual[i] == pmv); i++);
    if(i < MAX_NUM_TAREAS) {
        nuevo_ca = &paginas_mapeadas_visual[i].visible;
    }
    screen_ubicar_debajo_cursores(nuevo_ca, pmv->posicion.x, pmv->posicion.y);

    for(; i < CANT_JUGADORES && (visual_jugadores[i].cursor.posicion.x != pmv->posicion.x || visual_jugadores[i].cursor.posicion.y != pmv->posicion.y); i++);
    if(i == CANT_JUGADORES) {
        screen_mapa_imprimir_sinattr(nuevo_ca, pmv->posicion.x, pmv->posicion.y);
    }
}

void screen_mapa_imprimir_tarea_infectadora(int jug, unsigned int x, unsigned int y) {
    screen_mapa_imprimir(&screen_obtener_visual_jugador(jug)->tarea_infectadora, x, y);
    screen_ubicar_debajo_cursores(&screen_obtener_visual_jugador(jug)->tarea_infectadora, x, y);
    screen_ubicar_debajo_paginas_mapeadas(&CA_TAREA_SANA, x, y);
}

void screen_infectar(int jug) {
    screen_mapa_imprimir_sinattr(&screen_obtener_visual_jugador(jug)->tarea_infectada, sched_info_tarea_actual()->pos_x, sched_info_tarea_actual()->pos_y);
    screen_ubicar_debajo_cursores(&screen_obtener_visual_jugador(jug)->tarea_infectada, sched_info_tarea_actual()->pos_x, sched_info_tarea_actual()->pos_y);
    screen_ubicar_debajo_paginas_mapeadas(&CA_TAREA_SANA, sched_info_tarea_actual()->pos_x, sched_info_tarea_actual()->pos_y);
}

void screen_mapa_imprimir_tarea_sana(unsigned int x, unsigned int y) {
    screen_mapa_imprimir_sincar(&CA_TAREA_SANA, x, y);
    screen_ubicar_debajo_cursores(&CA_TAREA_SANA, x, y);
    screen_ubicar_debajo_paginas_mapeadas(&CA_TAREA_SANA, x, y);
}
