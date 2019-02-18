package assign6;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.util.concurrent.Semaphore;

import javax.imageio.ImageIO;
import javax.swing.*;

public class Gui extends JFrame implements ActionListener, MouseListener
{
	JPanel panel;
	JLabel label;
	BufferedImage image;
	Color color;
	Semaphore write;
	boolean sc;
	
	
	public Gui(String filename, Color color, Semaphore write, boolean sc)
	{
		this.color = color;
		this.sc = sc;
		
		panel = new JPanel(new GridLayout(1,1));
		panel.setBackground(color);
		try
		{
			image = ImageIO.read(new File(filename));
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		label = new JLabel(new ImageIcon(new ImageIcon(image).getImage().getScaledInstance(500, 500, Image.SCALE_DEFAULT)));
		
		panel.add(label);
		panel.addMouseListener(this);
		this.add(panel);
		
		this.write = write;
		
	}

	public void actionPerformed(ActionEvent e)
	{

	}
	
	public void mouseClicked(MouseEvent e)
	{
		write.release();
		changeImage();
	}


	public void mouseEntered(MouseEvent e)
	{
		
	}

	public void mouseExited(MouseEvent e)
	{
		
	}

	public void mousePressed(MouseEvent e)
	{
		
	}

	public void mouseReleased(MouseEvent e)
	{

	}
	
	public void changeImage()
	{
		
		String filename;
		Color color;
		if(sc)
		{
			filename = "img2.jpg";
			color = Color.GREEN;
		}
		else
		{
			filename = "img1.jpg";
			color = Color.BLUE;
		}
		
		try
		{
			image = ImageIO.read(new File(filename));
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		label.setIcon(new ImageIcon(new ImageIcon(image).getImage().getScaledInstance(500, 500, Image.SCALE_DEFAULT)));
		
		panel.setBackground(color);
		panel.repaint();
		
		sc = !sc;
	}
}