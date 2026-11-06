#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/* ========== FUNCIONES DE LECTURA SEGURA ========== */

/* Solicita un número entero y verifica que sea válido.
   Retorna el entero leído. */
int leerEntero(const char *mensaje);

/* Solicita un número decimal (float) y verifica que sea válido.
   Retorna el float leído. */
float leerFlotante(const char *mensaje);

/* Lee un carácter de forma segura (ignora espacios/enter previos).
   Retorna el carácter leído. */
char leerCaracter(const char *mensaje);

/* ========== FUNCIONES DE VALIDACIÓN ========== */

/* Verifica que un valor esté dentro de un intervalo [min, max].
   Retorna 1 si está dentro, 0 si no. */
int validarRango(float valor, float min, float max);

/* Verifica que la opción elegida exista en el menú (entre 1 y maxOpcion).
   Retorna 1 si es válida, 0 si no. */
int validarOpcionMenu(int opcion, int maxOpcion);

/* Evita masas, tiempos y distancias negativas o nulas cuando no corresponden.
   Retorna 1 si el valor es positivo (> 0), 0 si no. */
int validarValorPositivo(float valor);

/* Limpia el buffer de entrada para evitar que queden caracteres
   pendientes que afecten futuras lecturas. */
void limpiarBufferEntrada(void);

/* Controla que el ángulo ingresado sea un valor válido
   (generalmente entre 0 y 90 grados).
   Retorna 1 si es válido, 0 si no. */
int validarAngulo(float angulo);

#endif
