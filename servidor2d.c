#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"

/*==========SERVIDOR==========*/
int main(int argc, char** argv) {
    int socket_serv, socket_conex;
    struct sockaddr_in ipportserv;
    socklen_t size = sizeof(struct sockaddr_in);
    char *msg1 = "hola1";
    char *msg2 = "hola23fd";
    int str_len1 = strlen(msg1);
    int str_len2 = strlen(msg2);
    int puerto;

    if(argc == 2) {
        puerto = atoi(argv[1]);
    } else {
        puerto = 6666;
    }

    if ((socket_serv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
    
    ipportserv.sin_family = AF_INET;
    ipportserv.sin_addr.s_addr = htonl(INADDR_ANY);
    ipportserv.sin_port = htons(puerto);

    if (bind(socket_serv, (struct sockaddr *) &ipportserv, sizeof(struct sockaddr_in)) < 0) {
        perror("No se pudo asignar la dirección");
        exit(EXIT_FAILURE);
    }

    if(listen(socket_serv, 10) < 0) {
        perror("No se pudo marcar el socket como pasivo");
        exit(EXIT_FAILURE);
    }

    while(1) {
    
        struct sockaddr_in ipportcli;
        if ((socket_conex = accept(socket_serv, (struct sockaddr *) &ipportcli, &size)) < 0) {
            perror("No se pudo aceptar la conexión");
            close(socket_conex);
        }

        char ipcli[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, (const void*) &(ipportcli.sin_addr), ipcli, INET_ADDRSTRLEN) != NULL){
            printf("Se conectó cliente con IP: %s ", ipcli);
        }
        uint16_t puertocli = ntohs(ipportcli.sin_port);
        printf("Puerto: %d\n", puertocli);

        if(send(socket_conex, msg1, str_len1+1, 0) < 0) {
            perror("No se pudo enviar el mensaje");
            close(socket_conex);
        }

        if(send(socket_conex, msg2, str_len2+1, 0) < 0) {
            perror("No se pudo enviar el mensaje");
            close(socket_conex);
        }
    
        close(socket_conex);
    }
    
    close(socket_serv);

    return 0;
}
