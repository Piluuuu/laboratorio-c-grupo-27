/*
 * artemis.c
 * Simulador de Misiones Artemis - Escenario 4 (Reingreso, frenado y amerizaje)
 *
 * Parte del Integrante 3: Desaceleracion de la tripulacion y amerizaje.
 *
 * Las estructuras se reciben por PUNTERO (const cuando solo se leen) para no
 * copiarlas completas y para poder guardar los resultados en ResultadoFisico.
 */

#include <stdio.h>
#include <math.h>
#include "artemis.h"

/* =====================================================================
 *                 DESACELERACION DE LA TRIPULACION
 * ===================================================================== */

/* Desaceleracion media: a = (vi - vf) / t
 * Se usa valor absoluto para devolver el modulo (positivo).
 * Si el tiempo no es positivo se devuelve 0 para evitar dividir por cero
 * (el ingreso de datos tambien se controla en validaciones.c). */
double calcularDesaceleracionMedia(double velInicial, double velFinal, double tiempo)
{
    if (tiempo <= 0.0) return 0.0;
    return fabs(velFinal - velInicial) / tiempo;
}

/* Convierte la desaceleracion a multiplos de g: a / g */
double convertirDesaceleracionAG(double desaceleracion, double gravedad)
{
    if (gravedad <= 0.0) return 0.0;
    return desaceleracion / gravedad;
}

/* Clasifica la desaceleracion segun los limites definidos en artemis.h */
NivelDesaceleracion evaluarDesaceleracion(double desaceleracionG)
{
    if (desaceleracionG <= LIMITE_TOLERABLE) return DESACEL_TOLERABLE;
    if (desaceleracionG <= LIMITE_ELEVADA)   return DESACEL_ELEVADA;
    if (desaceleracionG <= LIMITE_PELIGROSA) return DESACEL_PELIGROSA;
    return DESACEL_CRITICA;
}

/* Calcula la desaceleracion, guarda los resultados en la estructura
 * 'resultado' (por puntero) y los muestra por pantalla. */
void mostrarAnalisisDesaceleracion(const DatosReingreso *reingreso,
                                   const DatosMision *mision,
                                   ResultadoFisico *resultado)
{
    NivelDesaceleracion nivel;

    resultado->desaceleracionMedia = calcularDesaceleracionMedia(
        reingreso->velocidadInicial, reingreso->velocidadFinal,
        reingreso->tiempoDesaceleracion);
    resultado->desaceleracionG = convertirDesaceleracionAG(
        resultado->desaceleracionMedia, mision->gravedad);
    nivel = evaluarDesaceleracion(resultado->desaceleracionG);

    printf("\n=== DESACELERACION DE LA TRIPULACION ===\n");
    printf("Velocidad inicial   : %.2f m/s\n", reingreso->velocidadInicial);
    printf("Velocidad final     : %.2f m/s\n", reingreso->velocidadFinal);
    printf("Tiempo              : %.2f s\n", reingreso->tiempoDesaceleracion);
    printf("Desaceleracion media: %.4f m/s^2\n", resultado->desaceleracionMedia);
    printf("En multiplos de g   : %.4f g\n", resultado->desaceleracionG);
    printf("Clasificacion       : ");
    switch (nivel) {
        case DESACEL_TOLERABLE: printf("TOLERABLE - segura para la tripulacion.\n"); break;
        case DESACEL_ELEVADA:   printf("ELEVADA - exigente pero soportable.\n"); break;
        case DESACEL_PELIGROSA: printf("PELIGROSA - riesgo de lesiones.\n"); break;
        default:                printf("CRITICA - puede ser letal.\n"); break;
    }
}

/* =====================================================================
 *                      AMERIZAJE E IMPACTO
 * ===================================================================== */

/* Energia cinetica justo antes del impacto: K = 1/2 * m * v^2 */
double calcularEnergiaImpacto(double masa, double velocidad)
{
    return 0.5 * masa * velocidad * velocidad;
}

/* Teorema trabajo-energia: W = K_final - K_inicial.
 * La capsula queda detenida (K_final = 0), entonces W = -K_inicial.
 * Es negativo porque el agua se opone al movimiento. */
double calcularTrabajoAgua(double energiaImpacto)
{
    return 0.0 - energiaImpacto;
}

/* El trabajo de una fuerza constante es W = -F * d, entonces F = |W| / d.
 * Se desprecia el peso durante el hundimiento (simplificacion de la consigna). */
double calcularFuerzaPromedioAgua(double trabajoAgua, double distancia)
{
    if (distancia <= 0.0) return 0.0;
    return fabs(trabajoAgua) / distancia;
}

/* Peso de la nave: P = m * g */
double calcularPesoNave(double masa, double gravedad)
{
    return masa * gravedad;
}

/* Fuerza del agua expresada como multiplo del peso: F / P */
double calcularFuerzaEnMultiploDePeso(double fuerza, double peso)
{
    if (peso <= 0.0) return 0.0;
    return fuerza / peso;
}

/* Interpreta el impacto segun cuantas veces el peso es la fuerza del agua */
const char *evaluarImpacto(double fuerzaEnPesos)
{
    if (fuerzaEnPesos <= 3.0)  return "Impacto suave: estructura y tripulacion seguras.";
    if (fuerzaEnPesos <= 6.0)  return "Impacto moderado: la estructura lo soporta, hay que revisarla.";
    if (fuerzaEnPesos <= 10.0) return "Impacto fuerte: riesgo de danos y lesiones.";
    return "Impacto severo: falla estructural probable.";
}

/* Calcula todas las magnitudes del amerizaje, las guarda en 'resultado'
 * (por puntero) y las muestra por pantalla. */
void mostrarAnalisisAmerizaje(const DatosAmerizaje *amerizaje,
                              const DatosMision *mision,
                              ResultadoFisico *resultado)
{
    resultado->energiaImpacto = calcularEnergiaImpacto(
        mision->masaOrion, amerizaje->velocidadImpacto);
    resultado->trabajoAgua = calcularTrabajoAgua(resultado->energiaImpacto);
    resultado->fuerzaPromedioAgua = calcularFuerzaPromedioAgua(
        resultado->trabajoAgua, amerizaje->distanciaHundimiento);
    resultado->pesoNave = calcularPesoNave(mision->masaOrion, mision->gravedad);
    resultado->fuerzaEnPesos = calcularFuerzaEnMultiploDePeso(
        resultado->fuerzaPromedioAgua, resultado->pesoNave);

    printf("\n=== AMERIZAJE E IMPACTO ===\n");
    printf("Velocidad de impacto : %.2f m/s\n", amerizaje->velocidadImpacto);
    printf("Distancia hundimiento: %.2f m\n", amerizaje->distanciaHundimiento);
    printf("Energia cinetica     : %.2f J\n", resultado->energiaImpacto);
    printf("Trabajo del agua     : %.2f J\n", resultado->trabajoAgua);
    printf("Fuerza promedio      : %.2f N\n", resultado->fuerzaPromedioAgua);
    printf("Peso de Orion        : %.2f N\n", resultado->pesoNave);
    printf("Fuerza / peso        : %.2f veces el peso\n", resultado->fuerzaEnPesos);
    printf("Evaluacion: %s\n", evaluarImpacto(resultado->fuerzaEnPesos));
}
