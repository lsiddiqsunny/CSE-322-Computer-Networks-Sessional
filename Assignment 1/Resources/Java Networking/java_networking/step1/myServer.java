import java.io.*;
import java.net.*;
import java.util.Scanner;



public class myServer
{
	public static void main (String[] args)throws Exception
	{
		
		
		System.out.println ("server");
		ServerSocket serverSocket= new ServerSocket( 33333 );
		Socket socket= serverSocket.accept();
		System.out.println ("got a client");
		
		Scanner sc= new Scanner( socket.getInputStream() );
		
		while(sc.hasNext())
		{
			System.out.println (sc.nextLine());
		}
		
		
		
		
    }
}