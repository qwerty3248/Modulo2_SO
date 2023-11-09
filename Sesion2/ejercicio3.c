#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


int main (int argc, char *argv[])
{

	if(argc != 2){
		perror("Error: Parametros incorrectos pruebe con: [programa] [pathname]\n");
		exit(EXIT_FAILURE);
	}
	
	DIR *directorio = opendir(argv[1]);
	
	if (directorio == NULL){
	
		perror("Error: No se puede abrir el directorio\n");
		exit(EXIT_FAILURE);
	}
	
	struct dirent *archivo;
	const char *direc = argv[1];
	const int tam_direc = strlen(argv[1]);
	const mode_t permisos_tener = 011;
	int num_archivos = 0;
	long num_bytes = 0;
	printf("Los i-nodos son:\n");
	while ((archivo = readdir(directorio)) != NULL){
		
		char *path;
		char *name = archivo->d_name;

		// Excluir . y ..
		if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
		    continue;
		}

		path = malloc(tam_direc + strlen(name) + 1);
		strcpy(path, direc);
		strcat(path + tam_direc, name);
		
		//ya tenemos el path del archivo, debe de tener la barra
        	
		struct stat atributos;
		if (stat(path, &atributos) < 0) {
		    perror("\nERROR: al acceder a los atributos del archivo\n");
		    printf("Archivo: %s\n", path);
		    exit(EXIT_FAILURE);
		}
		
		const mode_t permisos_archivo = atributos.st_mode & 0777;
		// si la interseccion es distinto de 0 tiene los permisos 
		//si no no los tiene
     		if ((permisos_archivo & permisos_tener) != 0){
     			printf("---%s---\n",path);
     			num_archivos++;
     			num_bytes += archivo->d_ino;
     			
     		
     		}
		
		free(path);
	}
	
	printf("Existen %d archivos regulares con permiso x para grupo y otros\n",num_archivos);
	printf("El tamaño total ocupado por dichos archivos es %ld bytes\n",num_bytes);


	closedir(directorio);
	return (EXIT_SUCCESS);

	
}
