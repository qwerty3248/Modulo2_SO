#define _XOPEN_SOURCE 500 // Para habilitar nftw en sistemas UNIX

#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>

long num_bytes = 0;
int num_archivos = 0;

int contar_archivos(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    if (typeflag == FTW_F) {
        const mode_t permisos_tener = 011;
        const mode_t permisos_archivo = sb->st_mode & 0777;

        // Verificar si el archivo tiene los permisos deseados
        if ((permisos_archivo & permisos_tener) != 0) {
            printf("---%s---\n", fpath);
            num_archivos++;
            num_bytes += ftwbuf->base;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Error: Parámetros incorrectos. Pruebe con: [programa] [pathname]\n");
        exit(EXIT_FAILURE);
    }

    // FTW_DEPTH: evita la recursión en subdirectorios
    int flags = FTW_DEPTH;
    printf("Los i-nodos son:\n");
    if (nftw(argv[1], contar_archivos, 20, flags) == -1) {
        perror("Error al realizar nftw\n");
        exit(EXIT_FAILURE);
    }

    printf("Existen %d archivos regulares con permiso x para grupo y otros\n", num_archivos);
    printf("El tamaño total ocupado por dichos archivos es %ld bytes\n", num_bytes);

    return EXIT_SUCCESS;
}

