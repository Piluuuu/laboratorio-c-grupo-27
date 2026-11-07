/*
 * validaciones.c
 * Lectura segura y validacion de datos de entrada.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include "validaciones.h"
#include "interfaz.h"
#include "artemis.h"

#define TAM_LINEA 128

/* Devuelve un puntero al primer caracter no blanco de 's'. */
static const char *saltarBlancos(const char *s)
{
    while (*s != '\0' && isspace((unsigned char)*s)) {
        s++;
    }
    return s;
}

/* ---------- LECTURA DE UNA LINEA ---------- */
ResultadoLectura leerLinea(const char *mensaje, char *buffer, size_t tam)
{
    size_t largo;

    printf("%s", mensaje);
    fflush(stdout);

    if (fgets(buffer, (int)tam, stdin) == NULL) {
        buffer[0] = '\0';
        return LECTURA_EOF;
    }

    largo = strlen(buffer);
    if (largo > 0 && buffer[largo - 1] == '\n') {
        buffer[--largo] = '\0';
    } else if (!feof(stdin)) {
        /* No entro la linea completa: se descarta el resto */
        limpiarBufferEntrada();
        return LECTURA_INVALIDA;
    }
    if (largo > 0 && buffer[largo - 1] == '\r') {   /* fin de linea de Windows */
        buffer[largo - 1] = '\0';
    }
    return LECTURA_OK;
}

/* ---------- LECTURA SEGURA DE ENTEROS ---------- */
ResultadoLectura leerEntero(const char *mensaje, int *destino)
{
    char linea[TAM_LINEA];
    const char *inicio;
    char *fin;
    long valor;
    ResultadoLectura lectura;

    lectura = leerLinea(mensaje, linea, sizeof linea);
    if (lectura != LECTURA_OK) {
        return lectura;
    }

    inicio = saltarBlancos(linea);
    if (*inicio == '\0') {
        return LECTURA_INVALIDA;                 /* linea vacia */
    }

    errno = 0;
    valor = strtol(inicio, &fin, 10);
    if (fin == inicio || *saltarBlancos(fin) != '\0') {
        return LECTURA_INVALIDA;                 /* "abc", "2abc", "3.7" */
    }
    if (errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
        return LECTURA_INVALIDA;                 /* desborde */
    }

    *destino = (int)valor;
    return LECTURA_OK;
}

/* ---------- LECTURA SEGURA DE FLOTANTES ---------- */
ResultadoLectura leerFlotante(const char *mensaje, double *destino)
{
    char linea[TAM_LINEA];
    const char *inicio;
    const char *p;
    char *fin;
    double valor;
    ResultadoLectura lectura;

    lectura = leerLinea(mensaje, linea, sizeof linea);
    if (lectura != LECTURA_OK) {
        return lectura;
    }

    inicio = saltarBlancos(linea);
    if (*inicio == '\0') {
        return LECTURA_INVALIDA;                 /* linea vacia */
    }

    /* Solo se admite notacion decimal comun: evita "nan", "inf" y hexadecimales */
    for (p = inicio; *p != '\0' && !isspace((unsigned char)*p); p++) {
        if (strchr("0123456789+-.eE", *p) == NULL) {
            return LECTURA_INVALIDA;
        }
    }

    errno = 0;
    valor = strtod(inicio, &fin);
    if (fin == inicio || *saltarBlancos(fin) != '\0') {
        return LECTURA_INVALIDA;                 /* "5.5abc", "5,5" */
    }
    if (errno == ERANGE || !isfinite(valor)) {
        return LECTURA_INVALIDA;                 /* "1e999" */
    }

    *destino = valor;
    return LECTURA_OK;
}

/* ---------- LECTURA SEGURA DE CARACTERES ---------- */
char leerCaracter(const char *mensaje)
{
    char linea[TAM_LINEA];
    const char *inicio;

    if (leerLinea(mensaje, linea, sizeof linea) != LECTURA_OK) {
        return '\0';
    }
    inicio = saltarBlancos(linea);
    return *inicio;                              /* '\0' si estaba vacia */
}

/* ---------- PEDIR DATOS HASTA QUE SEAN VALIDOS ---------- */
int pedirFlotanteEnRango(const char *mensaje, double min, double max, double *destino)
{
    char aviso[100];
    double valor;
    ResultadoLectura lectura;

    for (;;) {
        lectura = leerFlotante(mensaje, &valor);
        if (lectura == LECTURA_EOF) {
            return 0;
        }
        if (lectura == LECTURA_INVALIDA) {
            mostrarMensajeError("Entrada invalida: ingrese un numero decimal (ejemplo: 6.5).");
        } else if (!validarRango(valor, min, max)) {
            snprintf(aviso, sizeof aviso,
                     "El valor debe estar entre %g y %g.", min, max);
            mostrarMensajeError(aviso);
        } else {
            *destino = valor;
            return 1;
        }
    }
}

int pedirFlotantePositivo(const char *mensaje, double max, double *destino)
{
    char aviso[100];
    double valor;
    ResultadoLectura lectura;

    for (;;) {
        lectura = leerFlotante(mensaje, &valor);
        if (lectura == LECTURA_EOF) {
            return 0;
        }
        if (lectura == LECTURA_INVALIDA) {
            mostrarMensajeError("Entrada invalida: ingrese un numero decimal (ejemplo: 6.5).");
        } else if (!validarValorPositivo(valor) || valor > max) {
            snprintf(aviso, sizeof aviso,
                     "El valor debe ser mayor que 0 y no superar %g.", max);
            mostrarMensajeError(aviso);
        } else {
            *destino = valor;
            return 1;
        }
    }
}

/* ---------- VALIDAR RANGO ---------- */
int validarRango(double valor, double min, double max)
{
    return valor >= min && valor <= max;
}

/* ---------- VALIDAR OPCION DE MENU ---------- */
int validarOpcionMenu(int opcion, int min, int max)
{
    return opcion >= min && opcion <= max;
}

/* ---------- VALIDAR VALOR POSITIVO ---------- */
int validarValorPositivo(double valor)
{
    return isfinite(valor) && valor > 0.0;
}

/* ---------- LIMPIAR BUFFER DE ENTRADA ---------- */
void limpiarBufferEntrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta caracteres sobrantes */
    }
}

/* ---------- VALIDAR ANGULO ---------- */
int validarAngulo(double angulo)
{
    return isfinite(angulo) &&
           validarRango(angulo, ANGULO_MIN_ENTRADA, ANGULO_MAX_ENTRADA);
}
