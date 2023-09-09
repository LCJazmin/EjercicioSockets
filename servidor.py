import socket
import threading

# Configura el servidor
host = '127.0.0.1'
port = 23456

# Crea un socket TCP
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port))
server_socket.listen(5)

print(f"[SERVER-PY]: Escuchando en {host}:{port}")

# Lista para almacenar conexiones de clientes
clientes = []

# Función para manejar la conexión con un cliente
def handle_client(client_socket, i):
    j = i
    try:
        while True:
            buffer_recv = client_socket.recv(1024).decode("utf-8")
            aux = buffer_recv
            if not buffer_recv:
                break
            buffer_recv = ""
            buffer_recv = f"[{j}]{aux}"
            print(buffer_recv)
            # Enviar el mensaje a todos los otros clientes conectados
            for cliente in clientes:
                if cliente != client_socket:
                    cliente.send(buffer_recv.encode("utf-8"))
            buffer_recv = ""  
            aux = ""  

    except Exception as e:
        print(f"Error en la conexión: {str(e)}")
    finally:
        # Cierra la conexión con el cliente
        client_socket.close()
        clientes.remove(client_socket)

# Bucle principal del servidor para aceptar conexiones entrantes
i = 0
while True:
    client_socket, addr = server_socket.accept()
    print(f"[SERVER-PY]: Conexión desde {addr[0]}:{addr[1]}")
    
    # Agrega el cliente a la lista de clientes
    clientes.append(client_socket)
    # Inicia un hilo para manejar la comunicación con el cliente
    client_handler = threading.Thread(target=handle_client, args=(client_socket,i))
    client_handler.start()
    i += 1
    if (len(clientes) == 0):
        server_socket.close()