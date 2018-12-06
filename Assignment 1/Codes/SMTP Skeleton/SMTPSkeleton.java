
package smtpskeleton;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class SMTPSkeleton {

    public static void main(String[] args) throws UnknownHostException, IOException {
       String mailServer = "localhost";
        InetAddress mailHost = InetAddress.getByName(mailServer);
        InetAddress localHost = InetAddress.getLocalHost();
        Socket smtpSocket = new Socket(mailHost,25);
        BufferedReader in =  new BufferedReader(new InputStreamReader(smtpSocket.getInputStream()));
        PrintWriter pr = new PrintWriter(smtpSocket.getOutputStream(),true);
        String initialID = in.readLine();
        System.out.println(initialID);
        pr.println("HELO "+localHost.getHostName());
        //pr.flush();
        String welcome = in.readLine();
        System.out.println(welcome);
        pr.println("MAIL FROM:<lsiddiqsunny@gmail.com>");
        //pr.flush();
        welcome = in.readLine();
        System.out.println(welcome);

        pr.println("RCPT TO:<lsiddiqsunny@ieee.org>");
        //pr.flush();
        welcome = in.readLine();
        System.out.println(welcome);
        pr.println("DATA");
         welcome = in.readLine();
        System.out.println(welcome);
         Scanner sc=new Scanner(System.in);
         String line;
         while(true){
             line=sc.nextLine();
             if(line.length()==0) break;
             pr.println(line);
            // System.out.println(line);
         }
         pr.println(".");
      //  pr.flush();
       welcome = in.readLine();
        System.out.println(welcome);
       
        pr.println("QUIT");
        //pr.flush();
        welcome = in.readLine();
        System.out.println(welcome);
    }
}
