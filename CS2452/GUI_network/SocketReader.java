package assign6;

import java.net.*;

import javax.swing.JFrame;

import java.io.*;

public class SocketReader extends Thread
{
	BufferedReader br;
	Gui g;
	Thread t;

	public SocketReader(Socket s, Gui g)
	{
		try
		{
			br = new BufferedReader(new InputStreamReader(s.getInputStream()));
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		
		this.g = g;

		t = new Thread(this);
		t.start();
	}

	public void run()
	{
		String str = null;
		while (true)
		{
			try
			{
				str = br.readLine();
			} catch (IOException e)
			{
				e.printStackTrace();
			}
			if (str != null)
				g.changeImage();
		}

	}

}