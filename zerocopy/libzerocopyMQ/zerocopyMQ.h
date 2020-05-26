/*
 *
 * NO MODIFICAR
 *
 */
#include <stdbool.h>
#include <stddef.h>

int createMQ(const char *cola);
int destroyMQ(const char *cola);

int put(const char *cola, const void *mensaje, uint32_t tam);
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking);

int auxiliar(const char *opc,const char *cola,const char *mensaje);

int obtenerSocket();

//int obtenerSocket();
void prepararMensaje(char ** mensaje,const char *cola,char opc);