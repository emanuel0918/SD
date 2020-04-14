#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
int main(void){
 struct sockaddr_in direccionServidor;
 direccionServidor.sin_family=AF_INET;
 direccionServidor.sin_addr.s_addr=INADDR_ANY;
 direccionServidor.sin_port=htons(8080);
 int servidor=socket(AF_INET,SOCK_STREAM,0);
 if(bind(servidor,(void*)&direccionServidor,sizeof(direccionServidor))!=0){
  perror("Fallo el bind\n");
  return 1;
 }
 printf("Estoy escuchando\n");
 for(;;);
 return 0;
}
