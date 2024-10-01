#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"

/*==========SERVIDOR==========*/
int main(int argc, char** argv) {
    int socket_serv, socket_conex;
    struct sockaddr_in ipportserv;
    socklen_t size;
    char* msg = "hola";
    int str_len = strlen(msg);

    if ((socket_serv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
    
    ipportserv .sin_family = AF_INET;
    ipportserv.sin_addr.s_addr = htonl(INADDR_ANY);
    ipportserv.sin_port = htons(6666);

    if (bind(socket_serv, (struct sockaddr *) &ipportserv, sizeof(struct sockaddr_in)) < 0) {
        perror("No se pudo asignar la dirección");
        exit(EXIT_FAILURE);
    }

    if(listen(socket_serv, 10) < 0) {
        perror("No se pudo marcar el socket como pasivo");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in ipportcli;
    if ((socket_conex = accept(socket_serv, (struct sockaddr *) &ipportcli, &size)) < 0) {
        perror("No se pudo aceptar la conexión");
        exit(EXIT_FAILURE);
    }




    return 0;
}
