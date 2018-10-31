import java.net.*;

class client implements Runnable
{
	DatagramPacket pack;
	DatagramSocket sock;
	
	client()
	{
		new Thread(this).start();
	}
	
	public void run() 
	{
		while(true)
		{
			try
			{
				recieve();
				Thread.sleep(5000);
			}catch(Exception e)
			{
				System.out.println(e);
			}
		}
			
	}
	
	public void recieve() throws Exception
	{
		
		byte data[]=new byte[1000];
		pack=new DatagramPacket(data,data.length);
		sock=new DatagramSocket(8000);
		sock.receive(pack);
		System.out.println("Data::"+new String(pack.getData()));
		sock.close();
	}
}


class DatagramClient
{
	public static void main(String args[])
	{
		new client();
	}
}

		 	
		
	
		
	