#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
int main(int argc, char *argv[]){
	struct stat atributos;
	int origen, destino, size;
	char *memorigen, *memdestino;
	if(argc != 3){
		printf("Modo de ejecucion: %s origen destino\n\n",
		argv[0]);
		exit(1);
	}else{
		origen=open(argv[1], O_RDONLY);
	}
	if(fstat(origen, &atributos) == -1){
		printf("Error stat en el fichero %s\n", argv[1]);
		return 1;
	}
	if (!S_ISREG(atributos.st_mode)){
		printf ("El fichero de origen no es un archivo regular\n");
		return 1;
	}
	size=atributos.st_size;
	umask(0);
	destino=open(argv[2], O_CREAT|O_RDWR|O_EXCL, S_IRUSR|S_IWUSR);
	ftruncate(destino, size);
	memorigen=(char *) mmap(0, size, PROT_READ, MAP_SHARED, origen, 0);
	if(memorigen == MAP_FAILED){
		perror("Fallo mapeando el archivo de entrada");
		exit(2);
	}
	memdestino=(char *) mmap(0, size, PROT_WRITE, MAP_SHARED,
	destino, 0);
	if(memdestino == MAP_FAILED){
		perror("Fallo mapeando el archivo de salida");
		exit(2);
	}
	close(origen);
	close(destino);
	memcpy(memdestino, memorigen, size);
	munmap(memorigen, size);
	munmap(memdestino, size);
}
