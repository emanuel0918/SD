/*
 * Incluya en este fichero todas las implementaciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */
#include "comun.h"
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

char *
scan ()
{
  char * cadena;
  char c;
  int i = 0;
  cadena = (char *) malloc (sizeof (char));
  setbuf (stdin, NULL);
  while (c != '\n')
    {
      c = getc (stdin);
      if (c != '\n')
	{
	  cadena[i] = c;
	  i++;
	  cadena = (char *) realloc (cadena, sizeof (char) * (i + 1));
	}
    }
  cadena[i] = '\0';
  return cadena;
}
