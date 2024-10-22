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
    int socket_cliente;   // Identificador del socket del cliente
    struct sockaddr_in ipportserv;    // Estructura para almacenar la dirección del servidor
    socklen_t size = sizeof(struct sockaddr_in);
    char msg[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  // Cliente para el mensaje a enviar y recibir
    int puerto; // Puerto del servidor al que se conecta
    
    // Comprobamos que se introduzca el numero de argumentos correcto en línea de comandos
    if(argc == 2) {
        puerto = atoi(argv[1]);   // Primer argumento: puerto (convertido de string a entero)
    } else {
        // Valores por defecto si no se proporcionan argumentos
        printf("No se proporcionaron argumentos correctos, trabajaremos con valores por defecto: 6666\nSi se quisieran introducir argumentos, hacer: ./ejecutable puerto\n");
        puerto = 6666;    // Puerto por defecto
    }
    
    /*Creación del socket del cliente
        domain: AF_INET para IPv4
        style: SOCK_STREAM para orientado a conexion (TCP)
        protocol: por defecto, 0
    */
    if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
    
    
    char IP_text[]="127.0.0.1";
    /*Convertir la dirección IP de texto a formato binario
        af: AF_INET para IPv4
        src: puntero a cadena de texto con la IP
        dst: puntero a struct in_addr donde se almacenará el resultado
    */
    if(inet_pton(AF_INET,IP_text,(void *) &ipportserv.sin_addr)!=1)
    {
        perror("Formato de dirección incorrecto\n");
        close(socket_cliente);
        exit(EXIT_FAILURE);
    }
    
    // Inicializacion de la estructura sockaddr_in
    ipportserv.sin_family = AF_INET;  // Familia de direcciones: IPv4
    ipportserv.sin_port = htons(puerto); // Convertir el puerto de orden de host a orden de red, short
    
    
    /*Solicitamos conexion al servidor
        socket: el socket del cliente
        addr: puntero a struct sockaddr con la IP y puerto del servidor, hay que convertirlo a tipo generico
        length: tamaño de la estructura sockaddr
    */
    if (connect(socket_cliente, (struct sockaddr *) &ipportserv, size) < 0) {
        perror("ERROR conectando al servidor\n");
        close(socket_cliente);
        exit(EXIT_FAILURE);
    }
    
    /*Recibimos el mensaje del servidor con recv
            socket: identificador del socket del cliente
            buffer: puntero a donde se va a guardar el mensaje
            size: numero maximo de bytes a recibir
            flags: opciones de recepcion, por defecto 0
        */
    if(recv(socket_cliente,msg,sizeof(msg),0)<0)
    {
        perror("ERROR recibiendo los datos\n");
        close(socket_cliente);
        exit(EXIT_FAILURE);
    }
    
    // Sacamos por pantalla el mensaje recibido
    printf("El mensaje es: %s\n",msg);
    
    // Cerramos el socket al acabar el intercambio
    close(socket_cliente);
    return(EXIT_SUCCESS);

}
