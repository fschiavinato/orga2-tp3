#ifndef __debug_H__
#define __debug_H__

#include "i386.h"

typedef struct error_msg_t{
	unsigned char orig;
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	unsigned int esi;
	unsigned int edi;
	unsigned int ebp;
	unsigned int esp;
	unsigned int eip;
	unsigned short cs;
	unsigned short ds;
	unsigned short es;
	unsigned short fs;
	unsigned short gs;
	unsigned short ss;
	unsigned int eflags;
	unsigned int cr0;
	unsigned int cr2;
	unsigned int cr3;
	unsigned int cr4;
	(unsigned int)* stack;

} error_msg;

error_msg get_error();
void print_error(error_msg);




#endif