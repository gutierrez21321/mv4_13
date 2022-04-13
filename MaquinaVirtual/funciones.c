#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

//2 operandos

void setCC(int a){ //setea bit de signo
  int reg = registros[CC];
  if(a == 0){
    reg = reg & 0x000000001;
  }else{
    reg = reg & 0x000000000;
  }
  if(a < 0){
    reg = reg & 0x800000001;
  }else{
    reg = reg & 0x000000001;
  }
  registros[CC] = reg;
}

void modificarRegistro(int a,int valor){ //testear
  int esp,aux,reg;
  esp = a & 0xf;  //accedo registro
  aux = (a >> 4) & 0x3;  //sector de registro
  switch(aux){
      case 0: registros[esp] = registros[esp] & (~E);
              reg = valor;
              break;
      case 1: registros[esp] = registros[esp] & (~L);
              reg = valor & 0x000f;

              break;
      case 2: registros[esp] = registros[esp] & (~H);
              reg = (valor & 0x000f)<<8;

              break;
      case 3: registros[esp] = registros[esp] & (~X);
              reg = valor & 0x00ff;

              break;
    }
  registros[esp] = registros[esp] | reg;
}

int valor(int tipo,int operador){ //devuelve el valor para operar
  int valor,aux;
  if(tipo==1 && operador>=10 && operador<=16){ //registro
    aux = operador & 0xf; //accedo registro
    valor = registros[aux];

    aux = (operador >> 4) & 0x3; //sector de registro

    switch(aux){
      case 0: valor = valor & X;
              break;
      case 1: valor = valor & L;
              valor = valor<<24; //propago signo
              valor = valor>>24;
              break;
      case 2: valor = (valor & H)>>8;
              valor = valor<<24; //propago signo
              valor = valor>>24;
              break;
      case 3: valor = valor & X;
              valor = valor<<16; //propago signo
              valor = valor>>16;
              break;
    }

  }else if(tipo==2) //memoria
    valor = memoria[registros[DS] + operador];
  else //inmediato
    valor = operador;
  return valor;
}

void mov(int opA,int a,int opB,int b){
  int valorB;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
   valorB = valor(opB,b);
   if(opA==1)//registro
     modificarRegistro(a,valorB);
   else//memoria
     memoria[registros[DS]+a] = valorB;
  }
}

void add(int opA,int a,int opB,int b){
  int suma;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    suma = valor(opA,a) + valor(opB,b);
    if(opA==1)  //registro
      modificarRegistro(a,suma);
    else //memoria
      memoria[registros[DS+a]] = suma;
    setCC(suma);
  }
}

void sub(int opA,int a,int opB,int b){
  int resta;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    resta = valor(opA,a) - valor(opB,b);
    if(opA==1)  //registro
      modificarRegistro(a,resta);
    else //memoria
      memoria[registros[DS+a]] = resta;
    setCC(resta);
  }
}

void mul(int opA,int a,int opB,int b){
  int multiplicacion;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    multiplicacion = valor(opA,a) * valor(opB,b);
    if(opA==1)  //registro
      modificarRegistro(a,multiplicacion);
    else //memoria
      memoria[registros[DS+a]] = multiplicacion;
    setCC(multiplicacion);
  }
}

void division(int opA,int a,int opB,int b){
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    div_t resultado = div(valor(opA,a),valor(opB,b));
    if(opA==1) //registro
      modificarRegistro(a,resultado.quot); //resultado.quot cociente entero
    else      //memoria
      memoria[registros[DS+a]] = resultado.quot;
    registros[AC] = abs(resultado.rem);    //resto entero positivo
    setCC(resultado.quot);
  }
}

void swap(int opA,int a,int opB,int b){
  int aux,valorB;
  if(opA!=3 && opA!=0 && opB!=3 && opB!=0){//si es valido
    aux = valor(opA,a);
    valorB = valor(opB,b);
    if(opA==1){   //registro
      modificarRegistro(a,valorB);
    }else  //memoria
      memoria[registros[DS+a]] = valorB;

    if(opB==1){   //registro
      modificarRegistro(b,aux);
    }else  //memoria
      memoria[registros[DS+b]] = aux;
  }
}

void cmp(int opA,int a,int opB,int b){ //a puede ser inmediato?? aca supongo que si
  int aux;
  if(opA!=3 && opB!=3)
    aux = valor(opA,a) - valor(opB,b);
  setCC(aux);
}

void and(int opA,int a,int opB,int b){
  int andBit;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    andBit = valor(opA,a) & valor(opB,b);
    if(opA==1) //registro
      modificarRegistro(a,andBit);
    else       //memoria
      memoria[registros[DS+a]] = andBit;
  }
  setCC(andBit);
}

void or(int opA,int a,int opB,int b){
  int orBit;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    orBit = valor(opA,a) & valor(opB,b);
    if(opA==1) //registro
      modificarRegistro(a,orBit);
    else       //memoria
      memoria[registros[DS+a]] = orBit;
  }
  setCC(orBit);
}

