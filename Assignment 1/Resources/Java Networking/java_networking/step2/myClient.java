import java.io.*;
import java.net.*;
import java.util.Scanner;




class ReadThread implements Runnable
{
	private Thread thr;
	private Scanner sc;
	
	public ReadThread(InputStream is) 
	{
		sc = new Scanner( is );
		this.thr = new Thread(this);
		thr.start();
	}
	
	public void run() 
	{
		while( true )
		{
			String msg=sc.nextLine();
			System.out.println (msg);
		}
	}
}

class WriteThread implements Runnable
{
	private Thread thr;
	private PrintWriter pw;
	
	public WriteThread(OutputStream os) 
	{
		pw = new PrintWriter( os );
		this.thr = new Thread(this);
		thr.start();
	}
	
	public void run() 
	{
		Scanner sc=new Scanner( System.in );
		while( true )
		{
			
			String msg=sc.nextLine();
			pw.println (msg );
			pw.flush();
		}
	}
}



public class myClient
{
	public static void main (String[] args)throws Exception
	{
		
		System.out.println ("client");
		
		String serverAddress="127.0.0.1";
        int serverPort=33333;
		Socket socket= new Socket(serverAddress,serverPort);
		
		new ReadThread( socket.getInputStream() );
		new WriteThread(  socket.getOutputStream() );
		
    }
}