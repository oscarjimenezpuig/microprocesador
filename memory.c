/*
============================================================
  Fichero: memory.c
  Creado: 24-10-2025
  Ultima Modificacion: vie 24 oct 2025 12:31:06
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "memory.h"

static void mem_init() {
	u1* ptr=memory;
	while(ptr!=memory+DMEM) *ptr++=0;
}

u1 ins(u1 byte) {
	u1* ptr=memory+OPRG;
	u1* end=ptr+DPRG;
	while(*ptr!=0) {
		if(ptr==end) return 0;
		ptr++;
	}
	*ptr=byte;
	return 1;
}

static void ins_inc() {
	//se incrementa en uno la instruccion a leer
	memory[OLIN]++;
}

static u1 ins_read() {
	//se da la instruccion que se lee a continuacion y se incrementa en uno
	u1* p=memory+OLIN;
	return *p++;
}

static u1 ins_jmp(u1 nd) {
	//se establece una nueva direccion de salto
	memory[OLIN]=OLIN+nd;
}