void xor(int opA,int a,int opB,int b){
  int xorBit;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    xorBit = valor(opA,a) & valor(opB,b);
    if(opA==1) //registro
      modificarRegistro(a,xorBit);
    else       //memoria
      memoria[registros[DS+a]] = xorBit;
  }
  setCC(xorBit);
}

void shl(int opA,int a,int opB,int b){
  int corrimiento;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    corrimiento = valor(opA,a) << valor(opB,b);
    if(opA==1)  //registro
      modificarRegistro(a,corrimiento);
    else        //memoria
      memoria[registros[DS+a]] = corrimiento;
  }
  setCC(corrimiento);
}

void shr(int opA,int a,int opB,int b){
  int corrimiento;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    corrimiento = valor(opA,a) >> valor(opB,b);
    if(opA==1) //registro
      modificarRegistro(a,corrimiento);
    else    //memoria
      memoria[registros[DS+a]] = corrimiento;
  }
  setCC(corrimiento);
}

//1 operando

void read(){
  char car,lectura[MAXSTRING],binario[MAXBINARIO];
  int i,ingreso,conf,base;
  int alm = registros[0xE] & E; //almaceno desde posicion en memoria que esta en registro EDX
  conf = registros[0xA] & X;
  if((conf>>8) & 0x0001 == 0){ //interpreta dsp del enter
    for(i=0;i<registros[0xC]&X;i++){
      itoa(alm, binario, 2); //convierte la memoria en un string de ese numero en binario
      if((conf>>11) & 0x0001 == 0)) //escribe el prompt
        printf("[%s"+ binario + "]: "); //alm en binario
      scanf("%d",&ingreso); //leo
      printf("\n");
      if((conf>>3) & 0x0001 == 1) //hexa
        base = 16;
      else if((conf>>2) & 0x0001 == 1) //octal
        base = 8;
      else if(conf & 0x0001 == 1) //decimal
        base = 10;
      itoa(alm, cadena, base);
      memoria[alm++] = stringToInt(cadena,4BYTES);
    }
  }else{ //lee caracter a caracter dsp del enter
    i = 0;
    itoa(alm, binario, 2);
    if((conf>>11) & 0x0001 == 0)) //escribe el prompt
        printf("[%s" + binario + "]: "); //alm a binario
    scanf("%s",lectura);
    car = lectura[i]; //caracter
    while(car!=NULL && i<registros[0xC]&X){
      memoria[alm++] = car;
      car = lectura[++i];
    }
    memoria[alm] = 0x00000000;
  }
}

void write(){
  char almBinario,num[MAXSTRING],porcentaje,arroba;
  int prompt,endline,i,escribe;
  int alm = registros[0xE] & E; //leo desde posicion en memoria que esta en registro EDX
  porcentaje = '%';
  arroba = '@';
  conf = registros[0xA] & X;
  endline = (conf>>8) &0x0001; //0 agregar endline
  prompt = (conf>>11) & 0x0001;//agregar prompt
  for(i=0;i<registros[0xC]&X;i++){
   itoa(almBinario,alm,2);    //convierte la direccion de memoria en un string de ese numero en binario
   escribe = memoria[alm++];
   if((conf>>4) & 0x0001 == 1){ //caracter
    escribe = escribe & 0x000000ff; //byte menos significativo
    if(escribe<32 || escribe>126)  //ascii no imprimible
      escribe = '.';
    if(prompt) printf("[%s"+almBinario+"]");
      printf("%c ",escribe);
    if(endline) printf("\n");
   }
   if((conf>>3) & 0x0001 == 1){ //hexadecimal
    //itoa(escribe,num, 16);
    if(prompt) printf("[%s"+almBinario+"]");
    printf("%c%0X ",porcentaje,escribe);
    if(endline) printf("\n");
   }
   if((conf>>2) & 0x0001 == 1){ //octal
    //itoa(escribe,num,8);
    if(prompt) printf("[%s"+almBinario+"]");
    printf("%c%o ",arroba,escribe);
    if(endline) printf("\n");
   }
   if(conf& 0x0001 == 1)  //decimal
    if(prompt) printf("[%s"+almBinario+"]");
    printf("%d ",escribe);
    if(endline) printf("\n");
  }
}

void leerFlags(char flags[],int *cantFlags){
  int i,k;
  k = 0;
  for(i=3;i<argc;i++)
    strcpy(flags[k++],argv[i]);
  *cantFlags = k;
}

