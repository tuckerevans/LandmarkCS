package Skype;

import java.util.*;
import java.util.concurrent.Semaphore;
import java.io.*;
import java.net.*;
import javax.sound.sampled.*;

class speaker extends Thread
{
	Socket s;
	AudioFormat format;
	audio aud;
	Semaphore flg;
	Gui g;

	public speaker(Socket socket, AudioFormat format, audio aud, Semaphore audflg, Gui g)
	{
		this.g = g;
		this.aud = aud;
		this.flg = audflg;
		s = socket;
		this.format = format;
	}

	public void run()
	{
			try
			{
				DataLine.Info info = new DataLine.Info(SourceDataLine.class, format);
				final SourceDataLine line = (SourceDataLine) AudioSystem.getLine(info);
				line.open(format, 1024);
				line.start();

				final AudioInputStream ais = new AudioInputStream(s.getInputStream(), format, 2000000);
				int count;
				int bufferSize = (int) format.getSampleRate() * format.getFrameSize();
				byte buffer[] = new byte[bufferSize];

				//System.out.printf("speakers started\n");
				while (((count = ais.read(buffer, 0, buffer.length)) != -1) && aud.open)
				{
					flg.release();
					if (count > 0)
					{
						if (flg.tryAcquire())
						{

							for (int i = 0; i < buffer.length; i++ )
							{
								buffer[i] *= aud.vol;
							}
							flg.release();
						}

						line.write(buffer, 0, count);
					}

				}
				line.drain();
				line.close();
			} catch (IOException e)
			{
				System.err.println("I/O problems: " + e);

			} catch (LineUnavailableException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try
			{
				flg.acquire();
			} catch (InterruptedException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		//System.out.println("out of while");
		try
		{
			s.close();
			//System.out.println("S closed");
		} catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
