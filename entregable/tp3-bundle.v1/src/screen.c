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
        POS_MARCADOR_VIDAS_JUGA,
        POS_MARCADOR_JUGA,
        JUGA

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
        POS_MARCADOR_VIDAS_JUGB,
        POS_MARCADOR_JUGB,
        JUGB
    }
};

cursor cursores_paginas[MAX_NUM_TAREAS]; // Usamos el mismo indice que en el arreglo de ts.

unsigned char estados_relojes[CANT_ESTADOS_RELOJ+1] = {'\\','|','/','-', 'X'};

ca backup_pantalla[DEBUG_ALTO][DEBUG_ALTO];

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


void mapa_cambiar_color_fondo(const ca* ch, unsigned int x, unsigned int y) {
    ca* punto = screen_mapa_obtener(x, y);
    punto->a = C_GET_BG(ch->a) | C_GET_FG(punto->a);
    int i = 0;
    for(; i < CANT_JUGADORES; i++) {
        if(visual_jugadores[i].cursor.posicion.x == x && visual_jugadores[i].cursor.posicion.y == y) {
            visual_jugadores[i].cursor.abajo.a = punto->a;
        }
    }
    for(; i < MAX_NUM_TAREAS; i++) {
        if(cursores_paginas[i].posicion.x == x && cursores_paginas[i].posicion.y == y) {
            cursores_paginas[i].abajo.a = punto->a;
        }
    }
}

void mapa_cambiar_color_frente(const ca* ch, unsigned int x, unsigned int y) {
    ca* punto = screen_mapa_obtener(x, y);
    int i = 0;
    for(; i < CANT_JUGADORES; i++) {
        if(visual_jugadores[i].cursor.posicion.x == x && visual_jugadores[i].cursor.posicion.y == y) {
            visual_jugadores[i].cursor.abajo.a = C_GET_FG(ch->a) | C_GET_BG(punto->a);
        }
    }
    for(; i < MAX_NUM_TAREAS; i++) {
        if(cursores_paginas[i].posicion.x == x && cursores_paginas[i].posicion.y == y) {
            cursores_paginas[i].abajo.a = C_GET_FG(ch->a) | C_GET_BG(punto->a);
        }
    }

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
    for(i = 0; i < CANT_JUGADORES; i++) {
        if(visual_jugadores[i].cursor.posicion.x == x && visual_jugadores[i].cursor.posicion.y == y) {
            visual_jugadores[i].cursor.abajo.c = ch->c;
            visual_jugadores[i].cursor.abajo.a = ch->a;
        }
    }
    for(i = 0; i < MAX_NUM_TAREAS; i++) {
        if(cursores_paginas[i].posicion.x == x && cursores_paginas[i].posicion.y == y) {
            cursores_paginas[i].abajo.c = ch->c;
            cursores_paginas[i].abajo.a = ch->a;
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
    screen_actualizar_vidas(JUGA, CANT_VIDAS);
    screen_actualizar_vidas(JUGB, CANT_VIDAS);

    screen_ubicar_cursor(&screen_obtener_visual_jugador(JUGA)->cursor, CURSOR_POS_DEF_JUGA.x, CURSOR_POS_DEF_JUGA.y);
    screen_ubicar_cursor(&screen_obtener_visual_jugador(JUGB)->cursor, CURSOR_POS_DEF_JUGB.x, CURSOR_POS_DEF_JUGB.y);
}

void screen_actualizar_vidas(int jug, unsigned int vidas) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    pos* offset = &screen_obtener_visual_jugador(jug)->marcador_vidas;
    p[offset->y][offset->x].c = ' ';
    p[offset->y][offset->x+1].c = ' ';
    print_int_sinattr(vidas, offset->x+1, offset->y);
}

void screen_actualizar_puntajes() {
    int i = 0;
    for(; i < CANT_JUGADORES; i++) {

        ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
        unsigned int puntos = sched_infectados(visual_jugadores[i].jug);
        p[visual_jugadores[i].marcador.y][visual_jugadores[i].marcador.x].c = ' ';
        p[visual_jugadores[i].marcador.y][visual_jugadores[i].marcador.x+1].c = ' ';
        print_int_sinattr(puntos, visual_jugadores[i].marcador.x+1, visual_jugadores[i].marcador.y);
    }
}

void screen_actualizar_reloj(unsigned int queue_idx, unsigned int tarea_idx, unsigned int estado_reloj) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    switch(queue_idx) {
        case SCHED_QUEUE_IDX_JUGA:
            p[RELOJESA_OFFSETY][RELOJESA_OFFSETX + tarea_idx*(ESPACIO_ENTRE_RELOJES + 1)].c = estados_relojes[estado_reloj];
            break;
        case SCHED_QUEUE_IDX_JUGB:
            p[RELOJESB_OFFSETY][RELOJESB_OFFSETX + tarea_idx*(ESPACIO_ENTRE_RELOJES + 1)].c = estados_relojes[estado_reloj];
            break;
        case SCHED_QUEUE_IDX_SANAS:
            p[RELOJESS_OFFSETY][RELOJESS_OFFSETX + tarea_idx*(ESPACIO_ENTRE_RELOJES + 1)].c = estados_relojes[estado_reloj];
            break;
    }
}

