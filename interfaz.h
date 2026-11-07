/*
 * interfaz.h
 * Prototipos de las funciones visuales (menus y mensajes).
 * NOTA: el Integrante 5 agregara aqui el resto de las funciones
 * mostrar...() de su tabla (resultados, datos iniciales, resumen, etc.).
 */
#ifndef INTERFAZ_H
#define INTERFAZ_H
#include "artemis.h"
void mostrarPresentacion(void);
void mostrarMenuPrincipal(void);
void mostrarMenuParametros(void);
void mostrarMensajeError(const char *mensaje);
void mostrarMensajeAdvertencia(const char *mensaje);
void pausar(void);

void mostrarDatosIniciales(const EstadoSimulador *estado);
void mostrarResultadosReingreso(const EstadoSimulador *estado);
void mostrarResultadosAmerizaje(const EstadoSimulador *estado);
void mostrarResumenMision(const EstadoSimulador *estado);

#endif /* INTERFAZ_H */
