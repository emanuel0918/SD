#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"
int create_destroy(const char * cola, char opc){

	int s=obtenerSocket();
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	int leido;
	char *opcion=(char*)malloc(2*sizeof(char));
	opcion[0]=opc;opcion[1]='\0';
	char * sizeof_cola_s;
	sizeof_cola_s=intToString(strlen(cola));
	ssize_t nwritten;
	struct iovec iov[3];
	iov[0].iov_base=cola;
	iov[0].iov_len=atoi(sizeof_cola_s);
	iov[1].iov_base=sizeof_cola_s;
	iov[1].iov_len=strlen(sizeof_cola_s);
	iov[2].iov_base=opcion;
	iov[2].iov_len=strlen(opcion);
	while((nwritten=writev(s,iov,3))>0){
		if(read(s,respuesta,sizeof(respuesta))>0){
			close(s);
			return sizeof(respuesta);
		}
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
	return atoi(respuesta);
}


