#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"

int main(int argc, char *argv[]){

    if(argc!=2) {
        fprintf(stderr, "Uso: %s puerto\n", argv[0]);
        return 1;
    }
    char *cadena;
    struct sockaddr_in direccionBroker;
    direccionBroker.sin_family=PF_INET;
    direccionBroker.sin_addr.s_addr=INADDR_ANY;
    direccionBroker.sin_port=htons(atoi(argv[0]));
    int brk=socket(PF_INET,SOCK_STREAM,0);
    int activado=1;
    setsockopt(brk,SOL_SOCKET,SO_REUSEADDR,&activado,sizeof(activado));
    if(bind(brk,(void*)&direccionBroker,sizeof(direccionBroker))!=0){
     return 1;
    }
    listen(brk,100);
    cadena= scan();
    printf("%s\n",cadena);
    return 0;
}
