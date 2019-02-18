import java.io.*;

class P1
{

	public static void main(String[] args)
	{
		String str = null;
		BufferedReader br;
		br = new BufferedReader(new InputStreamReader(System.in));
		BufferedWriter bw = null;

		try
		{
			File file = new File(args[0]);
			
			if (! file.exists())
			{
				file.createNewFile();
			}

			bw = new BufferedWriter(new FileWriter(file));
			while(true)
			{
				str = br.readLine();
				bw.write(str);
				bw.newLine();
				bw.flush();
			}
		} catch(Exception e)
		{
			System.out.printf("Unable to write to file %s\n", args[0]);
		}
	
				

		
	}
}
