#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"

void * servicio(void *arg);
struct thread_data{
	long s_conect;
	struct diccionario * d;
	//char buff[6];
	//int cont;
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
	//t_d->cont=0;
	//int error=0;
	while (1) {
		tam_dir=sizeof(dir_cliente);
		if ((t_d->s_conect=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
			perror("error en accept");
			close(s);
			return 1;
		}
		//pthread_create(&thid, &atrib_th, servicio, (void *)(struct thread_data *)t_d);
		servicio ((void *)(struct thread_data *)t_d);
		//printf("mensaje : %s\n",(char*)cola_pop_front(dic_get(t_d->d,"c1",&error),&error));
		//printf("desps del read %s\n",t_d->buf);

		/* Esta forma de pasar el parámetro no sería válida
		puesto que se produce una condición de carrera */
		//pthread_create(&thid, NULL, servicio, &s_conec);
	}

	close(s);
	return 0;
}
void * servicio(void *arg){
	int i;
	int s;
	int error=0;
	int leido;
	int leido1,leido2,leido3,leido4,leido5,leido6,leido8,leido9;
	char op=0;
	char opc[2];
	char resp1[2];
	void * memoria_dinamica=malloc(sizeof(char));
	//
	char sizeof_mensaje_s[TAM_LONG];
	char sizeof_cola_s[TAM_LONG];
	//
	for( i=0;i<TAM_LONG;i++){
		sizeof_cola_s[i]='\0';
	}
	int sizeof_cola;
	int sizeof_mensaje;

	//
	struct thread_data * t_d;
	t_d=(struct thread_data*)arg;
	//
	s=t_d->s_conect;
	/*
	LA MEMORIA DINAMICA QUE QUIERO DEBE CONTENER EL
	iovec[3]
	pero
	
	*/
    while ((leido=recv(s, opc,2,MSG_CONFIRM | MSG_WAITALL))>0) {
		op=opc[0];

		while ((leido=recv(s, sizeof_cola_s,TAM_LONG,MSG_CONFIRM | MSG_WAITALL))>0) {
			sizeof_cola=atoi(sizeof_cola_s);
			sizeof_cola+=1;
			//printf("Prueba%d\nsizeof_cola: %d\n",t_d->cont,sizeof_cola);

			//
			
/*
			char nombre_cola[sizeof_cola];
			for( i=0;i<sizeof_cola;i++){
				nombre_cola[i]='\0';
			}
*/
			char *nombre_cola=malloc(sizeof_cola);
			//printf("sizeof(nombre_cola) : %d\n",(int)sizeof(nombre_cola));
			while ((leido=recv(s, nombre_cola,sizeof_cola,MSG_WAITALL))>0) {
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

								close(s);
								return NULL;
							}else{
								send(s,"-1\0",(4*sizeof(char)),0);
								perror("El registro ya existe\n");

								close(s);
								return NULL;
							}
							break;

						case 'd':
							// destroyMQ()
							dic_get(t_d->d,nombre_cola,&error);
							if(error==-1){
								send(s,"-1\0",(4*sizeof(char)),0);
								perror("El registro no existe\n");

								close(s);	
								return NULL;
							}else{
								//free
								free(dic_get(t_d->d,nombre_cola,&error));
								//remover registro
								dic_remove_entry(t_d->d,nombre_cola,NULL);
								send(s,"0\0",(4*sizeof(char)),0);

								close(s);
								return NULL;
							}
							break;
					}

				}else{
					// FICH
					switch(op){
						case 'p':
							while ((leido=read(s, sizeof_mensaje_s,TAM_LONG))>0) {
								sizeof_mensaje=atoi(sizeof_mensaje_s)+1;
								//
								//
								
								char *mensaje=malloc(sizeof_mensaje*sizeof(char));
								for( i=0;i<sizeof_mensaje;i++){
									mensaje[i]='\0';
								}
								//printf("sizeof(mensaje) : %d\n",(int)sizeof_mensaje);

								//while ((leido=recv(s, mensaje, 256, MSG_WAITALL))>0){
								while ((leido=read(s, mensaje,sizeof_mensaje))>0) {
									//
									dic_get(t_d->d,nombre_cola,&error);
									if(error==-1){
										send(s,"-1\0",(4*sizeof(char)),0);
										perror("El registro no existe\n");

										close(s);
										return NULL;
									}else{
										//push
										cola_push_back(dic_get(t_d->d,nombre_cola,&error),mensaje);
										//printf("cadena : %s\n",(char*)cola_pop_front(dic_get(t_d->d,nombre_cola,&error),&error));
										send(s,"0\0",(4*sizeof(char)),0);

										close(s);
										return NULL;
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
							char *cadena0=(char*)cola_pop_front(dic_get(t_d->d,nombre_cola,&error),&error);
							if(error==-1){
								send(s,"\000",sizeof(char),0);
								perror("El registro no existe\n");
								close(s);
								return NULL;
							}else{
								//pop

								int sizeof_cadena=strlen(cadena0);
								sizeof_cadena+=1;
								//printf("sizeof_cadena : %d\n",sizeof_cadena);

								struct iovec iov[2];
								char cadena[sizeof_cadena];

								for( i=0;i<sizeof_cadena;i++){
									cadena[i]='\0';
								}
								strcpy(cadena,cadena0);
								//strcpy(cadena,);
								//

								
								//char *sizeof_mensaje_s1=intToString(sizeof_cadena);
								//for( i=0;i<strlen(sizeof_mensaje_s1);i++){
									//sizeof_mensaje_s[i]=sizeof_mensaje_s1[i];
								//}
								//strcpy(sizeof_mensaje_s,sizeof_mensaje_s1);
								sprintf(sizeof_mensaje_s, "%d", sizeof_cadena);
								iov[0].iov_base=sizeof_mensaje_s;
								iov[0].iov_len=TAM_LONG;
								iov[1].iov_base=cadena;
								iov[1].iov_len=sizeof_mensaje-1;
								while((leido9=writev(s,iov,2))>0){
									//send(s,"0\0",(4*sizeof(char)),0);
									close(s);
									return NULL;
								}
								if(leido9<0){

									send(s,"-1\0",(4*sizeof(char)),0);
									close(s);
									return NULL;
								}
								
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
	close(s);
	return NULL;

}