/*
 * reportes.c
 * Historial y reporte final (Integrante 5).
 * Todo lo de este archivo es PROVISIONAL, salvo que se indique otra cosa.
 */
#include <stdio.h>
#include "reportes.h"
#include "interfaz.h"

/* PROVISIONAL: deja el historial vacio. */
void inicializarHistorial(EstadoSimulador *estado)
{
    estado->cantidadHistorial = 0;
}

/* PROVISIONAL (Integrante 5): opcion 6. */
void mostrarHistorialSimulaciones(const EstadoSimulador *estado)
{
    mostrarMensajeAdvertencia("[PROVISIONAL] Opcion 6 - Historial: pendiente (Integrante 5).");
    printf("Simulaciones registradas hasta ahora: %d\n", estado->cantidadHistorial);
}

/* PROVISIONAL (Integrante 5): opcion 7. */
void generarReporteFinal(const EstadoSimulador *estado)
{
    (void)estado;
    mostrarMensajeAdvertencia("[PROVISIONAL] Opcion 7 - Reporte final: pendiente (Integrante 5).");
}
