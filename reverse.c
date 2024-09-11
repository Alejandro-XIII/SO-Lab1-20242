#include <stdio.h>      // Necesario para operaciones de entrada/salida estándar como printf y fprintf
#include <stdlib.h>     // Necesario para funciones de asignación de memoria (malloc, realloc) y para exit()
#include <string.h>     // Necesario para funciones de manipulación de cadenas como strcmp y strdup
#include <sys/stat.h>   // Necesario para usar struct stat y la función stat

// Función para mostrar un mensaje de uso en caso de error
void printUsage() {
    fprintf(stderr, "usage: reverse <input> <output>\n");  // Imprime el mensaje de uso en la salida de error estándar
    exit(1);  // Termina el programa con un código de salida de 1 indicando un error
}

// Función para invertir el contenido de un archivo
void reverseFile(FILE *input, FILE *output) {
    char **lines = NULL;  // Puntero a un array de punteros a cadenas para almacenar las líneas leídas
    size_t lineCount = 0; // Contador de líneas leídas
    char *line = NULL;    // Puntero para almacenar la línea leída temporalmente
    size_t len = 0;       // Variable para almacenar el tamaño de la línea leída

    // Leer todas las líneas del archivo
    while (getline(&line, &len, input) != -1) {  // Lee una línea del archivo de entrada
        lineCount++;  // Incrementa el contador de líneas
        lines = realloc(lines, lineCount * sizeof(char*));  // Realloca memoria para el array de líneas
        if (lines == NULL) {  // Verifica si realloc falló
            fprintf(stderr, "malloc failed\n");  // Imprime un mensaje de error
            exit(1);  // Termina el programa con un código de salida de 1 indicando un error
        }
        lines[lineCount - 1] = strdup(line);  // Duplica la línea leída y la almacena en el array
    }
    free(line);  // Libera la memoria de la línea leída

    // Imprimir las líneas en orden inverso
    for (int i = lineCount - 1; i >= 0; i--) {  // Itera a través del array de líneas en orden inverso
        fprintf(output, "%s", lines[i]);  // Escribe cada línea en el archivo de salida
        free(lines[i]);  // Libera la memoria de la línea almacenada
    }

    free(lines);  // Libera la memoria del array de líneas
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;   // Inicializa el archivo de entrada con la entrada estándar
    FILE *output = stdout; // Inicializa el archivo de salida con la salida estándar

    if (argc > 3) {  // Verifica si hay más de 3 argumentos en la línea de comandos
        printUsage();  // Muestra el mensaje de uso y termina el programa
    }

    if (argc >= 2) {  // Verifica si se proporciona al menos el archivo de entrada
        input = fopen(argv[1], "r");  // Intenta abrir el archivo de entrada en modo lectura
        if (input == NULL) {  // Verifica si el archivo no pudo abrirse
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);  // Imprime un mensaje de error
            exit(1);  // Termina el programa con un código de salida de 1 indicando un error
        }
    }

    if (argc == 3) {  // Verifica si se proporciona también el archivo de salida
        struct stat inputStat, outputStat;  // Declara estructuras para almacenar la información de los archivos

        // Obtener información del archivo de entrada
        if (stat(argv[1], &inputStat) != 0) {  // Obtiene la información del archivo de entrada
            fprintf(stderr, "reverse: cannot stat file '%s'\n", argv[1]);  // Imprime un mensaje de error
            fclose(input);  // Cierra el archivo de entrada
            exit(1);  // Termina el programa con un código de salida de 1 indicando un error
        }

        // Intentar abrir el archivo de salida
        output = fopen(argv[2], "w");  // Intenta abrir el archivo de salida en modo escritura
        if (output == NULL) {  // Verifica si el archivo no pudo abrirse
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);  // Imprime un mensaje de error
            fclose(input);  // Cierra el archivo de entrada
            exit(1);  // Termina el programa con un código de salida de 1 indicando un error
        }

        // Obtener información del archivo de salida
        if (stat(argv[2], &outputStat) != 0) {  // Obtiene la información del archivo de salida
            fprintf(stderr, "reverse: cannot stat file '%s'\n", argv[2]);  // Imprime un mensaje de error
            fclose(input);  // Cierra el archivo de entrada
            fclose(output);  // Cierra el archivo de salida
            exit(1);  // Termina el programa con un código de salida de 1 indicando un error
        }

        // Verificar si ambos archivos son el mismo
        if (inputStat.st_dev == outputStat.st_dev && inputStat.st_ino == outputStat.st_ino) {  // Compara el dispositivo y el inode
            fprintf(stderr, "reverse: input and output file must differ\n");  // Imprime un mensaje de error
            fclose(input);  // Cierra el archivo de entrada
            fclose(output);  // Cierra el archivo de salida
            exit(1);  // Termina el programa con un código de salida de 1 indicando un error
        }
    }

    reverseFile(input, output);  // Llama a la función para invertir el contenido del archivo

    if (input != stdin) fclose(input);  // Cierra el archivo de entrada si no es la entrada estándar
    if (output != stdout) fclose(output);  // Cierra el archivo de salida si no es la salida estándar

    return 0;  // Termina el programa con un código de salida de 0 indicando éxito
}
