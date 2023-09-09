// Librerias
import java.io.*;
import java.net.*;
import java.util.*;

public class cliente_ {
    public static void main(String[] args) {
        try {
            Scanner in = new Scanner(System.in); // Para leer datos de consola
            // Configura el cliente
            String host = "127.0.0.1"; //ip host
            System.out.print("SERVIDOR\tPUERTO\nJAVA\t\t12345\nPYTHON\t\t23456\nC\t\t34567\n\nPUERTO -> ");
            int port = in.nextInt(); // obtiene puereto
            // Crea un socket TCP y conecta al servidor
            Socket socket = new Socket(host, port);
            System.out.println("[CLIENT-JAVA]: Aceptado por " + host + ":" + port);
            // Establece flujos de entrada y salida
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
            // Crea un hilo para recibir mensajes del servidor
            Thread recibirThread = new Thread(() -> {
                try{
                    while(true){
                        String buffer_recv = entrada.readLine(); // Lee mensaje de servidor
                        System.out.println(buffer_recv); // Imprime mensaje
                        if (buffer_recv == null) {
                            break;
                        }
                        buffer_recv = "";
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            });
            recibirThread.start(); // Inicia el hilo
            
            // Lee mensajes del usuario y los envía al servidor
            BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in));
            while (true) {
                String buffer_send = teclado.readLine();
                salida.println(buffer_send);
            }
                //socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    } 
}