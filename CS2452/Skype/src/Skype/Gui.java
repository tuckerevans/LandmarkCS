package Skype;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.concurrent.Semaphore;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.AbstractDocument;

public class Gui extends JFrame implements ActionListener, ChangeListener
{
	JButton call;
	JCheckBox mute;
	JSlider volume;
	JLabel time, ipL, portL, volumeL,ipLH,portLH;
	JTextField ipT, portT;
	JPanel panel, ip, volumeP, slider;
	GridBagConstraints c;
	Timer timer;
	int sec;

	boolean incall, servercall;

	Semaphore audflg;
	Semaphore svrflg;
	Semaphore guiflg;
	audio aud;

	public Gui(audio aud, Semaphore svrflg, Semaphore audflg, Semaphore guiflg,int port)
	{
		this.guiflg = guiflg;
		timer = new Timer(1000, this);
		incall = false;
		this.svrflg = svrflg;
		this.audflg = audflg;
		this.aud = aud;

		c = new GridBagConstraints();
		c.ipadx = 10;
		c.ipady = 10;

		panel = new JPanel(new GridBagLayout());

		call = new JButton("Call");
		call.addActionListener(this);

		//Volume Panel setup
		volume = new JSlider(SwingConstants.VERTICAL);
		volume.addChangeListener(this);
		volume.setPaintTicks(true);
		volume.setMajorTickSpacing(25);
		volume.setMinorTickSpacing(5);
		volume.setPaintTicks(true);
		volume.setPaintLabels(true);

		volumeL = new JLabel("Volume");

		volumeP = new JPanel();
		volumeP.setLayout(new GridBagLayout());

		volumeP.add(volume, c);
		c.gridx = 0;
		c.gridy = 1;
		volumeP.add(volumeL, c);

		mute = new JCheckBox("Mute");
		mute.addActionListener(this);
		c.gridx = 1;
		c.gridy = 1;
		volumeP.add(mute, c);
		volumeP.setVisible(true);

		//Address setup
		ipL = new JLabel("IP:");
		ipT = new JTextField(15);
		ipT.setText("127.0.0.1");
		((AbstractDocument) ipT.getDocument()).setDocumentFilter(new numberFilter());
		ip = new JPanel(new GridBagLayout());
		c.gridx = 0;
		c.gridy = 0;
		c.anchor = GridBagConstraints.EAST;
		ip.add(ipL, c);
		c.gridx = 1;

		c.anchor = GridBagConstraints.WEST;
		ip.add(ipT, c);

		portL = new JLabel("Port:");
		portT = new JTextField(5);
		portT.setText("7777");
		((AbstractDocument) portT.getDocument()).setDocumentFilter(new numberFilter());
		c.gridy = 1;
		c.anchor = GridBagConstraints.WEST;
		ip.add(portT, c);
		c.gridx = 0;
		c.anchor = GridBagConstraints.EAST;
		ip.add(portL, c);
		
		try
		{
			ipLH = new JLabel("Your IP: ".concat(InetAddress.getLocalHost().getHostAddress().toString()));
		} catch (UnknownHostException e)
		{
			e.printStackTrace();
		}
		portLH = new JLabel("Your Port: ".concat(Integer.toString(port)));
		

		c.anchor = GridBagConstraints.WEST;
		c.gridy = 2;
		c.gridx = 1;
		ip.add(ipLH,c);
		c.gridy++;
		ip.add(portLH,c);

		time = new JLabel("TIME");

		c.gridx = 0;
		c.gridy = 0;
		c.anchor = GridBagConstraints.NORTHWEST;
		panel.add(ip, c);
		c.gridx = 1;
		panel.add(volumeP, c);
		c.gridy = 1;
		c.anchor = GridBagConstraints.CENTER;
		panel.add(call, c);
		c.gridx = 0;
		panel.add(time, c);
		panel.setVisible(true);

		this.add(panel);

	}

	public void actionPerformed(ActionEvent e)
	{
		try
		{
			guiflg.acquire();

			if (e.getSource() == mute)
			{
				if (mute.isSelected())
				{
					volume.setEnabled(false);
					aud.setVolume(0);
				}
				else
				{
					volume.setEnabled(true);
					aud.setVolume(volume.getValue());
				}
			}
			else if (e.getSource() == call)
			{
				if (incall)
				{
					call.setText("Call");
					timer.stop();
					//System.out.println("Hung up");
					incall = false;
					try
					{
						audflg.acquire();
					} catch (InterruptedException e1)
					{
						e1.printStackTrace();
					}
					//System.out.println("False");
					aud.setOpen(false);
					audflg.release();
					svrflg.release(); //allow server to accept connections again.
				}
				else
				{
					call.setText("Hang up");
					sec = 0;
					timer.start();
					Client c = new Client(this, ipT.getText(), portT.getText(), aud, audflg,guiflg);
					//System.out.printf("IP: %s, Port: %s\n", ipT.getText(), portT.getText());
					incall = true;

				}
			}
			else if ((e.getSource() == timer) && incall)
			{
				sec++ ;
				if (sec < 60 * 60) time.setText(String.format("%02d:%02d", sec / 60, sec % 60));
				else time.setText(String.format("%d:%02d:%02d", sec / (60 * 60), (sec / 60) % 60, sec % 60));
			}
		} catch (InterruptedException e2)
		{
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
		guiflg.release();

	}

	public void stateChanged(ChangeEvent e)
	{
		if (e.getSource() == volume)
		{
			if (audflg.tryAcquire())
			{
				aud.setVolume(volume.getValue());
				//change volume .getValue();	
				audflg.release();
			}

		}
	}

}
