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
    int socket_serv, socket_conex;    // Identificadores del socket del servidor y el de conexion
    struct sockaddr_in ipportserv;    // Estructura para almacenar la dirección del servidor
    socklen_t size = sizeof(struct sockaddr_in);
    char msg[1024];   // Buffer para el mensaje a enviar y recibir
    int puerto;   // Puerto del servidor al que se conecta el cliente
    
    if (argc == 2) {    // Comprobamos que se introduzca el numero de argumentos correcto en línea de comandos
        puerto = atoi(argv[1]);   // Primer argumento: puerto
    } else {
        puerto = 6666;    // Puerto por defecto
    }
    
    
    /*Creación del socket del servidor
        domain: AF_INET para IPv4
        style: SOCK_STREAM para orientado a conexion (TCP)
        protocol: por defecto, 0  
    */
    if ((socket_serv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
    
    // Inicializacion de la estructura sockaddr_in    
    ipportserv.sin_family = AF_INET;    // Familia de direcciones: IPv4
    ipportserv.sin_addr.s_addr = htonl(INADDR_ANY);   // Acepta cualquier direccion IP
    ipportserv.sin_port = htons(puerto);    // Convertir el puerto de orden de host a orden de red, short
    
    
    /*Asignamos una direccion al socket con bind
        socket: identificador del socket
        addr: puntero a struct sockaddr con la direccion a asignar (ipportserv)
        length: tamaño de la estructura addr
    */
    if (bind(socket_serv, (struct sockaddr *) &ipportserv, sizeof(struct sockaddr_in)) < 0) {
        perror("No se pudo asignar la dirección");
        exit(EXIT_FAILURE);
    }
    
    
    /*Marcamos el socket como pasivo con listen
        socket: identificador del socket
        n: numero peticiones que se pueden atender a la vez
    */
    if(listen(socket_serv, 10) < 0) {
        perror("No se pudo marcar el socket como pasivo");
        exit(EXIT_FAILURE);
    }

    while(1) {
    
        struct sockaddr_in ipportcli; // Estructura para guardar la direccion del cliente que se conecte
        
        /*Aceptamos la conexion de un cliente (esperamos a que solicite) con accept
            socket: identificador del socket de servidor
            addr: puntero a struct sockaddr donde se guarda la direccion del cliente que se ha conectado
            length_ptr: puntero al tamaño de la estructura addr (como entrada), y espacio real consumido (como salida)
        */
        if ((socket_conex = accept(socket_serv, (struct sockaddr *) &ipportcli, &size)) < 0) {
            perror("No se pudo aceptar la conexión");
            close(socket_conex);
        }

        char ipcli[INET_ADDRSTRLEN];    // Cadena para guardar la IP del cliente
        
        /*Convertir la dirección IP de binario a texto
            af: AF_INET para IPv4
            src: puntero a una struct in_addr (para IPv4)
            dst: puntero a cadena donde se guarda el resultado
            size: tamaño en bytes de la cadena destino
        */
        if (inet_ntop(AF_INET, (const void*) &(ipportcli.sin_addr), ipcli, INET_ADDRSTRLEN) != NULL){
            printf("Se conectó cliente con IP: %s ", ipcli);    //Sacamos por pantalla la ip del cliente, en formato de texto
        }
        
        uint16_t puertocli = ntohs(ipportcli.sin_port);   // Sacamos por pantalla el puerto del cliente, pasado de orden de red a orden de host
        printf("Puerto: %d\n", puertocli);
        
        while(1){
        
            /*Recibimos la linea del cliente con recv
                socket: identificador del socket de conexion
                buffer: puntero a donde se va a guardar el mensaje
                size: numero maximo de bytes a recibir
                flags: opciones de recepcion, por defecto 0
            */
            if(recv(socket_conex,msg,sizeof(msg),0)<0)    //Recibe línea en minusculas del cliente
            {
                perror("ERROR recibiendo los datos\n");
                exit(EXIT_FAILURE);
            }
        
            
            for (int i = 0; msg[i] != '\0'; i++)    //Recorremos hasta encontrar un fin de palabra
            {
                msg[i] = toupper(msg[i]);   //Pasamos a mayuscula el caracter
            }
            
            
            /*Enviamos la linea en mayusculas al cliente con send
                socket: identificador del socket de conexion
                buffer: puntero al mensaje
                size: numero de bytes a enviar (longitud de la cadena)
                flags: opciones de envio, por defecto 0
            */
            if(send(socket_conex, msg, strlen(msg), 0) < 0) {
                perror("No se pudo enviar el mensaje");
                close(socket_conex);
            }

        }

        close(socket_conex);    //Cerramos el socket de conexion al acabar la conexion

    }
    
    close(socket_serv);   //Al acabar el servidor cerramos el socket de servidor

    return(EXIT_SUCCESS);
}
