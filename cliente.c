//Librerias
#include <netdb.h> 
#include <stdio.h> // Entrada y salida de datos
#include <stdlib.h> 
#include <string.h> // Funciones de cadena
#include <sys/socket.h> // Funciones de socket
#include <unistd.h> //Manejo de datos
#include <netinet/in.h> //Informacion de socket
#include <arpa/inet.h>  //Informacion de red de socket
#include <pthread.h> // Hilos

#define host        "127.0.0.1" // ip host
#define len_buffer  1024        // tamaño de buffer

void *recibir_mensajes(void *); //Funciones para recibir los mensajes - hilo

int port; // puerto del socker servidor  
char buffer_recv[len_buffer]; // buffer de recepcion
char buffer_send[len_buffer]; // buffer de envio
int sk; // identificador de socket

struct sockaddr_in server_socket, client; // estructura de sockets 

int main(){ 
    printf("SERVIDOR\tPUERTO\nJAVA\t\t12345\nPYTHON\t\t23456\nC\t\t34567\n\nPUERTO -> ");
    scanf("%d", &port); // obtiene puerto
    printf("\n");
    // crea el socket
    if ((sk = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
        printf("[CLIENT-C]: socket creation failed...\n"); 
        return 0;  
    } 
    memset(&server_socket, 0, sizeof(server_socket)); // reserva memoria para el socket servidor
    memset(&client, 0, sizeof(client)); // reserva memoria para el socket cliente
    // informacion para el socket servidor
    server_socket.sin_family = AF_INET; // ASigna la familia de protocolos
    server_socket.sin_addr.s_addr = inet_addr(host); // Asigna dir IP
    server_socket.sin_port = htons(port); // Asigna puerot
    // Conecta el cliente al servidor
    if (connect(sk, (struct sockaddr*)&server_socket, sizeof(server_socket)) != 0) {   
        return 0;
    } 
    printf("[CLIENT-C]: Aceptado por %s:%d \n", inet_ntoa(server_socket.sin_addr), ntohs(server_socket.sin_port)); 
    pthread_t recibir_thread, enviar_thread; //id para los hilos
    pthread_create(&recibir_thread, NULL, recibir_mensajes, (void*)&sk); // Crea eñ hilo par recepcion de mensajes        
    
    while(1){// Proceso para envio de mensajes
        memset(buffer_send, 0, len_buffer); // Reserva memoria para buffer de envio
        fgets(buffer_send, strlen(buffer_send), stdin); // Le datos de consola
        if (strlen(buffer_send) > 0){
            send(sk, buffer_send, strlen(buffer_send), 0); // Envia mensaje
            if (strcmp(buffer_send,"x") == 0){
                close(sk);
            }
        }
    }   

} 

void *recibir_mensajes(void *socket){ // Funcion para la recepcion de mensajes en el cliente
    memset(buffer_recv, 0, len_buffer); // Reserva memoria para el buffer
    int sock = *(int*)socket; // Convierte a entero
    while(recv(sock, buffer_recv, strlen(buffer_recv), 0) > 0){
        printf("%s\n", buffer_recv); // Iprime mensaje
        //memset(buffer, 0, len_buffer);
    }
}