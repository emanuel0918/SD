#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"
int create_destroy(const char * cola, char opc){

	int s=obtenerSocket();
	int i;
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	int leido;
	char *opcion=(char*)malloc(sizeof(char));
	opcion[0]=opc;
	ssize_t nwritten;
	struct iovec iov[2];
	iov[0].iov_base=opcion;
	iov[0].iov_len=1;
	iov[1].iov_base=cola;
	iov[1].iov_len=strlen(cola);
	while((nwritten=writev(s,iov,2))>0){
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
	int i;
	char sizeof_mensaje_s[TAM_LONG];
	for(i=0;i<TAM_LONG;i++){
		sizeof_mensaje_s[i]='\0';
	}
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	return atoi(respuesta);
}


