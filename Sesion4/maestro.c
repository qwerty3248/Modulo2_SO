#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include <sys/wait.h>




int main (int argc, char *argv[]){
	
	int fd1[2];
	int fd2[2];
	pid_t PID;
	pid_t PID2;
	int primer_intervalo = atoi(argv[1]);
	int segundo_intervalo = atoi(argv[2]);
	
	pipe(fd1);
	if ((PID = fork()) < 0){
		perror("Error en el primer esclavo\n");
		exit(-1);
	}//ya tenemos nuestro hijo con su pipe sin nombre
	
	if (PID == 0){//el hijo tiene que eliminar comunicaciones
		close(fd1[0]);//cerramos lectura
		
		dup2(fd1[1],STDOUT_FILENO);//cambiamos la entrada estandar de salida
		
		execlp("./esclavo","./esclavo",primer_intervalo,segundo_intervalo/2,NULL);
					
	}else{//el padre tiene que eliminar comunicaciones
		close(fd1[1]);//cerramos escritura
		
		wait(NULL);
		
		//ahora vamos a crear al otro hijo
		pipe(fd2);//segundo canal de comunicacion
		if ((PID2 = fork()) < 0){
			perror("Error en el segundo esclavo\n");
			exit(-1);
		}
		
		if (PID2 == 0){//el segundo esclavo
			close(fd2[0]);//cerramos lectura del hijo 
			
			dup2(fd2[1],STDOUT_FILENO);
			
			execlp("./esclavo","./esclavo",(segundo_intervalo/2)+1,segundo_intervalo,NULL);
			
		
		}else{//el padre cierra escritura del cauce 2
			close(fd2[1]);//fuera escritura
			//ahora vamos a imprimir los mensajes que nos envien en
			//cada uno de los cauces
	            // Leer y procesar la salida del primer esclavo
		    char buffer[80];
		    while (read(fd1[0], buffer, sizeof(buffer)) > 0) {
		        printf("Primos del primer esclavo: %s", buffer);
		    }

		    // Leer y procesar la salida del segundo esclavo
		    while (read(fd2[0], buffer, sizeof(buffer)) > 0) {
		        printf("Primos del segundo esclavo: %s", buffer);
		    }

			
			
		}
	}
	

	printf("Aqui acaba la secuencia de numeros primos en los intervalos [%d] [%d]\n",primer_intervalo,segundo_intervalo);
	return 0;
}

