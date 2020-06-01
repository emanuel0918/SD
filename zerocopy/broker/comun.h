/*
 * Incluya en este fichero todas las definiciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */
#include <sys/socket.h>
//#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define TAM_LONG 10
//#define TAM_COLA 256*256
//#define TAM_MENSAJE 32
//#define VLEN 10
//#define BUFSIZE 200
//#define TIMEOUT 1

//---------------------------------------------------------------------------------------------

int obtenerSocket();
//---------------------------------------------------------------------------------------------

char * scan();
char* subString (const char* input, int offset, int len, char* dest);
int stringToInt(char * s);
char * intToString(long n);
char * reverseString(char *s,int l);
char rand_char();

