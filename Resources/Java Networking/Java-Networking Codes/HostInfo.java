import java.net.*;


public class HostInfo 
{
  	public static void main(String args[] ) 
  	{
  		try 
    	{
      		InetAddress ipAddress = InetAddress.getLocalHost();
      		System.out.println(ipAddress);
    	}
    	catch ( UnknownHostException ex )
    	{
      		System.out.println(ex);
    	}
  	}
} 