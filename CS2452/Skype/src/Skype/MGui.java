package Skype;

import java.util.concurrent.Semaphore;

import javax.swing.JFrame;

public class MGui
{
	public static void main(String[] args)
	{
		audio aud = new audio(50);
		Semaphore volflg = new Semaphore(1);
		Semaphore svrflg = new Semaphore(1);
		Semaphore guiflg = new Semaphore(1);
		
		int port = 7777;
		
		Gui g = new Gui(aud,svrflg,volflg,guiflg,port);
		g.setSize(500, 500);
		g.setLocation(32, 32);
		g.setVisible(true);
		g.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		Server server = new Server(g, port,aud,svrflg,volflg,guiflg);
	}

}
