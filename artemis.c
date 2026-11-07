/*
 * artemis.c
 * Simulador de Misiones Artemis - Escenario 4 (Reingreso, frenado y amerizaje)
 *
 * Calculos fisicos (energia, trabajo, desaceleracion, amerizaje), analisis del
 * angulo de reingreso y gestion de los parametros de la mision.
 *
 * Las estructuras se reciben por PUNTERO (const cuando solo se leen) para no
 * copiarlas completas y para poder guardar los resultados en ResultadoFisico.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "artemis.h"
#include "validaciones.h"
#include "interfaz.h"

/* ============================================================
 *          ENERGIAS Y TRABAJO DURANTE EL REINGRESO
 * ============================================================ */

/* Teorema trabajo-energia: W_gravedad + W_rozamiento = K_final - K_inicial.
 * El trabajo del peso depende solo del desnivel: W_g = m*g*(h_i - h_f). */
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

    printf("\n=== REINGRESO ATMOSFERICO: TRABAJO Y ENERGIA ===\n");
    printf("Energia cinetica inicial  : %.2f J\n", resultado->energiaCineticaInicial);
    printf("Energia cinetica final    : %.2f J\n", resultado->energiaCineticaFinal);
    printf("Energia potencial inicial : %.2f J\n", resultado->energiaPotencialInicial);
    printf("Energia potencial final   : %.2f J\n", resultado->energiaPotencialFinal);
    printf("Trabajo de la gravedad    : %+.2f J\n", resultado->trabajoGravedad);
    printf("Trabajo del rozamiento    : %+.2f J\n", resultado->trabajoRozamiento);
    printf("Energia disipada          : %.2f J\n", resultado->energiaDisipada);
    printf("La resistencia del aire reduce la rapidez de la capsula.\n");
    printf("El escudo termico protege la capsula del calentamiento durante el reingreso.\n");
}

/* =====================================================================
 *                 DESACELERACION DE LA TRIPULACION
 * ===================================================================== */

/* Aceleracion media: a = (vf - vi) / t. Aqui se devuelve su MODULO
 * (desaceleracion, positiva). Si el tiempo no es positivo se devuelve 0 para
 * evitar dividir por cero (los datos tambien se validan al ingresarlos). */
double calcularDesaceleracionMedia(double velInicial, double velFinal, double tiempo)
{
    if (tiempo <= 0.0) return 0.0;
    return fabs(velFinal - velInicial) / tiempo;
}

/* Convierte la desaceleracion a multiplos de g: a_g = |a| / g */
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

/* Texto asociado a cada nivel (lo usan la pantalla y el reporte). */
const char *descripcionNivelDesaceleracion(NivelDesaceleracion nivel)
{
    switch (nivel) {
        case DESACEL_TOLERABLE: return "TOLERABLE - segura para la tripulacion.";
        case DESACEL_ELEVADA:   return "ELEVADA - exigente pero soportable.";
        case DESACEL_PELIGROSA: return "PELIGROSA - riesgo de lesiones.";
        default:                return "CRITICA - puede ser letal.";
    }
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
    printf("Velocidad inicial    : %.2f m/s\n", reingreso->velocidadInicial);
    printf("Velocidad final      : %.2f m/s\n", reingreso->velocidadFinal);
    printf("Tiempo               : %.2f s\n", reingreso->tiempoDesaceleracion);
    printf("Aceleracion media    : %.4f m/s^2 (a = (vf - vi) / t, negativa: frena)\n",
           -resultado->desaceleracionMedia);
    printf("Desaceleracion media : %.4f m/s^2\n", resultado->desaceleracionMedia);
    printf("En multiplos de g    : %.4f g\n", resultado->desaceleracionG);
    printf("Clasificacion        : %s\n", descripcionNivelDesaceleracion(nivel));
}

/* =====================================================================
 *                      AMERIZAJE E IMPACTO
 * ===================================================================== */

