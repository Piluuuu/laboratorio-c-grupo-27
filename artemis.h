/*
 * artemis.h
 * Escenario 4: Reingreso Atmosferico, Frenado Aerodinamico y Amerizaje.
 *
 * Contiene SOLO declaraciones: constantes, estructuras (struct/typedef) y
 * prototipos de las funciones publicas de la logica del simulador.
 * Las implementaciones estan en artemis.c.
 */
#ifndef ARTEMIS_H
#define ARTEMIS_H

/* ---------- Valores predeterminados de la mision (consigna) ---------- */
#define MASA_ORION_PREDET             10400.0   /* kg    */
#define GRAVEDAD_TERRESTRE            9.80      /* m/s^2 */
#define VELOCIDAD_INICIAL_PREDET      11000.0   /* m/s   */
#define ALTURA_INICIAL_PREDET         120000.0  /* m     */
#define VELOCIDAD_FINAL_PREDET        25.0      /* m/s   */
#define ALTURA_FINAL_PREDET           2500.0    /* m     */
#define TIEMPO_DESACELERACION_PREDET  400.0     /* s     */
#define VELOCIDAD_IMPACTO_PREDET      8.0       /* m/s   */
#define DISTANCIA_HUNDIMIENTO_PREDET  1.50      /* m     */

/* ---------- Ventana de seguridad del angulo de reingreso ---------- */
#define ANGULO_MINIMO_SEGURO          5.5       /* grados */
#define ANGULO_MAXIMO_SEGURO          7.0       /* grados */

/* ---------- Tamanios de arreglos ---------- */
#define MAX_HISTORIAL                 50
#define TAM_DESCRIPCION               60

/* ---------- Opciones del menu principal ---------- */
#define OPCION_REINGRESO              1
#define OPCION_DESACELERACION         2
#define OPCION_AMERIZAJE              3
#define OPCION_ANGULO                 4
#define OPCION_PARAMETROS             5
#define OPCION_HISTORIAL              6
#define OPCION_REPORTE                7
#define OPCION_SALIR                  8

/* ---------- Opciones del submenu de parametros ---------- */
#define PARAM_MODIFICAR               1
#define PARAM_RESTABLECER             2
#define PARAM_VOLVER                  3

/* ---------- Estructuras de datos ---------- */

/* Masa de Orion, gravedad y constantes generales. */
typedef struct {
    double masaOrion;          /* kg    */
    double gravedad;           /* m/s^2 */
} DatosMision;

/* Datos de la fase de reingreso y frenado aerodinamico. */
typedef struct {
    double velocidadInicial;   /* m/s */
    double alturaInicial;      /* m   */
    double velocidadFinal;     /* m/s (al abrir paracaidas) */
    double alturaFinal;        /* m   */
    double tiempoDesaceleracion; /* s */
} DatosReingreso;

/* Datos del impacto en el oceano. */
typedef struct {
    double velocidadImpacto;     /* m/s */
    double distanciaHundimiento; /* m   */
} DatosAmerizaje;

/* Resultados de los calculos fisicos (los completan los Integrantes 2 y 3). */
typedef struct {
    double energiaCineticaInicial;
    double energiaCineticaFinal;
    double energiaPotencialInicial;
    double energiaPotencialFinal;
    double trabajoGravedad;
    double trabajoRozamiento;
    double energiaDisipada;
    double desaceleracionMedia;      /* m/s^2 */
    double desaceleracionEnG;        /* multiplos de g */
    double energiaImpacto;
    double trabajoAgua;
    double fuerzaPromedioAgua;       /* N */
    double pesoNave;                 /* N */
    double fuerzaEnMultiploDePeso;
} ResultadoFisico;

/* Resultado del analisis del angulo (lo completa el Integrante 4). */
typedef struct {
    double anguloIngresado;          /* grados */
    int    diagnostico;              /* codigo definido por el Integrante 4 */
} ResultadoSeguridad;

/* Un registro del historial (lo completa/amplia el Integrante 5). */
typedef struct {
    int    numeroSimulacion;
    int    tipoAnalisis;             /* usar OPCION_REINGRESO, etc. */
    char   descripcion[TAM_DESCRIPCION];
    double resultadoPrincipal;
} HistorialSimulacion;

/*
 * Contenedor de TODO el estado del simulador. main() crea una unica
 * variable de este tipo y se la pasa por puntero a cada modulo.
 */
typedef struct {
    DatosMision         mision;
    DatosReingreso      reingreso;
    DatosAmerizaje      amerizaje;
    ResultadoFisico     fisico;
    ResultadoSeguridad  seguridad;
    HistorialSimulacion historial[MAX_HISTORIAL];
    int                 cantidadHistorial;
} EstadoSimulador;

/* ---------- Prototipos de funciones publicas ---------- */

/* Gestion de datos */
void cargarDatosPredeterminados(EstadoSimulador *estado);
void modificarDatosMision(EstadoSimulador *estado);
void restablecerDatosMision(EstadoSimulador *estado);

/* Analisis conectados a las opciones 1 a 4 del menu */
void mostrarAnalisisRozamiento(EstadoSimulador *estado);      /* opcion 1 */
void mostrarAnalisisDesaceleracion(EstadoSimulador *estado);  /* opcion 2 */
void mostrarAnalisisAmerizaje(EstadoSimulador *estado);       /* opcion 3 */
void mostrarAnalisisSeguridad(EstadoSimulador *estado);       /* opcion 4 */

#endif /* ARTEMIS_H */
