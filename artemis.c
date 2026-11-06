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
#include <string.h>
#include "artemis.h"

/* ============================================================
 *          ENERGIAS Y TRABAJO DURANTE EL REINGRESO
 * ============================================================ */

void calcularResultadosReingreso(const DatosMision *mision,
                                 const DatosReingreso *reingreso,
                                 ResultadoFisico *resultado)
{
    double masa = mision->masaOrion;
    double gravedad = mision->gravedad;
    double vi = reingreso->velocidadInicial;
    double vf = reingreso->velocidadFinal;

    resultado->energiaCineticaInicial = 0.5 * masa * vi * vi;
    resultado->energiaCineticaFinal = 0.5 * masa * vf * vf;
    resultado->energiaPotencialInicial = masa * gravedad * reingreso->alturaInicial;
    resultado->energiaPotencialFinal = masa * gravedad * reingreso->alturaFinal;

    resultado->trabajoGravedad =
        resultado->energiaPotencialInicial - resultado->energiaPotencialFinal;
    resultado->trabajoRozamiento =
        resultado->energiaCineticaFinal - resultado->energiaCineticaInicial
        - resultado->trabajoGravedad;
    resultado->energiaDisipada = -resultado->trabajoRozamiento;
}
/* Muestra los resultados del reingreso atmosferico. */
void mostrarAnalisisRozamiento(const DatosReingreso *reingreso,
                               const DatosMision *mision,
                               ResultadoFisico *resultado)
{
    calcularResultadosReingreso(mision, reingreso, resultado);

    printf("\n--- Reingreso atmosferico ---\n");
    printf("Energia cinetica inicial: %.2f J\n", resultado->energiaCineticaInicial);
    printf("Energia cinetica final: %.2f J\n", resultado->energiaCineticaFinal);
    printf("Energia potencial inicial: %.2f J\n", resultado->energiaPotencialInicial);
    printf("Energia potencial final: %.2f J\n", resultado->energiaPotencialFinal);
    printf("Trabajo de la gravedad: %.2f J\n", resultado->trabajoGravedad);
    printf("Trabajo del rozamiento: %.2f J\n", resultado->trabajoRozamiento);
    printf("Energia disipada: %.2f J\n", resultado->energiaDisipada);
    printf("La resistencia del aire reduce la rapidez de la capsula.\n");
    printf("El escudo termico protege la capsula del calentamiento durante el reingreso.\n");
}

/* Carga los valores originales del escenario Artemis. */
void cargarDatosPredeterminados(EstadoSimulador *estado)
{
    estado->mision = (DatosMision){
        .masaOrion = 10400.0,
        .gravedad = GRAVEDAD_TERRESTRE
    };

    estado->reingreso = (DatosReingreso){
        .velocidadInicial = 11000.0,
        .velocidadFinal = 25.0,
        .tiempoDesaceleracion = 400.0,
        .alturaInicial = 120000.0,
        .alturaFinal = 2500.0
    };

    estado->amerizaje = (DatosAmerizaje){
        .velocidadImpacto = 8.0,
        .distanciaHundimiento = 1.50
    };
}
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
/* ============================================================
 *     ANGULO DE REINGRESO Y SEGURIDAD  (Integrante 4)
 * ============================================================ */

/* Verifica si el angulo esta dentro de la ventana de seguridad (5.5° a 7.0°) */
int validarAnguloReingreso(double angulo)
{
    if (angulo >= ANGULO_MIN_SEGURO && angulo <= ANGULO_MAX_SEGURO) {
        return 1;
    }
    return 0;
}

/* Clasifica el tipo de trayectoria segun el angulo */
void evaluarTrayectoriaReingreso(double angulo, char *resultado)
{
    if (angulo < ANGULO_MIN_SEGURO) {
        strcpy(resultado, "REBOTE_ATMOSFERICO");
    } else if (angulo > ANGULO_MAX_SEGURO) {
        strcpy(resultado, "DESACELERACION_EXCESIVA");
    } else {
        strcpy(resultado, "SEGURA");
    }
}

