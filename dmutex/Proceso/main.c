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
int obtener_index(char ** IDs,char * ID,int length_process);
int main(int argc, char* argv[])
{
  int port;
  char line[80],proc[80];
  char **IDs;
  struct sockaddr_in * sockets_bind;
  int *ports;
  int *sockets;
  int socket_p;
  int leido;
	unsigned int tam_dir;
	struct sockaddr_in dir;
  int index_proceso;
  void *mensaje;
  int sscanf_success;
  int tam_mensaje;
  int TAM_MAXIMO=200;
  if(argc<2)
  {
    fprintf(stderr,"Uso: proceso <ID>\n");
    return 1;
  }
  setvbuf(stdout,(char*)malloc(sizeof(char)*80),_IOLBF,80);
  setvbuf(stdin,(char*)malloc(sizeof(char)*80),_IOLBF,80);
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
  struct process_d * p=(struct process_d*)malloc(sizeof(struct process_d));
  p->ID=(char *)malloc(sizeof(char));
  int l_p=0;
  char c =argv[1][0];
  while(c!='\000'){
    c=argv[1][l_p];
    p->ID[l_p]=c;
    l_p++;
    p->ID=(char *) realloc (p->ID, sizeof (char) * (l_p + 1));
  }
  fprintf(stdout,"%s: %d\n",p->ID,puerto_udp);
  IDs=(char **)malloc(TAM_MAXIMO*80);
  ports=(int*)malloc(TAM_MAXIMO*sizeof(int));
  sockets=(int*)malloc(TAM_MAXIMO*sizeof(int));
  sockets_bind=(struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
  int length_process=0;



  for(;fgets(line,80,stdin);/* length_process++ */)
  {
    if(!strcmp(line,"START\n"))
      break;

    sscanf_success=sscanf(line,"%[^:]: %d",proc,&port); //validar que no aumente length_process
    //printf("sscanf_success : %d\n",sscanf_success);
    if(sscanf_success==2){
      /* Habra que guardarlo en algun sitio */
      //
      //COPIAR PUERTO
      ports[length_process]=port;
      //ports=(int *)realloc(ports,(length_process+1)*sizeof(int));
      //sockets=(int *)realloc(sockets,(length_process+1)*sizeof(int));
      //COPIAR CADENA
      IDs[length_process]=(char*)malloc(sizeof(char));
      l_p=0;
      c=proc[0];
      while(c!='\000'){
        c=proc[l_p];
        IDs[length_process][l_p]=c;
        l_p++;
        IDs[length_process]=(char *) realloc (IDs[length_process], sizeof (char) * (l_p + 1));
        //IDs=(char **)realloc(IDs,sizeof(IDs)+sizeof(IDs[length_process]));
      }
      //
      //IDs=(char **)realloc(IDs,sizeof(IDs)+sizeof(IDs[length_process]));
      //
      //
      //

      if(!strcmp(proc,p->ID))
      { /* Este proceso soy yo */
        p->pid=length_process;
        sockets[length_process]=socket_p;
        sockets_bind[length_process].sin_addr.s_addr=dir.sin_addr.s_addr;
        sockets_bind[length_process].sin_port=dir.sin_port;
        sockets_bind[length_process].sin_family=dir.sin_family;
        
      }else{
        //
        //Socket s
        if ((sockets[length_process]=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
          perror("error creando socket");
          /*return 1;*/
        }
        sockets_bind[length_process].sin_addr.s_addr=INADDR_ANY;
        sockets_bind[length_process].sin_port=htons(ports[length_process]);
        sockets_bind[length_process].sin_family=PF_INET;
        
        if (bind(sockets[length_process], (struct sockaddr *)&sockets_bind[length_process], sizeof(sockets_bind[length_process])) < 0) {
          //perror("error en bind");
          //close(sockets[length_process]);
          /*return 1;*/
        }
        
      }

      sockets_bind=(struct sockaddr_in*)realloc(sockets_bind,(length_process+1)*sizeof(struct sockaddr_in));
      length_process++;
      strcpy(line,"");
      strcpy(proc,"");
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
    if(!strcmp(line,"FINISH\n"))
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
      tam_mensaje=sizeof(int)+((length_process)*sizeof(int));
      mensaje=malloc(tam_mensaje);
      if((leido=read(socket_p,mensaje,tam_mensaje))>0){
      printf("%s: RECEIVE(MSG,%s)\n",p->ID,IDs[((int*)mensaje)[0]]);
      /* Algoritmo de Vectores Logicos de Lamport */
      for(int i=0;i<length_process;i++){
        if(i==p->pid){
          if(((int*)mensaje)[(i+1)] > p->vector[i]){
            p->vector[i]=((int*)mensaje)[(i+1)];
            p->vector[i]++;
          }else{
            p->vector[i]++;
          }
        }else{
          if(((int*)mensaje)[(i+1)] > p->vector[i]){
            p->vector[i]=((int*)mensaje)[(i+1)];
          }
        }
      }
      printf("%s: TICK\n",p->ID);

      }
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
      index_proceso=obtener_index(IDs,proc,length_process);
      //printf("Puerto: %d\n",port);
      tam_dir=sizeof(sockets_bind[index_proceso]);
      tam_mensaje=sizeof(int)+(length_process)*sizeof(int);
      mensaje=malloc(tam_mensaje);
      memcpy(mensaje+sizeof(int),p->vector,(length_process)*sizeof(int));
      ((int*)mensaje)[0]=p->pid;
      sendto(socket_p, mensaje, tam_mensaje, 0,(struct sockaddr *)&sockets_bind[index_proceso], tam_dir);
      printf("%s: SEND(MSG,%s)\n",p->ID,proc);
      strcpy(line2,"");
      strcpy(proc,"");
    }


    
    

  }

  //Cerrar socket
  close(socket_p);

  return 0;
}
int obtener_index(char ** IDs,char * ID,int length_process){
  int index=0;
  for(int i=0;i<length_process;i++){
    if(!strcmp(ID,IDs[i])){
      index=i;
      break;
    }
  }
  return index;
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