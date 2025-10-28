/*
============================================================
  Fichero: assembler.c
  Creado: 26-10-2025
  Ultima Modificacion: dimarts, 28 d’octubre de 2025, 05:23:51
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

#define FLAG ORAM
#define LINE ORAM+1
#define WORD ORAM+2

#define FWORD 1
#define FLINE 2
#define FFILE 4
#define FCOMMENT 8
#define FERROR 16

/*
static struct {
	u1 word : 1;
	u1 line : 1;
	u1 file : 1;
	u1 comment: 1;
	u1 error : 1;
} flag ={0,0,0,0,0};

static char word[10]="";

static u1 line=0;
*/

#define flagset(F) (memory[FLAG]|=(F))
#define flagunset(F) (memory[FLAG]&=~(F))
#define flagis(F) ((memory[FLAG] & (F))?1:0)

#define line (memory[LINE])

static void error(u1 line,const char* desc,...) {
	va_list list;
	va_start(list,desc);
	char str[100];
	sprintf(str,"ERROR(%i): %s.\n",line,desc);
	vprintf(str,list);
	va_end(list);
	flagset(FERROR);
}

static void read_word(FILE* file) {
	u1* pw=memory+WORD;
	char c=0;
repeat:
	while((c=getc(file)) && c!=EOW && c!=EOL && c!=EOF) {
		if(c==COC) flagset(FCOMMENT);
		else if (flagis(FCOMMENT)==0) *pw++=c;
	}
	*pw=EON;
	if(memory[WORD]==EON && c!=EOF && c!=EOL) goto repeat;
	switch(c) {
		case EOF:
			flagset(FFILE);
		case EOL:
			flagset(FLINE);
			flagunset(FCOMMENT);
		case EOW:
			flagset(FWORD);
	}
}

static u1 wordisempty() {
	return memory[WORD]=0;
}

static u1 wordtoint() {
	u1 value=0;
	sscanf((char*)(memory+WORD),"%hhi",&value);
	return value;
}

static u1 read_number(u1* nline) {
	//lee el numero de linea y devuelve cual es
	*nline=line=wordtoint();
	if(ins_prg_dir(line)!=0) {
		error(line,"Can't allow direction %i. Maybe already writed",line+OPRG);
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
	while(err==0 && *p!=EON) {
		if(*p<'0' || *p>'9') err=1;
		p++;
	}
	if(err) error(line,"Expected a number to be a complement but found <%s>",word);
	else {
		*complement=wordtoint();
		ins_prg_ins(*complement);
	}
	return err;
}

static u1 number_complements(u1 i) {
	const u1 NOC[]=INNC;
	return NOC[i-1];
}
	
static void read_line(FILE* file) {
	const Read READF[]={read_number,read_instruction,read_complement,read_complement};
	const char* MESEX[]={"line number","instruction","complement","complement"};
	u1 counter=0;
	u1 counter_end=2;
	while(counter<counter_end && flag.error==0 && flag.line==0) {
		read_word(file);
		if(!wordisempty()) {
			u1 value;
			READF[counter](&value);
			if(counter==1) counter_end+=number_complements(value);
			counter++;
		} else if(counter!=0) {
			error(line,"Expected %s",MESEX[counter]);
		}
	}
}

static void read_program(FILE* file) {
	while(flag.file==0 && flag.error==0) {
		read_line(file);
		flag.word=flag.line=0;
	}
}

static void inpstr(u1 len,char* str) {
	char c=0;
	char* p=str;
	while(p-str<len && (c=getchar())!=EOL) {
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
	printf("Enter filename: ");
	inpstr(20,nof);
	FILE* file=fopen(nof,"r");
	if(file) {
		read_program(file);
		if(flag.error) {
			prgerase();
		}
		fclose(file);
		file=NULL;
	} else error(0,"File <%s> not found",nof);
}






		








