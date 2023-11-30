#include <stdio.h>
#include <signal.h>
int main(int argc, char * argv[])
{
	sigset_t new_mask;
	//Inicializar mascara con todas las señales
	sigfillset(&new_mask);
	//Quitamos la que queremos
	sigdelset(&new_mask, SIGUSR1);
	//el proceso esperará mientras se le manden las señales de la
	//mascara
	sigsuspend(&new_mask);
}
