/*
 * validaciones.c
 * Ingreso seguro de enteros. Se lee SIEMPRE una linea completa con fgets()
 * y despues se convierte con strtol(), porque scanf("%d") se rompe (o deja
 * basura en el buffer) cuando el usuario escribe letras.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include "validaciones.h"
#include "interfaz.h"

#define TAM_LINEA   64
#define LINEA_OK     1
#define LINEA_EOF    0
#define LINEA_LARGA -1

/* Descarta lo que quede en stdin hasta el proximo '\n' (o EOF). */
void limpiarBufferEntrada(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/*
 * Lee una linea y le saca el '\n'.
 * Devuelve LINEA_OK, LINEA_EOF (entrada cerrada) o LINEA_LARGA (no entro
 * en el arreglo; en ese caso limpia el buffer).
 */
static int leerLinea(char *linea, int tamanio)
{
    int estado = LINEA_OK;
    size_t largo;

    if (fgets(linea, tamanio, stdin) == NULL) {
        estado = LINEA_EOF;
    } else {
        largo = strlen(linea);
        if (largo > 0 && linea[largo - 1] == '\n') {
            linea[largo - 1] = '\0';
        } else if (largo == (size_t)(tamanio - 1)) {
            limpiarBufferEntrada();      /* sobraron caracteres */
            estado = LINEA_LARGA;
        }
    }
    return estado;
}

/*
 * Convierte el texto a int. Muestra el error correspondiente y devuelve 0
 * si el texto esta vacio, tiene letras/decimales o no entra en un int.
 */
static int convertirEntero(const char *texto, int *valor)
{
    char *fin;
    long numero;
    int valido = 0;

    while (isspace((unsigned char)*texto)) {      /* saltar espacios */
        texto++;
    }

    if (*texto == '\0') {
        mostrarMensajeError("No ingreso nada. Escriba un numero entero.");
    } else {
        errno = 0;
        numero = strtol(texto, &fin, 10);
        while (isspace((unsigned char)*fin)) {    /* espacios finales */
            fin++;
        }
        if (fin == texto || *fin != '\0') {
            mostrarMensajeError("Entrada invalida. Solo se permiten numeros enteros (sin letras ni decimales).");
        } else if (errno == ERANGE || numero > INT_MAX || numero < INT_MIN) {
            mostrarMensajeError("El numero es demasiado grande.");
        } else {
            *valor = (int)numero;
            valido = 1;
        }
    }
    return valido;
}

int leerEntero(const char *mensaje, int *valor)
{
    char linea[TAM_LINEA];
    int estado;
    int valido = 0;
    int entradaAbierta = 1;

    do {
        printf("%s", mensaje);
        estado = leerLinea(linea, TAM_LINEA);

        if (estado == LINEA_EOF) {
            entradaAbierta = 0;
        } else if (estado == LINEA_LARGA) {
            mostrarMensajeError("La entrada es demasiado larga.");
        } else {
            valido = convertirEntero(linea, valor);
        }
    } while (!valido && entradaAbierta);

    return valido;
}

int validarRango(int valor, int minimo, int maximo)
{
    return (valor >= minimo && valor <= maximo);
}

int validarOpcionMenu(int opcion, int minimo, int maximo)
{
    return validarRango(opcion, minimo, maximo);
}
