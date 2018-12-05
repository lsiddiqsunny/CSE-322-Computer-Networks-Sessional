/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Rifat
 */

public class Client 
{
	public static void main(String args[])
	{
		try
		{
            String serverAddress="127.0.0.1";
            int serverPort=33333;
			NetworkUtil nc=new NetworkUtil(serverAddress,serverPort);
			new ReadThread(nc);
			new WriteThread(nc,"Client");		
		}catch(Exception e)
		{
			System.out.println (e);
		}
		
	}

}

