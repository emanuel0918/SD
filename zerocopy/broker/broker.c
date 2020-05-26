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
void print_thread(struct thread_data * t_d,char *cadena){
	printf("Socket %ld\n",t_d->s_conect);
	int error;
	dic_get(t_d->d,cadena,&error);
	int length=sizeof(t_d->d);
	printf("Size Of Dic: %d\n",length);
	
	printf("Cola: %s\nError: %d\n",cadena,error);
}

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
	//t_d->cont=2;
	while (1) {
		tam_dir=sizeof(dir_cliente);
		if ((t_d->s_conect=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
			perror("error en accept");
			close(s);
			return 1;
		}
		pthread_create(&thid, &atrib_th, servicio, (void *)(struct thread_data *)t_d);
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
	char nombre_cola[TAM_COLA];
	char mensaje[TAM_MENSAJE];
	//
	char op=0;
	char opc[2];

	//
	struct thread_data * t_d;
	t_d=(struct thread_data*)arg;
	//
        //s=(long) arg;
	s=t_d->s_conect;
	//inicializar
	//for(int i=0;i<TAM_COLA;i++){
	//	buffer_cola[i]='\0';
	//}
	
	//

    if ((leido=read(s, opc,2*sizeof(char)))>0) {
		op=opc[0];
	}
        if (leido<0) {
                perror("error en read");
                close(s);
                return NULL;
        }

	//
	
    if ((leido=read(s, nombre_cola,TAM_COLA*sizeof(char)))>0) {
		//printf("a1\n%s\n",buffer_cola);
	}
	if (leido<0) {
			perror("error en read");
			close(s);
			return NULL;
	}
	//
	//
	//
	if(op=='c' || op=='d'){

		switch(op){
		 case 'c':
			// createMQ()
			dic_get(t_d->d,nombre_cola,&error);
			if(error==-1){
				dic_put(t_d->d,nombre_cola,(void*)(struct cola *)cola_create());
				send(s,"0\0",(4*sizeof(char)),0);
			}else{
				send(s,"-1\0",(4*sizeof(char)),0);
				printf("El registro ya existe\n");
			}
			break;

		 case 'd':
			// destroyMQ()
			dic_get(t_d->d,nombre_cola,&error);
			if(error==-1){
				send(s,"-1\0",(4*sizeof(char)),0);
				printf("El registro no existe\n");
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
	
		if ((leido=read(s, mensaje,TAM_MENSAJE*sizeof(char)))>0) {
			printf("mensaje: %s\n",mensaje);
		}
		if (leido<0) {
				perror("error en read");
				close(s);
				return NULL;
		}

	}
	
        close(s);
        return NULL;

}

