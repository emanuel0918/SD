#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
int stringToInt(char *s);
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
 send(cliente,"Ingresa el tamnio del buffer: ",30,0);
 //----------------------------------
 //uint32_t tam;
 char *buffer =malloc(3);
 int bytesRecibidos=recv(cliente,buffer,2,0);
 //int bytesRecibidos=recv(cliente,buffer,4,MSG_WAITALL);
 if(bytesRecibidos<0){
  perror("El chabon se desconecto o bla\n");
  return 1;
 }
 buffer[bytesRecibidos]='\0';
 int tam=stringToInt(buffer);
 //printf("Me llegaron %d bytes con %s\n",bytesRecibidos,buffer);
 printf("Me llegaron %d bytes con %d\n",bytesRecibidos,tam);
 free(buffer);

 char *buf =malloc(tam);
 int bytesRecibidos2=recv(cliente,buf,(tam-1),0);
 //int bytesRecibidos=recv(cliente,buf,4,MSG_WAITALL);
 if(bytesRecibidos2<0){
  perror("El chabon se desconecto o bla\n");
  return 1;
 }
 buf[bytesRecibidos2]='\0';
 printf("Me llegaron %d bytes con %s\n",bytesRecibidos2,buf);
 
 free(buf);
 //for(;;);
 return 0;
}
int stringToInt(char *s){
 int d=0;
 int i=0;
 int j=0;
 int l=0;
 int diez=1;
 int n=0;
 char c=0;
 while((c=s[i])!='\0'){
  l++;
  i++;
 }
 //printf("tamano es %d\n",l);
 i=0;
 while((c=s[i])!='\0'){
  //printf("%d",(c-48));
  n=c-48;
  for(j=0,diez=1;j<(l-(i+1));j++){
   diez*=10;
  }
  d+=n*diez;
  i++;
 }
 //printf("El valor es %d\n",d);
 return d;
}
