# Compilacion del Simulador Artemis (GCC)
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -pedantic
LDLIBS  = -lm
TARGET  = artemis
OBJS    = main.o artemis.o validaciones.o interfaz.o reportes.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main.o:         main.c artemis.h validaciones.h interfaz.h reportes.h
artemis.o:      artemis.c artemis.h validaciones.h interfaz.h
validaciones.o: validaciones.c validaciones.h interfaz.h artemis.h
interfaz.o:     interfaz.c interfaz.h artemis.h
reportes.o:     reportes.c reportes.h artemis.h interfaz.h

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe reporte_artemis.txt
