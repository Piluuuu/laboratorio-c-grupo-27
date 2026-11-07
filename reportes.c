/*
 * reportes.c
 * Historial de simulaciones y reporte final.
 */
#include <stdio.h>
#include <string.h>
#include "reportes.h"
#include "interfaz.h"

/* Deja el historial vacio. */
void inicializarHistorial(EstadoSimulador *estado)
{
    memset(estado->historial, 0, sizeof estado->historial);
    estado->cantidadHistorial = 0;
    estado->totalSimulaciones = 0;
}

/* Nombre legible de cada tipo de simulacion. */
static const char *nombreSimulacion(int tipo)
{
    switch (tipo) {
        case OPCION_REINGRESO:     return "Reingreso (energia y trabajo)";
        case OPCION_DESACELERACION: return "Desaceleracion";
        case OPCION_AMERIZAJE:     return "Amerizaje e impacto";
        case OPCION_ANGULO:        return "Angulo de reingreso";
        default:                   return "Desconocida";
    }
}

/* Guarda una copia de los datos y resultados de la simulacion recien hecha. */
void registrarSimulacion(EstadoSimulador *estado, int tipo)
{
    RegistroSimulacion *reg;

    switch (tipo) {
        case OPCION_REINGRESO:      estado->reingresoCalculado = 1;      break;
        case OPCION_DESACELERACION: estado->desaceleracionCalculada = 1; break;
        case OPCION_AMERIZAJE:      estado->amerizajeCalculado = 1;      break;
        case OPCION_ANGULO:         estado->anguloCalculado = 1;         break;
        default: return;            /* solo se registran los analisis */
    }

    if (estado->cantidadHistorial == MAX_HISTORIAL) {
        /* Historial lleno: se descarta el registro mas antiguo */
        memmove(&estado->historial[0], &estado->historial[1],
                (MAX_HISTORIAL - 1) * sizeof estado->historial[0]);
        estado->cantidadHistorial--;
    }

    estado->totalSimulaciones++;
    reg = &estado->historial[estado->cantidadHistorial];
    reg->numero = estado->totalSimulaciones;
    reg->tipo = tipo;
    reg->mision = estado->mision;
    reg->reingreso = estado->reingreso;
    reg->amerizaje = estado->amerizaje;
    reg->fisico = estado->fisico;
    reg->seguridad = estado->seguridad;
    estado->cantidadHistorial++;
}

/* Opcion 6: lista las simulaciones guardadas con sus datos y resultados. */
void mostrarHistorialSimulaciones(const EstadoSimulador *estado)
{
    int i;
    const RegistroSimulacion *reg;

    printf("\n=== HISTORIAL DE SIMULACIONES ===\n");
    if (estado->cantidadHistorial == 0) {
        printf("Todavia no se ejecuto ninguna simulacion.\n");
        printf("Ejecute alguna de las opciones 1 a 4 para registrarla aqui.\n");
        return;
    }

    printf("Se muestran las ultimas %d de %d simulaciones (maximo %d).\n",
           estado->cantidadHistorial, estado->totalSimulaciones, MAX_HISTORIAL);

    for (i = 0; i < estado->cantidadHistorial; i++) {
        reg = &estado->historial[i];
        printf("\n#%d - %s\n", reg->numero, nombreSimulacion(reg->tipo));
        switch (reg->tipo) {
            case OPCION_REINGRESO:
                printf("   Datos     : m=%.2f kg, g=%.2f m/s^2, vi=%.2f m/s, vf=%.2f m/s, hi=%.2f m, hf=%.2f m\n",
                       reg->mision.masaOrion, reg->mision.gravedad,
                       reg->reingreso.velocidadInicial, reg->reingreso.velocidadFinal,
                       reg->reingreso.alturaInicial, reg->reingreso.alturaFinal);
                printf("   Resultado : W gravedad=%+.2f J, W rozamiento=%+.2f J, energia disipada=%.2f J\n",
                       reg->fisico.trabajoGravedad, reg->fisico.trabajoRozamiento,
                       reg->fisico.energiaDisipada);
                break;
            case OPCION_DESACELERACION:
                printf("   Datos     : g=%.2f m/s^2, vi=%.2f m/s, vf=%.2f m/s, t=%.2f s\n",
                       reg->mision.gravedad, reg->reingreso.velocidadInicial,
                       reg->reingreso.velocidadFinal, reg->reingreso.tiempoDesaceleracion);
                printf("   Resultado : %.4f m/s^2 = %.4f g (%s)\n",
                       reg->fisico.desaceleracionMedia, reg->fisico.desaceleracionG,
                       descripcionNivelDesaceleracion(evaluarDesaceleracion(reg->fisico.desaceleracionG)));
                break;
            case OPCION_AMERIZAJE:
                printf("   Datos     : m=%.2f kg, g=%.2f m/s^2, v=%.2f m/s, d=%.2f m\n",
                       reg->mision.masaOrion, reg->mision.gravedad,
                       reg->amerizaje.velocidadImpacto, reg->amerizaje.distanciaHundimiento);
                printf("   Resultado : F agua=%.2f N, F/peso=%.4f. %s\n",
                       reg->fisico.fuerzaPromedioAgua, reg->fisico.fuerzaEnPesos,
                       evaluarImpacto(reg->fisico.fuerzaEnPesos));
                break;
            case OPCION_ANGULO:
                printf("   Datos     : angulo=%.2f grados\n", reg->seguridad.angulo);
                printf("   Resultado : %s\n", reg->seguridad.diagnostico);
                break;
            default:
                break;
        }
    }
}

