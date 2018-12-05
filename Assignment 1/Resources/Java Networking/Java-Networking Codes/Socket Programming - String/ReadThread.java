/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Rifat
 */

public class ReadThread implements Runnable
{
	private Thread thr;
	private NetworkUtil nc;
	

	public ReadThread(NetworkUtil nc) 
	{
		this.nc = nc;
		this.thr = new Thread(this);
		thr.start();
	}
	
	public void run() 
	{
		try
		{
			while(true)
			{
				String t=(String)nc.read();
				if(t != null) System.out.println(t);
				
			}
		}catch(Exception e)
		{
			System.out.println (e);                        
		}			
                nc.closeConnection();
		
	}
}



