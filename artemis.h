/*
 * artemis.h
 * Simulador de Misiones Artemis - Escenario 4 (Reingreso, frenado y amerizaje)
 *
 * Parte del Integrante 3: Desaceleracion de la tripulacion y amerizaje.
 * Este archivo declara las constantes, las estructuras de datos y los
 * prototipos de las funciones que implementa artemis.c.
 */

#ifndef ARTEMIS_H
#define ARTEMIS_H

/* ---------------------------- CONSTANTES ---------------------------- */

/* Gravedad terrestre en m/s^2 */
#define GRAVEDAD_TERRESTRE 9.81

/* Limites de desaceleracion expresados en multiplos de g.
   La consigna no los define: son criterios elegidos por el grupo
   para clasificar el resultado. */
#define LIMITE_TOLERABLE   3.0
#define LIMITE_ELEVADA     5.0
#define LIMITE_PELIGROSA   9.0

/* Niveles posibles de desaceleracion (enum: nombres para los valores 0,1,2,3) */
typedef enum {
    DESACEL_TOLERABLE = 0,
    DESACEL_ELEVADA,
    DESACEL_PELIGROSA,
    DESACEL_CRITICA
} NivelDesaceleracion;

/* ---------------------------- ESTRUCTURAS ---------------------------- */

/* Datos generales de la mision */
typedef struct {
    double masaOrion;        /* masa de la capsula Orion [kg]  */
    double gravedad;         /* gravedad terrestre [m/s^2]     */
} DatosMision;

/* Datos de la fase de reingreso (los que usa la desaceleracion) */
typedef struct {
    double velocidadInicial;     /* [m/s] */
    double velocidadFinal;       /* [m/s] */
    double tiempoDesaceleracion; /* [s]   */
    double alturaInicial;   /* [m] */
    double alturaFinal;   /* [m] */
} DatosReingreso;

/* Datos del amerizaje */
typedef struct {
    double velocidadImpacto;     /* [m/s] */
    double distanciaHundimiento; /* [m]   */
} DatosAmerizaje;

/* Resultados fisicos calculados */
typedef struct {
    double desaceleracionMedia;  /* [m/s^2]                  */
    double desaceleracionG;      /* [g]                      */
    double energiaImpacto;       /* [J]                      */
    double trabajoAgua;          /* [J]                      */
    double fuerzaPromedioAgua;   /* [N]                      */
    double pesoNave;             /* [N]                      */
    double fuerzaEnPesos;        /* fuerza / peso (adimens.) */
    double energiaCineticaInicial;  /* [J] */
    double energiaCineticaFinal;  /* [J] */
    double energiaPotencialInicial;  /* [J] */
    double energiaPotencialFinal;  /* [J] */
    double trabajoGravedad;  /* [J] */
    double trabajoRozamiento;  /* [J] */
    double energiaDisipada;  /* [J] */
} ResultadoFisico;

/* ------------- PROTOTIPOS: DESACELERACION DE LA TRIPULACION ------------- */

double calcularDesaceleracionMedia(double velInicial, double velFinal, double tiempo);
double convertirDesaceleracionAG(double desaceleracion, double gravedad);
NivelDesaceleracion evaluarDesaceleracion(double desaceleracionG);
void mostrarAnalisisDesaceleracion(const DatosReingreso *reingreso,
                                   const DatosMision *mision,
                                   ResultadoFisico *resultado);

/* ------------------- PROTOTIPOS: AMERIZAJE E IMPACTO ------------------- */

double calcularEnergiaImpacto(double masa, double velocidad);
double calcularTrabajoAgua(double energiaImpacto);
double calcularFuerzaPromedioAgua(double trabajoAgua, double distancia);
double calcularPesoNave(double masa, double gravedad);
double calcularFuerzaEnMultiploDePeso(double fuerza, double peso);
const char *evaluarImpacto(double fuerzaEnPesos);
void mostrarAnalisisAmerizaje(const DatosAmerizaje *amerizaje,
                              const DatosMision *mision,
                              ResultadoFisico *resultado);
void calcularResultadosReingreso(const DatosMision *mision, const DatosReingreso *reingreso, ResultadoFisico *resultado);
void mostrarAnalisisRozamiento(const DatosReingreso *reingreso, const DatosMision *mision, ResultadoFisico *resultado);
typedef enum {
    OPCION_REINGRESO = 1,
    OPCION_DESACELERACION,
    OPCION_AMERIZAJE,
    OPCION_ANGULO,
    OPCION_PARAMETROS,
    OPCION_HISTORIAL,
    OPCION_REPORTE,
    OPCION_SALIR
} OpcionPrincipal;

typedef enum {
    PARAM_MODIFICAR = 1,
    PARAM_RESTABLECER,
    PARAM_VOLVER
} OpcionParametros;

typedef struct {
    DatosMision mision;
    DatosReingreso reingreso;
    DatosAmerizaje amerizaje;
    ResultadoFisico fisico;
    int cantidadHistorial;
} EstadoSimulador;

void cargarDatosPredeterminados(EstadoSimulador *estado);

#endif
