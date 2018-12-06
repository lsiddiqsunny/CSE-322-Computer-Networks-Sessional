import java.io.*;
import java.net.*;
import java.util.Scanner;

public class myClient
{
	public static void main (String[] args)throws Exception
	{
		
		System.out.println ("client");
		
		String serverAddress="127.0.0.1";
        int serverPort=33333;
		Socket socket= new Socket(serverAddress,serverPort);
		
		PrintWriter pw= new PrintWriter( socket.getOutputStream() );
		
		Scanner sc= new Scanner( System.in );
		
		while( sc.hasNext() )
		{
			pw.println(sc.nextLine());
			pw.flush();
		}
		
		
    }
}