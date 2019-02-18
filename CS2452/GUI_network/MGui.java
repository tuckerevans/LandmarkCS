package assign6;

import java.net.*;
import java.util.concurrent.Semaphore;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import java.io.*;

public class MGui
{

	public static void main(String[] args)
	{
		Gui g = null;
		Semaphore write = new Semaphore(1);
		try
		{
			write.acquire();
		} catch (InterruptedException e1)
		{
			e1.printStackTrace();
		}
		ServerSocket ss;
		Socket s = null;
		
		if (args[0].compareTo("server") == 0)
		{
			try
			{
				ss = new ServerSocket(Integer.parseInt(args[1]));
				s = ss.accept();
			} catch (NumberFormatException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			g = new Gui("img1.jpg",Color.BLUE, write,true);
			g.setSize(1000, 1000);
			g.setLocation(32, 32);
			g.setVisible(true);
			g.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

			SocketWriter sw = new SocketWriter(s,write);


		}
		else if (args[0].compareTo("client") == 0)
		{

			InetAddress address = null;
			int port = 0;
			
			try
			{
				address = InetAddress.getByName(args[1]);
				port = Integer.parseInt(args[2]);
				
			} catch (UnknownHostException e)
			{
				e.printStackTrace();
			}
			
			try
			{
				s = new Socket(address, port);
			} catch (IOException e)
			{
				e.printStackTrace();
			}

			g = new Gui("img2.jpg",Color.GREEN,write,false);
			g.setSize(1000, 1000);
			g.setLocation(500, 500);
			g.setVisible(true);
			g.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

			SocketWriter sw = new SocketWriter(s,write);

		}
		
		SocketReader sr = new SocketReader(s,g);

	}

}