cursor* cursor_distinto_misma_posicion(cursor* cur) {
  unsigned int i = 0;
  cursor *res = NULL;
  for( i = 0; i < CANT_JUGADORES && res == NULL; i++)
      if(visual_jugadores[i].cursor.posicion.x == cur->posicion.x &&
          visual_jugadores[i].cursor.posicion.y == cur->posicion.y &&
          &visual_jugadores[i].cursor != cur &&
          visual_jugadores[i].cursor.en_mapa == TRUE
        )
          res = &visual_jugadores[i].cursor;

  for(i = 0; i < MAX_NUM_TAREAS && res == NULL; i++)
      if(cursores_paginas[i].posicion.x == cur->posicion.x &&
        cursores_paginas[i].posicion.y == cur->posicion.y &&
        &cursores_paginas[i] != cur &&
        cursores_paginas[i].en_mapa == TRUE
      )
          res = &cursores_paginas[i];
  return res;
}


void screen_quitar_cursor(cursor* cur) {
    cursor* cur_misma_pos = cursor_distinto_misma_posicion(cur);

    if(cur_misma_pos == NULL) {
        screen_mapa_imprimir(&cur->abajo, cur->posicion.x, cur->posicion.y);
    }
    else {
        screen_mapa_imprimir_sinattr(&cur_misma_pos->visible, cur->posicion.x, cur->posicion.y);
    }
    cur->en_mapa = FALSE;
}

void screen_ubicar_cursor(cursor* cur, unsigned int x, unsigned int y) {
    x %= ANCHO_MAPA;
    y %= ALTO_MAPA;
    cur->posicion.x = x;
    cur->posicion.y = y;

    cursor* cur_misma_pos = cursor_distinto_misma_posicion(cur);

    if(cur_misma_pos == NULL) {
        cur->abajo.c = screen_mapa_obtener(x, y)->c;
        cur->abajo.a = screen_mapa_obtener(x, y)->a;
        cur->visible.a = cur->visible.a | C_GET_BG(screen_mapa_obtener(x, y)->a);

    }
    else {
        cur->abajo.c = cur_misma_pos->abajo.c;
        cur->abajo.a = cur_misma_pos->abajo.a;
        cur->visible.a = cur_misma_pos->visible.a;
    }

    cur->en_mapa = TRUE;
    screen_mapa_imprimir(&cur->visible, cur->posicion.x, cur->posicion.y);
}

pos* screen_obtener_pos_cursor(int jug) {
    return &screen_obtener_visual_jugador(jug)->cursor.posicion;
}

