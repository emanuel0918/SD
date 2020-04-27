#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
int main(void){
 struct sockaddr_in direccionServidor;
 direccionServidor.sin_family=AF_INET;
 direccionServidor.sin_addr.s_addr=INADDR_ANY;
 direccionServidor.sin_port=htons(8080);
 int servidor=socket(AF_INET,SOCK_STREAM,0);
 int activado =1;
 setsockopt(servidor,SOL_SOCKET,SO_REUSEADDR,&activado,sizeof(activado));
 if(bind(servidor,(void*)&direccionServidor,sizeof(direccionServidor))!=0){
  perror("Fallo el bind\n");
  return 1;
 }
 printf("Estoy escuchando\n");
 listen(servidor,100);
 //----------------------------------
 struct sockaddr_in direccionCliente;
 //unsigned int tamanoDireccion = sizeof(struct sockaddr_in);
 unsigned int tamanoDireccion;
 int cliente = accept(servidor,(void*) &direccionCliente,&tamanoDireccion);
 printf("Recibi una conexion en %d\n",cliente);
 send(cliente,"Hola NetCat!\n",14,0);
 send(cliente, ":)\n",4,0);
 for(;;);
 return 0;
}