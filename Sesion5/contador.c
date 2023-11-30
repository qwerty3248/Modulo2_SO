#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
static int cont[_NSIG]; // _NSIG habría que buscarlo dentro de los
			//archivos del sistema
void handler(int signum) {
	printf("\nRecibida la senal numero %d, veces: %d\n", signum,
	++cont[signum]);
	}
int main(int argc, char *argv[])
{
	struct sigaction sa;
	int i;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	for (i = 1; i <= _NSIG; i++) {
		cont[i] = 0;
		if (sigaction(i, &sa, NULL) < 0)
		{
			printf("Error al intentar establecer el manejador de la senal %d\n", i);
		}
	}
	while(1);
}
