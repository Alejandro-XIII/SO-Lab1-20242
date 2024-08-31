#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Función para mostrar un mensaje de uso en caso de error
void printUsage() {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
}

// Función para invertir el contenido de un archivo
void reverseFile(FILE *input, FILE *output) {
    char **lines = NULL;
    size_t lineCount = 0;
    char *line = NULL;
    size_t len = 0;

    // Leer todas las líneas del archivo
    while (getline(&line, &len, input) != -1) {
        lineCount++;
        lines = realloc(lines, lineCount * sizeof(char*));
        if (lines == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        lines[lineCount - 1] = strdup(line);
    }
    free(line);

    // Imprimir las líneas en orden inverso
    for (int i = lineCount - 1; i >= 0; i--) {
        fprintf(output, "%s", lines[i]);
        free(lines[i]);
    }

    free(lines);
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;

    if (argc > 3) {
        printUsage();
    }

    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        output = fopen(argv[2], "w");
        if (output == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            fclose(input);
            exit(1);
        }
    }

    reverseFile(input, output);

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}
