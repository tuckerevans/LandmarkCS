package assign6;

import java.io.*;
import java.net.*;
import java.util.concurrent.Semaphore;

public class SocketWriter extends Thread
{
	Thread t;
	BufferedWriter bw;
	Semaphore write;
	boolean sc;
	
	public SocketWriter(Socket s, Semaphore write)
	{
		try{
		this.bw  = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		
		this.write = write;
		
			

		t = new Thread(this);
		t.start();
	}
	
	public void run()
	{
	
		while(true)
		{
			try
			{
				write.acquire();
			} catch (InterruptedException e1)
			{
				e1.printStackTrace();
			}
			try
			{
				bw.newLine();
				bw.flush();
			} catch (IOException e)
			{
				e.printStackTrace();
			}
		}
				
	}

}
