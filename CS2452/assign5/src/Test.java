class Test
{
	public static void main(String[] args)
	{
		try
		{
			if (args.length != 1)
			{
				System.out.println("usage: java Test listeningPort");
				System.exit(1);
			}
			System.out.println("Test progam starts");

			WRServer server = new WRServer(Integer.parseInt(args[0]));
			server.t.join();

			System.out.println("Test progam ends");
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	}
}