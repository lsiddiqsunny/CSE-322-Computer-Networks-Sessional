import java.net.*;


public class Resolver 
{
  	public static void main(String args[] ) 
  	{
  		try 
    	{
      		InetAddress ipAddress = InetAddress.getByName("samsung-PC");
      		System.out.println(ipAddress);
    	}
    	catch ( UnknownHostException ex )
    	{
      		System.out.println(ex);
    	}
  	}
} 