#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char *argv[]){
	//Declaracion de cerrojos
	struct flock cerrojo1;
	struct flock cerrojo2;
	//Declaracion descriptores
	int fd1;
	int fd2;
	//Declaracion del pid;
	pid_t pid;
	//*--------------------------------------Lo primero es establecer los cerrojos
	//Esta es la estructura estandar.
	//Cerrojos para fd1
	//Type--> tipo de cerrojo
	cerrojo1.l_type=F_WRLCK;
	//Cerrojo de escritura el de lectura es F_RDLCK y F_UNLCK34:
	//Interpretar l_start: Seek_set, seek_curr, seek_end
	cerrojo1.l_whence=SEEK_SET;
	//Donde se inicia el bloqueo
	cerrojo1.l_start=0;
	//Numero de bytes bloqueados 0 significa hasta eof
	cerrojo1.l_len=0;
	//Cerrojos para fd2
	cerrojo2.l_type=F_WRLCK;
	cerrojo2.l_whence=SEEK_SET;
	cerrojo2.l_start=0;
	cerrojo2.l_len=0;
	//cerrojo2.l_pid --> previene nuestro bloqueo = F_GETLINK
	//Abrimos el recurso1
	if((fd1=open("recurso_1",O_RDWR|O_CREAT|O_TRUNC,0664)) == -1){
	perror("open recurso1 ha fallado");
	}
	printf("Recurso1 abierto\n");
	//Bloqueamos el recurso_1 para que solo este proceso pueda manejarlo
	fcntl(fd1, F_SETLK, &cerrojo1);
	printf("RERCURSO1 BLOQUEADO \n");
	//Creamos un hijo que necesite el recurso1
	if((pid=fork())<0){
		perror("\nError en el fork");
		exit(-1);
	}else if(pid==0){ //proceso hijo ejecutando el programa
		printf("Entrando en el padre y durmiendo\n");
		while(1){}
		printf("Abriendo el recurso2\n");
		sleep(1);
		if ((fd2 = open("recurso_2", O_RDWR, 0664)) == -1 ){
			perror("open recurso 2fallo");
		}
		printf("Recurso2 abierto\n");
		if (write(fd2, "Hola Mundo\n", strlen("Hola Mundo\n")) == -1)
		{
			perror("fallo en write recurso2");
			exit(-1);
		}
		return(0);
	}else{//(if pid != 0) proceso padre ejecutando el programa
		//El hijo abre el recurso2 y lo bloquea
		printf("Entrando en el hijo\n");
		//Abrimos recurso1
		if ((fd1 = open("recurso_1", O_RDWR | O_CREAT | O_TRUNC,0664)) == -1 )
			perror("open recurso1 fallo");
		//Abrimos recurso2
		if ((fd2 = open("recurso_2", O_RDWR | O_CREAT | O_TRUNC,0664)) == -1 )
			perror("open recurso2 fallo");
			//Bloqueamos recurso2
			fcntl(fd2, F_SETLK, &cerrojo2);
			printf("RECURSO2 BLOQUEADO\n");
			while(1){}
			return(0);
	}
}
