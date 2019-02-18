
public class Q8
{

	public static void main(String[] args)
	{
		int[] str= new int[(int) Math.ceil(args[0].length()/6.00)];
		
		args[0] = args[0].toLowerCase();
		
		while (args[0].length() % 6 != 0)
		{
			args[0] = args[0].concat(String.format("%c",'a' + 31));
		}
		
		for(int i = 0; i < str.length; i++)
		{
			str[i] = 0;
			for(int j = 0; j < 6; j++)
			{
				str[i] <<= 5;
				str[i] += args[0].charAt(j + (i*6)) - 'a' ;
			}
		}
		
		for(int i = 0; i < str.length; i++)
		{
			System.out.printf("%x\n", str[i]);
		}

	}

}
