/*
 * artemis.h
 * Simulador de Misiones Artemis - Escenario 4 (Reingreso, frenado y amerizaje)
 *
 * Declara las constantes, las estructuras de datos y los prototipos de las
 * funciones que implementa artemis.c.
 */

#ifndef ARTEMIS_H
#define ARTEMIS_H

#include <stddef.h>   /* size_t */

/* ---------------------------- CONSTANTES ---------------------------- */

/* Gravedad terrestre en m/s^2 (valor indicado en los datos del escenario) */
#define GRAVEDAD_TERRESTRE 9.80

/* Modelo fisico del amerizaje:
 *   1 = la gravedad tambien realiza trabajo durante el hundimiento (W_g = m*g*d)
 *       y el agua debe frenar la capsula Y compensar su peso:
 *       W_agua = DeltaK - W_g
 *   0 = se desprecia el peso durante el hundimiento: W_agua = DeltaK
 * Es un supuesto del grupo: debe documentarse en el README y en el reporte. */
#define AMERIZAJE_INCLUYE_GRAVEDAD 1

/* Limites de desaceleracion expresados en multiplos de g.
   La consigna no los define: son criterios elegidos por el grupo
   para clasificar el resultado. */
#define LIMITE_TOLERABLE   3.0
#define LIMITE_ELEVADA     5.0
#define LIMITE_PELIGROSA   9.0

/* Ventana de seguridad del angulo de reingreso */
#define ANGULO_MIN_SEGURO   5.5
#define ANGULO_MAX_SEGURO   7.0

/* Rango de angulos aceptados como entrada (grados) */
#define ANGULO_MIN_ENTRADA  0.0
#define ANGULO_MAX_ENTRADA 90.0

/* Maximos aceptados al modificar parametros (evitan desbordes en los calculos) */
#define MAX_MASA            1.0e7   /* kg  */
#define MAX_GRAVEDAD       30.0     /* m/s^2 */
#define MAX_VELOCIDAD       3.0e4   /* m/s */
#define MAX_TIEMPO          1.0e5   /* s   */
#define MAX_ALTURA          1.0e6   /* m   */
#define MAX_VEL_IMPACTO     1.0e3   /* m/s */
#define MAX_HUNDIMIENTO     1.0e2   /* m   */

/* Cantidad maxima de simulaciones guardadas en el historial */
#define MAX_HISTORIAL 20

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

/* Datos de la fase de reingreso */
typedef struct {
    double velocidadInicial;     /* [m/s] */
    double velocidadFinal;       /* [m/s] */
    double tiempoDesaceleracion; /* [s]   */
    double alturaInicial;        /* [m]   */
    double alturaFinal;          /* [m]   */
} DatosReingreso;

/* Datos del amerizaje */
typedef struct {
    double velocidadImpacto;     /* [m/s] */
    double distanciaHundimiento; /* [m]   */
} DatosAmerizaje;

/* Resultados fisicos calculados */
typedef struct {
    double desaceleracionMedia;      /* [m/s^2] (modulo)         */
    double desaceleracionG;          /* [g]                      */
    double energiaImpacto;           /* [J]                      */
    double trabajoGravedadAmerizaje; /* [J] trabajo del peso durante el hundimiento */
    double trabajoAgua;              /* [J]                      */
    double fuerzaPromedioAgua;       /* [N]                      */
    double pesoNave;                 /* [N]                      */
    double fuerzaEnPesos;            /* fuerza / peso (adimens.) */
    double energiaCineticaInicial;   /* [J] */
    double energiaCineticaFinal;     /* [J] */
    double energiaPotencialInicial;  /* [J] */
    double energiaPotencialFinal;    /* [J] */
    double trabajoGravedad;          /* [J] */
    double trabajoRozamiento;        /* [J] */
    double energiaDisipada;          /* [J] */
} ResultadoFisico;

