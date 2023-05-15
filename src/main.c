#include "infix.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char nombreArchivo[100];

  printf("Ingrese el nombre del archivo de expresiones: ");
  fgets(nombreArchivo, sizeof(nombreArchivo), stdin);

  // Eliminar el salto de línea al final del nombre del archivo
  
    /*https://en.cppreference.com/w/c/io/fscanf*/
  sscanf(nombreArchivo, "%[^\n]", nombreArchivo);

  // Procesar el archivo de expresiones
  procesarArchivo(nombreArchivo);

  return 0;
}
