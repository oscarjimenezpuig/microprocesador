/*
============================================================
  Fichero: memory.c
  Creado: 24-10-2025
  Ultima Modificacion: diumenge, 26 d’octubre de 2025, 13:28:44
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>

#include "memory.h"

u1 memory[DMEM];

static void mem_init() {
	u1* ptr=memory;
	while(ptr!=memory+DMEM) *ptr++=0;
	memory[OLIN]=OPRG-1;
	memory[OPLO]=OOUT-1;
}

u1 ins_prg_dir(u1 d) {
	u1 dd=OPRG+d;
	if(dd>memory[OLIN] && dd<OPRG+DPRG) {
		memory[OLIN]=dd;
		return 0;
	} else return 1;
}

u1 ins_prg_ins(u1 b) {
	u1* ptr=memory+OLIN;
	if(*ptr!=OPRG+DPRG) {
		*(memory+*ptr)=b;
		*ptr+=1;
		return 0;
	}
	return 1;
}

u1 instruction(u1 d,u1 i) {
	u1 dir=memory[OLIN];
	u1 dd=d+OPRG;
	if(d>dir && d<OPRG+DPRG) {
		memory[d]=i;
		memory[OLIN]=d;
		return 0;
	}
	return 1;
}

static u1 ins_read() {
	//se da la instruccion que se lee a continuacion y se incrementa en uno
	u1* p=memory+OLIN;
	u1 ret=*(memory+*p);
	(*p)++;
	return ret;
}

static void dir_to_out(u1 dir) {
	u1 lastdir=memory[OPLO];
	if(lastdir<OOUT+DOUT-4) { //verifica que la ultima direccion no sea el final del buffe
		u1 val=memory[dir];
		u1* ptr=memory+lastdir+1;
		*ptr++=0;
		u1* u=ptr++;
		u1* d=ptr++;
		u1* c=ptr;
		*u=val%10+'0';
		val=val/10;
		*d=val%10+'0';
		*c=val/10+'0';
		memory[OPLO]=c-memory;
	}
}

static void out_to_dir(u1 dir) {
	u1* ptr=memory+memory[OPLO];
	if(ptr!=memory+OOUT-1) {
		u1 val=0;
		u1 fac=1;
		while(*ptr!=0) {
			val+=fac*(*ptr-'0');
			fac=fac*10;
			ptr--;
		}
		ptr--;
		memory[OPLO]=ptr-memory;
		memory[dir]=val;
	}
}

static void out_to_print() {
	printf("> ");
	if(memory[OPLO]>=OOUT) {
		u1* ptr=memory+memory[OPLO];
		while(*ptr!=0) {
			printf("%c",*ptr);
			ptr--;
		}
		ptr--;
		memory[OPLO]=ptr-memory;
	}
	printf("\n");
}

static void input_to_out() {
	printf("< ");
	u1* ptr=memory+memory[OPLO]+1;
	u1* end=memory+OOUT+DOUT;
	if(ptr!=end) {
		char c=0;
		*ptr=0;
		while(ptr!=end &&(c=getchar())!='\n') *(++ptr)=c;
		memory[OPLO]=ptr-memory;
	}
}

static u1 ins_zero_com(u1 ins) {
	switch(ins) {
		case END:
			memory[OLIN]=OPRG+DPRG;
			break;
		case PRT:
			out_to_print();
			break;
		case INP:
			input_to_out();
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
			dir_to_out(com);
			break;
		case OTV:
			out_to_dir(com);
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
	*pmo=OPRG;
	u1 err=0;
	while(*pmo!=OPRG+DPRG && !err) {
		u1 ins=ins_read();
		switch(ins) {
			case IGN:
				break;
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
