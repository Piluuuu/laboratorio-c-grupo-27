/*
 * main.c
 * Punto de entrada y navegacion del Simulador Artemis (Escenario 4).
 * Aqui NO hay calculos fisicos: solo se coordina a los demas modulos.
 */
#include <stdio.h>
#include <string.h>
#include "artemis.h"
#include "validaciones.h"
#include "interfaz.h"
#include "reportes.h"

/* Prototipos de las funciones de este archivo (solo las usa main.c) */
void inicializarSimulador(EstadoSimulador *estado);
void ejecutarMenuPrincipal(EstadoSimulador *estado);
int  leerOpcionPrincipal(void);
void procesarOpcionPrincipal(int opcion, EstadoSimulador *estado);
void ejecutarSubmenuParametros(EstadoSimulador *estado);
void finalizarSimulador(void);

int main(void)
{
    EstadoSimulador estado;           /* todo el estado del simulador */

    inicializarSimulador(&estado);
    mostrarPresentacion();
    ejecutarMenuPrincipal(&estado);
    finalizarSimulador();

    return 0;
}

/* Deja el simulador listo: parametros originales, resultados en 0, historial vacio. */
void inicializarSimulador(EstadoSimulador *estado)
{
    cargarDatosPredeterminados(estado);
    memset(&estado->fisico, 0, sizeof(ResultadoFisico));
    memset(&estado->seguridad, 0, sizeof(ResultadoSeguridad));
    inicializarHistorial(estado);
}

/* Ciclo principal: muestra el menu, lee, ejecuta y vuelve, hasta elegir Salir. */
void ejecutarMenuPrincipal(EstadoSimulador *estado)
{
    int opcion;

    do {
        opcion = leerOpcionPrincipal();
        procesarOpcionPrincipal(opcion, estado);
    } while (opcion != OPCION_SALIR);
}

/*
 * Muestra el menu y pide una opcion hasta que sea valida (1 a 8).
 * Si la entrada se cierra (EOF, Ctrl+D / Ctrl+Z) devuelve OPCION_SALIR
 * para no quedar en un bucle infinito.
 */
int leerOpcionPrincipal(void)
{
    int opcion = 0;
    int opcionValida = 0;

    do {
        mostrarMenuPrincipal();
        if (!leerEntero("Seleccione una opcion: ", &opcion)) {
            mostrarMensajeAdvertencia("Entrada cerrada. Se sale del programa.");
            opcion = OPCION_SALIR;
            opcionValida = 1;
        } else if (validarOpcionMenu(opcion, OPCION_REINGRESO, OPCION_SALIR)) {
            opcionValida = 1;
        } else {
            mostrarMensajeError("Opcion inexistente. Elija un numero entre 1 y 8.");
        }
    } while (!opcionValida);

    return opcion;
}

/* Deriva la opcion elegida a la funcionalidad de cada integrante. */
void procesarOpcionPrincipal(int opcion, EstadoSimulador *estado)
{
    switch (opcion) {
        case OPCION_REINGRESO:
            mostrarAnalisisRozamiento(&estado->reingreso, &estado->mision,
                              &estado->fisico);
            break;
        case OPCION_DESACELERACION:
            mostrarAnalisisDesaceleracion(estado);
            break;
        case OPCION_AMERIZAJE:
            mostrarAnalisisAmerizaje(estado);
            break;
        case OPCION_ANGULO:
            mostrarAnalisisSeguridad(estado);
            break;
        case OPCION_PARAMETROS:
            ejecutarSubmenuParametros(estado);
            break;
        case OPCION_HISTORIAL:
            mostrarHistorialSimulaciones(estado);
            break;
        case OPCION_REPORTE:
            generarReporteFinal(estado);
            break;
        case OPCION_SALIR:
            break;                    /* no hace nada: el ciclo termina */
        default:
            mostrarMensajeError("Opcion no reconocida.");
            break;
    }

    if (opcion != OPCION_SALIR && opcion != OPCION_PARAMETROS) {
        pausar();                     /* tiempo para leer antes de volver al menu */
    }
}

/* Submenu de la opcion 5: modificar o restablecer parametros. */
void ejecutarSubmenuParametros(EstadoSimulador *estado)
{
    int opcion = PARAM_VOLVER;
    int terminar = 0;

    do {
        mostrarMenuParametros();
        if (!leerEntero("Seleccione una opcion: ", &opcion)) {
            terminar = 1;             /* EOF: volver; el menu principal tambien saldra */
        } else if (!validarOpcionMenu(opcion, PARAM_MODIFICAR, PARAM_VOLVER)) {
            mostrarMensajeError("Opcion inexistente. Elija un numero entre 1 y 3.");
        } else if (opcion == PARAM_MODIFICAR) {
            modificarDatosMision(estado);
        } else if (opcion == PARAM_RESTABLECER) {
            restablecerDatosMision(estado);
        } else {
            terminar = 1;             /* PARAM_VOLVER */
        }
    } while (!terminar);
}

/* Cierra el programa con un mensaje de despedida. */
void finalizarSimulador(void)
{
    printf("\nGracias por utilizar el Simulador Artemis. Fin del programa.\n");
}
