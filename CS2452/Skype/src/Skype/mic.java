package Skype;

import java.util.*;
import java.util.concurrent.Semaphore;

import javax.sound.sampled.*;

import java.io.*;
import java.net.*;

class mic extends Thread
{
	Socket s;
	AudioFormat format;
	Gui g;
	Semaphore guiflg;

	public mic(Socket socket, AudioFormat format, Gui g, Semaphore guiflg)
	{
		s = socket;
		this.format = format;
		this.g = g;
		this.guiflg = guiflg;
	}

	public void run()
	{
		BufferedOutputStream bout = null;
		try
		{
			DataLine.Info info = new DataLine.Info(TargetDataLine.class, format);
			final TargetDataLine line = (TargetDataLine) AudioSystem.getLine(info);

			line.open(format, 1024);
			line.start();
			int bufferSize = 32;
			byte buffer[] = new byte[bufferSize];
			try
			{
				bout = new BufferedOutputStream(s.getOutputStream());

				//System.out.printf("mic started\n");
				while (true)
				{
					int count = line.read(buffer, 0, buffer.length);

					if (count > 0) bout.write(buffer, 0, count);

					bout.flush();
				}
			}catch (SocketException se)
			{
				try
				{
					guiflg.acquire();
				} catch (InterruptedException e)
				{
					e.printStackTrace();
				}
				
				g.call.setText("Call");
				g.incall = false;
				guiflg.release();
				
				
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		} catch (LineUnavailableException e1)
		{
			e1.printStackTrace();
		}
	}

}
