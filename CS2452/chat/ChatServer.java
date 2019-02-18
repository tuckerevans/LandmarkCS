import java.io.*;
import java.util.*;
import java.net.*;

class ChatServer
{
	public static void main(String[] args)
	{
		ServerSocket ss;
		Socket s;
		BufferedReader br, tr;
		BufferedWriter bw;

		tr = new BufferedReader(new InputStreamReader(System.in));

		try
		{
			ss = new ServerSocket(2452);
			
			s = ss.accept();
			System.out.printf("Client connected...\n");


			br = new BufferedReader(new InputStreamReader(s.getInputStream()));
			bw = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
			
			//String[] address = new String[2];
			//address[0] = InetAddress.toString(s.getAddress());
			//address[1] = InetAddress.toString(s.getLocalAddress());

			while(true)
			{
				System.out.printf("%s: %s\n%s>", "Client", br.readLine(), "Server");
				bw.write(tr.readLine());
				bw.newLine();
				bw.flush();
			}
			
		}catch (Exception e)
		{
			e.printStackTrace();
		}
	}
}
