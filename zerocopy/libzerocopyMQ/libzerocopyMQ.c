#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"

int createMQ(const char *cola) {
	int s;
	struct sockaddr_in dir;
	struct hostent *host_info;
	char buf[TAM];
	if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("error creando socket");
		return 1;
	}

	host_info=gethostbyname("127.0.0.1");
	// 2 alternativas
	//memcpy(&dir.sin_addr.s_addr, host_info->h_addr, host_info->h_length);
	dir.sin_addr=*(struct in_addr *)host_info->h_addr;
	dir.sin_port=htons(8080);
	dir.sin_family=PF_INET;
	if (connect(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error en connect");
		close(s);
		return 1;
	}
	//
	
	char mensaje[3];
	//nombre de la cola
	mensaje[0]=cola[0];
	//flag de op
	mensaje[1]='c';
	mensaje[2]='\0';
	send(s,mensaje,strlen(mensaje),0);
	//
	return 0;
}
int destroyMQ(const char *cola){
    return 0;
}
int put(const char *cola, const void *mensaje, uint32_t tam) {
    return 0;
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
    return 0;
}
