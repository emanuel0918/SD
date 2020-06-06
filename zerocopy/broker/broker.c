#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"

int main(int argc, char *argv[]){
	int s;
	int s_conect;
	int leido;
	unsigned int tam_dir;
	struct sockaddr_in dir, dir_cliente;
	int opcion=1;


	struct diccionario *d;
	d=dic_create();



	int error=0;
	char op=0;
	char opc[2];
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

	if (argc!=2) {
			fprintf(stderr, "Uso: %s puerto\n", argv[0]);
			return 1;
	}


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
		if ((s_conect=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
			perror("error en accept");
			close(s);
			return 1;
		}

		while ((leido=read(s_conect, opc,sizeof(opc)))>0) {
			op=opc[0];
			send(s_conect,"0\0",(4*sizeof(char)),0);

			while ((leido=read(s_conect, sizeof_cola_s,sizeof(sizeof_cola_s)))>0) {
				sizeof_cola=atoi(sizeof_cola_s);
				char nombre_cola[sizeof_cola+1];
				for(int i=0;i<sizeof_cola+1;i++){
					nombre_cola[i]='\0';
				}
				send(s_conect,"0\0",(4*sizeof(char)),0);
				while ((leido=read(s_conect, nombre_cola,sizeof_cola))>0) {
					if(op=='c' || op=='d'){
						switch(op){
							case 'c':
								dic_get(d,nombre_cola,&error);
								if(error==-1){
									dic_put(d,nombre_cola,(void*)(struct cola *)cola_create());
									send(s_conect,"0\0",(4*sizeof(char)),0);
								}else{
									send(s_conect,"-1\0",(4*sizeof(char)),0);
									perror("El registro ya existe\n");
								}
								break;

							case 'd':
								dic_get(d,nombre_cola,&error);
								if(error==-1){
									send(s_conect,"-1\0",(4*sizeof(char)),0);
									perror("El registro no existe\n");
								}else{
									free(dic_get(d,nombre_cola,&error));
									dic_remove_entry(d,nombre_cola,NULL);
									send(s_conect,"0\0",(4*sizeof(char)),0);
								}
								break;
						}

					}else{
						switch(op){
							case 'p':
								send(s_conect,"0\0",(4*sizeof(char)),0);
								while ((leido=read(s_conect, sizeof_mensaje_s,sizeof(sizeof_mensaje_s)))>0) {
									sizeof_mensaje=atoi(sizeof_mensaje_s);
									char *mensaje=malloc(sizeof_mensaje*sizeof(char));
									for(int i=0;i<sizeof_mensaje+1;i++){
										mensaje[i]='\0';
									}
									send(s_conect,"0\0",(4*sizeof(char)),0);
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
										return 1;
									}else{
										dic_get(d,nombre_cola,&error);
										if(error==-1){
											send(s_conect,"-1\0",(4*sizeof(char)),0);
											perror("El registro no existe\n");
										}else{
											cola_push_back(dic_get(d,nombre_cola,&error),mensaje);
											send(s_conect,"0\0",(4*sizeof(char)),0);
										}
									}
								}
								if (leido<0) {
										perror("error en read4");
										close(s_conect);
										return 1;
								}

								break;
							case 'g':
							case 'b':
								dic_get(d,nombre_cola,&error);
								char *cadena0=(char*)cola_pop_front(dic_get(d,nombre_cola,&error),&error);
								if(error==-1){
									send(s_conect,"\000",sizeof(char),0);
									perror("El registro no existe\n");
									close(s_conect);
									return 1;
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
									send(s_conect,sizeof_mensaje_s,strlen(sizeof_mensaje_s),0);
									char resp[4];
									while((leido=read(s_conect, resp,sizeof(resp)))>0) {
										send(s_conect,cadena,(sizeof_cadena+1)*sizeof(char),0);

									}
									if (leido<0) {
										perror("error en read6");
										close(s_conect);
										return 1;
									}
									
									send(s_conect,"\000",sizeof(char),0);
								}
								break;
						}

					}
				}
				if (leido<0) {
						perror("error en read3");
						close(s_conect);
						return 1;
				}
			}
			if (leido<0) {
					perror("error en read2");
					close(s_conect);
					return 1;
			}
		}
		if (leido<0) {
				perror("error en read1");
				close(s_conect);
				return 1;
		}
		close(s_conect);
		return 1;
		
		
	}

	close(s);
}
