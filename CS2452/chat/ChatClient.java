import java.util.*;
import java.io.*;
import java.net.*;

class ChatClient
{	
	public static void main(String[] args)
	{
		Socket s;
		BufferedReader br, tr;
		BufferedWriter bw;
		InetAddress address = null;
		int port = 0;

		tr = new BufferedReader(new InputStreamReader(System.in));
	
		try{	
		address = InetAddress.getByName(args[0]);
		port = Integer.parseInt(args[1]);
		}catch (UnknownHostException ex)
		{
			ex.printStackTrace();
		}

		try
		{
			System.out.printf("Connecting...\n");
			s = new Socket(address, port);
			
			br = new BufferedReader(new InputStreamReader(s.getInputStream()));	

			bw = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
			System.out.printf("Client>");

			while(true)
			{
				bw.write(tr.readLine());
				bw.newLine();
				bw.flush();
				System.out.printf("%s: %s\n%s>", "Server", br.readLine(), "Client");
			}
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	}
}
