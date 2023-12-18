#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 4897
#define QLEN 200

void main()
{
  struct sockaddr_in servidor;
  struct sockaddr_in cliente;
  struct hostent* info_cliente;
  int fd_s, fd_c;
  int longClient;
  char buf[256];
  int n;
  int num_alumnos = 0;

  printf( "1. Se llama a la funcion socket(...) y devuelve el file descriptor del servidor fd_s\n" );
  printf( "fd_s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);\n\n" );
  fd_s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	      
  printf( "2. Se inicializan los atributos de la variable struct sockaddr_in llamada servidor\n\n" );
  memset((char *) &servidor, 0, sizeof(servidor));
  servidor.sin_family = AF_INET;
  servidor.sin_addr.s_addr = INADDR_ANY;
  servidor.sin_port = htons((u_short) PORT);
  memset(&(servidor.sin_zero), '\0', 8);

  printf( "3. bind(fd_s, (struct sockaddr *) &servidor, sizeof(servidor));\n\n" );
  bind(fd_s, (struct sockaddr *) &servidor, sizeof(servidor));

  printf( "4. listen(fd_s, QLEN);\n\n" );
  listen(fd_s, QLEN);

  longClient = sizeof(cliente);

  while(1)
  {
    printf( "------------ inicia while(1) --------\n" );
    printf( "La variable num_alumnos vale: %d\n\n", num_alumnos );
    printf( "5. La funcion accept() recibe como argumento el file descriptor del servidor y recibe tambien\n" );
    printf( "la struct sockaddr_in del cliente y devuelve el file descriptor del cliente\n" );
    printf( "fd_c = accept(fd_s, (struct sockaddr *) &cliente, &longClient);\n" );
    printf( "La funcion accept(...) se quedará ahi indefinidamente hasta que se conecte un cliente\n\n" );
    fd_c = accept(fd_s, (struct sockaddr *) &cliente, &longClient);

    printf( "6. Se llama a la funcion gethostbyaddr(...) para obtener la informacion\n" );
    printf( "propia del cliente y lo que devuelve la funcion, se asigna a la variable\n" );
    printf( "de tipo struct hostent* llamada info_cliente:\n" );
    printf( "info_cliente = gethostbyaddr((char *) &cliente.sin_addr, sizeof(struct in_addr), AF_INET);\n\n" );
    info_cliente = gethostbyaddr((char *) &cliente.sin_addr, sizeof(struct in_addr), AF_INET);

    printf( "7. Gracias a la sentencia anterior, por ejemplo, podemos saber el nombre del host cliente\n");
    printf( "info_cliente -> h_name\n" );
    printf("Conectado desde: %s\n\n", info_cliente -> h_name);

    printf( "8. Se incrementa en 1 la variable num_alumnos\n\n" );
    num_alumnos++;

    printf( "9. Como el alumno va a enviar su nombre desde el cliente, yo servidor voy a recibir con recv(...)\n" );
    n = recv(fd_c, buf, sizeof(buf), 0);
    printf("numero real de bytes leidos=%d\n",n);
    write(1, buf, n);
    printf( "\n" );

    printf( "Y se le envia al cliente la frase \"Alumno registrado\", con la funcion send(...)\n\n" );
    char *respuesta = "Alumno registrado";
    send(fd_c, respuesta, strlen(respuesta), 0);

    printf( "10. Se cierra el file descriptor del cliente. IMPORTANTE: el file descriptor del servidor no se cierra dentro del while\n" );
    printf( "Debe hacerse asi para que en el siguiente ciclo o loop, se empiece con la llamada a la función accept()\n" );
    printf( "close(fd_c);\n" );
    close(fd_c);
    printf( "------------ termina un ciclo del while(1) --------\n\n" );
  }
  printf( "11. Ya afuera del while, por ultimo, ahora si se cierra el file descriptor del servidor\n" );
  printf( "close(fd_s);\n" );
  close(fd_s);
  shutdown( fd_s, SHUT_RDWR );
  exit(0);
}
