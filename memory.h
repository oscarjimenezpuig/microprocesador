/*
============================================================
  Fichero: memory.h
  Creado: 24-10-2025
  Ultima Modificacion: vie 24 oct 2025 12:31:00
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

typedef unsigned char u1;

#define DMEM 256


#define DRAM 128
#define DOUT 10
#define DIN 10
#define DLIN 1
#define DPRG (DMEM-DRAM-DOUT-DIN)
#define ORAM 0
#define OLIN (ORAM+DRAM)
#define OPRG (OLIN+DLIN)
#define OOUT (OPRG+DPRG)
#define DIN (OOUT+DOUT)

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
#define INC(A) (MEM(A)++
#define DEC(A) (MEM(A))--

extern u1 memory[MEMORY];

u1 ins(u1 byte); //insercion de programa en el siguiente byte vacio

void program(); //aqui dentro va el programa donde se introducen en la posicion relativa al programa todas las lineas.
			   


//funciones


