import socket
import threading

def recibir_mensajes(sock): # Funcion para recibir mensajes
    print("Se creo hilo para %d", sock)
    buffer_recv = ""
    while True:
        buffer_recv = sock.recv(1024).decode("utf-8")
        if not buffer_recv:
            break
        print(f"{buffer_recv}")
        buffer_recv = ""

try:
    # Configura el cliente
    host = '127.0.0.1'
    port = int(input("SERVIDOR\tPUERTO\nJAVA\t\t12345\nPYTHON\t\t23456\nC\t\t34567\n\nPUERTO -> "))
    print("\n")
    # Crea un socket TCP y conecta al servidor
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((host, port))
    print(f"[CLIENT-PY]: Aceptado por {host}:{port}")
    
    # Inicia un hilo para recibir mensajes del servidor
    recibir_thread = threading.Thread(target=recibir_mensajes, args=(client_socket,))
    recibir_thread.start()

    # Lee mensajes del usuario y los envía al servidor
    buffer_send = ""
    while True:
        buffer_send = input()
        client_socket.send(buffer_send.encode("utf-8"))
        if ((buffer_send == "x")):
            client_socket.close()
        buffer_send = ""

except Exception as e:
    print("Error: " + str(e))
