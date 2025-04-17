#include<string.h>

#define PRUEBA 0
#define PROCESO  1
#define CONTEXTO 2
#define PROXIMA_INSTRUCCION 3
#define INTERRUMPIR_PROCESO 4

typedef struct
{
    int Valor; 
    int largoCadena;
    char * Cadena; 
}t_Prueba;

typedef struct {
    int size; // Tamaño del payload
    int offset; // Desplazamiento dentro del payload
    void* stream; // Payload
} t_buffer;


typedef struct {
    int TID;
    int PID;
    int resultado;
    int Tipo;
} t_Proceso;



typedef struct {
    int codigo_operacion;
    t_buffer* buffer;
} t_paquete;

typedef struct
{
uint8_t PID;    //Identificador de Proceso que esta corriendo
uint32_t PC;     //Program Counter, indica la próxima instrucción a ejecutar
uint8_t AX;        //Registro Numérico de propósito general
uint8_t BX;        //Registro Numérico de propósito general
uint8_t CX;        //Registro Numérico de propósito general
uint8_t DX;        //Registro Numérico de propósito general
uint32_t EAX;    //Registro Numérico de propósito general
uint32_t EBX;    //Registro Numérico de propósito general
uint32_t ECX;    //Registro Numérico de propósito general
uint32_t EDX;    //Registro Numérico de propósito general
uint32_t SI;     //Contiene la dirección lógica de memoria de origen desde donde se va a copiar un string.
uint32_t DI;     //Contiene la dirección lógica de memoria de destino a donde se va a copiar un string.
}t_Registros;

typedef struct
{
char Instruccion[10]; // Instruccion
char Parametro1[18]; //Paramatro 1

char Parametro2[18]; //Paramatro 2
char Parametro3[18]; //Paramatro 3
char Parametro4[18]; //Paramatro 4
char Parametro5[18]; //Paramatro 5
}t_Instruccion;
