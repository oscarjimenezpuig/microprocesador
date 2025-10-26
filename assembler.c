/*
============================================================
  Fichero: assembler.c
  Creado: 26-10-2025
  Ultima Modificacion: diumenge, 26 d’octubre de 2025, 13:35:04
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>
#include <stdarg.h>

#include "assembler.h"

#define EOL '\n'
#define EOW ' '
#define EON '\0'

#define ERROR_FLAG 255 //si hay error la direccion de linea de programa tiene la error flag

char word[10];

static void error(u1 line,const char* desc,...) {
	va_list list;
	va_start(list,desc);
	char str[100];
	sprintf(str,"ERROR(%i): %s.\n",line,desc);
	vprintf(str,list);
	va_end(list);
	memory[OLIN]=ERROR_FLAG;
}

static char word_get(FILE* f) {
	char* pw=word;
	char c=0;
repeat:
	while((c=getc(f)) && c!=EOW && c!=EOL && c!=EOF) *pw++=c;
	*pw=EON;
	if(*word==EON && c!=EOF && c!=EOL) goto repeat;
}

static u1 read_number() {
	//lee el numero de linea y devuelve cual es
	u1 value=0;
	sscanf(word,"%hhi",&value);
	if(ins_prg_dir(value)) {
		error(value,"Can't allow direction %hhi. Maybe already writed");
		return DPRG;
	}
	return value;
}

static u1 read_instruction(u1 line) {
	char* STIN[]=INSD;
	for(u1 k=0;k<DINS;k++) {
		char* pc=STIN[k];
		char* pw=word;
		while(*pw!=EON) {
			if(*pw!=*pc) break;
			pw++;
			pc++;
		}
		if(*pw==EON && *pc==EON) {
			if(ins_prg_ins(line,"Can't write %s in memory. Program too long",word)) return 1;
			return 0;
		}
	}
	error(line,"Instruction %s not recognised",word);
	return IGN;
}

static u1 read_complements(u1 line,u1 comps) {
	for(u1 k=0;k<comp;k++) {