cursor* screen_cursor_pagina_tarea_actual() {
    return &cursores_paginas[sched_info_tarea_actual()->ts_idx];

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

void screen_mapa_imprimir_tarea_infectadora(int jug, unsigned int x, unsigned int y) {
    screen_mapa_imprimir(&screen_obtener_visual_jugador(jug)->tarea_infectadora, x, y);
    screen_ubicar_debajo_cursores(&screen_obtener_visual_jugador(jug)->tarea_infectadora, x, y);
}

void screen_infectar_sana(int jug, unsigned int x, unsigned int y) {
    jugador_visual* v_jug = screen_obtener_visual_jugador(jug);
    mapa_cambiar_color_fondo(&v_jug->tarea_infectada, x, y);
}

void screen_infectar_infectadora(int jug, unsigned int x, unsigned int y) {
    jugador_visual* v_jug = screen_obtener_visual_jugador(jug);
    mapa_cambiar_color_frente(&v_jug->tarea_infectada, x, y);
}

void screen_matar(unsigned int x, unsigned int y) {
  screen_mapa_imprimir(&CA_MAPA, x, y);
  screen_ubicar_debajo_cursores(&CA_MAPA, x, y);
}

void screen_mapa_imprimir_tarea_sana(unsigned int x, unsigned int y) {
    screen_mapa_imprimir_sincar(&CA_TAREA_SANA, x, y);
    screen_ubicar_debajo_cursores(&CA_TAREA_SANA, x, y);
}

 void screen_mostrar_consola_debug(unsigned int* info) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for( i = 0; i < DEBUG_ALTO; i++) {
        int j;
      for( j = 0; j < DEBUG_ANCHO; j++) {
        backup_pantalla[i][j].c = p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].c;
        backup_pantalla[i][j].a = p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].a;
        p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].c = CA_MAPA.c;
        p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].a = CA_MAPA.a;
      }
    }
    // Imprimimos el marco.
    for( i = 0; i < DEBUG_MARCO_INF_SIZE; i++) {
        int j;
      for( j = 0; j < DEBUG_ANCHO; j++) {
        p[DEBUG_OFFSETY + DEBUG_ALTO - 1 + i][DEBUG_OFFSETX + j].c = DEBUG_MARCO_COLOR.c;
        p[DEBUG_OFFSETY + DEBUG_ALTO - 1 + i][DEBUG_OFFSETX + j].a = DEBUG_MARCO_COLOR.a;
      }
    }
    for( i = 0; i < DEBUG_MARCO_SUP_SIZE; i++) {
        int j;
      for( j = 0; j < DEBUG_ANCHO; j++) {
        p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].c = DEBUG_MARCO_COLOR.c;
        p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].a = DEBUG_MARCO_COLOR.a;
      }
    }
    for( i = 0; i < DEBUG_ALTO; i++) {
        int j;
      for( j = 0; j < DEBUG_MARCO_DER_SIZE; j++) {
        p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + DEBUG_ANCHO - 1 - j].c = DEBUG_MARCO_COLOR.c;
        p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + DEBUG_ANCHO - 1 - j].a = DEBUG_MARCO_COLOR.a;
      }
    }
    for( i = 0; i < DEBUG_ALTO; i++) {
        int j;
      for( j = 0; j < DEBUG_MARCO_IZQ_SIZE; j++) {
        p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].c = DEBUG_MARCO_COLOR.c;
        p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].a = DEBUG_MARCO_COLOR.a;
      }
    }

    for( i = 0; i < DEBUG_TITULO_SIZE; i++) {
        int j;
      for(j = 0; j < DEBUG_ANCHO - 2; j++) {
        p[DEBUG_OFFSETY + 1 + i][DEBUG_OFFSETX + 1 + j].c = DEBUG_TITULO_COLOR.c;
        p[DEBUG_OFFSETY + 1 + i][DEBUG_OFFSETX + 1 + j].a = DEBUG_TITULO_COLOR.a;
      }
    }

    // INFO -> | cr4     | 0
    //         | cr3     | 1
    //         | cr2     | 2
    //         | cr0     | 3
    //         | gs      | 4
    //         | fs      | 5
    //         | es      | 6
    //         | ds      | 7
    //         | EDI     | 8
    //         | ESI     | 9
    //         | EBP0    | 10
    //         | ESP0    | 11
    //         | EBX     | 12
    //         | EDX     | 13
    //         | ECX     | 14
    //         | EAX     | 15
    //         | EIP     | 16
    //         | CS      | 17
    //         | EFLAGS  | 18
    //         | ESP     | 19
    //         | SS      | 20



    print("eax", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_EAX_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY, DEBUG_REGISTER_COLOR.a);

    print("ebx", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 1*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_EBX_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY + 1*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("ecx", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 2*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_ECX_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY + 2*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("edx", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 3*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_EDX_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY + 3*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("esi", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 4*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_EBX_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY + 4*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("edi", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 5*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_EDI_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY + 5*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("ebp", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 6*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_EBP_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY + 6*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("esp", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 7*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_ESP_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY + 7*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("eip", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 8*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_EIP_IDX], 8, DEBUG_COL1_OFFSETX + 4, DEBUG_COL1_OFFSETY + 8*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("cs", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 9*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_CS_IDX], 4, DEBUG_COL1_OFFSETX + 3, DEBUG_COL1_OFFSETY + 9*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("ds", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 10*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_DS_IDX], 4, DEBUG_COL1_OFFSETX + 3, DEBUG_COL1_OFFSETY + 10*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("es", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 11*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_ES_IDX], 4, DEBUG_COL1_OFFSETX + 3, DEBUG_COL1_OFFSETY + 11*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("fs", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 12*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_FS_IDX], 4, DEBUG_COL1_OFFSETX + 3, DEBUG_COL1_OFFSETY + 12*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("gs", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 13*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_GS_IDX], 4, DEBUG_COL1_OFFSETX + 3, DEBUG_COL1_OFFSETY + 13*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("ss", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 14*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_SS_IDX], 4, DEBUG_COL1_OFFSETX + 3, DEBUG_COL1_OFFSETY + 14*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("eflags", DEBUG_COL1_OFFSETX, DEBUG_COL1_OFFSETY + 15*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_EFLAGS_IDX], 8, DEBUG_COL1_OFFSETX + 7, DEBUG_COL1_OFFSETY + 15*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("cr0", DEBUG_COL2_OFFSETX, DEBUG_COL2_OFFSETY, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_CR0_IDX], 8, DEBUG_COL2_OFFSETX + 4, DEBUG_COL1_OFFSETY, DEBUG_REGISTER_COLOR.a);

    print("cr2", DEBUG_COL2_OFFSETX, DEBUG_COL2_OFFSETY + 1*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_CR2_IDX], 8, DEBUG_COL2_OFFSETX + 4, DEBUG_COL1_OFFSETY + 1*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("cr3", DEBUG_COL2_OFFSETX, DEBUG_COL2_OFFSETY + 2*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_CR3_IDX], 8, DEBUG_COL2_OFFSETX + 4, DEBUG_COL1_OFFSETY + 2*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("cr4", DEBUG_COL2_OFFSETX, DEBUG_COL2_OFFSETY + 3*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(info[DEBUG_INFO_CR4_IDX], 8, DEBUG_COL2_OFFSETX + 4, DEBUG_COL1_OFFSETY + 3*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);

    print("STACK", DEBUG_COL2_OFFSETX, DEBUG_COL2_OFFSETY + 8*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_TITLE_COLOR.a);
    print_hex(((unsigned char*)(info[DEBUG_INFO_ESP_IDX]))[0], 8, DEBUG_COL2_OFFSETX, DEBUG_COL1_OFFSETY + 9*DEBUG_ESPACIO_ENTRADAS, DEBUG_REGISTER_COLOR.a);
    print_hex(((unsigned char*)(info[DEBUG_INFO_ESP_IDX]))[1], 8, DEBUG_COL2_OFFSETX, DEBUG_COL1_OFFSETY + 9*DEBUG_ESPACIO_ENTRADAS+1, DEBUG_REGISTER_COLOR.a);
    print_hex(((unsigned char*)(info[DEBUG_INFO_ESP_IDX]))[2], 8, DEBUG_COL2_OFFSETX, DEBUG_COL1_OFFSETY + 9*DEBUG_ESPACIO_ENTRADAS+2, DEBUG_REGISTER_COLOR.a);
    print_hex(((unsigned char*)(info[DEBUG_INFO_ESP_IDX]))[3], 8, DEBUG_COL2_OFFSETX, DEBUG_COL1_OFFSETY + 9*DEBUG_ESPACIO_ENTRADAS+3, DEBUG_REGISTER_COLOR.a);
    print_hex(((unsigned char*)(info[DEBUG_INFO_ESP_IDX]))[4], 8, DEBUG_COL2_OFFSETX, DEBUG_COL1_OFFSETY + 9*DEBUG_ESPACIO_ENTRADAS+4, DEBUG_REGISTER_COLOR.a);


 }

 void screen_ocultar_consola_debug() {
   ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
   int i;
   for( i = 0; i < DEBUG_ALTO; i++) {
    int j;
     for(j = 0; j < DEBUG_ANCHO; j++) {
       p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].c = backup_pantalla[i][j].c;
       p[DEBUG_OFFSETY + i][DEBUG_OFFSETX + j].a = backup_pantalla[i][j].a;
     }
   }
 }
