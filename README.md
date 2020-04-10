# lab-Computación-Grafica

### Directorios:
- Carpeta **dependences**: Contiene las librerías _glfw_ y _glad_
- Carpeta **src**: Contiene todos los programas. 
    * **Test**: Programa que demuestra la instalación de Opengl sin el uso de la libreria _glad_.
    * **First_program**: programa ejemplo usando _glad_.
    Para ejecutar cambiar en _CMakeLists.txt_:
    ```plain
         # Source files
         set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/First_program")
         set(SOURCES "${SRC_DIR}/con_glad.cpp")
     ```
     * **Triangle**: segundo tutorial con modificaciones.
     Para ejecutar cambiar en _CMakeLists.txt_:
     ```plain
         # Source files
         set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Triangle")
         set(SOURCES "${SRC_DIR}/triangle.cpp")
     ```
- Carpeta **results**: Muestra la imagen de ejecución de los programas.
    * firstc.png: ejecución del **First_program**
    * triangle.png: ejecución de **Triangle**

### Ejecución general de programas:
```plain
cmake .
make
./nombre_ejecutable
```
