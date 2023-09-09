// Librerias
import java.io.*;
import java.net.*;
import java.util.ArrayList;

public class servidor_ {
    private static int len_buffer = 1024; // Tamaño del buffer
    private static final int PORT = 12345; // puerto
    private static ArrayList<PrintWriter> clientes = new ArrayList<>(); // Lista de clientes
    public static void main(String[] args) {
        try {
            ServerSocket serverSocket = new ServerSocket(PORT); // Crea socker servidor
            System.out.println("[SERVER-JAVA]: Escuchando en " + PORT);
            int i = 0;
            while (true) {
                new ClientHandler(serverSocket.accept(), i).start(); // Crea hilo para atender a los clientes
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static class ClientHandler extends Thread {
        private Socket socket;
        private PrintWriter out;
        public ClientHandler(Socket socket, int i) { // Constructor de hilo
            this.socket = socket;
            System.out.println("[SERVER-JAVA]: Conexión desde " + socket.getInetAddress() + ":" + socket.getPort());
        }

        public void run() {
            try {
                out = new PrintWriter(socket.getOutputStream(), true); // Obtiene informacion del socket cliente
                clientes.add(out); // Guarda id cliente
                int i = clientes.indexOf(out);
                // Flujo de salida de datos
                OutputStreamWriter outw = new OutputStreamWriter(socket.getOutputStream(), "UTF8");
                // Flujo de entrada de datos
                InputStreamReader inw = new InputStreamReader(socket.getInputStream(), "UTF8");
        
                char[] buffer_recv = new char[len_buffer]; // Buffer de recepcion de datos
                String mensaje = "["; // Agrega numero de cliente
                mensaje += Integer.toString(i); // Convierte de entero a string
                mensaje += "]";
                while (true) {
                    inw.read(buffer_recv); // lee buffer de recepcion
                    for (char c : buffer_recv) { // Obtiene mensaje caracter por caracter
                        mensaje += c;
                        if (c == 00) {
                            break;
                        }
                    }
                    System.out.println(mensaje);
                    outw.write(mensaje.toCharArray()); // Envia 
                    outw.flush();
                    buffer_recv = new char[len_buffer];
                }
                /*
                String buffer_recv;
                String aux = "[";
                aux += Integer.toString(i);
                aux += "]";
                while ((buffer_recv = in.readLine()) != null) {
                    aux += buffer_recv;
                    System.out.println(aux);
                    for (PrintWriter cliente : clientes) {
                        if (cliente != out){
                            cliente.println(aux);
                        }
                    }
                    //buffer_recv = "";
                    //aux = "";
                }*/

            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (out != null) {
                    clientes.remove(out);
                }
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}

