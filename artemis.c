/*
 * artemis.c
 * Logica del simulador (calculos fisicos y gestion de datos).
 *
 * Integrante 1 solo escribio cargarDatosPredeterminados().
 * Las funciones marcadas "PROVISIONAL" son reemplazos TEMPORALES para que el
 * proyecto compile y se pueda probar el menu. Cada integrante debe BORRAR
 * el cuerpo provisional y poner su implementacion, MANTENIENDO el prototipo.
 */
#include <stdio.h>
#include "artemis.h"
#include "interfaz.h"

/* Carga los valores originales del escenario (los de la consigna). */
void cargarDatosPredeterminados(EstadoSimulador *estado)
{
    estado->mision.masaOrion = MASA_ORION_PREDET;
    estado->mision.gravedad = GRAVEDAD_TERRESTRE;

    estado->reingreso.velocidadInicial = VELOCIDAD_INICIAL_PREDET;
    estado->reingreso.alturaInicial = ALTURA_INICIAL_PREDET;
    estado->reingreso.velocidadFinal = VELOCIDAD_FINAL_PREDET;
    estado->reingreso.alturaFinal = ALTURA_FINAL_PREDET;
    estado->reingreso.tiempoDesaceleracion = TIEMPO_DESACELERACION_PREDET;

    estado->amerizaje.velocidadImpacto = VELOCIDAD_IMPACTO_PREDET;
    estado->amerizaje.distanciaHundimiento = DISTANCIA_HUNDIMIENTO_PREDET;
}

/* ===================== PROVISIONALES ===================== */

/* PROVISIONAL (Integrante 4 o quien se asigne): modificar parametros. */
void modificarDatosMision(EstadoSimulador *estado)
{
    (void)estado;
    mostrarMensajeAdvertencia("[PROVISIONAL] Modificar parametros: pendiente de implementar.");
}

/* PROVISIONAL: la version final puede agregar mas mensajes o validaciones. */
void restablecerDatosMision(EstadoSimulador *estado)
{
    cargarDatosPredeterminados(estado);
    printf("[PROVISIONAL] Parametros restablecidos a los valores originales.\n");
}

/* PROVISIONAL (Integrante 2): opcion 1, trabajo de rozamiento y energia. */
void mostrarAnalisisRozamiento(EstadoSimulador *estado)
{
    (void)estado;
    mostrarMensajeAdvertencia("[PROVISIONAL] Opcion 1 - Reingreso atmosferico: pendiente (Integrante 2).");
}

/* PROVISIONAL (Integrante 3): opcion 2, desaceleracion media. */
void mostrarAnalisisDesaceleracion(EstadoSimulador *estado)
{
    (void)estado;
    mostrarMensajeAdvertencia("[PROVISIONAL] Opcion 2 - Desaceleracion: pendiente (Integrante 3).");
}

/* PROVISIONAL (Integrante 3): opcion 3, impacto y fuerza del agua. */
void mostrarAnalisisAmerizaje(EstadoSimulador *estado)
{
    (void)estado;
    mostrarMensajeAdvertencia("[PROVISIONAL] Opcion 3 - Amerizaje: pendiente (Integrante 3).");
}

/* PROVISIONAL (Integrante 4): opcion 4, seguridad del angulo. */
void mostrarAnalisisSeguridad(EstadoSimulador *estado)
{
    (void)estado;
    mostrarMensajeAdvertencia("[PROVISIONAL] Opcion 4 - Angulo de reingreso: pendiente (Integrante 4).");
}
