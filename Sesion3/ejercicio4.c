#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    int num_hijos = 5;

    for (int i = 0; i <= num_hijos; i++) {
        pid_t pid;
        if ((pid = fork()) < 0) {
            perror("Error creando a un hijo\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            printf("Soy el hijo %d\n", getpid());
            exit(EXIT_SUCCESS); // Cada hijo debe salir después de imprimir su mensaje
        }
    }

    while (num_hijos > 0) {
        printf("Soy el padre y me quedan %d hijos\n", num_hijos);
        pid_t child_pid = wait(NULL); // Esperar a que un hijo termine cualquiera 
        //si quisieramos un hijo en particular tendriamos que ponerle su status
        printf("Acaba de finalizar mi hijo con PID %d\n", child_pid);
        num_hijos--;
    }

    return EXIT_SUCCESS;
}

