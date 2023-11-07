#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int contarArchivosEjecutables(const char *ruta, int *tamanoTotal) {
    DIR *dir = opendir(ruta);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return 0;
    }

    struct dirent *entrada;
    struct stat atributos;
    int total = 0;
    int tamanoTotalLocal = 0;

    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
            char ruta_completa[PATH_MAX];
            snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta, entrada->d_name);

            if (lstat(ruta_completa, &atributos) < 0) {
                perror("Error al intentar acceder a los atributos");
                return 0;
            }

            if (S_ISREG(atributos.st_mode) && (atributos.st_mode & S_IXGRP) && (atributos.st_mode & S_IXOTH)) {
                printf("%s %lu\n", ruta_completa, (unsigned long)atributos.st_ino);
                total++;
                tamanoTotalLocal += atributos.st_size;
            }

            if (S_ISDIR(atributos.st_mode)) {
                tamanoTotalLocal += contarArchivosEjecutables(ruta_completa, &total);
            }
        }
    }

    closedir(dir);
    *tamanoTotal = tamanoTotalLocal;
    return total;
}

int main(int argc, char *argv[]) {
    const char *directorio = (argc > 1) ? argv[1] : "."; // Ruta al directorio, o el directorio actual si no se proporciona un argumento
    int tamanoTotal = 0;
    int total = contarArchivosEjecutables(directorio, &tamanoTotal);

    if (total > 0) {
        printf("Existen %d archivos regulares con permiso x para grupo y otros\n", total);
        printf("El tamaño total ocupado por dichos archivos es %d bytes\n", tamanoTotal);
    }

    return 0;
}

