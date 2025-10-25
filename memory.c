/*
============================================================
  Fichero: memory.c
  Creado: 24-10-2025
  Ultima Modificacion: dissabte, 25 d’octubre de 2025, 10:04:59
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>

#include "memory.h"

u1 memory[DMEM];

static void mem_init() {
	u1* ptr=memory;
	while(ptr!=memory+DMEM) *ptr++=0;
	memory[OLIN]=OPRG;
}

void insert(u1 bs,u1* b) {
	u1* pb=b;
	u1* pm=memory+OPRG;
	while(pm!=memory+OPRG+DPRG && pb!=b+bs) {
		*pm++=*pb++;
	}
}

static u1 ins_read() {
	//se da la instruccion que se lee a continuacion y se incrementa en uno
	u1* p=memory+OLIN;
	u1 ret=*(memory+*p);
	(*p)++;
	return ret;
}

static void move_to_output(u1 dir) {
	u1 val=memory[dir];
	u1* ptr=memory+OOUT;
	while(*ptr!=0 && ptr!=memory+OOUT+DOUT) ptr++;
	u1 ocups=ptr-(memory+OOUT);
	if(DOUT-ocups>=3) {
		ptr=ptr+2;
		*ptr=*(ptr-1)=*(ptr-2)='0';
		while(val!=0) {
			if(*ptr=='9') {
				*ptr='0';
				if(*(ptr-1)=='9') {
					*(ptr-1)='0';
					*(ptr-2)=*(ptr-2)+1;
				} else *(ptr-1)=*(ptr-1)+1;
			} else *ptr=*ptr+1;
			val--;
		}
	}
}								

static void print_output() {
	//en la impresion del output el buffer se va vaciando conforme se imprime
	u1* ptr=memory+OOUT;
	printf("> ");
	while(*ptr!=0 && ptr!=memory+OOUT+DOUT) {
		printf("%c",*ptr);
		*ptr++=0;
	}
	printf("\n");
}

static void output_to_dir(u1 dir) {
	u1 *ptr,*init,*end;
	init=memory+OOUT;
	ptr=init+3;
	end=init+DOUT;
	while(*ptr!=0) {
		for(u1 n=0;n<3;n++) {
			if(ptr==end) return;
			else ptr++;
		}
	}
	u1* u=ptr;
	u1* d=ptr-1;
	u1* c=ptr-2;
	u1 val=0;
	while(*u!=0) {
		if(*u=='0') {
			if(*d==0) *u=0;
			else if(*d=='0') {
				if(*c==0) *d=0;
				else if (*c=='0') *c=0;
				else {
					*c=*c-1;
					*d=*d+9;
				}
			} else {
				*d=*d-1;
				*u=*u+9;
			}
		} else {
			*u=*u-1;
			val++;
		}
	}
	memory[dir]=val;
}

static void output_input() {
	u1 *ptr,*init,*end;
	init=ptr=memory+OOUT;
	end=init+DOUT;
	while(*ptr!=0) {
		if(ptr==end) return;
		else ptr++;
	}
	char c=0;
	printf("< ");
	while((c=getchar())!='\n' && ptr!=end) {
		*ptr++=c;
	}
	mem_prt(OOUT,DOUT);//dbg
}

static u1 ins_zero_com(u1 ins) {
	switch(ins) {
		case END:
			memory[OLIN]=OPRG+DPRG;
			break;
		case PRT:
			print_output();
			break;
		case INP:
			output_input();
			break;
		default:
			return 1;
	}
	return 0;
}

static u1 ins_one_com(u1 ins) {
	u1 com=ins_read();
	switch(ins) {
		case NOT:
			MNOT(com);
			break;
		case LFT:
			MLFT(com);
			break;
		case RGT:
			MRGT(com);
			break;
		case INC:
			MINC(com);
			break;
		case DEC:
			MDEC(com);
			break;
		case JMP:
			MJMP(com);
			break;
		case VTO:
			move_to_output(com);
			break;
		case OTV:
			output_to_dir(com);
			break;
		default:
			return 1;
	}
	return 0;
}

static u1 ins_two_com(u1 ins) {
	u1 com1=ins_read();
	u1 com2=ins_read();
	switch(ins) {
		case LOD:
			MLOD(com1,com2);
			break;
		case MOV:
			MMOV(com1,com2);
			break;
		case VDR:
			MVDR(com1,com2);
			break;
		case AND:
			MAND(com1,com2);
			break;
		case OR:
			MOR(com1,com2);
			break;
		case IF:
			MIF(com1,com2);
			break;
		case IFN:
			MIFN(com1,com2);
			break;
		default:
			return 1;
	}
	return 0;
}

static u1 prg_exe() {
	u1* pmo=memory+OLIN;
	u1 err=0;
	while(*pmo!=OPRG+DPRG && !err) {
		u1 ins=ins_read();
		switch(ins) {
			case INP:
			case PRT:
			case END:
				err=ins_zero_com(ins);
				break;
			case NOT:
			case LFT:
			case RGT:
			case INC:
			case DEC:
			case JMP:
			case VTO:
			case OTV:
				err=ins_one_com(ins);
				break;
			case LOD:
			case MOV:
			case VDR:
			case AND:
			case OR:
			case IF:
			case IFN:
				err=ins_two_com(ins);
				break;
			default:
				err=1;
		}
	}
	mem_prt(OOUT,DOUT);//dbg
	mem_prt(0,1);//dbg
	if(err) return memory[OLIN];
	else return 0;
}

void mem_prt(u1 di,u1 dl) {
	u1* p=memory+di;
	while(p!=memory+di+dl) {
		if(*p!=0) printf("\033[7m");
		printf("%03li->%03i",p-memory,*p);
		printf("\033[0m");
		if((p-memory+di)%6==0 && p!=memory+di) puts("");
		else printf("    ");
		p++;
	}
	puts("");
}
				
int main() {
	mem_init();
	program();
	return prg_exe();
}

//prueba

void program() {
	u1 prg[]={INP,OTV,0,VTO,0,PRT,END};
	insert(7,prg);
}




