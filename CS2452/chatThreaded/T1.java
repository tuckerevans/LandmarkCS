import java.util.*;
import java.io.*;
import java.net.*;


class T1 extends Thread
{
	Socket s;

	public T1(Socket socket)
	{
		s = socket;
	}
	public void run()
	{
		BufferedReader tr = new BufferedReader(new InputStreamReader(System.in));
		
		try
		{
			BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
			
			while(true)	
			{
				bw.write(tr.readLine());
				bw.newLine();
				bw.flush();
			}
			}catch(Exception e){
				e.printStackTrace();
		}
	}
}
	
