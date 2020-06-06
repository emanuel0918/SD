#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"
int create_destroy(const char * cola, char opc){

	int s=obtenerSocket();
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	int leido;
	char opcion[2];
	opcion[0]=opc;opcion[2]='\0';
	send(s,opcion,2*sizeof(char),0);
    if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		char * sizeof_cola_s;
		sizeof_cola_s=intToString(strlen(cola));
		send(s,sizeof_cola_s,sizeof(sizeof_cola_s),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		send(s,cola,strlen(cola),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
			}
			if (leido<0) {
					respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
					close(s);
					return atoi(respuesta);
			}
		}
		if (leido<0) {
				respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
				close(s);
				return atoi(respuesta);
		}
	}
	if (leido<0) {
			respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
			close(s);
			return atoi(respuesta);
	}
	return atoi(respuesta);
}
int createMQ(const char *cola) {
	return create_destroy(cola,'c');
}

int destroyMQ(const char *cola){
	return create_destroy(cola,'d');
}
int put(const char *cola, const void *mensaje, uint32_t tam) {

	int s=obtenerSocket();
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	int leido;
	char opcion[2];
	opcion[0]='p';opcion[2]='\0';
	send(s,opcion,2*sizeof(char),0);
    if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		char * sizeof_cola_s;
		sizeof_cola_s=intToString(strlen(cola));
		send(s,sizeof_cola_s,sizeof(sizeof_cola_s),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		send(s,cola,strlen(cola),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
				char * sizeof_mensaje_s;
				sizeof_mensaje_s=intToString(strlen(mensaje));
				send(s,sizeof_mensaje_s,sizeof(sizeof_mensaje_s),0);
				if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
					send(s,mensaje,strlen(mensaje),0);
					if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
							close(s);
							return atoi(respuesta);
					}
					if (leido<0) {
							respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
							close(s);
							return atoi(respuesta);
					}
				}
				if (leido<0) {
						respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
						close(s);
						return atoi(respuesta);
				}
			}
			if (leido<0) {
					respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
					close(s);
					return atoi(respuesta);
			}
		}
		if (leido<0) {
				respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
				close(s);
				return atoi(respuesta);
		}
	}
	if (leido<0) {
			respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
			close(s);
			return atoi(respuesta);
	}
	return atoi(respuesta);
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {

	int s=obtenerSocket();
	char sizeof_mensaje_s[TAM_LONG];
	for(int i=0;i<TAM_LONG;i++){
		sizeof_mensaje_s[i]='\0';
	}
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	int leido;
	char opcion[2];
	opcion[0]='g';opcion[2]='\0';
	send(s,opcion,2*sizeof(char),0);
    if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		char * sizeof_cola_s;
		sizeof_cola_s=intToString(strlen(cola));
		send(s,sizeof_cola_s,sizeof(sizeof_cola_s),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		send(s,cola,strlen(cola),0);
    	if((leido=read(s, sizeof_mensaje_s,TAM_LONG))>0) {
			if(sizeof_mensaje_s[0]!='\000'){ 
				int sizeof_mensaje= atoi(sizeof_mensaje_s);
				char mensaje_s[sizeof_mensaje+1];
				for(int i=0;i<sizeof_mensaje+1;i++){
					mensaje_s[i]='\0';
				}
				*mensaje=(char*)malloc(sizeof_mensaje*sizeof(char));
				*tam=(uint32_t )sizeof_mensaje;
				send(s,respuesta,sizeof(respuesta),0);
				if((leido=read(s,mensaje_s,sizeof_mensaje)>0)){
					strcpy(*mensaje,mensaje_s);
					respuesta[0]='0';respuesta[1]='\0';
					return atoi(respuesta);
				}
				if (leido<0) {
					respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
						close(s);
						return atoi(respuesta);
				}
			}else{
				respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
				close(s);
				return atoi(respuesta);
			}
			}
			if (leido<0) {
					respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
					close(s);
					return atoi(respuesta);
			}
		}
		if (leido<0) {
				respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
				close(s);
				return atoi(respuesta);
		}
	}
	if (leido<0) {
			respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
			close(s);
			return atoi(respuesta);
	}
	return atoi(respuesta);
}


