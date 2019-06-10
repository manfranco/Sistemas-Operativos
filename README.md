# Practica Sistemas Operativos

## Pre Requisitos
 * Tener instalado g++ y git
 * Tener el repositorio "Sistemas Operativos" Clonado
 * Este Proyecto hace uso de C++ Version 11

## Compilación
 La compilación de este proyecto se ejecuta a través del comando make, ejecutado en la carpeta "evaluador".
 En cuanto este proceso finalice, encontrará dentro de la carpeta bin, un ejecutable llamado "main".

## Ejecución
 Los siguientes comandos se encuentran disponibles para la ejecución

 * ./build/mainbasura init -i[integer] -ie [integer] -oe[integer] -n[string] -b[integer] -d[integer] -s[integer] -q[integer]

 * ./build/mainbasura reg -n[string] donde se pueden ingresar los elementos manualmente o 

 * ./build/mainbasura reg -n[string]{{filename1}...|}

 * ./build/mainbasura ctrl -n[string] (para este comando, la consola permitirá el ingreso de las siguientes opciones de sub comandos)

    * >list processing | waiting | reported | reactive | all  NL