/* Energia cinetica justo antes del impacto: K = 1/2 * m * v^2 */
double calcularEnergiaImpacto(double masa, double velocidad)
{
    return 0.5 * masa * velocidad * velocidad;
}

/* Trabajo del peso durante el hundimiento: W_g = m * g * d (positivo, porque
 * la capsula desciende). Con AMERIZAJE_INCLUYE_GRAVEDAD = 0 se desprecia. */
double calcularTrabajoGravedadHundimiento(double masa, double gravedad, double distancia)
{
#if AMERIZAJE_INCLUYE_GRAVEDAD
    return masa * gravedad * distancia;
#else
    (void)masa; (void)gravedad; (void)distancia;
    return 0.0;
#endif
}

/* Teorema trabajo-energia: W_gravedad + W_agua = K_final - K_inicial.
 * La capsula queda detenida (K_final = 0), entonces
 *     W_agua = -K_impacto - W_gravedad.
 * Es negativo porque el agua se opone al movimiento. */
double calcularTrabajoAgua(double energiaImpacto, double trabajoGravedad)
{
    return -energiaImpacto - trabajoGravedad;
}

/* Fuerza media constante del agua: W = -F * d, entonces F = |W| / d. */
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

/* Interpreta el impacto segun cuantas veces el peso es la fuerza del agua.
 * Los umbrales son un criterio del grupo (la consigna no los define). */
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
    resultado->trabajoGravedadAmerizaje = calcularTrabajoGravedadHundimiento(
        mision->masaOrion, mision->gravedad, amerizaje->distanciaHundimiento);
    resultado->trabajoAgua = calcularTrabajoAgua(
        resultado->energiaImpacto, resultado->trabajoGravedadAmerizaje);
    resultado->fuerzaPromedioAgua = calcularFuerzaPromedioAgua(
        resultado->trabajoAgua, amerizaje->distanciaHundimiento);
    resultado->pesoNave = calcularPesoNave(mision->masaOrion, mision->gravedad);
    resultado->fuerzaEnPesos = calcularFuerzaEnMultiploDePeso(
        resultado->fuerzaPromedioAgua, resultado->pesoNave);

    printf("\n=== AMERIZAJE E IMPACTO ===\n");
    printf("Velocidad de impacto  : %.2f m/s\n", amerizaje->velocidadImpacto);
    printf("Distancia hundimiento : %.2f m\n", amerizaje->distanciaHundimiento);
    printf("Energia cinetica      : %.2f J\n", resultado->energiaImpacto);
    printf("Trabajo de la gravedad: %+.2f J\n", resultado->trabajoGravedadAmerizaje);
    printf("Trabajo del agua      : %+.2f J\n", resultado->trabajoAgua);
    printf("Fuerza promedio (agua): %.2f N\n", resultado->fuerzaPromedioAgua);
    printf("Peso de Orion         : %.2f N\n", resultado->pesoNave);
    printf("Fuerza / peso         : %.4f veces el peso\n", resultado->fuerzaEnPesos);
    printf("Evaluacion: %s\n", evaluarImpacto(resultado->fuerzaEnPesos));
#if AMERIZAJE_INCLUYE_GRAVEDAD
    printf("Modelo: la gravedad tambien trabaja durante el hundimiento.\n");
#else
    printf("Modelo: se desprecia el peso durante el hundimiento.\n");
#endif
}

/* ============================================================
 *     ANGULO DE REINGRESO Y SEGURIDAD
 * ============================================================ */

/* Verifica si el angulo esta dentro de la ventana de seguridad (5.5 a 7.0 grados).
 * Un valor no finito (nan, inf) nunca es seguro. */
int validarAnguloReingreso(double angulo)
{
    if (isfinite(angulo) && angulo >= ANGULO_MIN_SEGURO && angulo <= ANGULO_MAX_SEGURO) {
        return 1;
    }
    return 0;
}

/* Clasifica el tipo de trayectoria segun el angulo.
 * 'resultado' es un buffer de 'tam' bytes. */
