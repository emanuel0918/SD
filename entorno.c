#include<stdio.h>
#include<stdlib.h>

int scan(char **cadena);
int main(int argc, char *argv[]){
 char * cadena;
 scan(&cadena); 
 printf("Variable de entorno de %s:\n%s\n",cadena,getenv(cadena));
 return 0;
}

int
scan (char **cadena)
{
  char c;
  int i = 0;
  *cadena = (char *) malloc (sizeof (char));
  setbuf (stdin, NULL);
  while (c != '\n')
    {
      c = getc (stdin);
      if (c != '\n')
	{
	  (*cadena)[i] = c;
	  i++;
	  *cadena = (char *) realloc (*cadena, sizeof (char) * (i + 1));
	}
    }
  (*cadena)[i] = '\0';
  return i;
}
