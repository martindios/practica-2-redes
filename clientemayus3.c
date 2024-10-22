#include <ctype.h>
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
    char name_file[50], IP_text[INET_ADDRSTRLEN];
    char msg[1024];
    int puerto;

    if(argc == 4) {
        strcpy(name_file, argv[1]);
        strcpy(IP_text, argv[2]);
        puerto = atoi(argv[3]);
    } else {
        strcpy(name_file, "file.txt");
        strcpy(IP_text, "127.0.0.1");
        puerto = 6666;
    }

    if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
    
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
    
    FILE *entrada;
    FILE *salida;
    entrada=fopen(name_file,"r");
    for (int i = 0; name_file[i] != '\0'; i++) {
        name_file[i] = toupper(name_file[i]);
    }
    salida=fopen(name_file,"w");
   
    size_t len = 0;
    while(fgets(msg,sizeof(msg), entrada)!=NULL){ //Mientras no llega al final del archivo
        len = strlen(msg);

        if(send(socket_cliente, msg, len+1, 0) < 0)   //Envía línea al servidor
        {
            perror("No se pudo enviar el mensaje");
            close(socket_cliente);
            fclose(entrada);
            fclose(salida);
            exit(EXIT_FAILURE);
        }
      
      if(recv(socket_cliente,msg,sizeof(msg),0)<0)    //Recibe línea en mayúsculas del servidor y la escribe en el archivo de salida
      {
            perror("ERROR recibiendo los datos\n");
            close(socket_cliente);
            fclose(entrada);
            fclose(salida);
            exit(EXIT_FAILURE);
      }
      fputs(msg, salida);
        
    }
    
    close(socket_cliente);
    fclose(entrada);
    fclose(salida);
    
    return(EXIT_SUCCESS);

}