void evaluarTrayectoriaReingreso(double angulo, char *resultado, size_t tam)
{
    const char *texto;

    if (!isfinite(angulo)) {
        texto = "INDETERMINADA";
    } else if (angulo < ANGULO_MIN_SEGURO) {
        texto = "REBOTE_ATMOSFERICO";
    } else if (angulo > ANGULO_MAX_SEGURO) {
        texto = "DESACELERACION_EXCESIVA";
    } else {
        texto = "SEGURA";
    }
    snprintf(resultado, tam, "%s", texto);
}

/* Explica las consecuencias fisicas segun el angulo.
 * 'explicacion' es un buffer de 'tam' bytes. */
void analizarConsecuenciasAngulo(double angulo, char *explicacion, size_t tam)
{
    if (!isfinite(angulo)) {
        snprintf(explicacion, tam, "ANGULO NO VALIDO: no se puede analizar la trayectoria.");
    } else if (angulo < ANGULO_MIN_SEGURO) {
        snprintf(explicacion, tam,
            "ANGULO BAJO (%.2f < %.1f):\n"
            "  - La capsula no penetra lo suficiente en la atmosfera.\n"
            "  - Se produce un REBOTE ATMOSFERICO.\n"
            "  - La nave sale de nuevo al espacio y se pierde la mision.\n"
            "  - No se logra el amerizaje previsto.",
            angulo, ANGULO_MIN_SEGURO);
    } else if (angulo > ANGULO_MAX_SEGURO) {
        snprintf(explicacion, tam,
            "ANGULO ALTO (%.2f > %.1f):\n"
            "  - La capsula entra demasiado pronunciada en la atmosfera.\n"
            "  - Se genera DESACELERACION EXCESIVA.\n"
            "  - La tripulacion experimenta fuerzas g peligrosas.\n"
            "  - Riesgo de dano estructural o lesiones graves.",
            angulo, ANGULO_MAX_SEGURO);
    } else {
        snprintf(explicacion, tam,
            "ANGULO DENTRO DE LA VENTANA DE SEGURIDAD (%.2f):\n"
            "  - Trayectoria OPTIMA de reingreso.\n"
            "  - La desaceleracion se mantiene en niveles seguros.\n"
            "  - El escudo termico funciona correctamente.\n"
            "  - Se garantiza un amerizaje controlado en el oceano.",
            angulo);
    }
}

/* Funcion principal de seguridad: pide el angulo, valida y muestra el analisis.
 * Es la que llama main.c cuando el usuario elige OPCION_ANGULO.
 * Devuelve 1 si se hizo el analisis, 0 si la entrada se cerro (EOF). */
int mostrarAnalisisSeguridad(EstadoSimulador *estado)
{
    double angulo = 0.0;
    char tipoTrayectoria[40];
    char consecuencias[500];
    ResultadoLectura lectura;
    int anguloValido = 0;

    printf("\n--- ANALISIS DE SEGURIDAD DEL ANGULO DE REINGRESO ---\n");
    printf("Ventana de seguridad: %.1f grados a %.1f grados\n\n",
           ANGULO_MIN_SEGURO, ANGULO_MAX_SEGURO);

    /* Se pide el angulo hasta que sea un numero finito entre 0 y 90 */
    do {
        lectura = leerFlotante("Ingrese el angulo de reingreso (grados): ", &angulo);
        if (lectura == LECTURA_EOF) {
            mostrarMensajeAdvertencia("Entrada cerrada. Se cancela el analisis.");
            return 0;
        }
        if (lectura == LECTURA_INVALIDA) {
            mostrarMensajeError("Entrada invalida: ingrese un numero decimal (ejemplo: 6.5).");
        } else if (!validarAngulo(angulo)) {
            mostrarMensajeError("El angulo debe estar entre 0 y 90 grados.");
        } else {
            anguloValido = 1;
        }
    } while (!anguloValido);

    /* Guardamos en la estructura del estado */
    estado->seguridad.angulo = angulo;
    estado->seguridad.esSeguro = validarAnguloReingreso(angulo);

    if (estado->seguridad.esSeguro) {
        snprintf(estado->seguridad.diagnostico, sizeof estado->seguridad.diagnostico,
                 "SEGURO - Dentro de la ventana de reingreso");
    } else if (angulo < ANGULO_MIN_SEGURO) {
        snprintf(estado->seguridad.diagnostico, sizeof estado->seguridad.diagnostico,
                 "PELIGRO - Riesgo de rebote atmosferico");
    } else {
        snprintf(estado->seguridad.diagnostico, sizeof estado->seguridad.diagnostico,
                 "PELIGRO - Riesgo de desaceleracion excesiva");
    }

    evaluarTrayectoriaReingreso(angulo, tipoTrayectoria, sizeof tipoTrayectoria);
    analizarConsecuenciasAngulo(angulo, consecuencias, sizeof consecuencias);

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
    return 1;
}

