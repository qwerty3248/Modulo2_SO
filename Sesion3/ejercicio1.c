#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{

if (argc != 2){
	perror("ERROR numero de argumentos erroneo pruebe con: [programa] [numero entero]\n");
	exit(EXIT_FAILURE);
}


int var = atoi(argv[1]);
pid_t hijo;
if ((hijo = fork()) < 0){
	perror("ERROR con el proceso hijo\n");
	exit(EXIT_FAILURE);
}

if (hijo != 0){//si no es 0 es el padre quien esta ejecutando
	bool divisible = false;
	if (var % 4 == 0){
		divisible = true;
	
	}
	
	if (divisible){
		printf("El numero %i es divisible entre 4\n",var);
	}else{
		printf("El numero %i NO es divisible entre 4\n",var);
	
	}
	
	
}else{//en otro caso es el hijo
	bool par = false;
	if (var % 2 == 0){
		par = true;
	}
	if (par){
		printf("El numero %i es par\n",var);
	}else{
		printf("El numero %i NO es par\n",var);
	
	}

}//acaba el programa 


	return (EXIT_SUCCESS);
}

