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
void imprimir_arreglo_ids(char ** IDs,int length_process);
int obtener_port(int *ports,char ** IDs,char * ID,int length_process);
int main(int argc, char* argv[])
{
  int port;
  char line[80],proc[80];
  //
  char **IDs;
  int *ports;
  //
  int socket_p;
	unsigned int tam_dir;
	struct sockaddr_in dir;
	unsigned int tam_dir_otro;
  struct sockaddr_in dir_otro;
  //

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
  //
  //Inicializar struct
  struct process_d * p=(struct process_d*)malloc(sizeof(struct process_d));
  //Copiar valor
  //
  p->ID=(char *)malloc(sizeof(char));
  int l_p=0;
  char c =argv[1][0];
  while(c!='\000'){
    c=argv[1][l_p];
    p->ID[l_p]=c;
    l_p++;
    p->ID=(char *) realloc (p->ID, sizeof (char) * (l_p + 1));
  }
  //
  //

  fprintf(stdout,"%s: %d\n",p->ID,puerto_udp);

  //
  //ARREGLO DE IDS de los demas procesos
  IDs=(char **)malloc(sizeof(char*));
  //ARREGLO DE puertos udp de los demas procesos
  ports=(int*)malloc(sizeof(int));
  // longitud de tamanio de n de procesos
  int length_process=0;



  for(;fgets(line,80,stdin);length_process++)
  {
    if(!strcmp(line,"START\n"))
      break;

    sscanf(line,"%[^:]: %d",proc,&port);
    /* Habra que guardarlo en algun sitio */
    //
    //COPIAR PUERTO
    ports[length_process]=port;
    ports=(int *)realloc(ports,(length_process+1)*sizeof(int));
    //
    //COPIAR CADENA
    IDs[length_process]=(char*)malloc(sizeof(char));
    l_p=0;
    c=proc[0];
    while(c!='\000'){
      c=proc[l_p];
      IDs[length_process][l_p]=c;
      l_p++;
      IDs[length_process]=(char *) realloc (IDs[length_process], sizeof (char) * (l_p + 1));
    }
    //
    IDs=(char **)realloc(IDs,(length_process+1)*sizeof(char*));
    //
    //
    //

    if(!strcmp(proc,p->ID))
    { /* Este proceso soy yo */
      p->pid=length_process;
    }
  }
  //printf("Procesos %d\n",length_process);
  //imprimir_arreglo_ids(p,length_process);

  /* Inicializar Reloj */
  p->vector=(int*)malloc(length_process*sizeof(int));
  for(int i=0;i<length_process;i++){
    p->vector[i]=0;
  }

  //
  //
  //
  //
  char line2[80];
  /* Procesar Acciones */
  //
  for(;fgets(line,80,stdin);)
  {
    if(!strcmp(line,"EXIT\n"))
      break;

    //GETCLOCK
    if(!strcmp(line,"GETCLOCK\n")){
        printf("%s: ",p->ID);
        printf("LC[");
        for(int p_i=0;p_i<length_process;p_i++){
          if(p_i==length_process-1){
            printf("%d",p->vector[p_i]);
          }else{
            printf("%d,",p->vector[p_i]);
          }
        }
        printf("]\n");
    }
    //RECEIVE
    if(!strcmp(line,"RECEIVE\n")){
      p->vector[p->pid]+=1;
      printf("%s: TICK\n",p->ID);
    }
    //EVENT
    if(!strcmp(line,"EVENT\n")){
      p->vector[p->pid]+=1;
      printf("%s: TICK\n",p->ID);
    }
    //
    //

    sscanf(line,"%s %s",line2,proc);

    //MESSAGETO
    if(!strcmp(line2,"MESSAGETO")){
      p->vector[p->pid]+=1;
      printf("%s: TICK\n",p->ID);
      port=obtener_port(ports,IDs,proc,length_process);
      printf("Puerto: %d\n",port);
      tam_dir_otro=sizeof(dir_otro);
      //sendto(s, p, sizeof(p), 0,(struct sockaddr *)&dir_otro, tam_dir_otro);
      //printf("%s: SEND(MSG,%s)\n",p->ID,proc);
      //Limpiar buffer s
      //strcpy(line,"");
      strcpy(line2,"");
      strcpy(proc,"");
    }


    
    

  }

  //Cerrar socket
  close(socket_p);

  return 0;
}
int obtener_port(int * ports,char ** IDs,char * ID,int length_process){
  int index=0;
  int port;
  for(int i=0;i<length_process;i++){
    if(!strcmp(ID,IDs[i])){
      index=i;
      break;
    }
  }
  port=ports[index];
  return port;
}

void imprimir_arreglo_ids(char ** IDs,int length_process){
  printf("[");
  for(int i=0;i<length_process;i++){
    if(i==length_process-1){
      printf("%s",IDs[i]);
    }else{
      printf("%s, ",IDs[i]);
    }
  }
  printf("]\n");
}