/* Explica las consecuencias fisicas segun el angulo */
void analizarConsecuenciasAngulo(double angulo, char *explicacion)
{
    if (angulo < ANGULO_MIN_SEGURO) {
        sprintf(explicacion,
            "ANGULO BAJO (%.2f < 5.5):\n"
            "  - La capsula no penetra lo suficiente en la atmosfera.\n"
            "  - Se produce un REBOTE ATMOSFERICO.\n"
            "  - La nave sale de nuevo al espacio y se pierde la mision.\n"
            "  - No se logra el amerizaje previsto.",
            angulo);
    } else if (angulo > ANGULO_MAX_SEGURO) {
        sprintf(explicacion,
            "ANGULO ALTO (%.2f > 7.0):\n"
            "  - La capsula entra demasiado pronunciada en la atmosfera.\n"
            "  - Se genera DESACELERACION EXCESIVA.\n"
            "  - La tripulacion experimenta fuerzas g peligrosas.\n"
            "  - Riesgo de dano estructural o lesiones graves.",
            angulo);
    } else {
        sprintf(explicacion,
            "ANGULO DENTRO DE LA VENTANA DE SEGURIDAD (%.2f):\n"
            "  - Trayectoria OPTIMA de reingreso.\n"
            "  - La desaceleracion se mantiene en niveles seguros.\n"
            "  - El escudo termico funciona correctamente.\n"
            "  - Se garantiza un amerizaje controlado en el oceano.",
            angulo);
    }
}

/* Funcion principal de seguridad: pide el angulo, valida y muestra el analisis.
   Es la que llama main.c cuando el usuario elige OPCION_ANGULO. */
void mostrarAnalisisSeguridad(EstadoSimulador *estado)
{
    double angulo;
    char tipoTrayectoria[40];
    char consecuencias[500];
    int anguloValido = 0;

    printf("\n--- ANALISIS DE SEGURIDAD DEL ANGULO DE REINGRESO ---\n");
    printf("Ventana de seguridad: %.1f grados a %.1f grados\n\n",
           ANGULO_MIN_SEGURO, ANGULO_MAX_SEGURO);

    /* Pedimos el angulo hasta que sea valido (0 a 90) */
    do {
        printf("Ingrese el angulo de reingreso (grados): ");
        if (scanf("%lf", &angulo) != 1) {
            printf("Error: debe ingresar un numero.\n");
            while (getchar() != '\n');  /* limpiar buffer */
            continue;
        }
        while (getchar() != '\n');  /* limpiar buffer */

        if (angulo < 0.0 || angulo > 90.0) {
            printf("Error: el angulo debe estar entre 0 y 90 grados.\n");
        } else {
            anguloValido = 1;
        }
    } while (!anguloValido);

    /* Guardamos en la estructura del estado */
    estado->seguridad.angulo = angulo;
    estado->seguridad.esSeguro = validarAnguloReingreso(angulo);

    if (estado->seguridad.esSeguro) {
        strcpy(estado->seguridad.diagnostico, "SEGURO - Dentro de la ventana de reingreso");
    } else if (angulo < ANGULO_MIN_SEGURO) {
        strcpy(estado->seguridad.diagnostico, "PELIGRO - Riesgo de rebote atmosferico");
    } else {
        strcpy(estado->seguridad.diagnostico, "PELIGRO - Riesgo de desaceleracion excesiva");
    }

    evaluarTrayectoriaReingreso(angulo, tipoTrayectoria);
    analizarConsecuenciasAngulo(angulo, consecuencias);

    printf("\n============================================================\n");
    printf("       ANALISIS DE SEGURIDAD - ANGULO DE REINGRESO\n");
    printf("============================================================\n");
    printf("  Angulo ingresado     : %.2f grados\n", angulo);
    printf("  Ventana de seguridad : %.1f a %.1f grados\n",
           ANGULO_MIN_SEGURO, ANGULO_MAX_SEGURO);
    printf("------------------------------------------------------------\n");
    printf("  Diagnostico          : %s\n", estado->seguridad.diagnostico);
    printf("  Tipo de trayectoria  : %s\n", tipoTrayectoria);
    printf("------------------------------------------------------------\n");
    printf("%s\n", consecuencias);
    printf("============================================================\n");
}
