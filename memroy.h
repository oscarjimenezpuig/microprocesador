/*
============================================================
  Fichero: memroy.h
  Creado: 24-10-2025
  Ultima Modificacion: divendres, 24 d’octubre de 2025, 05:29:26
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

typedef unsigned char u1;

#define MEMORY 256

extern u1 memory[MEMORY];

#define REGA 1
#define REGB 2

//instruccines

#define END 0 //final de programa
#define LOD 1 //cargar valor <direccion,valor>
#define MOV 2 //mover de origen a destino <direccion_o,direccion_d>
#define VDR 3 //da el valor de la direccion contenida en la direccion apuntada y la guarda en destino <direccion_o,direccion_d>
#define AND 3 //operacion and entre el registro A y un valor <valor> se guarda en A

//macros

#define MEM(A) *(memory+(A))

#define MLOD(A,B) MEM((A))=(B)
#define MMOV(O,D) MEM((D))=MEM((O))
#define MVDR(O,D) MEM((D))=MEM(MEM((O)))
#define AND(V) MEM(REGA)=MEM(REGA) & (V) 



//funciones


