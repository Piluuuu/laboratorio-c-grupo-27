/*
 * reportes.h
 * Historial de simulaciones y reporte final.
 */
#ifndef REPORTES_H
#define REPORTES_H

#include "artemis.h"

/* Nombre del archivo donde se guarda el reporte final */
#define ARCHIVO_REPORTE "reporte_artemis.txt"

void inicializarHistorial(EstadoSimulador *estado);

/* Guarda en el historial la simulacion 'tipo' (OPCION_REINGRESO,
 * OPCION_DESACELERACION, OPCION_AMERIZAJE u OPCION_ANGULO) con una copia de los
 * parametros y resultados actuales, y la marca como calculada. */
void registrarSimulacion(EstadoSimulador *estado, int tipo);

void mostrarHistorialSimulaciones(const EstadoSimulador *estado);   /* opcion 6 */

/* Muestra el reporte por pantalla y lo guarda en ARCHIVO_REPORTE. */
void generarReporteFinal(const EstadoSimulador *estado);            /* opcion 7 */

/* Cantidad de simulaciones de cada tipo (parte de la opcion 7). */
void mostrarEstadisticasMision(const EstadoSimulador *estado);

#endif /* REPORTES_H */
