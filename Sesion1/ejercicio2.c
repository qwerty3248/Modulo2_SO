#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	/* Primitive system data types for abstraction\
			   of implementation-dependent data types.
			   POSIX Standard: 2.6 Primitive System Data Types
			   <sys/types.h>
			*/
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

int main (int argc, char *argv[])
{
	int fd,fd1;
	if (argc != 2){
		perror("\nError, numero de argumentos mal [nombre pro] [archivo lectura]");
		exit(-1);
	
	}
	
	if ((fd=open("salida.txt",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR)) < 0){
		printf("\nError %d en open",errno);
		perror("\nError en open");
		exit(EXIT_FAILURE);
	
	}
	if ((fd1=open(argv[1],O_CREAT,S_IRUSR)) < 0){
		printf("\nError %d en open",errno);
		perror("\nError en open");
		exit(EXIT_FAILURE);
	
	}
	int cnt = 0;
	char buf[80];
	int aux;//para el numero de bytes que lee es decir que pone en buf
	//movemos una linea el fd
	if (lseek(fd,33,SEEK_SET) < 0){
		perror("\nError al mover el fd");
		exit(EXIT_FAILURE);
	
	}
	while ((aux = read(fd1,buf,80)) != 0){
		cnt++;
		dprintf(fd,"\nBloque %d \n",cnt);//se lo pasa al fd el mensaje
						 // que quieras
		if (write(fd,buf,aux) != aux){
			perror("\nError al escribir\n");
			exit(EXIT_FAILURE);
		}
			
		
	}
	//una vez copiado todo debemos poner el fd al principio
	if (lseek(fd,0,SEEK_SET) < 0){
		perror("\nError al mover el fd");
		exit(EXIT_FAILURE);
	
	}
	dprintf(fd,"El tamaño de bloques total es: %d\n",cnt);
	
	
	

	close(fd);
	close(fd1);
	return EXIT_SUCCESS;
}



