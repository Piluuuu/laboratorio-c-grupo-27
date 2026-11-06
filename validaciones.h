#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/* ========== FUNCIONES DE LECTURA SEGURA ========== */

/* Solicita un entero. Lo guarda en *destino.
   Retorna 1 si se leyo bien, 0 si hubo error o EOF. */
int leerEntero(const char *mensaje, int *destino);

/* Solicita un decimal. Lo guarda en *destino.
   Retorna 1 si se leyo bien, 0 si hubo error o EOF. */
int leerFlotante(const char *mensaje, double *destino);

/* Lee un caracter de forma segura.
   Retorna el caracter leido. */
char leerCaracter(const char *mensaje);

/* ========== FUNCIONES DE VALIDACION ========== */

/* Verifica que un valor este dentro de [min, max].
   Retorna 1 si esta dentro, 0 si no. */
int validarRango(double valor, double min, double max);

/* Verifica que la opcion este entre min y max (inclusive).
   Retorna 1 si es valida, 0 si no. */
int validarOpcionMenu(int opcion, int min, int max);

/* Evita masas, tiempos y distancias negativas o nulas.
   Retorna 1 si el valor es positivo (> 0), 0 si no. */
int validarValorPositivo(double valor);

/* Limpia el buffer de entrada. */
void limpiarBufferEntrada(void);

/* Controla que el angulo este entre 0 y 90 grados.
   Retorna 1 si es valido, 0 si no. */
int validarAngulo(double angulo);

#endif
