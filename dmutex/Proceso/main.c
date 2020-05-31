/* DMUTEX (2009) Sistemas Operativos Distribuidos
 * Codigo de Apoyo
 *
 * ESTE CODIGO DEBE COMPLETARLO EL ALUMNO:
 *    - Para desarrollar las funciones de mensajes, reloj y
 *      gestion del bucle de tareas se recomienda la implementacion
 *      de las mismas en diferentes ficheros.
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int puerto_udp;
struct process_d{
  char* ID;
  int  pid;
  int * vector;
};
int main(int argc, char* argv[])
{
  int port;
  char line[80],proc[80];
  //
  int socket_p;
	unsigned int tam_dir;
	struct sockaddr_in dir;

  if(argc<2)
  {
    fprintf(stderr,"Uso: proceso <ID>\n");
    return 1;
  }

  /* Establece el modo buffer de entrada/salida a linea */
  setvbuf(stdout,(char*)malloc(sizeof(char)*80),_IOLBF,80);
  setvbuf(stdin,(char*)malloc(sizeof(char)*80),_IOLBF,80);
  //
  //
  //DEFINIR EL SOCKET UDP
	if ((socket_p=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("error creando socket");
		return 1;
	}
	dir.sin_addr.s_addr=INADDR_ANY;
	dir.sin_port=0;
	dir.sin_family=PF_INET;
	if (bind(socket_p, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error en bind");
		close(socket_p);
		return 1;
	}
	
	tam_dir = sizeof(dir);
	if (!(getsockname(socket_p, (struct sockaddr *)&dir, &tam_dir) == -1)){
		puerto_udp=ntohs(dir.sin_port);

	}
  //
  //puerto_udp=1111; 
  /* Se determina el puerto UDP que corresponda.
                      Dicho puerto debe estar libre y quedara
                      reservado por dicho proceso. */

  fprintf(stdout,"%s: %d\n",argv[1],puerto_udp);

  //Leer procesos
  struct process_d * p=(struct process_d*)malloc(sizeof(struct process_d));
  strcpy(p->ID,argv[1]);

  int length_process=0;

  for(;fgets(line,80,stdin);length_process++)
  {
    if(!strcmp(line,"START\n"))
      break;

    sscanf(line,"%[^:]: %d",proc,&port);
    /* Habra que guardarlo en algun sitio */

    if(!strcmp(proc,argv[1]))
    { /* Este proceso soy yo */
      p->pid=length_process;
    }
  }
  printf("Procesos %d\n",length_process);

  /* Inicializar Reloj */
  p->vector=(int*)malloc(length_process*sizeof(int));
  for(int i=0;i<length_process;i++){
    p->vector[i]=0;
  }

  /* Procesar Acciones */
  close(socket_p);

  return 0;
}
