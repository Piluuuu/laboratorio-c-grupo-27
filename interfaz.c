/*
 * interfaz.c
 * Presentacion, menus y mensajes. Solo imprime; no calcula nada.
 * (Se escribe sin tildes para que se vea bien en cualquier consola.)
 */
#include <stdio.h>
#include "interfaz.h"
#include "artemis.h"

void mostrarPresentacion(void)
{
    printf("\n==============================================================\n");
    printf("            SIMULADOR DE MISIONES ARTEMIS - NASA\n");
    printf("==============================================================\n");
    printf(" Escenario 4: Reingreso atmosferico, frenado aerodinamico\n");
    printf("              y amerizaje (Trabajo, Energia e Impacto)\n");
    printf("--------------------------------------------------------------\n");
    printf(" El modulo de tripulacion Orion regresa de la Luna, frena en\n");
    printf(" la atmosfera, abre sus paracaidas y ameriza en el Pacifico.\n");
    printf("==============================================================\n");
}

void mostrarMenuPrincipal(void)
{
    printf("\n================= MENU PRINCIPAL =================\n");
    printf(" %d. Simulacion de reingreso atmosferico\n", OPCION_REINGRESO);
    printf(" %d. Analisis de desaceleracion\n", OPCION_DESACELERACION);
    printf(" %d. Simulacion de amerizaje\n", OPCION_AMERIZAJE);
    printf(" %d. Analisis del angulo de reingreso\n", OPCION_ANGULO);
    printf(" %d. Parametros de la mision (modificar / restablecer)\n", OPCION_PARAMETROS);
    printf(" %d. Historial de simulaciones\n", OPCION_HISTORIAL);
    printf(" %d. Reporte final de mision\n", OPCION_REPORTE);
    printf(" %d. Salir\n", OPCION_SALIR);
    printf("==================================================\n");
}

void mostrarMenuParametros(void)
{
    printf("\n------------ PARAMETROS DE LA MISION ------------\n");
    printf(" %d. Modificar parametros\n", PARAM_MODIFICAR);
    printf(" %d. Restablecer valores predeterminados\n", PARAM_RESTABLECER);
    printf(" %d. Volver al menu principal\n", PARAM_VOLVER);
    printf("-------------------------------------------------\n");
}

void mostrarMensajeError(const char *mensaje)
{
    printf("[ERROR] %s\n", mensaje);
}

void mostrarMensajeAdvertencia(const char *mensaje)
{
    printf("[ADVERTENCIA] %s\n", mensaje);
}

/* Espera un ENTER para que el usuario alcance a leer los resultados. */
void pausar(void)
{
    int c;
    printf("\nPresione ENTER para volver al menu...");
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
