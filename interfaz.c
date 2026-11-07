/*
 * interfaz.c
 * Presentacion, menus y mensajes. Solo imprime; no calcula nada.
 * (Se escribe sin tildes para que se vea bien en cualquier consola.)
 */
#include <stdio.h>
#include "interfaz.h"
#include "artemis.h"

void mostrarPresentacion(void)
{
    printf("\n==============================================================\n");
    printf("            SIMULADOR DE MISIONES ARTEMIS - NASA\n");
    printf("==============================================================\n");
    printf(" Escenario 4: Reingreso atmosferico, frenado aerodinamico\n");
    printf("              y amerizaje (Trabajo, Energia e Impacto)\n");
    printf("--------------------------------------------------------------\n");
    printf(" El modulo de tripulacion Orion regresa de la Luna, frena en\n");
    printf(" la atmosfera, abre sus paracaidas y ameriza en el Pacifico.\n");
    printf("==============================================================\n");
}

void mostrarMenuPrincipal(void)
{
    printf("\n================= MENU PRINCIPAL =================\n");
    printf(" %d. Simulacion de reingreso atmosferico\n", OPCION_REINGRESO);
    printf(" %d. Analisis de desaceleracion\n", OPCION_DESACELERACION);
    printf(" %d. Simulacion de amerizaje\n", OPCION_AMERIZAJE);
    printf(" %d. Analisis del angulo de reingreso\n", OPCION_ANGULO);
    printf(" %d. Parametros de la mision (modificar / restablecer)\n", OPCION_PARAMETROS);
    printf(" %d. Historial de simulaciones\n", OPCION_HISTORIAL);
    printf(" %d. Reporte final de mision\n", OPCION_REPORTE);
    printf(" %d. Salir\n", OPCION_SALIR);
    printf("==================================================\n");
}

void mostrarMenuParametros(void)
{
    printf("\n------------ PARAMETROS DE LA MISION ------------\n");
    printf(" %d. Modificar parametros\n", PARAM_MODIFICAR);
    printf(" %d. Restablecer valores predeterminados\n", PARAM_RESTABLECER);
    printf(" %d. Volver al menu principal\n", PARAM_VOLVER);
    printf("-------------------------------------------------\n");
}

void mostrarMensajeError(const char *mensaje)
{
    printf("[ERROR] %s\n", mensaje);
}

void mostrarMensajeAdvertencia(const char *mensaje)
{
    printf("[ADVERTENCIA] %s\n", mensaje);
}

/* Espera un ENTER para que el usuario alcance a leer los resultados. */
void pausar(void)
{
    int c;
    printf("\nPresione ENTER para volver al menu...");
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
void mostrarDatosIniciales(const EstadoSimulador *estado)
{
    printf("\n==================================================\n");
    printf("           DATOS INICIALES DE LA MISION\n");
    printf("==================================================\n");

    printf("Masa de Orion           : %.2f kg\n",
           estado->mision.masaOrion);

    printf("Gravedad terrestre      : %.2f m/s^2\n",
           estado->mision.gravedad);

    printf("Velocidad inicial       : %.2f m/s\n",
           estado->reingreso.velocidadInicial);

    printf("Velocidad final         : %.2f m/s\n",
           estado->reingreso.velocidadFinal);

    printf("Tiempo desaceleracion   : %.2f s\n",
           estado->reingreso.tiempoDesaceleracion);

    printf("Altura inicial          : %.2f m\n",
           estado->reingreso.alturaInicial);

    printf("Altura final            : %.2f m\n",
           estado->reingreso.alturaFinal);

    printf("Velocidad impacto       : %.2f m/s\n",
           estado->amerizaje.velocidadImpacto);

    printf("Distancia hundimiento   : %.2f m\n",
           estado->amerizaje.distanciaHundimiento);

    printf("==================================================\n");
}

void mostrarResultadosReingreso(const EstadoSimulador *estado)
{
    printf("\n==================================================\n");
    printf("          RESULTADOS DEL REINGRESO\n");
    printf("==================================================\n");

    printf("Energia cinetica inicial : %.2f J\n",
           estado->fisico.energiaCineticaInicial);

    printf("Energia cinetica final   : %.2f J\n",
           estado->fisico.energiaCineticaFinal);

    printf("Energia potencial inicial: %.2f J\n",
           estado->fisico.energiaPotencialInicial);

    printf("Energia potencial final  : %.2f J\n",
           estado->fisico.energiaPotencialFinal);

    printf("Trabajo gravedad         : %+.2f J\n",
           estado->fisico.trabajoGravedad);

    printf("Trabajo rozamiento       : %+.2f J\n",
           estado->fisico.trabajoRozamiento);

    printf("Energia disipada         : %.2f J\n",
           estado->fisico.energiaDisipada);

    printf("==================================================\n");
}

void mostrarResultadosAmerizaje(const EstadoSimulador *estado)
{
    printf("\n==================================================\n");
    printf("          RESULTADOS DEL AMERIZAJE\n");
    printf("==================================================\n");

    printf("Energia de impacto       : %.2f J\n",
           estado->fisico.energiaImpacto);

    printf("Trabajo gravedad         : %+.2f J\n",
           estado->fisico.trabajoGravedadAmerizaje);

    printf("Trabajo del agua         : %+.2f J\n",
           estado->fisico.trabajoAgua);

    printf("Fuerza promedio agua     : %.2f N\n",
           estado->fisico.fuerzaPromedioAgua);

    printf("Peso de Orion            : %.2f N\n",
           estado->fisico.pesoNave);

    printf("Relacion fuerza/peso     : %.4f\n",
           estado->fisico.fuerzaEnPesos);

    printf("==================================================\n");
}

void mostrarResumenMision(const EstadoSimulador *estado)
{
    printf("\n==================================================\n");
    printf("               RESUMEN DE MISION\n");
    printf("==================================================\n");

    printf("Reingreso calculado      : %s\n",
           estado->reingresoCalculado ? "SI" : "NO");

    printf("Desaceleracion calculada : %s\n",
           estado->desaceleracionCalculada ? "SI" : "NO");

    printf("Amerizaje calculado      : %s\n",
           estado->amerizajeCalculado ? "SI" : "NO");

    printf("Analisis de angulo       : %s\n",
           estado->anguloCalculado ? "SI" : "NO");

    printf("Total de simulaciones    : %d\n",
           estado->totalSimulaciones);

    printf("==================================================\n");
}
