//Librerias
#include <unistd.h>  // Manejo de daots
#include <netdb.h> 
#include <netinet/in.h>     //Informacion de red de socket
#include <sys/socket.h> //Funciones de socket
#include <sys/types.h>  
#include <arpa/inet.h>  // Informacion de socket
#include <errno.h>  // Datos de error
#include <stdio.h> // Entrada y salida de datos
#include <string.h>  // Funciones de cadenas
#include <pthread.h> // Hilos

#define port        34567           // puerto
#define host        "127.0.0.1"     // ip host
#define len_buffer  1024            // tamaño buffer
#define max_clients 5               // numero maximo de clientes

void *recibir_mensajes(void *); // Funcion para recibir los mensajes - hilo

int sk, sk_conn; // identificador de los socket
int clientes[max_clients]; // arreglo de los clientes
unsigned int len_client;    // tamaño de la strutura client
struct sockaddr_in server_socket, client; // estructuras de los sockets

char buffer_recv[len_buffer]; // Buffer para recepcion de datos
int i; // Id. del cliente por llegada al servidor

int main(){            
    //Crea el socket servidor
    if((sk = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        return 0;
    }
    // Reserva memoria para el socket
    memset(&server_socket, 0, sizeof(server_socket));
    // Informacion para el socket servidor
    server_socket.sin_family      = AF_INET; // Asigna familia de protoclos
    server_socket.sin_addr.s_addr = inet_addr(host); // Asigna la direccion IP
    server_socket.sin_port        = htons(port); // Asigna el puerto
    //Nombra al socket servidor
    if ((bind(sk, (struct sockaddr *)&server_socket, sizeof(server_socket))) != 0){ 
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
        return 0;
    }
    //Pone a escuchar al socket servidor
    if((listen(sk, max_clients)) == -1){ 
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        return 0;
    } else {
        printf("[SERVER-C]: Escuchando en %s:%d \n\n", inet_ntoa(server_socket.sin_addr), ntohs(server_socket.sin_port)); 
    }
    // Inicializa el numero de clientes
    i = 0;
    while(1){
        memset(&client, 0, sizeof(client)); // Reserva memoria para el socket cliente
        // Proceso al aceptar un cliente
        while(sk_conn = accept(sk, (struct sockaddr *)&client, &len_client)){ 
            clientes[i] = sk_conn;
            printf("[SERVER-C]: Conexion desde %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));            
            pthread_t recibir_thread; // Declara id de hilo
            i++; // Incrementa numero de cliente
            pthread_create(&recibir_thread, NULL, recibir_mensajes, (void*)&sk_conn); // Creacion de hilo para recibir mensajes
        } 
    }   
} 

// Funcion para recibir los mensajes de los clientes
void *recibir_mensajes(void *socket){
    int sock = *(int*)socket; //Conversion a entero
    int j = i; // Guarda el numero de cliente asignado al hilo
    char t[1]; 
    char aux[len_buffer]; // buffer auxiliar
    memset(&buffer_recv, 0, len_buffer); // REserva memoria para el buffer de recepcion
    memset(&aux, 0, len_buffer); // Reserva memoria para el buffer auxiliar
    while(recv(sock, buffer_recv, sizeof(buffer_recv), 0) > 0){ // Proceso al recibir datos por el socket
        sprintf(t, "%d", j); // Convierte entero a char
        strcpy(aux,buffer_recv); // Resguarda info de buffer de recepcion en el buffer auxiliar
        strcpy(buffer_recv,"["); // Agrega el numero de cliente al buffer
        strcat(buffer_recv,t);  
        strcat(buffer_recv,"]");
        strcat(buffer_recv,aux);
        printf("%s",buffer_recv); // Imprime buffer 
        for (int k = 0; k < i; k++){ // Manda el mensaje recibido a los demas clientes conectados
            if (clientes[k] != sock){
                send(clientes[k], buffer_recv, sizeof(buffer_recv), 0);
            }
        }
        memset(&buffer_recv, 0, len_buffer); // Limpia buffer de recepcion
        memset(&aux, 0, len_buffer);  // Limpia buffer auxiliar
    }  
} 