/* ============================================================
 *              DATOS Y PARAMETROS DE LA MISION
 * ============================================================ */

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

/* Borra los resultados calculados: tras cambiar los parametros dejan de
 * corresponder a los datos actuales. El historial no se toca. */
void invalidarResultados(EstadoSimulador *estado)
{
    memset(&estado->fisico, 0, sizeof estado->fisico);
    memset(&estado->seguridad, 0, sizeof estado->seguridad);
    estado->reingresoCalculado = 0;
    estado->desaceleracionCalculada = 0;
    estado->amerizajeCalculado = 0;
    estado->anguloCalculado = 0;
}

/* Muestra la lista numerada de parametros con su valor actual. */
static void mostrarListaParametros(const EstadoSimulador *estado)
{
    printf("\n--- PARAMETROS ACTUALES ---\n");
    printf(" 1. Masa de Orion              : %.2f kg\n", estado->mision.masaOrion);
    printf(" 2. Gravedad                   : %.2f m/s^2\n", estado->mision.gravedad);
    printf(" 3. Velocidad inicial          : %.2f m/s\n", estado->reingreso.velocidadInicial);
    printf(" 4. Velocidad final (paracaidas): %.2f m/s\n", estado->reingreso.velocidadFinal);
    printf(" 5. Tiempo de desaceleracion   : %.2f s\n", estado->reingreso.tiempoDesaceleracion);
    printf(" 6. Altura inicial             : %.2f m\n", estado->reingreso.alturaInicial);
    printf(" 7. Altura final               : %.2f m\n", estado->reingreso.alturaFinal);
    printf(" 8. Velocidad de impacto       : %.2f m/s\n", estado->amerizaje.velocidadImpacto);
    printf(" 9. Hundimiento en el agua     : %.2f m\n", estado->amerizaje.distanciaHundimiento);
    printf(" 0. Terminar\n");
}

/* Pide el nuevo valor del parametro 'indice' (1 a 9) y lo guarda.
 * Respeta que la velocidad final sea menor que la inicial y la altura final
 * menor que la inicial. Devuelve 1 si se modifico, 0 si la entrada se cerro. */
