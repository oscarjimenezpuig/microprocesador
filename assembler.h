/*
============================================================
  Fichero: assembler.h
  Creado: 26-10-2025
  Ultima Modificacion: diumenge, 26 d’octubre de 2025, 12:31:57
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "memory.h"

//ASSEMBLER
/* Cada linea empieza con un numero de linea, una linea debe contener una instruccion y
 * los complementos si los lleva. Los numeros de linea empiezan por uno y son sucesivos.
 * El comentario se escribe con el caracter detallado en COC.
 * Instrucciones y complementos separados por un espacio.
 * Los comentarios deben ocupar una linea sola, sin numeracion
 */

//caracter de comentarios

#define COC '#'

//numero de instrucciones

#define DINS (END-IGN+1)

//cadena de instrucciones

#define SLOD "LOAD"
#define SMOV "MOVE"
#define SVDR "VDR"
#define SAND "AND"
#define SOR "OR"
#define SNOT "NOT"
#define SLFT "LFT"
#define SRGT "RGT"
#define SIF "IF"
#define SIFN "IFN"
#define SJMP "JUMP"
#define SINC "INC"
#define SDEC "DEC"
#define SPRT "PRINT"
#define SINP "INPUT"
#define SVTO "PUSH"
#define SOTV "POP"
#define SEND "END"

#define INSD {SLOD,SMOV,SVDR,SAND,SOR,SNOT,SLFT,SRGT,SIF,SIFN,SJMP,SINC,SDEC,SPRT,SINP,SVTO,SOTV,SEND}

//complementos de cada instruccion

#define CLOD 2
#define CMOV 2
#define CVDR 2
#define CAND 2
#define COR 2
#define CNOT 1
#define CLFT 1
#define CRGT 1
#define CIF 2
#define CIFN 2
#define CJMP 1
#define CINC 1
#define CDEC 1
#define CPRT 0
#define CINP 0
#define CVTO 1
#define COTV 1
#define CEND 0

#define INNC {CLOD,CMOV,CVDR,CAND,COR,CNOT,CLFT,CRGT,CIF,CIFN,CJMP,CINC,CDEC,CPRT,CINP,CVTO,COTV,CEND}


