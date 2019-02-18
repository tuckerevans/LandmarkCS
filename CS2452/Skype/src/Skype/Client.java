package Skype;

import java.io.IOException;
import java.net.*;
import java.util.concurrent.Semaphore;

public class Client extends Thread
{
	Socket s;
	InetAddress ip;
	int port;
	audio aud;
	Gui g;
	Semaphore audflg;
	Semaphore guiflg;

	public Client(Gui g, String ip, String port, audio aud, Semaphore audflg,Semaphore guiflg)
	{
		this.aud = aud;
		this.audflg = audflg;
		this.guiflg = guiflg;
		this.g = g;
		
		try
		{
			this.ip = InetAddress.getByName(ip);
		} catch (UnknownHostException e)
		{
			e.printStackTrace();
		}
		this.port = Integer.parseInt(port);
		this.start();
	}

	public void run()
	{
		//System.out.printf("Client Started\n");
		try
		{
			s = new Socket(ip, port);
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		try
		{
			audflg.acquire();
		} catch (InterruptedException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		aud.setOpen(true);
		audflg.release();

		speaker t1 = new speaker(s, audio.getFormat(), aud, audflg,g);
		mic t2 = new mic(s, audio.getFormat(), g, guiflg);
		
		t1.start();
		t2.start();
	}

}
