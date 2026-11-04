/*
 * reportes.h
 * Prototipos del historial y del reporte final (Integrante 5).
 */
#ifndef REPORTES_H
#define REPORTES_H

#include "artemis.h"

void inicializarHistorial(EstadoSimulador *estado);
void mostrarHistorialSimulaciones(const EstadoSimulador *estado);   /* opcion 6 */
void generarReporteFinal(const EstadoSimulador *estado);            /* opcion 7 */

#endif /* REPORTES_H */
