# Simulador Artemis - Escenario 4 (Grupo 27)

Simulador por consola, escrito en C, del **reingreso atmosférico, frenado aerodinámico y amerizaje** de la cápsula Orion (misión Artemis). Trabajo Final Integrador de Algoritmos y Estructuras de Datos.

**Escenario seleccionado:** Escenario 4 - Reingreso Atmosférico, Frenado Aerodinámico y Amerizaje (Trabajo, Energía e Impacto).

## Objetivo

Aplicar programación estructurada y modularización en C para analizar, con los conceptos de trabajo y energía, las tres etapas del regreso de la cápsula: el frenado en la atmósfera, la desaceleración que soporta la tripulación y el impacto con el agua.

## Integrantes

> _Completar con los nombres, legajos y la división de tareas del grupo, según lo que pida la consigna._

## Uso de herramientas de IA

> _Completar según lo que exija la consigna (herramientas utilizadas y en qué partes)._

## Requisitos

- Compilador GCC con soporte para C99.
- `make` (en Windows, MinGW/MSYS2 con `make`).

## Compilar y ejecutar

```bash
make clean
make          # genera el ejecutable "artemis" (-Wall -Wextra -std=c99 -pedantic, sin advertencias)
./artemis     # en Windows: artemis.exe
```

También se puede compilar sin `make`:

```bash
gcc -Wall -Wextra -std=c99 -pedantic main.c artemis.c validaciones.c interfaz.c reportes.c -o artemis -lm
```

## Datos del escenario (valores predeterminados)

| Parámetro | Valor |
|---|---:|
| Masa de Orion | 10 400 kg |
| Velocidad inicial | 11 000 m/s |
| Velocidad final (al desplegar el paracaídas) | 25 m/s |
| Altura inicial | 120 000 m |
| Altura final | 2 500 m |
| Tiempo de desaceleración principal | 400 s |
| Velocidad de impacto | 8 m/s |
| Hundimiento en el agua | 1,50 m |
| Gravedad | 9,80 m/s² |

## Instrucciones de uso

Al iniciar se muestra la presentación y el menú principal. Se elige una opción escribiendo su número y presionando ENTER:

| Opción | Función |
|---|---|
| 1 | Reingreso atmosférico: energías cinética y potencial, trabajo de la gravedad y del rozamiento, energía disipada. |
| 2 | Desaceleración media (m/s² y en múltiplos de g) y su clasificación para la tripulación. |
| 3 | Amerizaje: energía de impacto, trabajo y fuerza promedio del agua, peso y relación fuerza/peso. |
| 4 | Análisis de seguridad del ángulo de reingreso (ventana segura: 5,5° a 7,0°). |
| 5 | Parámetros de la misión: modificar uno por uno o restablecer los valores originales. |
| 6 | Historial de las simulaciones ejecutadas (guarda las últimas 20 con sus datos y resultados). |
| 7 | Reporte final: se muestra por pantalla y se guarda en `reporte_artemis.txt`. |
| 8 | Salir. |

Al modificar o restablecer parámetros se descartan los resultados anteriores (el historial se conserva), así que hay que volver a ejecutar los análisis. Todas las entradas se validan: ante un dato inválido se informa el error y se vuelve a pedir.

## Modelo físico y supuestos

- **Reingreso:** teorema trabajo-energía, `W_gravedad + W_rozamiento = K_final - K_inicial`, con `W_gravedad = m·g·(h_inicial - h_final)`.
- **Desaceleración:** `a = (v_f - v_i) / t` y `a_g = |a| / g`. Se asume desaceleración uniforme.
- **Amerizaje:** `K = ½·m·v²`. Por defecto la gravedad también realiza trabajo durante el hundimiento (`W_g = m·g·d`), por lo que `W_agua = ΔK - W_g` y la fuerza media del agua es `|W_agua| / d`.
  Este supuesto se controla con la constante `AMERIZAJE_INCLUYE_GRAVEDAD` de `artemis.h`: con valor `0` se desprecia el peso durante el hundimiento (`W_agua = ΔK`).
- Los umbrales que clasifican la desaceleración y el impacto son criterios del grupo (no provienen de la consigna).

Resultados con los datos predeterminados:

| Magnitud | Valor |
|---|---:|
| Energía cinética inicial / final | 629 200 000 000 J / 3 250 000 J |
| Energía potencial inicial / final | 12 230 400 000 J / 254 800 000 J |
| Trabajo de la gravedad (reingreso) | +11 975 600 000 J |
| Trabajo del rozamiento / energía disipada | −641 172 350 000 J / 641 172 350 000 J |
| Desaceleración media | 27,4375 m/s² ≈ 2,7997 g |
| Energía cinética de impacto | 332 800 J |
| Peso de la nave | 101 920 N |
| Trabajo de la gravedad (hundimiento) | +152 880 J |
| Trabajo del agua | −485 680 J |
| Fuerza media del agua | 323 786,67 N (3,1769 veces el peso) |

Con el modelo que desprecia el peso (`AMERIZAJE_INCLUYE_GRAVEDAD 0`): trabajo del agua −332 800 J, fuerza 221 866,67 N (2,1769 veces el peso).

## Organización de los módulos

| Archivo | Contenido |
|---|---|
| `main.c` | Punto de entrada, menú principal y submenú de parámetros. No hace cálculos. |
| `artemis.h` / `artemis.c` | Constantes, estructuras, cálculos físicos, análisis del ángulo y gestión de parámetros. |
| `validaciones.h` / `validaciones.c` | Lectura segura por línea (enteros, decimales, caracteres) y funciones de validación. |
| `interfaz.h` / `interfaz.c` | Presentación, menús y mensajes. Solo imprime. |
| `reportes.h` / `reportes.c` | Historial de simulaciones y reporte final. |
| `Makefile` | Compilación con `make` / `make clean`. |

Los textos se escriben sin tildes para que se vean bien en cualquier consola.
