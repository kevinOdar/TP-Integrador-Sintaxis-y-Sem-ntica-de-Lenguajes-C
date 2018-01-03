#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "Rutinas.h"
#include "logger.c"

#define NUMESTADOS 15
#define NUMCOLS 13

FILE *archivo;

char buffer[100];
int sentenciaActual = 0;

///Parte 1 - SCANNER
int longitud(char v[]) {
	int i = 0;
	while (v[i] != '\0') {
		i++;
	}
	return i;
};

int estadoFinal(int e) {
	if (e == 0 || e == 1 || e == 3 || e == 11 || e == 14)
		return 0; //estados no finales retorna 0
	else
		return 1; //si es estado final retorna 1
}

///devuelve la columna a la que corresponde en el automata el caracter entrante
int columna(int caracter) {
	if (isalpha(caracter))
		return 0;
	if (isdigit(caracter))
		return 1;
	if (caracter == '+')
		return 2;
	if (caracter == '-')
		return 3;
	if (caracter == '(')
		return 4;
	if (caracter == ')')
		return 5;
	if (caracter == ',')
		return 6;
	if (caracter == ';')
		return 7;
	if (caracter == ':')
		return 8;
	if (caracter == '=')
		return 9;
	if (caracter == EOF)
		return 10;
	if (isspace(caracter))
		return 11;
	return 12;
}

int esPalabraReservada() {
	if (strcmp(buffer, "inicio") == 0)
		return 15;
	else if (strcmp(buffer, "fin") == 0)
		return 16;
	else if (strcmp(buffer, "leer") == 0)
		return 17;
	else if (strcmp(buffer, "escribir") == 0)
		return 18;
	else
		return 19;
}

TOKEN scanner() {
	int tabla[NUMESTADOS][NUMCOLS] = { { 1, 3, 5, 6, 7, 8, 9, 10, 11, 14, 13, 0,14 },
									 { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
									 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }
									 };
	int caracter;
	int col;
	int estado = 0;
	int i = 0; ///siempre vuelve a cargar el buffer desde 0

	///mientras no sea un estado final ni el estado de rechazo, va leyendo caracteres y los guarda en el buffer, el espacio lo ignora
	do {
		///toma un caracter del archivo de entrada
		caracter = fgetc(archivo);
		/// printf("\n %i",caracter);
		///busca la columna de ese caracter leido
		col = columna(caracter);
		///en la tabla busca en el estado en el que se encuentra la columna a la que pertenece el caracter leido y lo guarda nuevamente en estado con el nuevo estado al que se pasó
		estado = tabla[estado][col];
		///ignora los espacios:
		if (col != 11) {
			buffer[i] = caracter;
			i++;
		}
	} while (!estadoFinal(estado) && !(estado == 14));

	///corta la cadena
	buffer[i] = '\0';

	switch (estado) {
	///en los identificadores leyó un caracter de mas (!= space) en el buffer por lo que lo saca
	case 2:
		if (col != 11) {
			ungetc(caracter, archivo);
			buffer[i - 1] = '\0';
		}
		switch (esPalabraReservada()) {
		case 15:
			return INICIO;
		case 16:
			return FIN;
		case 17:
			return LEER;
		case 18:
			return ESCRIBIR;

		default:
			if (longitud(buffer) < 33)
				return ID;
			else
				return ERRORLEXICO;
		}
		break;

		///en las constantes leyó un caracter de mas (!= space) en el buffer por lo que lo saca
	case 4:
		if (col != 11) {
			ungetc(caracter, archivo);
			buffer[i - 1] = '\0';
		}
		return CONSTANTE;

	case 5:
		return SUMA;
	case 6:
		return RESTA;
	case 7:
		return PARENIZQUIERDO;
	case 8:
		return PARENDERECHO;
	case 9:
		return COMA;
	case 10:
		return PUNTOYCOMA;
	case 12:
		return ASIGNACION;
	case 13:
		return FDT;
	case 14:
		return ERRORLEXICO;
	}
}


