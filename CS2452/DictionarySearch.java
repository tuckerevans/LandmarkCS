import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class DictionarySearch
{
	public static String[] dict;

	public static void main(String[] args)
	{
		BufferedReader ir = new BufferedReader(new InputStreamReader(System.in));
		BufferedReader fr = null;
		try
		{
			fr = new BufferedReader(new FileReader("/home/tje/workspace/Recursion/dictionary.wwf"));
		} catch (FileNotFoundException e1)
		{
			e1.printStackTrace();
		}

		String str = null;
		dict = new String[0];
		System.out.println("Dictionary loading...");

		do
		{
			try
			{
				str = fr.readLine();
			} catch (IOException e)
			{
				e.printStackTrace();
			}
			
			String[] tmp = new String[dict.length + 1];
			int i;
			for (i = 0; i < dict.length; i++ )
			{
				tmp[i] = dict[i];
			}
			tmp[i] = str;
			dict = tmp;

		} while (str != null);
		System.out.println("Dictionary loaded.");
		
		while (true)
		{
			System.out.print("What word would you like to look up? ");
			try
			{
				System.out.println("The closest word is " + check(ir.readLine().toLowerCase(), 0, dict.length));
			} catch (IOException e)
			{
				e.printStackTrace();
			}
			
			

		}
	}

	public static String check(String word, int a, int b)
	{
		if (a == b -1)
		{
			return dict[a];
		}
		int mid = (a+b)/2;
		if(word.compareTo(dict[mid]) > 0)
		{
			return check(word, mid, b);
		}
		
		if (word.compareTo(dict[mid]) < 0)
		{
			return check(word, a, mid);
		}
		
		if (word.compareTo(dict[mid]) == 0)
		{
			return dict[mid];
		}
		
		
		return word;

	}
}