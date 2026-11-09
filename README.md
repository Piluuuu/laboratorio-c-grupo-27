# Simulador Artemis - Escenario 4 (Grupo 27)

Simulador por consola, escrito en C, del **reingreso atmosférico, frenado aerodinámico y amerizaje** de la cápsula Orion (misión Artemis). Trabajo Final Integrador de Algoritmos y Estructuras de Datos.

**Escenario seleccionado:** Escenario 4 - Reingreso Atmosférico, Frenado Aerodinámico y Amerizaje (Trabajo, Energía e Impacto).

## Objetivo

Aplicar programación estructurada y modularización en C para analizar, con los conceptos de trabajo y energía, las tres etapas del regreso de la cápsula: el frenado en la atmósfera, la desaceleración que soporta la tripulación y el impacto con el agua.

## Integrantes

| Integrante | Apellido y nombre | Legajo | Responsabilidad |
|:---:|---|:---:|---|
| 1 | Verón Romero, Pilar | 31096 | Arquitectura principal y menú |
| 2 | Nuñez, Lourdes Antonella | 31083 | Energía y trabajo del reingreso |
| 3 | Pavich, Yaritza | 31647 | Desaceleración y amerizaje |
| 4 | Canteros, Lara Ailin | 31094 | Seguridad y validaciones |
| 5 | Rolon, Victoria | 31014 | Interfaz, historial y reportes |

## Uso de herramientas de IA

Cada integrante registra a continuación qué herramienta utilizó, para qué, con qué prompts representativos y cómo validó y adaptó el resultado. En todos los casos las respuestas de la IA se tomaron como orientación y fueron revisadas antes de incorporarse al proyecto.

### Integrante 1 - Arquitectura principal y menú

**Herramientas:** ChatGPT (OpenAI) y Claude (Anthropic).

**Para qué se usaron:**

- Organizar la arquitectura inicial del proyecto: archivos `.c` y `.h` necesarios y distribución de responsabilidades entre los integrantes.
- Comprender los archivos de cabecera, los prototipos de funciones y la modularización en C.
- Planificar el menú principal, la navegación entre funcionalidades y las funciones de inicio, lectura de opciones y cierre del simulador.
- Desarrollar la primera versión de la estructura principal, dejando las funcionalidades de los demás integrantes como implementaciones provisionales.

**Prompts utilizados (resumen):**

- «Necesito un inventario de todas las funciones necesarias para el programa, sin escribir código todavía, para poder distribuir las responsabilidades entre los integrantes.»
- «Explicame qué son los archivos `.h` y cómo se relacionan con los `.c`.»
- «Necesito que Claude desarrolle exclusivamente la parte del Integrante 1: arquitectura principal, `main`, navegación y menú, sin implementar los cálculos físicos ni las funcionalidades de los demás.»

**Cómo se validó y adaptó:**

- Se delimitaron las funciones del Integrante 1 para evitar superposiciones con el trabajo del resto del grupo.
- Se compiló con GCC (`-Wall -Wextra -std=c99 -pedantic`) sin errores ni advertencias.
- Se probaron la presentación, el menú, la navegación, el submenú de parámetros y el manejo de entradas inválidas.

### Integrante 2 - Energía y trabajo del reingreso

**Herramienta:** ChatGPT (Codex).

**Para qué se usó:**

- Implementar en C los cálculos de energía y trabajo del reingreso atmosférico.
- Conectar la función con el menú sin afectar el trabajo de las demás integrantes en `main.c`.
- Entender los errores de compilación del proyecto completo.
- Subir los cambios a GitHub y abrir el pull request.

**Prompts utilizados (resumen):**

- «¿Cómo implemento en C los cálculos de energía y trabajo del reingreso atmosférico?»
- «¿Cómo conecto mi función con el menú sin borrar lo que hicieron mis compañeras en main.c?»
- «¿Cómo pruebo mi parte por separado si el programa completo todavía no compila?»

**Cómo se validó y adaptó:**

- Se probó el módulo por separado antes de integrarlo al programa completo.
- Se revisaron y corrigieron los errores de compilación que aparecieron al integrar el módulo.
- Se conectó la función al menú conservando el código existente en `main.c`.

### Integrante 3 - Desaceleración y amerizaje

**Herramienta:** Claude (Anthropic).

**Para qué se usó:**

