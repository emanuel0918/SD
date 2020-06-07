#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"
int create_destroy(const char * cola, char opc){

	int s=obtenerSocket();
	int i;
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	int leido;
	char *opcion=(char*)malloc(2*sizeof(char));
	char *sizeof_cola_s=intToString(strlen(cola));
	opcion[0]=opc;opcion[1]='\0';
	ssize_t nwritten;
	struct iovec iov[3];
	iov[0].iov_base=opcion;
	iov[0].iov_len=2;
	iov[1].iov_base=sizeof_cola_s;
	iov[1].iov_len=TAM_LONG;
	/*
	 AQUI EL TAMANIO VARIABLE DE LA COLA
	*/
	iov[2].iov_base=cola;
	iov[2].iov_len=atoi(sizeof_cola_s)+1;
	while((nwritten=writev(s,iov,3))>0){
		if(read(s,respuesta,sizeof(respuesta))>0){
			close(s);
			return atoi(respuesta);
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
	char *opcion=(char*)malloc(2*sizeof(char));
	char *sizeof_cola_s=intToString(strlen(cola));
	opcion[0]='p';opcion[1]='\0';
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';ssize_t nwritten;
	struct iovec iov[5];
	iov[0].iov_base=opcion;
	iov[0].iov_len=2;
	iov[1].iov_base=sizeof_cola_s;
	iov[1].iov_len=TAM_LONG;
	/*
	 AQUI EL TAMANIO VARIABLE DE LA COLA
	*/
	iov[2].iov_base=cola;
	iov[2].iov_len=atoi(sizeof_cola_s)+1;
	/*
	AQUI SOLAMENTE HE ENVIADO UNA TRANSMISION
	*/
	/*
	iov[3].iov_base="0\0";
	iov[3].iov_len=2;
	*/
	char * sizeof_mensaje_s;
	sizeof_mensaje_s=intToString((long)tam);
	iov[3].iov_base=sizeof_mensaje_s;
	iov[3].iov_len=TAM_LONG;
	iov[4].iov_base=((char*)mensaje);
	iov[4].iov_len=((int)tam)+1;
	while((nwritten=writev(s,iov,5))>0){
		if(read(s,respuesta,sizeof(respuesta))>0){
			//close(s);
			return atoi(respuesta);
		}
	}
	return atoi(respuesta);
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
	int i;
	int s=obtenerSocket();
	//
	char sizeof_mensaje_s[TAM_LONG];
	//inicializar cadena
	for( i=0;i<TAM_LONG;i++){
		sizeof_mensaje_s[i]='\0';
	}
	char respuesta[4];
	respuesta[0]='-';respuesta[1]='1';respuesta[2]='\0';
	//
	int leido;
	//send opc
	char opcion[2];
	if (blocking){
		opcion[0]='b';
	}else{
		opcion[0]='g';
	}
	opcion[2]='\0';
    if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		//send sizeof
		char * sizeof_cola_s;
		sizeof_cola_s=intToString(strlen(cola));
    	if ((leido=read(s, respuesta,sizeof(respuesta)))>0) {
		//send cola
    	if((leido=read(s, sizeof_mensaje_s,TAM_LONG))>0) {
			if(sizeof_mensaje_s[0]!='\000'){ 
				//printf("sizeof_mensaje : %d\n",(int)strlen(sizeof_mensaje_s));
				int sizeof_mensaje= atoi(sizeof_mensaje_s);
				char mensaje_s[sizeof_mensaje+1];
				for( i=0;i<sizeof_mensaje+1;i++){
					mensaje_s[i]='\0';
				}
				*mensaje=(char*)malloc(sizeof_mensaje*sizeof(char));
				*tam=(uint32_t )sizeof_mensaje;
				if((leido=read(s,mensaje_s,sizeof_mensaje)>0)){
					//printf("cadena : %s\n",mensaje_s);
					strcpy(*mensaje,mensaje_s);
					respuesta[0]='0';respuesta[1]='\0';
					return atoi(respuesta);
					//mensaje=(void **)&mensaje_s;
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

