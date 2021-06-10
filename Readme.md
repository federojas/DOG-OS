# TPE - Arquitectura de las Computadoras - 1C 2021

## Requerimientos previos 

Para garantizar el correcto funcionamiento, debe de instalarse Docker:
```
https://www.docker.com/
```
## Compilacion y ejecucion 

1. Ejecutar el comando `make all` dentro de la carpeta ***Toolchain***
2. De vuelta en la carpeta principal, ejecutar el comando `make all`
3. Ejecutar el programa mediante `./run.sh`





## Manejo correcto del programa 

### Cambio de ventanas
Para cambiar de terminal, presione **CTRL + TAB**
### Uso de la Shell: 

|  Comando |  Función |
| ------------ | ------------ |
|  **`/help`** |  Imprime el listado de comandos disponibles |
|  **`/clear`** |  Limpia la pantalla |
|  **`/user`** |  Cambia el nombre de usuario |
|  **`/inforeg`**  | Muestra el estado de todos los registros (presionar las teclas CTRL + R para capturar el estado de los mismos  |
|  **`/cpufeatures`** |  Muestra las características del CPU |
|  **`/date&time`** |  Muestra la fecha y hora actual |
|  **`/printmem`** |  Realiza un volcado de memoria de 32 bytes a partir de la dirección de memoria ingresada como argumento |
| **`/divzero`** |  Excepción división por cero|
|  **`/opcode`** |  Excepción opcode invalido |
|  **`/cpuvendor`** |  ID del fabricante  |
|  **`/roots`** | Ver **Calculadora de raices**  |
| **`/dog`** |   Imprime DOG-OS logo.|
|  **`/ftcolour`** | Cambia el color del font. (pasar el color como argumento usando la tabla de colores) |
| **`/bgcolour`** | Cambia el color sobre el cual se resalta el texto (no el color de toda la pantalla), pasar el color como argumento usando la tabla de colores|
| **`/exit`** | Finaliza la ejecución de la consola   |

### Calculadora de raices 

![formula](https://render.githubusercontent.com/render/math?math=x=\frac{-b\pm\sqrt{b^2-4ac}}{2a})

Se la debe llamar con el comando `/roots` y los tres coeficientes **a**, **b** y **c** respectivamente como argumentos.
