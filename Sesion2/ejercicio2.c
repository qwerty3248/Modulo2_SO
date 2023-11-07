#include<sys/types.h>	
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants   
#include<sys/stat.h>
#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>


int main (int argc, char *argv[])
{

	if (argc != 3){
		perror("\nError numero de argumentos invalidos [programa] [path] [octal]\n");
		exit(EXIT_FAILURE);
	}
	
	//comprobar que el directorio existe, si existe te metes dentro
	int num_elementos = /*sacar elementos del directorio*/
	int aux= num_elementos;
	while (/*El numero de ficheros que sean reguales != 0*/){
		int fd; //se crea uno para cada archivo
		struct stat atributos;
		
		if ((fd=open("/*Archivo[aux-num_elementos]*/",O_CREAT | O_RDONLY,S_IRUSR)) < 0){
			printf("\nError %d en open\n",errno);
			perror("\nError en open\n");
			exit(EXIT_FAILURE);
		}
		
		if(stat("Archivo[aux-num_elementos]",&atributos) < 0) {
			printf("\nError al intentar acceder a los atributos de archivo1\n");
			perror("\nError en lstat\n");
			exit(EXIT_FAILURE);
		}

		
		if(chmod("Archivo[aux-num_elementos]",(atributos.st_mode & ~S_IXGRP) | S_ISGID) < 
		    0) 
		{
			perror("\nError en chmod para Archivo[aux-num_elementos]\n");
			exit(EXIT_FAILURE);
		}

	
	
	
	//si se pueden cambiar los permisos
		printf("<nombre_archivo> : <permisos_antiguos> <permisos_nuevos>\n");
	
	//en el caso contrario
		printf("<nombre_archivo> : <errno> <permisos_antiguos>\n");
	
	
		num_elementos--;
		
	}//final del programa
	
	return EXIT_SUCCESS;
}

