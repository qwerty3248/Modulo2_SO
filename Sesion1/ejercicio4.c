#include<unistd.h>  /* POSIX Standard: 2.10 Symbolic Constants <unistd.h>
		     */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  /* Primitive system data types for abstraction	\
			   of implementation-dependent data types.
			   POSIX Standard: 2.6 Primitive System Data Types
			   <sys/types.h>
		       */
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
//la macro para ver si es regular
/*Explicacion de la macro
Bacicamente cogemos el "modo" que es lo q va a definir que es nuestro archivo,
un fichero regular, un enlace... Y le aplicamos una mascara de bits para archivos
esta mascara lo que nos va a devolver es unos bits en especifico que dicen de que 
tipo es el archivo es decir en este caso sy al aplicar la mascara nos da los mismos
bits que el de un archivo regular, == , devuelve true.
*/
#define S_IREG2(mode) (((mode) & S_IFMT) == S_IFREG) 

int main (int argc, char *argv[])
{
	
	if(argc != 2){
		printf("\nError numero de argumentos invalidos [programa] [archivo]\n");
		exit(EXIT_FAILURE);
	}
	
	struct stat fichero;//donde va a poner lstat o stat la informacion del fichero
	char regular[50];
	printf("%s: ",argv[1]);
	if (lstat(argv[1], &fichero) < 0 ){
		printf("\nError al intentar acceder a los atributos de %s",argv[1]);
		perror("\nError en lstat");
	}
	//si no da error ya tenemos la informacion en el struct
	if (S_IREG2(fichero.st_mode)){
	
		strcpy(regular,"Regular\n");
		
	}else{
	
		strcpy(regular,"NO Regular\n");
	
	}
	printf("%s",regular);
	
	
	return EXIT_SUCCESS;

}



