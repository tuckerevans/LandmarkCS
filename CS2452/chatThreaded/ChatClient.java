import java.util.*;
import java.io.*;
import java.net.*;



class ChatClient
{
	public static void main(String[] args)
	{
		Socket s;
		InetAddress address = null;
		int port = 0;
		
		

		try
		{
			address = InetAddress.getByName(args[0]);
			port = Integer.parseInt(args[1]);
			}catch (UnknownHostException ex){
			ex.printStackTrace();
		}		
	
		try
		{
			s = new Socket(address, port);

			T1 screenWriter = new T1(s);
			T2 socketWriter = new T2(s);
			screenWriter.start();
			socketWriter.start();

		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}	
