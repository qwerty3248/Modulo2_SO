/// servidor.c //////////////////////////////////////////////////////
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

/* VARIABLES GLOBALES ----------------------------------------*/
// (globales para que el manejador de SIGINT pueda usarlas)
char nombrefifoe[TAMANO]; // nombre de los cauces conocidos
char nombrefifos[TAMANO];
int peticiones = 0;       // nº de peticiones procesadas


/* SIGCHLD ---------------------------------------------------*/
static void sigchld(int signal) {
    // mata al hijo zombie que ha mandado la señal
    wait(NULL);
}


/* SIGINT ----------------------------------------------------*/
static void sigint(int signal) {
    // elimina los archivos
    unlink("lock.tmp");     // cerrojo
    unlink(nombrefifoe);    // cauce de entrada
    unlink(nombrefifos);    // cauce de salida

    // mensaje de salida
    printf("\n\n[+] cerrando el servidor... se ha atendido "
           "a (%d peticiones)\n",peticiones);

    exit(0);
}


/* MAIN ------------------------------------------------------*/
int main(int argc, char *argv[])
{
    /* ARGUMENTOS ------------------------------------------- */
    if (argc < 2) SERROR("uso: %s <fifo>\n", argv[0]);


    /* SEÑALES ---------------------------------------------- */
    if (signal(SIGPIPE,SIG_IGN)==SIG_ERR)PERROR("[-] signal(SIGPIPE)");
    if (signal(SIGCHLD,sigchld)==SIG_ERR)PERROR("[-] signal(SIGCHLD)");
    if (signal(SIGINT,sigint)==SIG_ERR)PERROR("[-] signal(SIGINT)");


    /* CAUCES ----------------------------------------------- */
    // nombres
    sprintf(nombrefifoe, "%se", argv[1]);
    sprintf(nombrefifos, "%ss", argv[1]);

    // creación
    if (mkfifo(nombrefifoe, S_IRWXU) < 0) PERROR("[-] mkfifo()");
    if (mkfifo(nombrefifos, S_IRWXU) < 0) PERROR("[-] mkfifo()");

    // apertura
    int dfifoe, dfifos;
    if ((dfifoe = open(nombrefifoe, O_RDONLY))<0) PERROR("[-] open()");
    if ((dfifos = open(nombrefifos, O_WRONLY))<0) PERROR("[-] open()");


    /* BUCLE DE EJECUCIÓN ----------------------------------- */
    while(1)
    {
        /* PETICIONES --------------------------------------- */
        int pid_cliente;
        while(!read(dfifoe, &pid_cliente, sizeof(int)));


        /* PROXY -------------------------------------------- */
        int pid_proxy = fork();
        if (!pid_proxy) { execl("./proxy", "./proxy", NULL); }


        /* SERVIDOR ----------------------------------------- */
        else {
            // envía el PID del proxy al cliente
            write(dfifos, &pid_proxy, sizeof(pid_proxy));

            // aumenta el contador de peticiones
            peticiones++;
        }
    }

    return 0;
}
