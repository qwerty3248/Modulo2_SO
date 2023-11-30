#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
int main(int argc, char *argv[]){
	if(argc != 4){
		printf("\nSintaxis de ejecucion: ./ej1 <comand linux><\"|\"> <comand linux>\n\n");
		exit(EXIT_FAILURE);
	}
	char tuberia = argv[2][0];
	char *command1 = argv[1];
	char *command2 = argv[3];
	//para crear un proceso hijo
	int fd[2];
	pid_t PID[2];
	//le manda el contenido del archivo al comando
	if(tuberia=='|'){
		pipe(fd); // Llamada al sistema para crear un pipe
		if((PID[0] = fork())<0) {
			perror("\nError en el fork del primer hijo");
			exit(EXIT_FAILURE);
		}
		if (PID[0] == 0) { // hijo
			printf("HIJO \n");
			//Cerrar el descriptor de escritura en cauce situado
			//en el proceso padre
			close(fd[1]);
			//es decir, cerramos el flujo de entrada del sistema	
			//y usamos nuestro propio flujo
			close(STDIN_FILENO);//cierra la stdin verdadera
			//Duplicar el descriptor de lectura de cauce en el
			//descriptor correspondiente a la entrada estándar (stdin),
			//cerrado previamente en la misma operación
			//dup2(fd[0],STDIN_FILENO); // (NO ES NECESARIO PORQUE YA LO HACE FCNTL)
			//duplica el descriptor de archivos especificado por fd
			if(fcntl(fd[0],F_DUPFD,STDIN_FILENO) == -1){
				perror("\n Error fcntl SALIDA DE DATOS");
				exit(EXIT_FAILURE);
			}
			//ejecutamos el comando que nos da por parametro
			if(execlp(command2,command2,NULL)==-1){
				printf("Error al ejecutar el comando %s",command2);
				exit(EXIT_FAILURE);
			}
		}
		// Proceso padre porque PID != 0.
		printf("PADRE \n");
		//es decir, cerramos el flujo de salida del sistema usamos nuestro propio flujo
		close(STDOUT_FILENO);//cerramos el stdout verdadero
		//Cerrar el descriptor de lectura de cauce en el proceso hijo
		close(fd[0]);
		//Duplicar el descriptor de escritura en cauce en el descriptor
		//correspondiente a la salida estandr (stdout), cerrado previamente en
		//la misma operacion
		//dup2(fd[1],STDOUT_FILENO); // (NO ES NECESARIO PORQUE YA LO HACE FCNTL)
		if(fcntl(fd[1],F_DUPFD,STDOUT_FILENO) == -1){
			perror("\n Error fcntl ENTRADA DE DATOS");
			exit(EXIT_FAILURE);
		}
		//ejecutamos el comando que nos da por parametro
		if(execlp(command1, command1, NULL) == -1){
			printf("Error al ejecutar el comando %s",command1);
			exit(EXIT_FAILURE);
		}
	}else{
		printf("Error el segundo parametro tiene que ser (\"|\") no: %c\n",tuberia);
	}
	
	return EXIT_SUCCESS;
}


