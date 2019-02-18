package Skype;

import java.io.IOException;
import java.net.*;
import java.util.concurrent.Semaphore;

public class Server extends Thread
{
	ServerSocket ss;
	Socket s;
	int port;
	Gui g;
	audio aud;
	Semaphore svrflg;
	Semaphore audflg;
	Semaphore guiflg;

	public Server(Gui g, int port, audio aud, Semaphore svrflg, Semaphore audflg, Semaphore guiflg)
	{
		this.guiflg = guiflg;
		this.audflg = audflg;
		this.svrflg = svrflg;
		this.aud = aud;
		this.port = port;
		this.g = g;
		try
		{
			this.ss = new ServerSocket(port);
		} catch (IOException e)
		{
			e.printStackTrace();
		}

		this.start();

	}

	public void run()
	{
		while (true)
		{
			//System.out.printf("Server is waiting\n");
			try
			{
				svrflg.acquire();
				//System.out.println(ss);
				s = ss.accept();
			} catch (IOException e)
			{
				e.printStackTrace();
			} catch (InterruptedException e)
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
			//System.out.println("ServerSpeaker");
			speaker t1 = new speaker(s, audio.getFormat(), aud, audflg,g);
			mic t2 = new mic(s, audio.getFormat(),g,guiflg);
			try
			{
				guiflg.acquire();
				g.call.setText("Hang up");
				g.sec = 0;
				g.timer.start();
				g.incall = true;
				guiflg.release();
			} catch (InterruptedException e)
			{
				e.printStackTrace();
			}

			t1.start();
			t2.start();

		}
	}

}