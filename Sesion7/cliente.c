/// cliente.c ///////////////////////////////////////////////////////
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

#define TAMANO 1024
#define LONGNOMBRE 50

#define PERROR(text){perror((text));exit(EXIT_FAILURE);}
#define SERROR(...){fprintf(stderr, __VA_ARGS__);exit(EXIT_FAILURE);}


/* OBTENER PROXY -------------------------------------------- */
int obtenerfifo(int dfifoe, int dfifos, int mipid) {
    int mififo;
    int resultado;
    int pidproxy;
    char nombrefifoproxy[LONGNOMBRE];

    // petición al servidor
    if (!(resultado = write(dfifoe, &mipid, sizeof(int))))
        PERROR("[-] cliente: error al enviar petición");

    printf("cliente %d: escrito en FIFO del servidor.\n", mipid);

    // recepción del PID del proxy
    if (!(resultado = read(dfifos, &pidproxy, sizeof(int))))
        PERROR("cliente: error al leer PID del proxy");

    printf("cliente %d: leido PID %d del proxy\n", mipid, pidproxy);

    // apertura del cauce con el proxy
    sprintf(nombrefifoproxy, "fifo.%d", pidproxy);
    while ((mififo = open(nombrefifoproxy, O_WRONLY)) == -1);

    return mififo;
}


/* PRODUCCIÓN Y ENVÍO --------------------------------------- */
void producir(char c, int mififo, int mipid) {
    char buffer[TAMANO];
    int contador = 0;
    int numcar;


    /* PRODUCCIÓN ------------------------------------------- */
    // nº de caracteres a generar (aleatorio)
    srand ((unsigned int) mipid);
    numcar = 1 + (int) (10000.0*rand()/(RAND_MAX+1.0));
    printf("[+] cliente %d: número de caracteres %c a generar es %d\n",
           mipid, c, numcar);


    /* SEÑALES ---------------------------------------------- */
    // ignora la señal SIGPIPE
    if (signal(SIGPIPE, SIG_IGN)==SIG_ERR)PERROR("[-] signal(SIGPIPE)");


    /* ENVÍO DE DATOS --------------------------------------- */
    while (contador < numcar) {
        // calcula los bytes a enviar
        const int a_enviar = (numcar-contador) < TAMANO
                           ? numcar-contador
                           : TAMANO;

        // llena el buffer de caracteres
        memset(buffer, c, a_enviar);

        // envía los datos
        if (write(mififo, buffer, a_enviar) == -1)
            PERROR("[-] error al enviar al proxy");
        contador += a_enviar;

        printf("[+] cliente %d: enviados %d bytes\n", mipid, a_enviar);
    }
}


int main (int argc, char **argv) {  
    /* VARIABLES -------------------------------------------- */
    char c = 'a';
    int numcli;
    int pid;
    int i;
    int dfifoe, dfifos;
    int mififo, mipid;
    char nombrefifoe[LONGNOMBRE], nombrefifos[LONGNOMBRE];

    // desactiva el buffer intermedio para que los hijos no lo hereden
    setvbuf(stdout, NULL, _IONBF, 0);

    /* ARGUMENTOS ------------------------------------------- */
    if (argc < 3) SERROR("uso: %s <fifo> <nº clientes>\n", argv[0]);

    numcli = atoi(argv[2]);
    if (numcli > 10 || numcli < 1)
        SERROR("clientes: <num_clientes> comprendido entre 1 y 10\n");


    /* APERTURA DE CAUCES ----------------------------------- */
    sprintf(nombrefifoe, "%se", argv[1]); // entrada
    sprintf(nombrefifos, "%ss", argv[1]); // salida

    if ((dfifoe = open(nombrefifoe, O_WRONLY)) == -1)
        PERROR ("[-] clientes: archivo fifo de entrada al servidor");

    if ((dfifos = open(nombrefifos, O_RDONLY)) == -1)
        PERROR ("[-] clientes: archivo fifo de salida al servidor");


    /* CLIENTES --------------------------------------------- */
    for (i=1; i<=numcli; i++) {
        if ((pid = fork()) == -1) PERROR ("clientes: fork()");

        /* HIJO --------------------------------------------- */
        if (pid == 0) {
            mipid = getpid();
            mififo = obtenerfifo(dfifoe, dfifos, mipid);
            printf("[+] cliente %d: obtenido FIFO\n", getpid());

            producir(c, mififo, mipid);
            printf("[+] cliente %d: fin de la transmisión\n", getpid());
            exit(EXIT_SUCCESS);
        }

        /* PADRE -------------------------------------------- */
        else c++;
    }

    while (wait(NULL) != -1); // espera a que mueran los hijos
    exit(EXIT_SUCCESS);
}
