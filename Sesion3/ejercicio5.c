/// actividad 5 /////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ASSERT(expr, ...)if(!(expr))\
                        { fprintf(stderr, __VA_ARGS__); exit(-1); }
#define NUM_PROC 6 

int main(int argc, char *argv[])
{
    pid_t pid_hijo;
    pid_t pid_padre = getpid();

    /* INVOCA A LOS HIJOS */
    for(int i=0; i<NUM_PROC; i++) {
        // crea al hijo
        pid_hijo = fork();
        ASSERT(pid_hijo >= 0,
               "[-] error: crear hijo %d: %s\n",i,strerror(errno));

        // si es el hijo, sale del bucle 
        if (!pid_hijo) break;
    }

    // espera a que termine cada hijo
    if (getpid() == pid_padre) {
        siginfo_t info_hijo;    // guarda el PID del hijo

        /* ESPERA A LOS HIJOS IMPARES */
        for(int i=1; i<NUM_PROC; i+=2) {
            waitid(P_PID, getpid() + i, &info_hijo, WEXITED);
            printf("[+] ha terminado el hijo %d\n",
                   info_hijo.si_pid - getpid());
        }

        /* ESPERA A LOS HIJOS PARES */
        for(int i=2; i<NUM_PROC; i+=2) {
            waitid(P_PID, getpid() + i, &info_hijo, WEXITED);
            printf("[+] ha terminado el hijo %d\n",
                   info_hijo.si_pid-getpid());
        }
    }

    // si es el hijo, imprime su índice y su PID
    else {
        printf("[+] hijo %d iniciado con PID %d\n",
               getpid() - getppid(), getpid());

        // retardo
        sleep((getpid() - getppid()));
    }

    return 0;
}
