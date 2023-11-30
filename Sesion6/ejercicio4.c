#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
int main (int argc, char * argv[])
{
	struct flock cerrojo;
	int fd;
	if ((fd=open("./cerrojo_programa", O_RDWR)) == -1 ){
		printf("Error de archivo");
		exit(-1);
	}
	cerrojo.l_type=F_WRLCK;
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;
	/* intentamos un bloqueo de escritura del archivo completo */
	if(fcntl (fd, F_SETLK, &cerrojo) == 0) {
		while(1){}
	}else{
		printf("Instancia ejecutandose");
		exit(-1);
	}
}
