import java.util.concurrent.Semaphore;

public class ResourceWriter implements Runnable
{
	Thread t;
	byte[] res = null;
	Semaphore flg = null;
	Semaphore flg2 = null;

	ResourceWriter(byte[] r, Semaphore s, Semaphore s2)
	{
		res = r;
		flg = s;
		flg2 = s2;
		t = new Thread(this, "ResourceWriter");
		t.start();
	}

	public void run()
	{
		int i;
		byte value = 33;
		boolean running = true;

		System.out.println("ResourceWriter: starts");
		while (running)
		{
			System.out.println("RW in while");
			try
			{
				flg2.acquire();
				flg.acquire();

				for (i = 0; i < res.length; i++ )
					res[i] = value;
				flg.release();
				flg2.release();
			} catch (Exception e0)
			{
				e0.printStackTrace();
			}

			value++ ;
			if (value == 100) value = 33;
			System.out.println("ResourceWriter: updated, waiting 15, 14, ...");
			try
			{
				Thread.sleep(15000);
			} catch (Exception e)
			{
			}
		}
		System.out.println("ResourceWriter: ends");

	}
}
