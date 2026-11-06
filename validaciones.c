#include "validaciones.h"

/* ============================================================
   MÓDULO DE VALIDACIONES
   Responsable: Integrante 4 - Seguridad y validaciones
   ============================================================ */

/* ---------- LECTURA SEGURA DE ENTEROS ---------- */
int leerEntero(const char *mensaje) {
    int valor;
    int resultado;

    do {
        printf("%s", mensaje);
        resultado = scanf("%d", &valor);

        if (resultado != 1) {
            printf("Error: Debe ingresar un número entero válido.\n");
            limpiarBufferEntrada();
        }
    } while (resultado != 1);

    limpiarBufferEntrada();
    return valor;
}

/* ---------- LECTURA SEGURA DE FLOTANTES ---------- */
float leerFlotante(const char *mensaje) {
    float valor;
    int resultado;

    do {
        printf("%s", mensaje);
        resultado = scanf("%f", &valor);

        if (resultado != 1) {
            printf("Error: Debe ingresar un número decimal válido.\n");
            limpiarBufferEntrada();
        }
    } while (resultado != 1);

    limpiarBufferEntrada();
    return valor;
}

/* ---------- LECTURA SEGURA DE CARACTERES ---------- */
char leerCaracter(const char *mensaje) {
    char caracter;

    printf("%s", mensaje);

    /* Limpiamos posibles residuos antes de leer el carácter */
    limpiarBufferEntrada();
    caracter = getchar();
    limpiarBufferEntrada();

    return caracter;
}

/* ---------- VALIDAR RANGO ---------- */
int validarRango(float valor, float min, float max) {
    if (valor >= min && valor <= max) {
        return 1;   /* Dentro del rango */
    }
    return 0;       /* Fuera del rango */
}

/* ---------- VALIDAR OPCIÓN DE MENÚ ---------- */
int validarOpcionMenu(int opcion, int maxOpcion) {
    if (opcion >= 1 && opcion <= maxOpcion) {
        return 1;
    }
    printf("Opción inválida. Debe elegir un número entre 1 y %d.\n", maxOpcion);
    return 0;
}

/* ---------- VALIDAR VALOR POSITIVO ---------- */
int validarValorPositivo(float valor) {
    if (valor > 0.0f) {
        return 1;
    }
    printf("Error: El valor debe ser mayor que cero.\n");
    return 0;
}

/* ---------- LIMPIAR BUFFER DE ENTRADA ---------- */
void limpiarBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Descarta todos los caracteres restantes en el buffer */
    }
}

/* ---------- VALIDAR ÁNGULO GENÉRICO ---------- */
int validarAngulo(float angulo) {
    /* Un ángulo de reingreso realista está entre 0° y 90° */
    if (angulo >= 0.0f && angulo <= 90.0f) {
        return 1;
    }
    printf("Error: El ángulo debe estar entre 0 y 90 grados.\n");
    return 0;
}
