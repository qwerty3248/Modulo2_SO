/// proxy.c /////////////////////////////////////////////////////////
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define PERROR(text){perror((text));exit(EXIT_FAILURE);}
#define SERROR(...){fprintf(stderr, __VA_ARGS__);exit(EXIT_FAILURE);}

#define TAMANO 1024

int main(int argc, char *argv[])
{
    /* VARIABLES -------------------------------------------- */
    char buffer[TAMANO]; // buffer de datos
    char nombrefifoproxy[TAMANO]; // nombre del cauce
    int dfifoe; // descriptor del cauce de entrada


    /* FICHERO DE BLOQUEO Y CERROJO ------------------------- */
    int lock_fd = open("lock.tmp", O_CREAT|O_APPEND|O_RDWR, S_IRWXU);

    // cerrojo de escritura
    struct flock wlock = {
        .l_type = F_WRLCK,
        .l_whence = SEEK_SET,
        .l_start = 0,
        .l_len = 0
    };


    /* CAUCE ------------------------------------------------ */
    // crea el cauce
    sprintf(nombrefifoproxy, "fifo.%d", getpid());
    mkfifo(nombrefifoproxy, S_IRWXU);

    // abre el cauce
    while((dfifoe = open(nombrefifoproxy, O_RDONLY)) == -1);


    /* RECEPCIÓN -------------------------------------------- */
    // creación del fichero temporal
    FILE *tmp = tmpfile();
    if (!tmp) PERROR("[-] tmpfile()");

    // escritura de datos en el fichero 
    int leido;
    while (leido = read(dfifoe, buffer, TAMANO)) {
        fwrite(buffer, sizeof(char), leido, tmp);
    }

    // mueve el puntero de escritura al principio
    rewind(tmp);


    /* IMPRESIÓN -------------------------------------------- */
    // cerrojo de escritura
    if (fcntl(lock_fd, F_SETLKW, &wlock) == -1)
        PERROR("[-] fcntl() (cerrojo de escritura)"); 

    // escribe en la salida estándar
    fprintf(stdout, "\n\n[+] proxy %d: mensaje recibido\n", getpid());
    while(leido = fread(buffer, sizeof(char), TAMANO, tmp)) {
        write(STDOUT_FILENO, buffer, leido);
    }


    /* BORRA EL CAUCE --------------------------------------- */
    close(dfifoe);
    unlink(nombrefifoproxy);

    exit(EXIT_SUCCESS);
}
