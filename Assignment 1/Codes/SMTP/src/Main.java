import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Scanner;


public class Main {
    static final String mailServer = "localhost";
    static final int port=25;

    public static void main(String[] args) throws IOException {


        Server smtpServer =new Server(InetAddress.getByName(mailServer),port);

        while(smtpServer.getState().equals("closed")){
            long startTime = System.currentTimeMillis();
            System.out.println("Trying again within 5 seconds.");
            while(startTime+5000>System.currentTimeMillis()){

            }
            System.out.println();
            smtpServer =new Server(InetAddress.getByName(mailServer),port);
        }
        System.out.println("Connection established successfully.");
        smtpServer.Reply();
        Scanner sc=new Scanner(System.in);
        String req;

        while(true){
            req=sc.nextLine();
            smtpServer.Request(req);
            smtpServer.Reply();
            if(smtpServer.getState().equals("data")){
                while(!smtpServer.getState().equals("deliver")){
                    req=sc.nextLine();
                    smtpServer.Request(req);

                }
                smtpServer.Reply();
            }

            if(smtpServer.getState().equals("closed")){
                break;
            }
        }

        System.out.println("Connection closed.");





    }
}



