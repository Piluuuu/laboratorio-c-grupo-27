/*
 * interfaz.h
 * Prototipos de las funciones visuales (menus y mensajes).
 */
#ifndef INTERFAZ_H
#define INTERFAZ_H

void mostrarPresentacion(void);
void mostrarMenuPrincipal(void);
void mostrarMenuParametros(void);
void mostrarMensajeError(const char *mensaje);
void mostrarMensajeAdvertencia(const char *mensaje);
void pausar(void);

#endif /* INTERFAZ_H */
