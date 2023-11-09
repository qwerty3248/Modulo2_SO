#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        perror("\nError: número de argumentos inválidos [programa] [path] [octal]\n");
        exit(EXIT_FAILURE);
    }

    DIR *Directorio = opendir(argv[1]);
    if (Directorio == NULL) {
        perror("\nError: el directorio no se puede abrir\n");
        exit(EXIT_FAILURE);
    }
    const char *direc = argv[1];
    const int tam_direc = strlen(argv[1]);
    const mode_t permisos = strtol(argv[2], 0, 8);
    struct dirent *archivo;
    while ((archivo = readdir(Directorio)) != NULL) {
        char *path;
        char *name = archivo->d_name;

        // Excluir . y ..
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }

        path = malloc(tam_direc + strlen(name) + 1);
        strcpy(path, direc);
        strcat(path + tam_direc, name);

        struct stat atributos;
        if (stat(path, &atributos) < 0) {
            perror("\nERROR: al acceder a los atributos del archivo\n");
            printf("Archivo: %s\n", path);
            exit(EXIT_FAILURE);
        }

        const mode_t per_originales = atributos.st_mode;
        // Utilizar una máscara para obtener solo los bits de permisos
        const mode_t permisos_solo = per_originales & 0777;

        if (chmod(path, permisos) < 0) {
            printf("%s: ERROR %o\n", path, per_originales);
        } else {
            printf("%s: %o %o\n", path, permisos_solo, permisos);
        }

        free(path);
    }

    closedir(Directorio);

    return EXIT_SUCCESS;
}

