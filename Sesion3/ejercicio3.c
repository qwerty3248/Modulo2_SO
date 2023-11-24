#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        perror("ERROR: número incorrecto de argumentos. Uso: [programa] [numero procesos]\n");
        exit(EXIT_FAILURE);
    }

    int procesos = atoi(argv[1]);
    pid_t childpid;

    // Jerarquia tipo 1
    for (int i = 1; i <= procesos; i++) {
        if ((childpid = fork()) == -1) {
            perror("Error al crear el hijo");
        }

        if (childpid) {	
            break;
        }
    }

    // Jerarquia tipo 2
    for (int i = 1; i <= procesos; i++) {
        if ((childpid = fork()) == -1) {
            perror("Error al crear el hijo");
            //break;
        }

        if (!childpid) {
            break;
        }
    }

    return EXIT_SUCCESS;
}

