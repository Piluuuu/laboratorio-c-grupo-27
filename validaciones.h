/*
 * validaciones.h
 * Lectura segura de datos por consola y funciones de validacion.
 * Todas las lecturas se hacen por linea completa (fgets) y se convierten con
 * strtol / strtod, comprobando que no sobre texto ni haya desbordes.
 */
#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <stddef.h>   /* size_t */

/* Resultado de una lectura */
typedef enum {
    LECTURA_OK = 0,      /* dato valido leido */
    LECTURA_INVALIDA,    /* texto que no es un numero valido (o demasiado largo) */
    LECTURA_EOF          /* la entrada se cerro (Ctrl+D / Ctrl+Z) */
} ResultadoLectura;

/* ========== FUNCIONES DE LECTURA SEGURA ========== */

/* Muestra 'mensaje' y lee una linea completa en 'buffer' (sin el '\n').
   Devuelve LECTURA_OK, LECTURA_INVALIDA (linea demasiado larga) o LECTURA_EOF. */
ResultadoLectura leerLinea(const char *mensaje, char *buffer, size_t tam);

/* Solicita un entero. Solo escribe en *destino si devuelve LECTURA_OK.
   Rechaza "2abc", "3.7", "", desbordes, etc. */
ResultadoLectura leerEntero(const char *mensaje, int *destino);

/* Solicita un decimal finito. Solo escribe en *destino si devuelve LECTURA_OK.
   Rechaza "5.5abc", "5,5", "nan", "inf", "1e999", "", etc. */
ResultadoLectura leerFlotante(const char *mensaje, double *destino);

/* Lee una linea y devuelve su primer caracter no blanco.
   Devuelve '\0' si la linea esta vacia o la entrada se cerro. */
char leerCaracter(const char *mensaje);

/* Repite la pregunta, informando el error, hasta obtener un decimal dentro de
   [min, max]. Devuelve 1 si lo obtuvo, 0 si la entrada se cerro (EOF). */
int pedirFlotanteEnRango(const char *mensaje, double min, double max, double *destino);

/* Igual, pero exige un valor estrictamente positivo y menor o igual a 'max'. */
int pedirFlotantePositivo(const char *mensaje, double max, double *destino);

/* ========== FUNCIONES DE VALIDACION ========== */

/* Verifica que un valor este dentro de [min, max]. Devuelve 1 si, 0 no. */
int validarRango(double valor, double min, double max);

/* Verifica que la opcion este entre min y max (inclusive). */
int validarOpcionMenu(int opcion, int min, int max);

/* Evita masas, tiempos y distancias negativas, nulas o no finitas.
   Devuelve 1 si el valor es finito y > 0. */
int validarValorPositivo(double valor);

/* Descarta lo que quede en la linea de entrada actual (hasta '\n' o EOF). */
void limpiarBufferEntrada(void);

/* Controla que el angulo sea finito y este entre 0 y 90 grados. */
int validarAngulo(double angulo);

#endif /* VALIDACIONES_H */
