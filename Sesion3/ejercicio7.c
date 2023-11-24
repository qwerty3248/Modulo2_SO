#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("ERROR: Uso incorrecto del programa. Prueba con [programa] [programa_a_ejecutar] [argumento1]...[argumento_n] [fg|bg]\n");
        exit(EXIT_FAILURE);
    }

    char* ejecutable = argv[1];
    char* parametros[argc-3];

    for (int i = 2, contador = 0; i <= argc - 2; i++, contador++) {
        parametros[contador] = argv[i];
    }
    
    parametros[argc-3]= NULL;

    if (strcmp(argv[argc-1], "bg") == 0) {
        if (execle("/usr/bin/bg",ejecutable,parametros) < 0) {
            perror("ERROR en el execlp");
            exit(EXIT_FAILURE);
        }
    } else {
        if (execv(ejecutable, parametros) < 0) {
            perror("ERROR en el execv");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}