void breakpoint(){
  int i,k,h,f,cantFlags;
  char a[5],b[5],car,lectura[100];
  char* flags[10];
  leerFlags(flags,cantFlags);
  f = 0;
  while(f<cantFlags){
    if(strcmp(argv[f],"[-c]") == 0){ //clear screen
      system("cls");
    }else if(strcmp(argv[f],"[-b]") == 0){
      printf("[0005] cmd: ");
      fgets(lectura,100,stdin);
      printf("lectura: %s",lectura);

      i = 0;
      car = lectura[i];
      while(car!=NULL && car!=' '){ //lee primer operando
        a[i] = car;
        car = lectura[++i];
      }
      printf("a: %s\n",a);
      //printf("caracter final: %c",car);
      while(lectura[i]!=NULL && car==' ') //lee los espacios en el medio
        car = lectura[++i];
      if(lectura[i]!=NULL && car!=' '){ //si hay segundo operando
        k = 0;
        car = lectura[i];
        while(car!=NULL && car!=' '){ //lee segundo operando
          b[k++] = car;
          car = lectura[++i];
        }
        printf("b: %s\n",b);
      }else{          //no hay segundo operando
        printf("b: nulo\n");
        b[0] = NULL;
      }
      if(a[0]=='0' || a[0]=='1' || a[0]=='2' || a[0]=='3' || a[0]=='4' || a[0]=='5' || a[0]=='6' || a[0]=='7'
        || a[0]=='8' || a[0]=='9'){ //si es numero el primer operando
        i = atoi(a); //de string a numero
        if(i>=0 && i<=4096){//es valido
          printf("numero valido\n");
          printf("a atoi: %d\n",i);
          h = i;
          if(b[0]!=NULL){ //2 numeros
            i = atoi(b);
            if(i>=0 && i<=4096) //es valido
              printf("numero valido\n");
            else
              printf("numero invalido\n");
            printf("b atoi: %d\n",i);
            for(h;h<=i;h++){ //muestra el rango de memoria
              //printf("%d\n",h);
              printf("[%04d]: %0x %d",h,memoria[h],memoria[h]);
            }
          }else{ //1 numero muestra la direccion de memoria absoluta
            printf("[%d]: %0x %d",i,memoria[i],memoria[i]);
          }
        }else
          printf("numero invalido\n");

      }else{    //caracter
        if(a[0]=='r')
          printf("caracter r");
        else if(a[0]=='p'){
          printf("caracter p");
          breakpointActivado = 0;
        }
      }
    }else if(strcmp(argv[f],"[-d]") == 0){
      //disassembler
    }
    f++;
  }
}

void mostrarRegistros(){
  printf("Registros:\n");
  printf("DS = %d\n",registros[DS]);
  printf("IP = %d\n",registros[IP]);
  printf("CC = %d\n",registros[CC]);
  printf("AC = %d\n",registros[AC]);
  printf("EAX = %d\n",registros[0xA]);
  printf("EBX = %d\n",registros[0xB]);
  printf("ECX = %d\n",registros[0xC]);
  printf("EDX = %d\n",registros[0xD]);
  printf("EEX = %d\n",registros[0xE]);
  printf("EFX = %d\n",registros[0xF]);
}

void sys(int opA,int a){ //supongo que el operandoA es inmediato
  if(a==0x000000001)//lectura
    read();
  else if(a==0x000000002) //escritura
    write();
  else if(a==0x00000000f) // breakpoint
    breakpoint();
}

void jmp(int opA,int a){
  if(opA!=3)//si es valido
    registros[IP] = valor(opA,a);
}

void jp(int opA,int a){
  if(opA!=3)//si es valido
    if(((registros[CC] & 0x800000000)>>31) == 0x0)
      registros[IP] = valor(opA,a);
}

void jn(int opA,int a){
  if(opA!=3)//si es valido
    if(((registros[CC] & 0x800000000)>>31) == 0x1)
      registros[IP] = valor(opA,a);
}

void jz(int opA,int a){
  if(opA!=3)//si es valido
    if(registros[CC] & 0x000000001 == 0x1)
      registros[IP] = valor(opA,a);
}

void jnz(int opA,int a){
  if(opA!=3)//si es valido
    if(registros[CC] & 0x800000000 == 0x0)
      registros[IP] = valor(opA,a);
}

void ldh(int opA,int a){
  int operando,aux;
  operando = valor(opA,a) & 0x00ff;
  aux = registros[CC] & 0x00ff;
  registros[CC] = (operando << 8) | aux;
}

void ldl(int opA,int a){
  int operando,aux;
  operando = valor(opA,a) & 0x00ff;
  aux = registros[CC] & 0xff00;
  registros[CC] = operando | aux;
}

void rnd(int opA,int a){
  if(opA!=3 && opA!=0){
    int random = rand() % valor(opA,a); //de 0 hasta a chequear negativo
    registros[AC] = random;
  }
}

void not(int opA,int a){
  int negacion;
  if(opA!=3 && opA!=0){
    negacion = ~valor(opA,a);
    if(opA==1) //registros
      modificarRegistro(a,negacion);
    else //memoria
      memoria[registros[DS+a]] = negacion;
  }
  setCC(negacion);
}

//0 operandos

void stop(){ //detiene programa
  registros[IP] == registros[DS];
}

