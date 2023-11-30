#include<stdlib.h>
#include<errno.h>
#include<sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	if(argc != 4){
		printf("\nSintaxis de ejecucion:<comando linux><\"<\" o \">\"> <archivo>\n\n");
	exit(EXIT_FAILURE);
}
	char min_max = argv[2][0];
	char *command = argv[1];
	char *path_file = argv[3];
	int fd;
	//le manda el contenido del archivo al comando
	if(min_max=='<'){
		//abre el archivo solo lectura
		if((fd = open(path_file, O_RDONLY))<0){
			perror("\nError en open");
			exit(EXIT_FAILURE);
		}
		//es decir, cerramos el flujo de entrada del sistema y usamos
		//nuestro propio flujo
		close(STDIN_FILENO);//cierra la stdin verdadera
		//duplica el descriptor de archivos especificado por fd
		if(fcntl(fd,F_DUPFD,STDIN_FILENO) == -1){
			perror("\n Error fcntl SALIDA DE DATOS");
			exit(EXIT_FAILURE);
		}
		//ejecutamos el comando que nos da por parametro
		if(execlp(command,command,NULL)==-1){
			printf("Error al ejecutar el comando %s",command);
			exit(EXIT_FAILURE);
		}
		//mando el resultado del comando al archivos ej: (ls >
		//temporal)
	}else if(min_max=='>'){
		//abro el archivo o creo el archivo con los permisos de
		//lectura y escritura en usuario
		//y le indico que voy a escribir en ese archivo
		if((fd=open(path_file,O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0){
			perror("\nError en open");
			exit(EXIT_FAILURE);
		}
		//es decir, cerramos el flujo de salida del sistema y
		//usamos nuestro propio flujo
		close(STDOUT_FILENO);//cerramos el stdout verdadero
		if(fcntl(fd,F_DUPFD,STDOUT_FILENO) == -1){
			perror("\n Error fcntl ENTRADA DE DATOS");
			exit(EXIT_FAILURE);
		}
		//ejecutamos el comando que nos da por parametro
		if(execlp(command, command, NULL) == -1){
			printf("Error al ejecutar el comando %s",command);
			exit(EXIT_FAILURE);
		}
	}else{
		printf("Error el segundo parametro tiene que ser (> o <)no: %c\n",min_max);
	}
	return EXIT_SUCCESS;
}


