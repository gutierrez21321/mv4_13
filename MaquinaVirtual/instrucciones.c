#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

void procesar(int memoria[],int registros[]){
  int instruccion,num,mnemo,opA,a,opB,b;
  registros[IP] = 0; //instruction pointer arranca en 0
  registros[CC] = 0x00000000;
  registros[AC] = 0x00000000;
  while(IP>=0 && registros[IP] < registros[DS]){
    instruccion = memoria[registros[IP]];  //leo instruccion

    leerInstruccion(instruccion,&mnemo,&opA,&a,&opB,&b); //decodifico instruccion y operandos
    if(mnemo>=0x00000000 && mnemo<=0x0000000b)//2 operandos
        (*fun[mnemo])(opA,a,opB,b);           //ejecutar instruccion 2 operandos
    else if(mnemo>=0x000000f0 && mnemo<=0x000000fb){
        mnemo = mnemo & 0x0000000f;
        (*fun[mnemo])(opA,a); //ejecutar instruccion 1 operandos
    }else     //1 operando
        *fun[0])(); //ejecutar instruccion 0 operandos (HARDCODEO)
    if(breakpointActivado) //si esta activado breakpoint dsp de cada operacion
      breakpoint();
    registros[IP]++;  //aumento IP
  }
}

void leerArchivo(int memoria[],int registros[]){ //segmento de codigo
  int tamanoCodigo,i,largoLinea;
  char *filename;
  largoLinea = 33;
  FILE *archivo;
  char linea[largoLinea];
  strcpy(filename,argv[2]); //el tercer argumento ruta del archivo traducido fijarme / o //

  if((archivo = fopen(filename,"r")) == NULL) return 1;

  for(i=0;i<6;i++){ //lee header
    fgets(linea,largoLinea,archivo);
    if(i==1)
      tamanoCodigo = stringToInt(linea);
  }

  if(tamanoCodigo<MAXMEMORIA){ //capaz de almacenar las intrucciones
    i = 0;
    while(fgets(linea,largoLinea,archivo) != NULL){ //leo instrucciones
      printf("%s\n",linea);
      memoria[i++] = stringToInt(linea,largoLinea-2); //configuro la memoria
    }
    registros[DS] = memoria[i]; //arranca segmento de datos
  }else
    registros[DS] = 0;

  fclose(archivo);
}

void leerInstruccion(int num,int *mnemo,int *opA,int *a,int *opB,int *b){
  *mnemo = num >> 28;          //leo el mnemonico
  *mnemo = *mnemo & 0x0000000f;
  if(*mnemo<=12){ //2 operandos
    *opA = num >> 26;
    *opA = *opA & 0x00000003;
    *a = num>>12;
    if(*opA==1)//registro
      *a = *a & 0x0000003f;
    else
      *a = *a & 0x00000fff;
    *opB = num >> 24;
    *opB = *opB & 0x00000003;
    *b = num & 0x00000fff;
    if(*opB==1)//registro
      *b = *b & 0x0000003f;
    else
      *b = *b & 0x00000fff;
    //propago signo
    *a = *a<<20;
    *a = *a>>20;
    *b = *b<<20;
    *b = *b>>20;
    //printf("2 operandos\n");
    //printf("operando A: %d  operando B: %d\n",*opA,*opB);
    //printf("valor A: %d valor B: %d\n",*a,*b);
  }else if(*mnemo == 0x0000000f){ //1 operando o ninguno
     *mnemo = (*mnemo<<4) & 0x000000f0;
     *mnemo = *mnemo | (0x0000000f & (num >> 24));
     *mnemo = *mnemo & 0x000000ff;
     if((*mnemo & 0x0000000f) <= 12){ //1 operando
      *opA = (num >> 22) & 0x00000003;
      *opB = *b = 0b11;
      if(*opA==1)
        *a = num & 0x0000003f;
      else
        *a = num & 0x0000ffff;
      //propago signo
      *a = *a<<16;
      *a = *a>>16;
      //printf("1 operando\n");
      //printf("operando A: %d\n",*opA);
      //printf("valor A: %d\n",*a);
     }else if(*mnemo==0x000000ff){ //no operandos
      *opA = *a = *opB = *b = 0b11;
      //printf("0 operandos\n");
     }
  }
  //printf("mnemonico: %d\n",*mnemo);
}

void configurarRegistros(){
  for(int i=0xA;i<=0xF;i++)
    registros[i] = 0x00000000;
}
