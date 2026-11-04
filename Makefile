# Compilacion del Simulador Artemis (GCC)
CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
OBJS   = main.o artemis.o validaciones.o interfaz.o reportes.o

artemis: $(OBJS)
	$(CC) $(CFLAGS) -o artemis $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main.o: main.c artemis.h validaciones.h interfaz.h reportes.h
artemis.o: artemis.c artemis.h interfaz.h
validaciones.o: validaciones.c validaciones.h interfaz.h
interfaz.o: interfaz.c interfaz.h artemis.h
reportes.o: reportes.c reportes.h artemis.h interfaz.h

clean:
	rm -f $(OBJS) artemis