- Organizar la estructura inicial del código de desaceleración y amerizaje (`artemis.c` / `artemis.h`), respetando la división de tareas del grupo.
- Obtener explicaciones sobre las fórmulas físicas necesarias para esta etapa.
- Comprender elementos de C usados en la implementación: `struct`, punteros, `enum`, `const` y `math.h`.
- Resolver errores de compilación y ubicar los archivos dentro del repositorio.

**Prompts utilizados (resumen):**

- «Esta es la parte del trabajo que me corresponde desarrollar: desaceleración y amerizaje. Teniendo en cuenta la consigna y la división de funciones del grupo, ayudame a organizar la estructura del código y las funciones que necesito implementar.»
- «Explicame qué fórmulas físicas debo utilizar para calcular la desaceleración y cómo aplicarlas en el código.»
- «Estoy teniendo problemas para compilar el programa en Visual Studio Code. Ayudame a identificar el error y a encontrar una solución.»

**Cómo se validó y adaptó:**

- Se contrastaron los resultados del programa con cálculos manuales, por ejemplo: K = ½ · 10 400 · 8² = 332 800 J y a = 10 975 / 400 = 27,4375 m/s².
- Se verificó que las operaciones implementadas fueran coherentes con los requerimientos de la consigna.
- Se adaptó la estructura propuesta a la organización general del proyecto y a las funciones asignadas.

### Integrante 4 - Seguridad y validaciones

**Herramienta:** Grok (xAI).

**Para qué se usó:**

- Organizar el código de validaciones y análisis de seguridad del ángulo de reingreso (`validaciones.c` / `validaciones.h` y parte de `artemis.c` / `artemis.h`), respetando la división de tareas del grupo.
- Comprender cómo implementar la lectura segura de datos y las validaciones de rango, opciones de menú, valores positivos y ángulos.
- Adaptar las firmas de las funciones de validación a la forma en que ya las utilizaba `main.c`.
- Resolver dónde agregar cada parte del código (constantes, estructuras, prototipos e implementaciones) sin sobrescribir el trabajo de otros integrantes.

**Prompts utilizados (resumen):**

- «Necesito el código en C de la parte de seguridad y validaciones (validaciones.c, artemis.c) del escenario 4.»
- «Pasame solo el código que tengo que agregar a artemis.h y artemis.c, y decime dónde colocarlo.»
- «Revisá si el código de validaciones coincide con lo que espera main.c.»

**Cómo se validó y adaptó:**

- Se revisó la distribución de tareas para implementar únicamente las funciones asignadas a este integrante.
- Se adaptaron las firmas de `leerEntero` y `validarOpcionMenu` para coincidir con el uso que ya hacía `main.c`.
- Se verificó en el repositorio que `ResultadoSeguridad`, las constantes de la ventana de seguridad (5,5° a 7,0°) y los prototipos quedaran bien ubicados y sin eliminar el código de desaceleración y amerizaje.

### Integrante 5 - Interfaz, historial y reportes

**Herramientas:** Microsoft Copilot y Antigravity.

**Para qué se usaron:**

- Analizar la estructura general del proyecto y la integración entre `main.c`, `interfaz.c` / `interfaz.h`, `reportes.c` / `reportes.h` y `artemis.h`, verificando que las funciones fueran coherentes con la división de tareas asignada.
- Identificar funciones faltantes en los módulos de interfaz y reportes y recibir orientación para integrarlas sin afectar la lógica de los demás integrantes.
- Revisar la organización del historial de simulaciones y la generación de reportes finales, detectando posibles problemas de compatibilidad antes de subir los cambios al repositorio.
- Diseñar la presentación visual de la consola (menús, resultados, resúmenes y reportes) y proponer una navegación más clara para el usuario.

**Prompts utilizados (resumen):**

- «Necesito un prompt para una IA de programación porque debo hacer un trabajo universitario sobre este escenario de reingreso atmosférico.»
- «Analiza estos archivos y dime qué funciones faltan implementar.»
- «Ayúdame a diseñar una interfaz de consola clara para el simulador y propón una presentación visual para los menús y resultados.»

**Cómo se validó y adaptó:**

- Se compararon las sugerencias con la consigna original y con la división de responsabilidades acordada por el grupo.
- Se verificó la compatibilidad de las funciones incorporadas con la estructura `EstadoSimulador` y que no alteraran los cálculos físicos de otros integrantes.
- Se revisó manualmente la ubicación de cada función y se adaptaron las propuestas a la arquitectura, los nombres de funciones y la organización del repositorio.

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
