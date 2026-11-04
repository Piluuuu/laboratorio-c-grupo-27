/*
 * interfaz.h
 * Prototipos de las funciones visuales (menus y mensajes).
 * NOTA: el Integrante 5 agregara aqui el resto de las funciones
 * mostrar...() de su tabla (resultados, datos iniciales, resumen, etc.).
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
