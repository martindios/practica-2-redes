#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include <ctype.h>

/*==========SERVIDOR==========*/
int main(int argc, char** argv) {
    int socket_serv, socket_conex;
    struct sockaddr_in ipportserv;
    socklen_t size = sizeof(struct sockaddr_in);
    char msg[1024];
    int puerto;

    if (argc == 2) {
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
    
        //printf("ANtes accepts\n");
        struct sockaddr_in ipportcli;
        if ((socket_conex = accept(socket_serv, (struct sockaddr *) &ipportcli, &size)) < 0) {
            perror("No se pudo aceptar la conexión");
            close(socket_conex);
        }
        //printf("Despues accept\n");

        char ipcli[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, (const void*) &(ipportcli.sin_addr), ipcli, INET_ADDRSTRLEN) != NULL){
            printf("Se conectó cliente con IP: %s ", ipcli);
        }
        uint16_t puertocli = ntohs(ipportcli.sin_port);
        printf("Puerto: %d\n", puertocli);
        
        while(1){
            if(recv(socket_conex,msg,sizeof(msg),0)<0)    //Recibe línea en minusculas del cliente
            {
                perror("ERROR recibiendo los datos\n");
                exit(EXIT_FAILURE);
            }
        
            for (int i = 0; msg[i] != '\0'; i++) 
            {
                msg[i] = toupper(msg[i]);
            }
        
            if(send(socket_conex, msg, strlen(msg), 0) < 0) {
                perror("No se pudo enviar el mensaje");
                close(socket_conex);
            }

        }

        close(socket_conex);

    }
    
    close(socket_serv);

    return 0;
}
