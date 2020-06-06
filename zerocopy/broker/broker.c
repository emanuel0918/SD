#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define TAM_LONG 10
#define TAM_PAQUETE 256


void * servicio(void *arg);
struct thread_data{
	long s_conect;
	struct diccionario * d;
};

int main(int argc, char *argv[]){
	struct thread_data * t_d;
	t_d=(struct thread_data*) malloc(sizeof(struct thread_data));
	t_d->d=dic_create();
	int s;
	unsigned int tam_dir;
	struct sockaddr_in dir, dir_cliente;
	int opcion=1;
	pthread_t thid;
	pthread_attr_t atrib_th;

	if (argc!=2) {
			fprintf(stderr, "Uso: %s puerto\n", argv[0]);
			return 1;
	}

	pthread_attr_init(&atrib_th);
	pthread_attr_setdetachstate(&atrib_th, PTHREAD_CREATE_DETACHED);

	if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("error creando socket");
		return 1;
	}
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion))<0){
			perror("error en setsockopt");
			return 1;
	}

	dir.sin_addr.s_addr=INADDR_ANY;
	dir.sin_port=htons(atoi(argv[1]));
	dir.sin_family=PF_INET;
	if (bind(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error en bind");
		close(s);
		return 1;
	}

	if (listen(s, 5) < 0) {
		perror("error en listen");
		close(s);
		return 1;
	}
	while (1) {
		tam_dir=sizeof(dir_cliente);
		if ((t_d->s_conect=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
			perror("error en accept");
			close(s);
			return 1;
		}
		pthread_create(&thid, &atrib_th, servicio, (void *)(struct thread_data *)t_d);
		
	}

	close(s);
}
void * servicio(void *arg){
	int s, leido;
	int error=0;
	char op=0;
	char opc[2];
	int ITER;
	char mensajito[TAM_PAQUETE];
	for(int i=0;i<TAM_PAQUETE;i++){
		mensajito[i]='\0';
	}
	char sizeof_mensaje_s[TAM_LONG];
	char sizeof_cola_s[TAM_LONG];
	for(int i=0;i<TAM_LONG;i++){
		sizeof_cola_s[i]='\0';
	}
	int sizeof_cola;
	int sizeof_mensaje;
	struct thread_data * t_d;
	t_d=(struct thread_data*)arg;
	s=t_d->s_conect;
	while ((leido=read(s, opc,sizeof(opc)))>0) {
		op=opc[0];
		send(s,"0\0",(4*sizeof(char)),0);

		while ((leido=read(s, sizeof_cola_s,sizeof(sizeof_cola_s)))>0) {
			sizeof_cola=atoi(sizeof_cola_s);
			char nombre_cola[sizeof_cola+1];
			for(int i=0;i<sizeof_cola+1;i++){
				nombre_cola[i]='\0';
			}
			send(s,"0\0",(4*sizeof(char)),0);
			while ((leido=read(s, nombre_cola,sizeof_cola))>0) {
				if(op=='c' || op=='d'){
					switch(op){
						case 'c':
							dic_get(t_d->d,nombre_cola,&error);
							if(error==-1){
								dic_put(t_d->d,nombre_cola,(void*)(struct cola *)cola_create());
								send(s,"0\0",(4*sizeof(char)),0);
							}else{
								send(s,"-1\0",(4*sizeof(char)),0);
								perror("El registro ya existe\n");
							}
							break;

						case 'd':
							dic_get(t_d->d,nombre_cola,&error);
							if(error==-1){
								send(s,"-1\0",(4*sizeof(char)),0);
								perror("El registro no existe\n");
							}else{
								free(dic_get(t_d->d,nombre_cola,&error));
								dic_remove_entry(t_d->d,nombre_cola,NULL);
								send(s,"0\0",(4*sizeof(char)),0);
							}
							break;
					}

				}else{
					switch(op){
						case 'p':
							send(s,"0\0",(4*sizeof(char)),0);
							while ((leido=read(s, sizeof_mensaje_s,sizeof(sizeof_mensaje_s)))>0) {
								sizeof_mensaje=atoi(sizeof_mensaje_s);
								char *mensaje=malloc(sizeof_mensaje*sizeof(char));
								for(int i=0;i<sizeof_mensaje+1;i++){
									mensaje[i]='\0';
								}
								send(s,"0\0",(4*sizeof(char)),0);
								int j=0;
								while ((leido=recv(s, mensajito,TAM_PAQUETE,MSG_WAITALL))>0) {
									for(int i=0;i<TAM_PAQUETE;i++){
										mensaje[j*TAM_PAQUETE+i]=mensajito[i];
									}
									j++;
								}
								if (leido<0) {
									perror("error en read5");
									close(s);
									return NULL;
								}else{

									dic_get(t_d->d,nombre_cola,&error);
									if(error==-1){
										send(s,"-1\0",(4*sizeof(char)),0);
										perror("El registro no existe\n");
									}else{
										cola_push_back(dic_get(t_d->d,nombre_cola,&error),mensaje);
										send(s,"0\0",(4*sizeof(char)),0);
									}
								}
							}
							if (leido<0) {
									perror("error en read4");
									close(s);
									return NULL;
							}

							break;
						case 'g':
						case 'b':
							dic_get(t_d->d,nombre_cola,&error);
							char *cadena0=(char*)cola_pop_front(dic_get(t_d->d,nombre_cola,&error),&error);
							if(error==-1){
								send(s,"\000",sizeof(char),0);
								perror("El registro no existe\n");
								close(s);
								return NULL;
							}else{
								int sizeof_cadena=strlen(cadena0);
								char cadena[sizeof_cadena+1];

								for(int i=0;i<sizeof_cadena+1;i++){
									cadena[i]='\0';
								}
								strcpy(cadena,cadena0);
								for(int i=0;i<TAM_LONG;i++){
									sizeof_mensaje_s[i]='\0';
								}
								sprintf(sizeof_mensaje_s, "%d", sizeof_cadena+1);
								send(s,sizeof_mensaje_s,strlen(sizeof_mensaje_s),0);
								char resp[4];
								if ((leido=read(s, resp,sizeof(resp)))>0) {
									if((sizeof_mensaje+1)%TAM_PAQUETE==0){
										ITER=(sizeof_mensaje+1)/TAM_PAQUETE;
									}else{
										ITER=sizeof_mensaje/TAM_PAQUETE+1;
									}
									for(int i=0;i<ITER;i++){
										if(write(s,cadena,TAM_PAQUETE)<0){
											perror("error en read7");
											close(s);
											return NULL;
										}
									}
								}
								if (leido<0) {
									perror("error en read6");
									close(s);
									return NULL;
								}
								
								send(s,"\000",sizeof(char),0);
							}
							break;
					}

				}
			}
			if (leido<0) {
					perror("error en read3");
					close(s);
					return NULL;
			}
		}
		if (leido<0) {
				perror("error en read2");
				close(s);
				return NULL;
		}
	}
	if (leido<0) {
			perror("error en read1");
			close(s);
			return NULL;
	}
	close(s);
	return NULL;

}
