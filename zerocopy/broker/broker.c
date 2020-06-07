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
	int i;
	int s, leido;
	int error=0;
	char op=0;
	char opc[2];
	void * memoria_dinamica=malloc(sizeof(char));
	//
	char sizeof_mensaje_s[TAM_LONG];
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
	//if(t_d->cont>700000){
		//t_d->cont=0;
	//}
	//t_d->cont++;
    //s=(long) arg;
	s=t_d->s_conect;
	
	//
	while((leido=read(s,memoria_dinamica,TAM_PAQUETE))>0){

		printf("opc : %s\n",((char*)memoria_dinamica));
		op=((char*)memoria_dinamica)[0];
		char * nombre_cola=(char*)malloc((sizeof(memoria_dinamica)-1)*sizeof(char));
		strcpy(nombre_cola+sizeof(char),memoria_dinamica);
		printf("op: %c\nnombre_cola : %s\n",op,nombre_cola);
		send(s,"0\0",(4*sizeof(char)),0);
	}
	if (leido<0) {
			perror("error en read1");
			send(s,"-1\0",(4*sizeof(char)),0);
			close(s);
			return NULL;
	}
	
	close(s);
	return NULL;

}