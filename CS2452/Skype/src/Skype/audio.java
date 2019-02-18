package Skype;

import java.net.Socket;

import javax.sound.sampled.AudioFormat;

public class audio
{
	double vol;
	boolean open;

	audio(int vol)
	{
		this.vol = vol / 100.00;
	}

	public static AudioFormat getFormat()
	{
		float sampleRate = 8000;
		int sampleSizeInBits = 8;
		int channels = 1;
		boolean signed = true;
		boolean bigEndian = true;
		return new AudioFormat(sampleRate, sampleSizeInBits, channels, signed, bigEndian);
	}

	public void setVolume(int volume)
	{
		vol = volume / 100.00;
	}
	
	public void setOpen(boolean a){
		open = a;
	}

}