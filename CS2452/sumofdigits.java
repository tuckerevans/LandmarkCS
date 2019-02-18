
public class sumofdigits
{
	public static void main(String[] args)
	{
		System.out.print(sum(23657));
	}
	
	public static int sum(int num)
	{
		if(num == 0)
		{
			return 0;
		}
		return (num % 10) + sum(num/10);
	}

}
