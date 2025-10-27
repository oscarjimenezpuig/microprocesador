/*
============================================================
  Fichero: assembler.c
  Creado: 26-10-2025
  Ultima Modificacion: lun 27 oct 2025 14:31:52
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>
#include <stdarg.h>

#include "assembler.h"

#define EOL '\n'
#define EOW ' '
#define EON '\0'

typedef u1 (*Read)(u1*);


static FILE* file=NULL;

struct {
	u1 word : 1;
	u1 line : 1;
	u1 file : 1;
	u1 error : 1;
} flag ={0,0,0,0};

char word[10]="";

u1 line=0;

static void error(u1 line,const char* desc,...) {
	va_list list;
	va_start(list,desc);
	char str[100];
	sprintf(str,"ERROR(%i): %s.\n",line,desc);
	vprintf(str,list);
	va_end(list);
	flag.error=1;
}

static void read_word() {
	char* pw=word;
	char c=0;
repeat:
	while((c=getc(file)) && c!=EOW && c!=EOL && c!=EOF) *pw++=c;
	*pw=EON;
	if(*word==EON && c!=EOF && c!=EOL) goto repeat;
	switch(c) {
		case EOF:
			flag.file=1;
		case EOL:
			flag.line=1;
		case EOW:
			flag.word=1;
	}

}

static u1 wordisempty() {
	return *word==EON;
}

static u1 wordtoint() {
	u1 value=0;
	sscanf(word,"%hhi",&value);
	return value;
}

static u1 read_number(u1* nline) {
	//lee el numero de linea y devuelve cual es
	*nline=line=wordtoint();
	if(ins_prg_dir(line)!=0) {
		error(line,"Can't allow direction %hhi. Maybe already writed",line+OPRG);
		return 0;
	}
	return 1;
}

static u1 read_instruction(u1* instruction) {
	const char* STIN[]=INSD;
	for(u1 k=0;k<DINS;k++) {
		const char* pc=STIN[k];
		char* pw=word;
		while(*pw!=EON) {
			if(*pw!=*pc) break;
			pw++;
			pc++;
		}
		if(*pw==EON && *pc==EON) {
			if(ins_prg_ins(k+1)) {
				error(line,"Can't write %s in memory. Program too long",word);
				return 1;
			}
			*instruction=k+1;
			return 0;
		}
	}
	error(line,"Instruction %s not recognised",word);
	return IGN;
}

static u1 read_complement(u1* complement) {
	char* p=word;
	u1 err=0;
	while(!err && p!=word+3) {
		if(*p<'0' || *p>'9') err=1;
	}
	if(err) error(line,"Expected a number to be a complement but found <%s>",word);
	else {
		*complement=wordtoint();
		ins_prg_ins(*complement);
	}
	return err;
}

static void read_comment() {
	if(*word==COC) {
		while(flag.line==0) read_word();
	}
}

static u1 number_complements(u1 i) {
	const u1 NOC[]=INNC;
	return NOC[i-1];
}
	
static void read_line() {
	const Read READF[]={read_number,read_instruction,read_complement,read_complement};
	const char* MESEX[]={"line number","instruction","complement","complement"};
	u1 counter=0;
	u1 counter_end=2;
	for(;counter<counter_end && flag.error==0 && flag.line==0;counter++) {
		read_word();
		if(!wordisempty()) {
			read_comment();
			if(flag.line==0) {
				u1 value;
				READF[counter](&value);
				if(counter==1) counter_end+=number_complements(value);
			}
		} else {
			error(line,"Expected %s",MESEX[counter]);
		}
	}
}

static void read_program() {
	while(flag.file==0) {
		read_line();
	}
}

static void inpstr(u1 len,char* str) {
	char c=0;
	char* p=str;
	while(p-str<le;n && (c=getchar())!=EOL) {
		*p++=c;
	}
	*p=EON;
}

static void prgerase() {
	u1* ptr=memory+OPRG;
	while(ptr!=memory+OPRG+DPRG) *ptr++=0;
}

void program() {
	char nof[21];
	inpstr(20,nof);
	file=fopen(nof,"r");
	if(file) {
		read_program();
		if(flag.error) {
			prgerase();
		}
		fclose(file);
		file=NULL;
	} else error(0,"File <%s> not found",nof);
}






		








