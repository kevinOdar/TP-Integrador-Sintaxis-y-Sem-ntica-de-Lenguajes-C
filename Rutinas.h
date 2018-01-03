#include "logger.h"

/// Tipo TOKEN
typedef enum
{
    INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,PARENDERECHO,
    PUNTOYCOMA,COMA, ASIGNACION, SUMA, RESTA, FDT, ERRORLEXICO
}TOKEN;


///                RUTINAS SEMÁNTICAS
typedef struct
{
    char nombre[15];
} REG_OPERACION;///TOKEN SUMA/RESTA

typedef struct///pag 51
{
    TOKEN clase; /// ID/CONSTANTE
    char nombre[32];
    int valor;
}REG_EXPRESION;/// contendrá el tipo de expresión y el valor que le corresponde

typedef struct///TABLA DE SIMBOLOS
{
    char identificador[32];///un solo campo porque , al no interesar las constantes, solo almacena los identificadores(son tokens tipo "ID")

}Tabla;

Tabla tablaDeSimbolos[100];

void leer(REG_EXPRESION id)
{
    generar("Leer ", id.nombre, "Entera", "");
} ///                 pág. 52

FILE *archivoConRutinas;

void generar(char s[], char s2[], char s3[], char s4[])
{
    printf("\n"); printf("%s ", s);  printf("%s ", s2);  printf("%s ", s3);  printf("%s ", s4);  printf("\n");
    LOG_PRINT("%s %s %s %s ", s,s2,s3,s4);

}

bool buscar(char s[])
{
    int contador = 0;
    bool seEncontro = false;
    while(contador<100 && (strcmp(tablaDeSimbolos[contador].identificador, s)!=0) ) /// Es un array
    {
        contador++;
    }
    if(strcmp(tablaDeSimbolos[contador].identificador, s)==0)
        return true;
    else
        return false;
}

int i; ///almacena el indice en donde se guardará el nuevo identificador

void colocar(char s[])
{
    strcpy(tablaDeSimbolos[i].identificador, s);
    i++;
}

void chequear(char s[])
{
    if(!buscar(s))
    {
        colocar(s);
        generar("Declara ", s, "entera", "");
    }
}

char* extraer(REG_EXPRESION cualquier)
{
    return cualquier.nombre;
}

void escribir(REG_EXPRESION exp)
{
    generar("Escribir ", exp.nombre , "Entera", "");
}

void comenzar(void)
{
    generar("Comienza", "","","");
}

void asignar(REG_EXPRESION izquierda, REG_EXPRESION derecha)
{
    ///generar("Almacena", extraer(derecha), izquierda.nombre, "");///arreglar extraer
    generar("Almacena", derecha.nombre, izquierda.nombre, "");///arreglar extraer

}

void terminar(void)
{
    generar("Detiene", "","","");
}

///---pág. 54---
char contadorDeTemporales;