static int pedirParametro(EstadoSimulador *estado, int indice)
{
    DatosReingreso *r = &estado->reingreso;
    double valor = 0.0;
    int ok = 0;

    switch (indice) {
        case 1:
            if (!pedirFlotantePositivo("Nueva masa de Orion [kg]: ", MAX_MASA, &valor)) return 0;
            estado->mision.masaOrion = valor;
            return 1;
        case 2:
            if (!pedirFlotantePositivo("Nueva gravedad [m/s^2]: ", MAX_GRAVEDAD, &valor)) return 0;
            estado->mision.gravedad = valor;
            return 1;
        case 3:
            do {
                if (!pedirFlotantePositivo("Nueva velocidad inicial [m/s]: ", MAX_VELOCIDAD, &valor)) return 0;
                if (valor <= r->velocidadFinal) {
                    mostrarMensajeError("La velocidad inicial debe ser mayor que la final.");
                } else {
                    ok = 1;
                }
            } while (!ok);
            r->velocidadInicial = valor;
            return 1;
        case 4:
            do {
                if (!pedirFlotantePositivo("Nueva velocidad final [m/s]: ", MAX_VELOCIDAD, &valor)) return 0;
                if (valor >= r->velocidadInicial) {
                    mostrarMensajeError("La velocidad final debe ser menor que la inicial.");
                } else {
                    ok = 1;
                }
            } while (!ok);
            r->velocidadFinal = valor;
            return 1;
        case 5:
            if (!pedirFlotantePositivo("Nuevo tiempo de desaceleracion [s]: ", MAX_TIEMPO, &valor)) return 0;
            r->tiempoDesaceleracion = valor;
            return 1;
        case 6:
            do {
                if (!pedirFlotantePositivo("Nueva altura inicial [m]: ", MAX_ALTURA, &valor)) return 0;
                if (valor <= r->alturaFinal) {
                    mostrarMensajeError("La altura inicial debe ser mayor que la final.");
                } else {
                    ok = 1;
                }
            } while (!ok);
            r->alturaInicial = valor;
            return 1;
        case 7:
            do {
                if (!pedirFlotanteEnRango("Nueva altura final [m]: ", 0.0, MAX_ALTURA, &valor)) return 0;
                if (valor >= r->alturaInicial) {
                    mostrarMensajeError("La altura final debe ser menor que la inicial.");
                } else {
                    ok = 1;
                }
            } while (!ok);
            r->alturaFinal = valor;
            return 1;
        case 8:
            if (!pedirFlotantePositivo("Nueva velocidad de impacto [m/s]: ", MAX_VEL_IMPACTO, &valor)) return 0;
            estado->amerizaje.velocidadImpacto = valor;
            return 1;
        case 9:
            if (!pedirFlotantePositivo("Nuevo hundimiento en el agua [m]: ", MAX_HUNDIMIENTO, &valor)) return 0;
            estado->amerizaje.distanciaHundimiento = valor;
            return 1;
        default:
            return 1;
    }
}

/* Permite modificar los parametros de la mision uno por uno. Al terminar se
 * descartan los resultados anteriores, que ya no corresponden a los datos. */
void modificarDatosMision(EstadoSimulador *estado)
{
    int indice = 0;
    int hubieronCambios = 0;
    int terminar = 0;
    ResultadoLectura lectura;

    do {
        mostrarListaParametros(estado);
        lectura = leerEntero("Numero de parametro a modificar (0 para terminar): ", &indice);
        if (lectura == LECTURA_EOF) {
            mostrarMensajeAdvertencia("Entrada cerrada.");
            terminar = 1;
        } else if (lectura == LECTURA_INVALIDA) {
            mostrarMensajeError("Entrada invalida: ingrese un numero entero.");
        } else if (!validarOpcionMenu(indice, 0, 9)) {
            mostrarMensajeError("Opcion inexistente. Elija un numero entre 0 y 9.");
        } else if (indice == 0) {
            terminar = 1;
        } else if (pedirParametro(estado, indice)) {
            hubieronCambios = 1;
            printf("Parametro actualizado.\n");
        } else {
            mostrarMensajeAdvertencia("Entrada cerrada.");
            terminar = 1;
        }
    } while (!terminar);

    if (hubieronCambios) {
        invalidarResultados(estado);
        printf("Los resultados anteriores se descartaron: vuelva a ejecutar los analisis.\n");
    }
}

/* Vuelve a los valores originales del escenario (con confirmacion). */
void restablecerDatosMision(EstadoSimulador *estado)
{
    char respuesta = leerCaracter("Restablecer los valores predeterminados? (s/n): ");

    if (respuesta == 's' || respuesta == 'S') {
        cargarDatosPredeterminados(estado);
        invalidarResultados(estado);
        printf("Parametros restablecidos a los valores originales del escenario.\n");
    } else {
        printf("Operacion cancelada: los parametros no se modificaron.\n");
    }
}
