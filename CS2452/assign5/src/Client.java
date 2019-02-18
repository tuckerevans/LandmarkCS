import java.net.*;
import java.io.*;

class Client
{
	public static void main(String[] args)
	{
		try
		{
			byte b = 0;
			int cnt = 0;
			Socket s = new Socket(args[0], Integer.parseInt(args[1]));
			DataInputStream di = new DataInputStream(s.getInputStream());
			while (true)
			{
				b = di.readByte();
				System.out.printf("%d - %d\t%x\t%c\n", ++cnt, b, b, b);
				if ((cnt % (1024 * 1024)) == 0)
				{
					System.out.println("------MILLION------\n\n");
					cnt = 0;
					Thread.sleep(1000);
					di.close();
					return;
				}
			}
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	}
}