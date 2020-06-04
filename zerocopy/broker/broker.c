#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"

void * servicio(void *arg);
struct thread_data{
	long s_conect;
	struct diccionario * d;
	//char buff[6];
	int cont;
};

int main(int argc, char *argv[]){
	//
	struct thread_data * t_d;
	t_d=(struct thread_data*) malloc(sizeof(struct thread_data));
	t_d->d=dic_create();

	//
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

	/* Creamos los thread de tipo detached para que liberen sus
	recursos al terminar sin necesidad de que se haga un
	pthread_join */
	pthread_attr_init(&atrib_th);
	pthread_attr_setdetachstate(&atrib_th, PTHREAD_CREATE_DETACHED);

	if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("error creando socket");
		return 1;
	}

	/* Para reutilizar puerto inmediatamente */
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
	//strcpy(t_d->buff,"as000\0");
	t_d->cont=0;
	//int error=0;
	while (1) {
		tam_dir=sizeof(dir_cliente);
		if ((t_d->s_conect=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
			perror("error en accept");
			close(s);
			return 1;
		}
		pthread_create(&thid, &atrib_th, servicio, (void *)(struct thread_data *)t_d);
		//printf("mensaje : %s\n",(char*)cola_pop_front(dic_get(t_d->d,"c1",&error),&error));
		//printf("desps del read %s\n",t_d->buf);

		/* Esta forma de pasar el parámetro no sería válida
		puesto que se produce una condición de carrera */
		//pthread_create(&thid, NULL, servicio, &s_conec);
	}

	close(s);
}
void * servicio(void *arg){
	int s, leido;
	int error=0;
	char op=0;
	char opc[2];
	//
	char sizeof_mensaje_s[TAM_LONG];
	char *sizeof_mensaje_s1;
	char sizeof_cola_s[TAM_LONG];
	//
	for(int i=0;i<TAM_LONG;i++){
		sizeof_cola_s[i]='\0';
	}
	int sizeof_cola;
	int sizeof_mensaje;

	//
	struct thread_data * t_d;
	t_d=(struct thread_data*)arg;
	//
	if(t_d->cont>700000)
		t_d->cont=0;
	if(t_d->cont==2){

		printf("mensaje : %s\n",(char*)cola_pop_front(dic_get(t_d->d,"c1",&error),&error));
	}
	t_d->cont++;
    //s=(long) arg;
	s=t_d->s_conect;
	
	//

    while ((leido=read(s, opc,sizeof(opc)))>0) {
		op=opc[0];
		send(s,"0\0",(4*sizeof(char)),0);

		while ((leido=read(s, sizeof_cola_s,sizeof(sizeof_cola_s)))>0) {
			sizeof_cola=atoi(sizeof_cola_s);
			//sizeof_cola+=2;
			//printf("Prueba%d\nsizeof_cola: %d\n",t_d->cont,sizeof_cola);

			//
			
			char nombre_cola[sizeof_cola+1];
			for(int i=0;i<sizeof_cola+1;i++){
				nombre_cola[i]='\0';
			}
			//printf("sizeof(nombre_cola) : %d\n",(int)sizeof(nombre_cola));
			send(s,"0\0",(4*sizeof(char)),0);
			while ((leido=read(s, nombre_cola,sizeof_cola))>0) {
				//printf("opc: %c\n%s\n",op,nombre_cola);
				//
				//
				if(op=='c' || op=='d'){
					//printf("t_d->d->nentradas : %d\n",t_d->d->nentradas);
					switch(op){
						case 'c':
							// createMQ()
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
							// destroyMQ()
							dic_get(t_d->d,nombre_cola,&error);
							if(error==-1){
								send(s,"-1\0",(4*sizeof(char)),0);
								perror("El registro no existe\n");
							}else{
								//free
								free(dic_get(t_d->d,nombre_cola,&error));
								//remover registro
								dic_remove_entry(t_d->d,nombre_cola,NULL);
								send(s,"0\0",(4*sizeof(char)),0);
							}
							break;
					}

				}else{
					// FICH
					switch(op){
						case 'p':
							send(s,"0\0",(4*sizeof(char)),0);
							while ((leido=read(s, sizeof_mensaje_s,sizeof(sizeof_mensaje_s)))>0) {
								sizeof_mensaje=atoi(sizeof_mensaje_s);
								//
								//
								
								char mensaje[sizeof_mensaje+1];
								for(int i=0;i<sizeof_mensaje+1;i++){
									mensaje[i]='\0';
								}
								//printf("sizeof(mensaje) : %d\n",(int)sizeof(mensaje));
								send(s,"0\0",(4*sizeof(char)),0);

								//while ((leido=recv(s, mensaje, 256, MSG_WAITALL))>0){
								while ((leido=read(s, mensaje,sizeof_mensaje))>0) {
									//
									dic_get(t_d->d,nombre_cola,&error);
									if(error==-1){
										send(s,"-1\0",(4*sizeof(char)),0);
										perror("El registro no existe\n");
									}else{
										//push
										cola_push_back(dic_get(t_d->d,nombre_cola,&error),(void *)(char *)mensaje);
										//printf("cadena : %s\n",(char*)cola_pop_front(dic_get(t_d->d,nombre_cola,&error),&error));
										send(s,"0\0",(4*sizeof(char)),0);
									}
								}
								if (leido<0) {
									perror("error en read5");
									close(s);
									return NULL;
								}
								//
								//
							}
							if (leido<0) {
									perror("error en read4");
									close(s);
									return NULL;
							}

							break;
						case 'g':
						case 'b':
							//
							dic_get(t_d->d,nombre_cola,&error);
							if(error==-1){
								send(s,"\000",sizeof(char),0);
								perror("El registro no existe\n");
							}else{
								//pop
								//strcpy(cadena,);
								printf("mensaje : %s\n",(char*)cola_pop_front(dic_get(t_d->d,nombre_cola,&error),&error));
								//

								/*
								sizeof_mensaje_s1=intToString(strlen(cadena));
								send(s,sizeof_mensaje_s1,sizeof(sizeof_mensaje_s1),0);
								char resp[4];
								while ((leido=read(s, resp,sizeof(resp)))>0) {
									send(s,cadena,sizeof(cadena),0);

								}
								if (leido<0) {
									perror("error en read6");
									close(s);
									return NULL;
								}
								*/
								send(s,"\000",sizeof(char),0);
								//send(s,cadena,sizeof(cadena),0);
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
	//
	
	close(s);
	return NULL;

}
