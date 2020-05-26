#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"


//---------------------------------------------------------------------------------------------

int createMQ(const char *cola) {
	int s=obtenerSocket();
	
	//send opc
	send(s,"c\0",2*sizeof(char),0);
	//send cola
	send(s,cola,strlen(cola),0);

	char respuesta[4];
	read(s,respuesta,(4*sizeof(char)));
	close(s);
	int resp=atoi(respuesta);
	
	return resp;
}
int destroyMQ(const char *cola){
	int s=obtenerSocket();
	
	//send opc
	send(s,"d\0",2*sizeof(char),0);
	//send cola
	send(s,cola,strlen(cola),0);

	char respuesta[4];
	read(s,respuesta,(4*sizeof(char)));
	close(s);
	int resp=atoi(respuesta);
	return resp;
}
int put(const char *cola, const void *mensaje, uint32_t tam) {
    return 0;
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
    return 0;
}

//---------------------------------------------------------------------------------------------

int obtenerSocket(){
	int s;
	struct sockaddr_in dir;
	struct hostent *host_info;
	if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("error creando socket");
		return -1;
	}
	host_info=gethostbyname(getenv("BROKER_HOST"));
	// 2 alternativas
	//memcpy(&dir.sin_addr.s_addr, host_info->h_addr, host_info->h_length);
	dir.sin_addr=*(struct in_addr *)host_info->h_addr;
	dir.sin_port=htons(atoi(getenv("BROKER_PORT")));
	dir.sin_family=PF_INET;
	if (connect(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error en connect");
		close(s);
		return -1;
	}
	//
	return s;

}



