# RunnerCC

Esta pequeña aplicación tiene como objetivo, facilita el proceso de ejecución y comparación de resultados de las pruebas del obligatorio de Algoritmos 2.

**Actualmente soporta los siguientes compiladores:**

* G++
* Visual C++ (Solo Windows)
* Java

## Primera Ejecución

La primera vez que se ejecute la aplicación se generaran varias carpetas y un archivo de configuración.

Carpeta | Función
------- | -------
**exercise**|En esta carpeta se debe de colocar ya sea el archivo .cpp o .java que desea compilar. (En el caso de colocar mas de un archivo intentara compilar el primer archivo por orden alfabético.)
**bin**|Aquí se generara el resultado de compilar el ejercicio.
**in**|En esta carpeta se deben de colocar los archivos de entrada del ejercicio.
**out**| En esta carpeta se generaran los archivos que son el resultado de correr el ejercicio con cada archivo de entrada.
**exp**|En esta carpeta se deben de colocar los archivos que contengan el resultado esperado del ejercicio, cada archivo de estos debe de compartir el nombre con el correspondiente archivo de entrada.

Explicación de los campos del archivo de configuración.

Carpeta | Campo | Valor Inicial | Ejemplo
------- | ------- | ------- | -------
**Compiler:GPPExtraOptions**|Aquí puede agregar parámetros extra que se utilizaran a la hora de compilar el ejercicio con g++. || ```-std=c++17 -lstdc++fs```
**Compiler:JavaExtraOptions**|Aquí puede agregar parámetros extra que se utilizaran a la hora de compilar el ejercicio con Java.|| ```-O -depend```
**Compiler:MVSCPPExtraOptions**|Aquí puede agregar parámetros extra que se utilizaran a la hora de compilar el ejercicio con Visual C++.|| ```-std=c++17 -lstdc++fs```
**Compiler:MVSCommandLineToolsPath**|Aquí se debe colocar el path a la **carpeta** que contiene el ```VsDevCmd.bat``` que es **obligatorio** si desea utilizar el compilador de Visual C++ ||```C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\```
**File:ExpExtension**|Extension de los archivos que contienen el resultado esperado.|**.xp.txt**|
**File:InExtension**|Extension de los archivos de entrada.|**.in.txt**|
**File:OutExtension**|Extension de los archivos de salida.|**.out.txt**|
**Other:CloseImmediatelyUponEnd**|Si desea que RunnerCC se cierre inmediatamente al correr todas las pruebas o no.|**false**| ```false``` O ```true```

## Consideraciones Java

Es necesario que todas las clases no se encuentren en **ningún** paquete para el correcto funcionamiento de el RunnerCC.

## Consideraciones Windows

RunnerCC siempre intentara usar el compilador Visual C++ si se define un path valido, en cualquier otro caso utilizara g++.

## Backlog

* Detección de paquetes Java. (**nota:** esto se podría lograr iterando en todas las carpetas pero no estoy completamente seguro como detectar el punto de entrada de la aplicación.)
* Detección de main a la hora de compilar para permitir multiples archivos.
* Colocar el resultado de la ejecución del RunnerCC en un archivo.
