#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> //the L2 protocols
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>


unsigned char MAC[6], MASK[4], IP[4];

void obtenerDatos(int ds) //ds = Descriptor de socket
{
 int i=0;
 char nombre[10];
 struct ifreq nic; //man netdevice
 printf("\nIntroduce la interfaz de red: ");
 scanf("%s",nombre);
 strcpy(nic.ifr_name,nombre);
 
 //Obtener el indice
 if(ioctl(ds,SIOCGIFINDEX, &nic)==-1){
  perror("\nError al obtener el indice");
 }else{
  printf("\nEl indice es: %d\n",nic.ifr_ifindex);
 }

 
 //Obtener MAC SIOCGIFHWADDR
 if(ioctl(ds,SIOCGIFHWADDR, &nic)==-1){
  perror("\nError al obtener la MAC");
 }else{
  memcpy(MAC,nic.ifr_hwaddr.sa_data,6);
  printf("\nMAC: ");
  for(int i=0;i<6;i++){
   printf("%2x:",MAC[i]);
  }
  printf("\n");
 }

 //Obtener SIOCGIFADDR
 if(ioctl(ds,SIOCGIFADDR, &nic)==-1){
  perror("\nError al obtener la IP");
 }else{
  memcpy(IP,nic.ifr_hwaddr.sa_data,6);
  printf("\nIP: ");
  for(int i=2;i<6;i++){
   printf("%2d.",IP[i]);
  }
  printf("\n");
 }

 //Obtener MASK SIOCGIFNETMASK 
 if(ioctl(ds,SIOCGIFNETMASK, &nic)==-1){
  perror("\nError al obtener la mascara");
 }else{
  memcpy(MASK,nic.ifr_hwaddr.sa_data,6);
  printf("\nMASK: ");
  for(int i=2;i<6;i++){
   printf("%2d.",MASK[i]);
  }
  printf("\n");
 }
}
int main(){
 int packet_socket;
 packet_socket=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
 if(packet_socket ==-1){
  perror("Error al abrir el socket\n");
  return 1;

 }else{
  printf("Socket creado\n");
  obtenerDatos(packet_socket);
 }

 close(packet_socket);
 return 0;
}
