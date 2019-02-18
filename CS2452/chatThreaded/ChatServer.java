import java.util.*;
import java.io.*;
import java.net.*;

class ChatServer
{
	public static void main(String[] args)
	{
		ServerSocket ss;
		Socket s;
		
		try
		{
			ss = new ServerSocket(2452);
			s = ss.accept();

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