/* Escribe el reporte en 'f' (pantalla o archivo). */
static void escribirReporte(FILE *f, const EstadoSimulador *estado)
{
    const ResultadoFisico *fis = &estado->fisico;

    fprintf(f, "\n==============================================================\n");
    fprintf(f, "   REPORTE FINAL - SIMULADOR ARTEMIS (Escenario 4)\n");
    fprintf(f, "   Reingreso atmosferico, frenado aerodinamico y amerizaje\n");
    fprintf(f, "==============================================================\n");

    fprintf(f, "\n--- PARAMETROS UTILIZADOS ---\n");
    fprintf(f, "Masa de Orion          : %.2f kg\n", estado->mision.masaOrion);
    fprintf(f, "Gravedad               : %.2f m/s^2\n", estado->mision.gravedad);
    fprintf(f, "Velocidad inicial      : %.2f m/s\n", estado->reingreso.velocidadInicial);
    fprintf(f, "Velocidad final        : %.2f m/s\n", estado->reingreso.velocidadFinal);
    fprintf(f, "Tiempo desaceleracion  : %.2f s\n", estado->reingreso.tiempoDesaceleracion);
    fprintf(f, "Altura inicial         : %.2f m\n", estado->reingreso.alturaInicial);
    fprintf(f, "Altura final           : %.2f m\n", estado->reingreso.alturaFinal);
    fprintf(f, "Velocidad de impacto   : %.2f m/s\n", estado->amerizaje.velocidadImpacto);
    fprintf(f, "Hundimiento en el agua : %.2f m\n", estado->amerizaje.distanciaHundimiento);

    fprintf(f, "\n--- REINGRESO ATMOSFERICO (trabajo y energia) ---\n");
    if (estado->reingresoCalculado) {
        fprintf(f, "Energia cinetica inicial  : %.2f J\n", fis->energiaCineticaInicial);
        fprintf(f, "Energia cinetica final    : %.2f J\n", fis->energiaCineticaFinal);
        fprintf(f, "Energia potencial inicial : %.2f J\n", fis->energiaPotencialInicial);
        fprintf(f, "Energia potencial final   : %.2f J\n", fis->energiaPotencialFinal);
        fprintf(f, "Trabajo de la gravedad    : %+.2f J\n", fis->trabajoGravedad);
        fprintf(f, "Trabajo del rozamiento    : %+.2f J\n", fis->trabajoRozamiento);
        fprintf(f, "Energia disipada          : %.2f J\n", fis->energiaDisipada);
    } else {
        fprintf(f, "No ejecutado (opcion %d del menu).\n", OPCION_REINGRESO);
    }

    fprintf(f, "\n--- DESACELERACION DE LA TRIPULACION ---\n");
    if (estado->desaceleracionCalculada) {
        fprintf(f, "Desaceleracion media : %.4f m/s^2\n", fis->desaceleracionMedia);
        fprintf(f, "En multiplos de g    : %.4f g\n", fis->desaceleracionG);
        fprintf(f, "Clasificacion        : %s\n",
                descripcionNivelDesaceleracion(evaluarDesaceleracion(fis->desaceleracionG)));
    } else {
        fprintf(f, "No ejecutado (opcion %d del menu).\n", OPCION_DESACELERACION);
    }

    fprintf(f, "\n--- AMERIZAJE E IMPACTO ---\n");
    if (estado->amerizajeCalculado) {
        fprintf(f, "Energia cinetica de impacto : %.2f J\n", fis->energiaImpacto);
        fprintf(f, "Trabajo de la gravedad      : %+.2f J\n", fis->trabajoGravedadAmerizaje);
        fprintf(f, "Trabajo del agua            : %+.2f J\n", fis->trabajoAgua);
        fprintf(f, "Fuerza promedio del agua    : %.2f N\n", fis->fuerzaPromedioAgua);
        fprintf(f, "Peso de Orion               : %.2f N\n", fis->pesoNave);
        fprintf(f, "Fuerza / peso               : %.4f\n", fis->fuerzaEnPesos);
        fprintf(f, "Evaluacion                  : %s\n", evaluarImpacto(fis->fuerzaEnPesos));
#if AMERIZAJE_INCLUYE_GRAVEDAD
        fprintf(f, "Supuesto: la gravedad tambien trabaja durante el hundimiento\n"
                   "          (W_agua = DeltaK - m*g*d).\n");
#else
        fprintf(f, "Supuesto: se desprecia el peso durante el hundimiento (W_agua = DeltaK).\n");
#endif
    } else {
        fprintf(f, "No ejecutado (opcion %d del menu).\n", OPCION_AMERIZAJE);
    }

    fprintf(f, "\n--- ANGULO DE REINGRESO ---\n");
    if (estado->anguloCalculado) {
        fprintf(f, "Angulo ingresado    : %.2f grados (ventana segura: %.1f a %.1f)\n",
                estado->seguridad.angulo, ANGULO_MIN_SEGURO, ANGULO_MAX_SEGURO);
        fprintf(f, "Diagnostico         : %s\n", estado->seguridad.diagnostico);
    } else {
        fprintf(f, "No ejecutado (opcion %d del menu).\n", OPCION_ANGULO);
    }

    fprintf(f, "\nSimulaciones ejecutadas en esta sesion: %d\n", estado->totalSimulaciones);
    fprintf(f, "==============================================================\n");
}

/* Opcion 7: muestra el reporte por pantalla y lo guarda en un archivo de texto. */
void generarReporteFinal(const EstadoSimulador *estado)
{
    FILE *archivo;

    escribirReporte(stdout, estado);

    archivo = fopen(ARCHIVO_REPORTE, "w");
    if (archivo == NULL) {
        mostrarMensajeAdvertencia("No se pudo crear el archivo del reporte.");
        return;
    }
    escribirReporte(archivo, estado);
    fclose(archivo);
    printf("Reporte guardado en el archivo %s\n", ARCHIVO_REPORTE);
}