///PARTE 2 PARSER
void errorSintactico(tokenEsperado)
{
    switch(tokenEsperado)
    {
        case INICIO:
            printf("se esperaba token Inicio \n");
            LOG_PRINT("Se esperaba token Inicio \n");
            exit(1);
        case FIN:
            printf("se esperaba token Fin \n");
            LOG_PRINT("Se esperaba token Fin \n");
            exit(1);
        default:
            printf("Error Sintactico en la sentencia %d \n", sentenciaActual);
            LOG_PRINT("Error Sintactico en la sentencia %d ", sentenciaActual);
            exit(1);
    }
}

void errorLexico(void) {
	printf("Error Lexico \n");
	LOG_PRINT("Error Lexico");
}
///.::::::::::::::::::::::::::::::inicio de MATCHEO:::::::::::::::::::::::::::::::.
TOKEN tokenActual;
TOKEN tokenFuturo;

TOKEN proximoToken() {
	tokenFuturo = scanner();
	if (tokenFuturo == ERRORLEXICO) {
		errorLexico();
	}
	return tokenFuturo;
}

void match(TOKEN t) {
	if (tokenActual != tokenFuturo) {
		if (t == tokenFuturo) {
			tokenActual = tokenFuturo;
		} else {
			errorSintactico(t);
		}
	} else {
		TOKEN tokenProximo = proximoToken();
		if (t == tokenProximo)
			tokenActual = tokenProximo;
		else {
			errorSintactico(t);
		}
	}
}
///.::::::::::::::::::::::::::::::fin de MATCHEO:::::::::::::::::::::::::::::::.

REG_EXPRESION genInfijo(REG_EXPRESION e1, REG_OPERACION op, REG_EXPRESION e2) ///RS pág. 54
{
	REG_EXPRESION t;
	char temporal[] = "Temp";
	temporal[4] = contadorDeTemporales;
	temporal[5] = '\0';
	generar("Declara", temporal, "Entera", "");
	generar(op.nombre, e1.nombre, e2.nombre, temporal);
	strcpy(t.nombre, temporal);
	contadorDeTemporales++;
	return t;
}


REG_OPERACION procesarOp(void) {
	REG_OPERACION t;
	if (strcmp(buffer, "+") == 0) {
		strcpy(t.nombre, "Suma");
	} else {
		strcpy(t.nombre, "Resta");
	}
	return t;
}

REG_EXPRESION procesarId(void) ///RS sacada del libro pág. 53(sin modificaciones)
{
	REG_EXPRESION t;
	chequear(buffer);
	t.clase = ID;
	strcpy(t.nombre, buffer);
	return t;
}

///se agrega una nueva producción (pag. 51): <identificador> -> ID #procesar_id
void identificador(REG_EXPRESION *t) {
	match(ID);
	*t = procesarId();
}

REG_EXPRESION procesarCte(void) ///RS sacada del libro pág. 53(sin modificaciones)
{
	REG_EXPRESION t;
	t.clase = CONSTANTE;
	strcpy(t.nombre, buffer);
	sscanf(buffer, "%d", &t.valor);
	return t;
}

void primaria(REG_EXPRESION* operando)   ///con RS's
{
	switch (proximoToken()) {
	case ID:
		identificador(operando);
		break;
	case CONSTANTE:
		match(CONSTANTE);
		*operando = procesarCte();   ///#procesar_cte
		break;
	case PARENIZQUIERDO:
		match(PARENIZQUIERDO);
		expresion(operando);
		match(PARENDERECHO);
		break;
	default:
		return;
	}
}

/// <operadorAditivo> -> uno de SUMA RESTA
void operadorAditivo(REG_OPERACION *op)   ///con RS's
{
	TOKEN t = tokenFuturo;   ///el token que generó proximoToken()
	///if (t == SUMA || t == RESTA)
        {
		match(t);
		*op = procesarOp();   ///#procesar_op
	}

}

