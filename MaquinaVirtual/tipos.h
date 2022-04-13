//tamaños
#define MAXREGISTRO 16
#define MAXMEMORIA 4096
#define MAXSTRING 100
#define MAXBINARIO 13 //hasta 4095 en binario 1111 1111 1111
#define CUATROBYTES 32
//registros
#define DS 0
#define IP 5
#define CC 8
#define AC 9
//mascaras para registros
#define E 0xffff
#define L 0x000f
#define H 0x00f0
#define X 0x00ff
//headers
void procesar(int [],int []);
void leerArchivo(int [],int []);
void leerInstruccion(int ,int *,int *,int *,int *,int *);
void configurarRegistros();
int stringToInt(char [],int );
//2 operandos
void mov(int,int,int,int);
void add(int,int,int,int);
void sub(int,int,int,int);
void swap(int,int,int,int);
void mul(int,int,int,int);
void division(int,int,int,int);
void cmp(int,int,int,int);
void shl(int,int,int,int);
void shr(int,int,int,int);
void and(int,int,int,int);
void or(int,int,int,int);
void xor(int,int,int,int);
//1 operando
void sys(int,int);
void jmp(int,int);
void jz(int,int);
void jp(int,int);
void jn(int,int);
void jnz(int,int);
void jnp(int,int);
void jnn(int,int);
void ldl(int,int);
void ldh(int,int);
void rnd(int,int,int,int); //duda
void not(int,int);
//0 operandos
void stop();


typedef void funciones0();
typedef void funciones1(int,int);
typedef void funciones2(int,int,int,int);


int registros[MAXREGISTRO];
int memoria[MAXMEMORIA];
int breakpointActivado = 1;



