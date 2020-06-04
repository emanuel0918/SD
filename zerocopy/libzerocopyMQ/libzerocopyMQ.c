#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"



//---------------------------------------------------------------------------------------------
int create_destroy(const char * cola, char opc){

	int s=obtenerSocket();
	//
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	//
	int leido;
	//send opc
	char opcion[2];
	opcion[0]=opc;opcion[2]='\0';
	send(s,opcion,2*sizeof(char),0);
    if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		//send sizeof
		char * sizeof_cola_s;
		sizeof_cola_s=intToString(strlen(cola));
		send(s,sizeof_cola_s,sizeof(sizeof_cola_s),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		//send cola
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
	//
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
	//
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	//
	int leido;
	//send opc
	char opcion[2];
	opcion[0]='p';opcion[2]='\0';
	send(s,opcion,2*sizeof(char),0);
    if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		//send sizeof
		char * sizeof_cola_s;
		sizeof_cola_s=intToString(strlen(cola));
		send(s,sizeof_cola_s,sizeof(sizeof_cola_s),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		//send cola
		send(s,cola,strlen(cola),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
				//send sizeof mensaje
				char * sizeof_mensaje_s;
				sizeof_mensaje_s=intToString(strlen(mensaje));
				send(s,sizeof_mensaje_s,sizeof(sizeof_mensaje_s),0);
				if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
					//send mensaje
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
	//
	return atoi(respuesta);
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {

	int s=obtenerSocket();
	//
	char sizeof_mensaje_s[TAM_LONG];
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	//
	int leido;
	//send opc
	char opcion[2];
	opcion[0]='g';opcion[2]='\0';
	send(s,opcion,2*sizeof(char),0);
    if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		//send sizeof
		char * sizeof_cola_s;
		sizeof_cola_s=intToString(strlen(cola));
		send(s,sizeof_cola_s,sizeof(sizeof_cola_s),0);
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		//send cola
		send(s,cola,strlen(cola),0);
    	if ((leido=read(s, sizeof_mensaje_s,TAM_LONG))>0) {
			if(sizeof_mensaje_s[0]!='\000'){ 
				int sizeof_mensaje= atoi(sizeof_mensaje_s);
				char mensaje_s[sizeof_mensaje+1];
				for(int i=0;i<sizeof_mensaje+1;i++){
					mensaje_s[i]='\0';
				}
				send(s,respuesta,sizeof(respuesta),0);
				if((leido=read(s,mensaje_s,sizeof(mensaje_s))>0)){
					*mensaje=(void **)&mensaje_s;
				}
				if (leido<0) {
					respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
						close(s);
						return atoi(respuesta);
				}
			}else{
				//
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
	//
	return atoi(respuesta);
}



