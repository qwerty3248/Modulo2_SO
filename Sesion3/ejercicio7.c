/*
Ejercicio 7.
Escribe un programa que acepte como argumentos el nombre de un
programa, sus argumentos si los tiene,
y opcionalmente la cadena "bg".
Nuestro programa deberá ejecutar el programa pasado como
primer argumento en foreground
si no se especifica la cadena "bg" y en background en caso
contrario.
Si el programa tiene argumentos hay que ejecutarlo con éstos.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    // Verifica el número de parámetros
    if (argc < 2) {
        printf("\nNúmero de parámetros incorrecto\n");
        exit(EXIT_FAILURE);
    }

    int background = 0;
    char *nombre_programa = argv[1];
    char **argumentos = argv + 1;  // Inicia desde el segundo argumento

    // Comprueba si el último parámetro recibido es "bg"
    if (strcmp(argv[argc - 1], "bg") == 0) {
        background = 1;
        argv[argc - 1] = NULL;  // Elimina "bg" de la lista de argumentos
    }

    // Ejecución
    pid_t pid;

    if (background) {
        printf("Ejecutando %s en background\n", nombre_programa);
        if ((pid = fork()) < 0) {
            perror("Error en fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Si es el hijo, ejecuta
            if (execvp(nombre_programa, argumentos) < 0) {
                perror("Error en execvp");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        printf("Ejecutando %s en foreground\n", nombre_programa);
        if (execvp(nombre_programa, argumentos) < 0) {
            perror("Error en execvp");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}

