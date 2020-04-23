/*
 * Incluya en este fichero todas las definiciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define TAM 4
char * scan();
int stringToInt(char * s);
char * intToString(long n);
char * reverseString(char *s,int l);
char rand_char();