/* Resultado del analisis de seguridad del angulo */
typedef struct {
    double angulo;              /* Angulo ingresado (grados) */
    int    esSeguro;            /* 1 = dentro de ventana, 0 = fuera */
    char   diagnostico[120];    /* Texto del diagnostico */
} ResultadoSeguridad;

/* Una simulacion guardada en el historial: tipo de analisis (valor de
 * OpcionPrincipal) y una copia de los parametros y resultados de ese momento. */
typedef struct {
    int numero;                 /* numero correlativo de la simulacion */
    int tipo;                   /* OPCION_REINGRESO, _DESACELERACION, ... */
    DatosMision mision;
    DatosReingreso reingreso;
    DatosAmerizaje amerizaje;
    ResultadoFisico fisico;
    ResultadoSeguridad seguridad;
} RegistroSimulacion;

/* ------------------------- MENUS Y ESTADO GLOBAL ------------------------- */

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
    ResultadoSeguridad seguridad;
    /* 1 si el analisis se ejecuto con los parametros actuales */
    int reingresoCalculado;
    int desaceleracionCalculada;
    int amerizajeCalculado;
    int anguloCalculado;
    /* historial de simulaciones */
    RegistroSimulacion historial[MAX_HISTORIAL];
    int cantidadHistorial;      /* registros guardados (0 a MAX_HISTORIAL) */
    int totalSimulaciones;      /* simulaciones ejecutadas desde el inicio */
} EstadoSimulador;

/* ------------------------- DATOS Y PARAMETROS ------------------------- */

void cargarDatosPredeterminados(EstadoSimulador *estado);
void invalidarResultados(EstadoSimulador *estado);
void modificarDatosMision(EstadoSimulador *estado);
void restablecerDatosMision(EstadoSimulador *estado);

/* ------------- PROTOTIPOS: ENERGIA Y TRABAJO DEL REINGRESO ------------- */

void calcularResultadosReingreso(const DatosMision *mision,
                                 const DatosReingreso *reingreso,
                                 ResultadoFisico *resultado);
void mostrarAnalisisRozamiento(const DatosReingreso *reingreso,
                               const DatosMision *mision,
                               ResultadoFisico *resultado);

/* ------------- PROTOTIPOS: DESACELERACION DE LA TRIPULACION ------------- */

double calcularDesaceleracionMedia(double velInicial, double velFinal, double tiempo);
double convertirDesaceleracionAG(double desaceleracion, double gravedad);
NivelDesaceleracion evaluarDesaceleracion(double desaceleracionG);
const char *descripcionNivelDesaceleracion(NivelDesaceleracion nivel);
void mostrarAnalisisDesaceleracion(const DatosReingreso *reingreso,
                                   const DatosMision *mision,
                                   ResultadoFisico *resultado);

/* ------------------- PROTOTIPOS: AMERIZAJE E IMPACTO ------------------- */

double calcularEnergiaImpacto(double masa, double velocidad);
double calcularTrabajoGravedadHundimiento(double masa, double gravedad, double distancia);
double calcularTrabajoAgua(double energiaImpacto, double trabajoGravedad);
double calcularFuerzaPromedioAgua(double trabajoAgua, double distancia);
double calcularPesoNave(double masa, double gravedad);
double calcularFuerzaEnMultiploDePeso(double fuerza, double peso);
const char *evaluarImpacto(double fuerzaEnPesos);
void mostrarAnalisisAmerizaje(const DatosAmerizaje *amerizaje,
                              const DatosMision *mision,
                              ResultadoFisico *resultado);

/* ------------- PROTOTIPOS: ANGULO DE REINGRESO Y SEGURIDAD ------------- */

int  validarAnguloReingreso(double angulo);
void evaluarTrayectoriaReingreso(double angulo, char *resultado, size_t tam);
void analizarConsecuenciasAngulo(double angulo, char *explicacion, size_t tam);
/* Devuelve 1 si se realizo el analisis, 0 si la entrada se cerro (EOF). */
int  mostrarAnalisisSeguridad(EstadoSimulador *estado);

#endif /* ARTEMIS_H */
