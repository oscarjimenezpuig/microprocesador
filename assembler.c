/*
============================================================
  Fichero: assembler.c
  Creado: 26-10-2025
  Ultima Modificacion: diumenge, 26 d’octubre de 2025, 09:28:46
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>
#include <stdarg.h>

#include "assembler.h"

#define EOL '\n'
#define EOW ' '

static u1 error_is=0;

static void error(u1 line,const char* desc,...) {
	va_list list;
	va_start(list,desc);
	char str[100];
	sprintf(str,"ERROR(%i): %s.\n",line,desc);
	vprintf(str,list);
	va_end(list);
	error_is=1;
}

static u1 compare(const char* a,char* b) {
	const char* pa=a;
	char* pb=b;
	while(*pa!='\0') {
		if(*pa!=*pb) return 0;
		pa++;
		pb++;
	}
	return (*pb=='\0');
}

static u1 find_ins(char* str) {
	const char* INSS[]=INSD;
	u1 k=0;
	for(;k<DINS;k++) if(compare(INSS[k],str)) break;
	return k;
}

static char find_end_line(FILE* f) {
	//esta funcion busca el final de linea (o archivo)
	char c=0;
	while((c=getc(f)) && c!=EOL && c!=EOF);
	return c;
}

static char read_word(FILE* f,char* word) {
	char* ptr=word;
	char c=0;
	do {
		while((c=getc(f)) && c!=EOW && c!=EOL && c!=EOF) {
			*ptr++=c;
			if(c==COC) break;
		}
		*ptr='\0';
	}while(*word==EOW && c!=EOF);
	return c;
}

static u1 numero_linea(char* word) {
	static u1 line=1;
	u1 num=0;
	sscanf(word,"%hhi",&num);
	if(num!=line) {
		error(line,"The line should be number %i",line);
		return 1;
	} else {
		++line;
		return 0;
	}
}

static char find_complement(u1 line,char c,FILE* f) {
	char wrd[10];
	if(c!=EOW) {
		error(line,"Expected a complement, find nothing");
		c=EOF;
	} else {
		c=read_word(f,wrd);
		u1 value=0;
		sscanf(wrd,"%hhi",&value);
		instruction(value);
	}
	return c;
}


static char read_line(FILE* f) {
	const u1 NCOI[]=INNC;
	static u1 line=0;
	char word[10];
	char c=read_word(f,word);
	if(c==EOF) return EOF;
	else if(*word==COC) {
		find_end_line(f);
		return EOL;
	} else if(!numero_linea(word)) {
		++line;
		if(c!=EOW) {
			error(line,"Expected an instruccion. Find nothing");
			return EOF;
		} else {
			c=read_word(f,word);
			u1 ni=find_ins(word);
			if(ni==DINS) {
				error(line,"Instruction <%s> not recognised",word);
				return EOF;
			} else {
				instruction(ni);
				u1 nc=NCOI[ni];
				if(nc==0 && c!=EOL && c!=EOF) {
					error(line,"Expected nothing after instruccion %s. Find something",word);
					return EOF;
				} else {
					for(u1 n=0;n<nc && c!=EOF;n++) {
						c=find_complement(line,c,f);
					}
					return c;
				}
			}
		}
	} else return EOF;
}

static u1 read_file(FILE* f) {
	char c=0;
	do {
		c=read_line(f);
	}while(c!=EOF);
	return error_is;
}

static u1 detect_line(u1 number) {
	//si hay error, envia a la direccion 0
	const u1 NCOI[]=INNC;
	u1 counter=1;
	u1 dir=0;
	u1* ptr=memory+OPRG;
	while(counter<number) {
		if(*ptr==END || ptr==memory+OPRG+DPRG) {
			dir=0;
			break;
		}
		u1 ncom=NCOI[*ptr];
		dir+=ncom+1;
		ptr+=ncom+1;
		counter++;
	}
	return dir;
}

static u1 detect_jump() {
	const u1 NCOI[]=INNC;
	u1* ptr=memory+OPRG;
	u1 line=1;
	while(*ptr!=END && ptr!=memory+OPRG+DPRG) {
		if(*ptr==JMP) {
			ptr++;
			u1 dir=detect_line(*ptr);
			if(dir) *ptr=dir;
			else goto error_line;
		} else if(*ptr==IF || *ptr==IFN) {
			ptr=ptr+2;
			u1 dir=detect_line(*ptr);
			if(dir) *ptr=dir;
			else goto error_line;
		} else {
			u1 step=NCOI[*ptr];
			ptr=ptr+step;
		}
		line++;
		ptr++;
	}
	return 0;
error_line:
	error(line,"The line to jump doesn't exist");
	return 1;
}

static FILE* file_find() {
	char str[20];
	char c=0;
	char* ptr=str;
	printf("Enter file name: ");
	while((c=getchar())!=EOL) {
		*ptr++=c;
	}
	*ptr='\0';
	FILE* f=fopen(str,"r");
	if(!f)	error(0,"File %s doesn't exists",str);
	return f;
}

static void erase_program() {
	u1* ptr=memory+OPRG;
	while(ptr!=memory+OPRG+DPRG) *ptr++=0;
}

void program() {
	FILE* f=file_find();
	if(f) {
		if(read_file(f) || detect_jump()) erase_program();
		fclose(f);
	}
}


	
