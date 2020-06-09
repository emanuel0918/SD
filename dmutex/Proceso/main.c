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
int seccionCriticaDisponible(char **seccionesCriticas,char* nombreSeccionCritica,int length_seccionesCriticas);
void borrar_elemento(char ***IDs,char*ID,int *length_process,int MAX_P);
char* substr (const char* cadena, int offset, int l, char* cadena2);
int main(int argc, char* argv[])
{
  int port;
  char line[80],proc[80];
  char *receive_msg;
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
  int TAM_MAXIMO=25;
  int TAM_MANDATO=10;
  int MAX_P=200;
  int n_sc;
  int * numero_OKs;
  //char nombreSeccionCritica[TAM_MAXIMO];
  //char * miSeccionCritica;
  char **misSeccionesCriticas;
  int m_sc;
  char *nombreSeccionCritica;
  char *mandato_send;
  char **seccionesCriticas;
  int * laSeccionCriticaEsDisponible;
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
  IDs=(char **)malloc(MAX_P*80);
  seccionesCriticas=(char **)malloc(MAX_P*80);
  misSeccionesCriticas=(char **)malloc(MAX_P*80);
  ports=(int*)malloc(MAX_P*sizeof(int));
  sockets=(int*)malloc(MAX_P*sizeof(int));
  numero_OKs=(int*)malloc(MAX_P*sizeof(int));
  
  sockets_bind=(struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
  int length_process=0;
  n_sc=0;
  m_sc=0;



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

  receive_msg=(char*)malloc(sizeof(char)*TAM_MANDATO);


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
      /*
      for(int i=0;i<TAM_MAXIMO;i++){
        nombreSeccionCritica[i]='\0';
      }
      */
      tam_mensaje=sizeof(int)
      +(length_process)*sizeof(int)
      +sizeof(char) * (TAM_MANDATO+1)
      +sizeof(char)*(TAM_MAXIMO+1);
      mensaje=malloc(tam_mensaje);
      //if((leido=recv(socket_p,mensaje,tam_mensaje,MSG_WAITALL))>0)
      if((leido=read(socket_p,mensaje,tam_mensaje))>0){
        /*
        for(int i=0;i<TAM_MAXIMO;i++){
          nombreSeccionCritica[i]=(char)((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+i];
        }
        */
        mandato_send=(char*)malloc(sizeof(char)*(TAM_MANDATO+1));
        strcpy(mandato_send,((char*)mensaje)+sizeof(int)+(length_process)*sizeof(int));
        nombreSeccionCritica=(char*)malloc(sizeof(char)*(TAM_MAXIMO+1));
        strcpy(nombreSeccionCritica,((char*)mensaje)+sizeof(int)+(length_process)*sizeof(int)+(sizeof(char)*(TAM_MANDATO+1)));
        //
        //OK
        //RECEIVE(MSG,)

        substr(mandato_send,0,3,receive_msg);
        if(!strcmp(receive_msg,"MSG")){
          printf("%s: RECEIVE(%s,%s)\n",p->ID,mandato_send,IDs[((int*)mensaje)[0]]);
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
        //
        //
        //RECEIVE(OK,)
        substr(mandato_send,0,2,receive_msg);
        if(!strcmp(receive_msg,"OK")){
          printf("%s: RECEIVE(%s,%s)\n",p->ID,mandato_send,IDs[((int*)mensaje)[0]]);
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
          p->vector[p->pid]+=1;
          //int obtener_mutex=get_mutex(nombreSeccionCritica,seccionesCriticas,length_seccionesCriticas,length_process);
          //SI TODOS YA ME ENVIARON SU OK
          //MUTEX(nombreSeccionCritica)
          int index_seccion_critica_mia=obtener_index(misSeccionesCriticas,nombreSeccionCritica,m_sc);
          numero_OKs[index_seccion_critica_mia]++;
          if(numero_OKs[index_seccion_critica_mia]==length_process-1){
            printf("%s: MUTEX(%s)\n",p->ID,misSeccionesCriticas[index_seccion_critica_mia]);

          }
        }
        //RECEIVE(UNLOCK,)
        substr(mandato_send,0,6,receive_msg);
        if(!strcmp(receive_msg,"UNLOCK")){
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
          p->vector[p->pid]+=1;
          
          int p_id_ok=((int*)mensaje)[0];
          printf("%s: RECEIVE(%s,%s)\n",p->ID,mandato_send,IDs[p_id_ok]);
          int existe_el_mutex=seccionCriticaDisponible(seccionesCriticas,nombreSeccionCritica,n_sc);
          if(existe_el_mutex==0){
            
            //borrar
            borrar_elemento(&seccionesCriticas,nombreSeccionCritica,&n_sc,MAX_P);
            //
            //
            //
            //
            strcpy(line2,"");
            strcpy(proc,"");
          }
        }
        //RECEIVE(LOCK,)
        substr(mandato_send,0,4,receive_msg);
        if(!strcmp(receive_msg,"LOCK")){
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
          //HABRIA QUE COMPARAR SI ESTA DISPONIBLE
          if(seccionCriticaDisponible(seccionesCriticas,nombreSeccionCritica,n_sc)==1){
            int p_id_ok=((int*)mensaje)[0];
            printf("%s: RECEIVE(%s,%s)\n",p->ID,mandato_send,IDs[p_id_ok]);
            //AGREGARLA A LAS YA storeds
            seccionesCriticas[n_sc]=(char*)malloc(sizeof(char)*(TAM_MAXIMO+1));
            strcpy(seccionesCriticas[n_sc],nombreSeccionCritica);
            n_sc++;
            //
            //
            //
            
            //
            
            //
            mandato_send=(char*)malloc(sizeof(char)*(TAM_MANDATO+1));
            strcpy(mandato_send,"OK\0");
            index_proceso=p_id_ok;
            //printf("Puerto: %d\n",port);
            //LOCK SEND OK
            p->vector[p->pid]+=1;
            printf("%s: TICK\n",p->ID);
            p->vector[p->pid]+=1;
            printf("%s: TICK\n",p->ID);
            tam_dir=sizeof(sockets_bind[index_proceso]);
            tam_mensaje=sizeof(int)
            +(length_process)*sizeof(int)
            +sizeof(char)*(TAM_MANDATO+1)
            +sizeof(char)*(TAM_MAXIMO+1);
            /*+sizeof(int) * (TAM_MAXIMO+1)*/
            mensaje=malloc(tam_mensaje);
            memcpy(mensaje+sizeof(int),p->vector,(length_process)*sizeof(int));
            strcpy(mensaje + sizeof(int)+(length_process)*sizeof(int), mandato_send);
            strcpy(mensaje + sizeof(int)+(length_process)*sizeof(int)+(sizeof(char)*(TAM_MANDATO+1)),nombreSeccionCritica);
            /*
            for(int i=0;i<TAM_MAXIMO;i++){
              ((int*)mensaje)[sizeof(int)
            +(length_process)*sizeof(int)+i]='\0';
            }
            ((int*)mensaje)[sizeof(int)
            +(length_process)*sizeof(int)+0]='M';
            ((int*)mensaje)[sizeof(int)
            +(length_process)*sizeof(int)+1]='S';
            ((int*)mensaje)[sizeof(int)
            +(length_process)*sizeof(int)+2]='G';
            */
            ((int*)mensaje)[0]=p->pid;
            sendto(socket_p, mensaje, tam_mensaje, 0,(struct sockaddr *)&sockets_bind[index_proceso], tam_dir);
            printf("%s: SEND(%s,%s)\n",p->ID,mandato_send,IDs[p_id_ok]);
            strcpy(line2,"");
            strcpy(proc,"");
          }
        }

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
      /*
      for(int i=0;i<TAM_MAXIMO;i++){
        nombreSeccionCritica[i]='\0';
      }
      nombreSeccionCritica[0]='M';
      nombreSeccionCritica[1]='S';
      nombreSeccionCritica[2]='G';
      */
      //COPIAR CADENA
      nombreSeccionCritica=(char*)malloc(sizeof(char)*(TAM_MAXIMO+1));
      l_p=0;
      c=proc[0];
      while(c!='\000'){
        c=proc[l_p];
        nombreSeccionCritica[l_p]=c;
        l_p++;
        //nombreSeccionCritica=(char *) realloc (nombreSeccionCritica, sizeof (char) * (l_p + 1));
        //
      }
      for(;l_p<TAM_MAXIMO+1;l_p++){
        nombreSeccionCritica[l_p]='\0';
      }
      //
      //
      mandato_send=(char*)malloc(sizeof(char)*(TAM_MANDATO+1));
      strcpy(mandato_send,"MSG\0");
      index_proceso=obtener_index(IDs,nombreSeccionCritica,length_process);
      //printf("Puerto: %d\n",port);
      tam_dir=sizeof(sockets_bind[index_proceso]);
      tam_mensaje=sizeof(int)
      +(length_process)*sizeof(int)
      +sizeof(char)*(TAM_MANDATO+1)
      +sizeof(char)*(TAM_MAXIMO+1);
      /*+sizeof(int) * (TAM_MAXIMO+1)*/
      mensaje=malloc(tam_mensaje);
      memcpy(mensaje+sizeof(int),p->vector,(length_process)*sizeof(int));
      strcpy(mensaje + sizeof(int)+(length_process)*sizeof(int), mandato_send);
      strcpy(mensaje + sizeof(int)+(length_process)*sizeof(int)+(sizeof(char)*(TAM_MANDATO+1)), p->ID);
      /*
      for(int i=0;i<TAM_MAXIMO;i++){
        ((int*)mensaje)[sizeof(int)
      +(length_process)*sizeof(int)+i]='\0';
      }
      ((int*)mensaje)[sizeof(int)
      +(length_process)*sizeof(int)+0]='M';
      ((int*)mensaje)[sizeof(int)
      +(length_process)*sizeof(int)+1]='S';
      ((int*)mensaje)[sizeof(int)
      +(length_process)*sizeof(int)+2]='G';
      */
      ((int*)mensaje)[0]=p->pid;
      sendto(socket_p, mensaje, tam_mensaje, 0,(struct sockaddr *)&sockets_bind[index_proceso], tam_dir);
      printf("%s: SEND(%s,%s)\n",p->ID,mandato_send,nombreSeccionCritica);
      strcpy(line2,"");
      strcpy(proc,"");
    }

    //LOCK
    if(!strcmp(line2,"LOCK")){
      /*
      for(int i=0;i<TAM_MAXIMO;i++){
        nombreSeccionCritica[i]='\0';
      }
      nombreSeccionCritica[0]='M';
      nombreSeccionCritica[1]='S';
      nombreSeccionCritica[2]='G';
      */
      //COPIAR CADENA
      misSeccionesCriticas[m_sc]=(char*)malloc(sizeof(char)*(TAM_MAXIMO+1));
      l_p=0;
      c=proc[0];
      while(c!='\000'){
        c=proc[l_p];
        misSeccionesCriticas[m_sc][l_p]=c;
        l_p++;
        //misSeccionesCriticas[m_sc]=(char *) realloc (misSeccionesCriticas[m_sc], sizeof (char) * (l_p + 1));
        //
      }
      for(;l_p<TAM_MAXIMO+1;l_p++){
        misSeccionesCriticas[m_sc][l_p]='\0';
      }
      //
      if(seccionCriticaDisponible(seccionesCriticas,misSeccionesCriticas[m_sc],n_sc)==1){
        //seccionesCriticas[n_sc]=(char*)malloc(sizeof(char)*(TAM_MAXIMO+1));
        //strcpy(seccionesCriticas[n_sc],misSeccionesCriticas[m_sc]);
        //n_sc++;
        numero_OKs[m_sc]=0;
        //
        mandato_send=(char*)malloc(sizeof(char)*(TAM_MANDATO+1));
        strcpy(mandato_send,"LOCK\0");
        //index_proceso=obtener_index(IDs,nombreSeccionCritica,length_process);
        //printf("Puerto: %d\n",port);
        printf("%s: TICK\n",p->ID);
        p->vector[p->pid]+=1;
        tam_mensaje=sizeof(int)
        +(length_process)*sizeof(int)
        +sizeof(char)*(TAM_MANDATO+1)
        +sizeof(char)*(TAM_MAXIMO+1);
        /*+sizeof(int) * (TAM_MAXIMO+1)*/
        mensaje=malloc(tam_mensaje);
        memcpy(mensaje+sizeof(int),p->vector,(length_process)*sizeof(int));
        strcpy(mensaje + sizeof(int)+(length_process)*sizeof(int), mandato_send);
        strcpy(mensaje + sizeof(int)+(length_process)*sizeof(int)+(sizeof(char)*(TAM_MANDATO+1)),misSeccionesCriticas[(m_sc)]);

        m_sc++;
        
        /*
        for(int i=0;i<TAM_MAXIMO;i++){
          ((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+i]='\0';
        }
        ((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+0]='M';
        ((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+1]='S';
        ((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+2]='G';
        */
        ((int*)mensaje)[0]=p->pid;
        for(int i=0;i<length_process;i++){
          if(i!=p->pid){
            tam_dir=sizeof(sockets_bind[i]);
            sendto(socket_p, mensaje, tam_mensaje, 0,(struct sockaddr *)&sockets_bind[i], tam_dir);
            printf("%s: SEND(%s,%s)\n",p->ID,mandato_send,IDs[i]);
            
          }
        }
        strcpy(line2,"");
        strcpy(proc,"");
      }
    }




    //UNLOCK
    if(!strcmp(line2,"UNLOCK")){
      /*
      for(int i=0;i<TAM_MAXIMO;i++){
        nombreSeccionCritica[i]='\0';
      }
      nombreSeccionCritica[0]='M';
      nombreSeccionCritica[1]='S';
      nombreSeccionCritica[2]='G';
      */
      //COPIAR CADENA
      misSeccionesCriticas[m_sc]=(char*)malloc(sizeof(char)*(TAM_MAXIMO+1));
      l_p=0;
      c=proc[0];
      while(c!='\000'){
        c=proc[l_p];
        misSeccionesCriticas[m_sc][l_p]=c;
        l_p++;
        //misSeccionesCriticas[m_sc]=(char *) realloc (misSeccionesCriticas[m_sc], sizeof (char) * (l_p + 1));
        //
      }
      for(;l_p<TAM_MAXIMO+1;l_p++){
        misSeccionesCriticas[m_sc][l_p]='\0';
      }
      //
      if(seccionCriticaDisponible(misSeccionesCriticas,misSeccionesCriticas[m_sc],m_sc)==0){
        if(numero_OKs[m_sc]==length_process-1){
          int index_mutex_free=obtener_index(seccionesCriticas,nombreSeccionCritica,n_sc);
            //borrar numero_OKs
            if(index_mutex_free!=n_sc-1){
            int i_OK=0;
            int j_OK=0;
            int * nuevo_vector_OKs=(int*)malloc(MAX_P*sizeof(int));
            for(;j_OK<n_sc;){
              if(i_OK==index_mutex_free){
                j_OK++;
              }
              nuevo_vector_OKs[i_OK]=numero_OKs[j_OK];
              i_OK++;
              j_OK++;
            }
            free(nuevo_vector_OKs);
            numero_OKs=nuevo_vector_OKs;
            free(nuevo_vector_OKs);
            }else{
              numero_OKs[index_mutex_free]=0;
            }
        m_sc++;
        //
        mandato_send=(char*)malloc(sizeof(char)*(TAM_MANDATO+1));
        strcpy(mandato_send,"UNLOCK\0");
        //index_proceso=obtener_index(IDs,nombreSeccionCritica,length_process);
        //printf("Puerto: %d\n",port);
        tam_mensaje=sizeof(int)
        +(length_process)*sizeof(int)
        +sizeof(char)*(TAM_MANDATO+1)
        +sizeof(char)*(TAM_MAXIMO+1);
        /*+sizeof(int) * (TAM_MAXIMO+1)*/
        mensaje=malloc(tam_mensaje);
        memcpy(mensaje+sizeof(int),p->vector,(length_process)*sizeof(int));
        strcpy(mensaje + sizeof(int)+(length_process)*sizeof(int), mandato_send);
        strcpy(mensaje + sizeof(int)+(length_process)*sizeof(int)+(sizeof(char)*(TAM_MANDATO+1)),misSeccionesCriticas[(m_sc)]);
        
        /*
        for(int i=0;i<TAM_MAXIMO;i++){
          ((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+i]='\0';
        }
        ((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+0]='M';
        ((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+1]='S';
        ((int*)mensaje)[sizeof(int)
        +(length_process)*sizeof(int)+2]='G';
        */
        ((int*)mensaje)[0]=p->pid;
        for(int i=0;i<length_process;i++){
          if(i!=p->pid){
            tam_dir=sizeof(sockets_bind[i]);
            sendto(socket_p, mensaje, tam_mensaje, 0,(struct sockaddr *)&sockets_bind[i], tam_dir);
            printf("%s: SEND(%s,%s)\n",p->ID,mandato_send,IDs[i]);
          }
        }
        strcpy(line2,"");
        strcpy(proc,"");
      }
      }
    }


    
    

  }

  //Cerrar socket
  close(socket_p);

  return 0;
}
void borrar_elemento(char ***IDs,char*ID,int *length_process, int MAX_P){
    int index=seccionCriticaDisponible(*IDs,ID,*length_process);
    if(index==0){
      index=obtener_index(*IDs,ID,*length_process);
      if(index!=*length_process-1){
      char **IDs_2=(char **)malloc(MAX_P*80);
      int i=0;
      int j=0;
      for(;j< *length_process ;){
        if(i==index){
          j++;
        }
          strcpy(IDs_2[i],*IDs[j]);
        
        i++;
        j++;
      }
      free(IDs);
      IDs=&IDs_2;
    }else{
        *IDs[*length_process-1]=malloc(sizeof(char)*80);
    }
      *length_process=*length_process-1;
    }
}
int seccionCriticaDisponible(char **seccionesCriticas,char* nombreSeccionCritica,int length_seccionesCriticas){
  int DISPONIBLE=1;
  for(int i=0;i<length_seccionesCriticas;i++){
    if(!strcmp(nombreSeccionCritica,seccionesCriticas[i])){
      DISPONIBLE=0;
      break;
    }
  }
  return DISPONIBLE;
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
char* substr (const char* cadena, int offset, int l, char* cadena2)
{
  int l2;
  l2 = strlen (cadena);

  if (offset + l > l2){
     return NULL;
  }

  strncpy (cadena2, cadena + offset, l);
  return cadena2;
}