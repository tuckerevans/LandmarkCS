import java.io.*;
import java.net.*;
import java.util.concurrent.Semaphore;

class WRServer implements Runnable
{
	int port;
	Thread t;
	byte[] theSharedResource = null;
	Semaphore flg = null;
	Semaphore flg2 = null;

	WRServer(int p)
	{
		port = p;
		theSharedResource = new byte[1024 * 1024];
		flg = new Semaphore(1);
		flg2 = new Semaphore(2);
		t = new Thread(this, "WRServer");
		t.start();
	}

	public void run()
	{
		ServerSocket ss = null;
		Socket s = null;
		DataOutputStream dout = null;
		boolean running = true;
		System.out.println("WRServer is alive");

		ResourceWriter rw = new ResourceWriter(theSharedResource, flg, flg2);

		try
		{
			ss = new ServerSocket(port);
		} catch (Exception e0)
		{
			e0.printStackTrace();
		}

		while (running)
		{
			try
			{
				System.out.println("WRServer: waiting for incoming connection");
				s = ss.accept();
				System.out.println("WRServer: received incoming connection");

				dout = new DataOutputStream(s.getOutputStream());
				ResourceReader rr = new ResourceReader(dout, theSharedResource, flg, flg2);
			} catch (Exception e)
			{
				e.printStackTrace();
			}
		}

		System.out.println("WRServer is finished");
	}
}
