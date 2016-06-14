#include "debug.h"

error_msg get_error(){
	error_msg e;
	e.eax=reax();
	e.ebx=rebx();
	e.ecx=recx();
	e.edx=redx();
	e.esi=resi();
	e.edi=redi();
	e.ebp=rebp();
	e.esp=resp();
	e.eip=reip();
	e.cs=rcs();
	e.ds=rds();
	e.es=res();
	e.fs=rfs();
	e.gs=rgs();
	e.ss=rss();
	e.eflags=rflags();
	e.cr0=rcr0();
	e.cr2=rcr2();
	e.cr3=rcr3();
	e.cr4=rcr4();

};