#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Error: número de argumentos inválidos [programa] [path] [octal]\n");
        exit(EXIT_FAILURE);
    }

    const char *directorio = argv[1]; // Ruta al directorio
    const mode_t permisos = strtol(argv[2], 0, 8); // Permisos octales

    DIR *dir = opendir(directorio);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        exit(EXIT_FAILURE);
    }

    struct dirent *entrada;

    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
            char ruta_completa[PATH_MAX];
            snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", directorio, entrada->d_name);

            struct stat atributos;
            if (lstat(ruta_completa, &atributos) < 0) {
                perror("Error al intentar acceder a los atributos");
                exit(EXIT_FAILURE);
            }

            mode_t permisos_originales = atributos.st_mode & 07777;

            if (chmod(ruta_completa, (permisos & 07777) | (permisos_originales & ~07777)) < 0) {
                printf("%s : Error %d %o\n", entrada->d_name, errno, (unsigned int)permisos);
            } else {
                printf("%s : %o %o\n", entrada->d_name, (unsigned int)permisos_originales, (unsigned int)permisos);
            }
        }
    }

    closedir(dir);
    return EXIT_SUCCESS;
}

