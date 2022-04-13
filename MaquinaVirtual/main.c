#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"

int main(int argc, char *argv[]) //argc cantidad de argumentos argv argumentos
{
    funciones0* fun0[] = {stop};
    funciones1* fun1[] = {sys,jmp,jz,jp,jn,jnz,jnp,jnn,ldl,ldh,rnd,not};
    funciones2* fun2[] = {mov,add,sub,swap,mul,div,cmp,shl,shr,and,or,xor};

    configurarRegistros();//configurar memoria y registros
    leerArchivo();
    procesar(memoria,registros);

    int mnemo,opA,a,opB,b;
    mnemo = 0;
    opA = 0x00000002;
    a = 0x00000003;
    opB = 0x00000000;
    b = 0x00000002;
    (*fun[mnemo])(opA,a,opB,b);
    return 0;
}
