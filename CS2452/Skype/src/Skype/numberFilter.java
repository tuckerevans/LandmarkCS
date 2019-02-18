package Skype;

import java.awt.Toolkit;

import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DocumentFilter;

class numberFilter extends DocumentFilter
{
	@Override
	public void insertString(DocumentFilter.FilterBypass fp, int offset, String string,AttributeSet aset) throws BadLocationException
	{
		int len = string.length();
		boolean isValidInteger = true;

		for (int i = 0; i < len; i++ )
		{
			if (!Character.isDigit(string.charAt(i)) && string.charAt(i) != '.')
			{
				isValidInteger = false;
				break;
			}
		}
		if (isValidInteger) super.insertString(fp, offset, string, aset);
		else 
			Toolkit.getDefaultToolkit().beep();
	}
	
	public void replace(DocumentFilter.FilterBypass fp, int offset, int length, String string, AttributeSet aset) throws BadLocationException
	    {
	        int len = string.length();
	        boolean isValidInteger = true;

	        for (int i = 0; i < len; i++)
	        {
	            if (!Character.isDigit(string.charAt(i)) && string.charAt(i) != '.')
	            {
	                isValidInteger = false;
	                break;
	            }
	        }
	        if (isValidInteger)
	            super.replace(fp, offset, length, string, aset);
	        else
	            Toolkit.getDefaultToolkit().beep();
	    }
}