/// <expresion> -> <primaria> {<operadorAditivo> <primaria> #gen_infijo}
void expresion(REG_EXPRESION *resultado) ///sacado del libro pág. 54 (sin modificaciones)
{
	REG_EXPRESION operandoIzq, operandoDer;
	REG_OPERACION op;
	TOKEN t;
	primaria(&operandoIzq);
	for (t = proximoToken(); t == SUMA || t == RESTA; t = proximoToken()) {
		operadorAditivo(&op);
		primaria(&operandoDer);
		operandoIzq = genInfijo(operandoIzq, op, operandoDer);   ///#gen_infijo
	}
	*resultado = operandoIzq;
}



void escribirExp(REG_EXPRESION exp) {

    escribir(exp);
}

void listaExpresiones(void) {

	REG_EXPRESION resultado;
	expresion(&resultado);
	escribirExp(resultado); ///#escribir_exp
	while (1) {
		switch (tokenFuturo) ///en vez de proximoToken utilizo tokenFuturo, ya que al invocar "expresion",
		{///previamente, se invoca proximoToken(que almacena el token en tokenFuturo)
		case COMA:
			match(COMA);
			expresion(&resultado);
			escribirExp(resultado); ///#escribir_exp
			break;
		default:
			return;
		}
	}
}

void leerID(REG_EXPRESION id) ///RS
{
	leer(id);
}

///<listaIdentificadores> -> ID {COMA ID}
void listaIdentificadores() {
	REG_EXPRESION t;
	identificador(&t);
	leerID(t);  ///#leer_id
	while (1) /// un ciclo indefinido
	{
		switch (proximoToken()) {
		case COMA:
			match(COMA);
			identificador(&t);
			leerID(t); ///#leer_id
			break;
		default:
			return;
		} /// fin switch
	}
}

int cantSentencias = 1;

void sentencia(void) {
	TOKEN token;
	sentenciaActual++;
	if (cantSentencias < 2)
		token = proximoToken();
	else
		token = tokenFuturo;
	REG_EXPRESION resultado;
	REG_EXPRESION t;
	switch (token) {
	case ID:	 /// <sentencia> -> ID := <expresion>; #asignar
		identificador(&t);
		match(ASIGNACION);
		expresion(&resultado);
		match(PUNTOYCOMA);
		asignar(t, resultado); ///#asignar
		break;
	case LEER: 	/// <sentencia> -> LEER ( <listaIdentificadores> );
		match(LEER);
		match(PARENIZQUIERDO);
		listaIdentificadores();
		match(PARENDERECHO);
		match(PUNTOYCOMA);
		break;
	case ESCRIBIR:	 /// <sentencia> -> ESCRIBIR (<listaExpresiones>);
		match(ESCRIBIR);
		match(PARENIZQUIERDO);
		listaExpresiones();
		match(PARENDERECHO);
		match(PUNTOYCOMA);
		break;
	default:
		errorSintactico(token);
		break;
	}
}

/// <listaSentencias> -> <sentencia> {<sentencia>}
void listaSentencias(void) {
	sentencia();
	while (1) /// ciclo infinito
	{
		cantSentencias++;
		switch (proximoToken()) {
		case ID:
		case LEER:
		case ESCRIBIR:  /// detectó token correcto
			sentencia();
			break;
		default:
			return;
		} /// fin switch
	}
}

void programa(void) {
	comenzar(); /// #comenzar
	match(INICIO);
	listaSentencias();
	match(FIN);
}

/// <objetivo> -> <programa> FDT
void objetivo(void) {
	programa();
	match(FDT);
	terminar(); /// #terminar
}

void parser(void) {
	objetivo();
}

int main() {
	archivo = fopen("programaFuente.txt", "r");
	tokenActual = tokenFuturo;
	i = 0;
	contadorDeTemporales = '1';
	parser();
	fclose(archivo);
	return 0;
}
