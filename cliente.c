#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/*==========CLIENTE==========*/
int main(int argc, char** argv) {
    int socket_cliente;
    struct sockaddr_in ipportserv;
    socklen_t size = sizeof(struct sockaddr_in);
    char msg[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    int puerto;

    if(argc == 2) {
        puerto = atoi(argv[1]);
    } else {
        puerto = 6666;
    }

    if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
    
    char IP_text[]="127.0.0.1";
    if(inet_pton(AF_INET,IP_text,(void *) &ipportserv.sin_addr)!=1)
    {
      perror("Formato de dirección incorrecto\n");
      exit(EXIT_FAILURE);
      
    }
    
    ipportserv.sin_family = AF_INET;
    ipportserv.sin_port = htons(puerto);
    
    if (connect(socket_cliente, (struct sockaddr *) &ipportserv, size) < 0) {
        perror("ERROR conectando al servidor\n");
        exit(EXIT_FAILURE);
    }
    
    if(recv(socket_cliente,msg,sizeof(msg),0)<0)
    {
      perror("ERROR recibiendo los datos\n");
      exit(EXIT_FAILURE);
    }
    
    printf("El mensaje es: %s\n",msg);
    
    close(socket_cliente);
    
    return(EXIT_SUCCESS);

}
