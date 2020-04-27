#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"

void * servicio(void *arg);
struct thread_data{
	long s_conect;
	struct diccionario * d;
	char buff[6];
	int cont;
};
int main(int argc, char *argv[]){
	//
	struct thread_data * t_d;
	t_d=(struct thread_data*) malloc(sizeof(struct thread_data));
	struct cola *cl;
	t_d->d=dic_create();
	char mensaje[4];
	char cola[2];

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
	strcpy(t_d->buff,"as000\0");
	t_d->cont=2;
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
        int s, leido,error;
        char buf[TAM];
	struct thread_data * t_d;
	t_d=(struct thread_data*)arg;
	//
	//
        //s=(long) arg;
	s=t_d->s_conect;
        //char *s_con=intToString((long)(t_d->s_conect));
	//(t_d->buff)[(2+((t_d->cont)%3))]=s_con[0];
	//t_d->cont++;
	//printf("desps del read %s\n",t_d->buff);
	char op=0;

	//nombre de la cola
	char cola[2];
        while ((leido=read(s, buf, TAM))>0) {
		//Flag de op
		op=buf[0];
		//parametro de referencia
		error=0;
		switch(op){
		 case 'c':
			// createMQ()
			//nombre cola 2 bytes
			cola[0]=buf[1];cola[1]='\0';
			dic_get(t_d->d,cola,&error);
			if(error==-1){
				dic_put(t_d->d,cola,(void*)(struct cola *)cola_create());
			}else{
				printf("El registro ya existe\n");
			}
			break;
		 case 'd':
			// destroyMQ()
			//nombre cola 2 bytes
			cola[0]=buf[1];cola[1]='\0';
			//free
			free(dic_get(t_d->d,cola,&error));
			//remover registro
			dic_remove_entry(t_d->d,cola,NULL);
			break;
		}
		//El buffer esta en el hilo pero se debe
		// gestionar el uso de memoria en este proceso
		// se asigna una variable de tipo diccionario que
		// elige si guardar en una nueva cola o en una
		// ya creada 
                //if (write(s, buf, leido)<0) {
                //        perror("error en write");
                //        close(s);
                //        return NULL;
                //}

        }
        if (leido<0) {
                perror("error en read");
                close(s);
                return NULL;
        }
        close(s);
        return NULL;
}

