import java.io.*;
import java.util.concurrent.Semaphore;

public class ResourceReader implements Runnable
{
	public static double cor = 0;
	public static Semaphore flg1 = new Semaphore(1);

	Thread t;
	byte[] res = null, sentinel = null;
	DataOutputStream dout = null;
	Semaphore flg = null;
	Semaphore flg2 = null;

	ResourceReader(DataOutputStream dot, byte[] r, Semaphore s, Semaphore s2)
	{
		res = r;
		dout = dot;
		flg = s;
		flg2 = s2;
		sentinel = new byte[32768];
		for (int i = 0; i < sentinel.length; i++ )
			sentinel[i] = 65;
		t = new Thread(this, "ResourceReader");
		t.start();
	}

	public void run()
	{
		int i;
		byte value = 33;
		boolean running = true;

		System.out.println("ResourceReader: starts");
		while (running)
		{
			try
			{

				flg2.acquire();
				flg2.release();
				flg1.acquire();
				if (cor == 0)
				{
					flg.acquire();
					cor++ ;
					flg1.release();
					for (int j = 0; j < 50; j++ )
						dout.write(res, 0, res.length);
					dout.write(sentinel, 0, sentinel.length);
					flg.release();
				}
				else
				{
					cor++ ;
					flg1.release();
					for (int j = 0; j < 50; j++ )
						dout.write(res, 0, res.length);
					dout.write(sentinel, 0, sentinel.length);
				}
				flg1.acquire();
				cor-- ;
				flg1.release();

				System.out.println("ResourceReader: sent a bunch");
				Thread.sleep(100);
				running = false;
			} catch (Exception e)
			{
				e.printStackTrace();
				try
				{
					dout.close();
				} catch (Exception e2)
				{
				}
				return;
			}
		}
		System.out.println("ResourceReader: ends");
		try
		{
			dout.close();
		} catch (Exception e2)
		{
		}

	}
}
