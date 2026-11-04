/*
 * validaciones.h
 * Prototipos del ingreso seguro de datos.
 * NOTA: el Integrante 4 agregara aqui leerFlotante(), leerCaracter(),
 * validarValorPositivo() y validarAngulo(). Integrante 1 solo necesita
 * las funciones declaradas abajo.
 */
#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Descarta los caracteres pendientes hasta el fin de linea. */
void limpiarBufferEntrada(void);

/* Pide un entero. Reintenta hasta que sea valido.
 * Devuelve 1 si lo leyo (guardado en *valor) o 0 si se cerro la entrada (EOF). */
int leerEntero(const char *mensaje, int *valor);

/* Devuelve 1 si minimo <= valor <= maximo; 0 en caso contrario. */
int validarRango(int valor, int minimo, int maximo);

/* Devuelve 1 si la opcion existe en el menu (entre minimo y maximo). */
int validarOpcionMenu(int opcion, int minimo, int maximo);

#endif /* VALIDACIONES_H */
