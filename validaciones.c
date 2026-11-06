#include "validaciones.h"

/* ============================================================
   MODULO DE VALIDACIONES
   Responsable: Integrante 4 - Seguridad y validaciones
   ============================================================ */

/* ---------- LECTURA SEGURA DE ENTEROS ---------- */
int leerEntero(const char *mensaje, int *destino)
{
    int resultado;

    printf("%s", mensaje);
    resultado = scanf("%d", destino);

    if (resultado != 1) {
        limpiarBufferEntrada();
        return 0;   /* error o EOF */
    }

    limpiarBufferEntrada();
    return 1;       /* ok */
}

/* ---------- LECTURA SEGURA DE FLOTANTES ---------- */
int leerFlotante(const char *mensaje, double *destino)
{
    int resultado;

    printf("%s", mensaje);
    resultado = scanf("%lf", destino);

    if (resultado != 1) {
        limpiarBufferEntrada();
        return 0;
    }

    limpiarBufferEntrada();
    return 1;
}

/* ---------- LECTURA SEGURA DE CARACTERES ---------- */
char leerCaracter(const char *mensaje)
{
    char caracter;

    printf("%s", mensaje);
    limpiarBufferEntrada();
    caracter = getchar();
    limpiarBufferEntrada();

    return caracter;
}

/* ---------- VALIDAR RANGO ---------- */
int validarRango(double valor, double min, double max)
{
    if (valor >= min && valor <= max) {
        return 1;
    }
    return 0;
}

/* ---------- VALIDAR OPCION DE MENU ---------- */
int validarOpcionMenu(int opcion, int min, int max)
{
    if (opcion >= min && opcion <= max) {
        return 1;
    }
    return 0;
}

/* ---------- VALIDAR VALOR POSITIVO ---------- */
int validarValorPositivo(double valor)
{
    if (valor > 0.0) {
        return 1;
    }
    return 0;
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
    if (angulo >= 0.0 && angulo <= 90.0) {
        return 1;
    }
    return 0;
}
