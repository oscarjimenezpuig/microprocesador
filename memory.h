/*
============================================================
  Fichero: memory.h
  Creado: 24-10-2025
  Ultima Modificacion: diumenge, 26 d’octubre de 2025, 05:59:21
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

typedef unsigned char u1;

#define DMEM 256


#define DRAM 128
#define DOUT 16
#define DLIN 1
#define DPLO 1
#define DPRG (DMEM-DRAM-DOUT-DPLO-DLIN)
#define ORAM 0
#define OLIN (ORAM+DRAM)
#define OPRG (OLIN+DLIN)
#define OPLO (OPRG+DPRG)
#define OOUT (OPLO+DPLO)

//instruccines

#define END 0 //final de programa
#define LOD 1 //cargar valor <direccion,valor>
#define MOV 2 //mover de origen a destino <direccion_o,direccion_d>
#define VDR 3 //da el valor de la direccion contenida en la direccion apuntada y la guarda en destino <direccion_o,direccion_d>
#define AND 4 //operacion and entre dos direcciones de memoria, se guarda el resultado en la primera
#define OR 5 //bajo las mismas condiciones que AND, pero operacion OR
#define NOT 6 //operacion complementaria de una direccion de memoria. El resultado se almacena en la misma direccion de memoria
#define LFT 7 //operacion desplazamiento derecha de una direccion de memoria, se da otra direccion de memoria donde se pone el carrybit <direccion,direccion>
#define RGT 8 //idem que left
#define IF 9 //si el valor en una direccion es 0 salta a la direccion relativa del programa <direccion_valor,direccion_salto>
#define IFN 10 //si el valor en una direccion no es 0 salta a la direccion relativa del programa <direccion_valor,direccion_salto>
#define JMP 11 //salta a una direccion del programa (relativa al inicio de programa) <direccion_salto>
#define INC 12 //se incrementa el valor almacenado en la direccion en uno <direccion>
#define DEC 13 //se decrementa el valor almacenado en la direccon en uno <direccion>
#define PRT 14 //se imprime lo que esta en el buffer de impresion
#define INP 15 //se guarda en el buffer de entrada un valor
#define VTO 16 //se mueve un valor de una direccion al buffer de impresion <direccion> donde se transforma en una sucesion de caracteres
#define OTV 17 //el valor registrado en el output se mueve a una direccion <direccion>


//macros

#define MEM(A) *(memory+(A))

#define MLOD(A,B) MEM((A))=(B)
#define MMOV(O,D) MEM((D))=MEM((O))
#define MVDR(O,D) MEM((D))=MEM(MEM((O)))
#define MAND(A,B) MEM(A)=(MEM(A) & MEM(B))
#define MOR(A,B) MEM(A)=(MEM(A) | MEM(B))
#define MNOT(A) MEM(A)=~MEM(A)
#define MLFT(A) (MEM(A)=(MEM(A)<<1))
#define MRGT(A) (MEM(A)=MEM(A)>>1)
#define MINC(A) (MEM(A))++
#define MDEC(A) (MEM(A))--
#define MIF(A,B) if (MEM(A)==0) MJMP(B)
#define MIFN(A,B) if(MEM(A)!=0) MJMP(B)
#define MJMP(A) MEM(OLIN)=(A)+OPRG

//funciones

extern u1 memory[DMEM];

void program(); 
//aqui dentro va el programa donde se introducen en la posicion relativa al programa todas las lineas.
			   
u1 instruction(u1 byte);
//se inserta la siguiente instruccion, se devuelve el siguiente bit libre (0 si no hay ninguno)

void insert(u1 bytes,u1* byte); 
//aqui se pone todo el programa para que sea introducido donde toca

void mem_prt(u1 dir_ini,u1 dir_len); 
//imprime la memoria desde la direccion inicio con la longitud dicha

