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
    return 0;
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
    return 0;
}



