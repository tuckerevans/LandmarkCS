
import java.util.*;
import java.io.*;
import java.net.*;


class T2 extends Thread
{
	Socket s;
	public T2(Socket socket)
	{
		s = socket;
	}	
	public void run()
	{
		
		try
		{

			BufferedReader bw = new BufferedReader( new InputStreamReader(s.getInputStream()));
			String str = "Connected";
			do	
			{
				System.out.printf("%s\n", str);
				str = bw.readLine();
			}while (str != null);
			}catch(Exception e){
				e.printStackTrace();
		}
	}
}
	
