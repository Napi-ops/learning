#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
	int i = 0, j = 0, k = 0;
	char ch = '0';	//"ch" accept the input
	int str_num = 0;	//"str_num" is the length of string "str"
	int flag = 0;	//"flag" is the match length of each string
	char str[2000][30] = { '0' };	//Array "str" is the input strings
	int result[2000] = { '0' };		//Array "result" is the final result strings

	//begin to input
	while (1)
	{
		ch = getchar();
		if (ch == EOF)
		{
			if (i == 0)
				break;
			else
			{
				str_num++;
				break;
			}
		}
		else if (ch == '\n')
		{
			str_num++;
			i = 0;
			continue;
		}
		str[str_num][i] = ch;
		i++;
	}

	//The first layer circle is to traversal all the strings
	for (i = 0; i < str_num; i++)
	{
		flag = 0;
		/*The second layer circle is to traversal all the strings, except the string which
		 *	is in the first layer circle*/
		for (j = 0; j < str_num; j++)
		{
			if (i == j)
			{
				continue;
			}
			/*The third layer circle is to traversal all the chars in the string, and find
			 *	the maximum match length of each string*/
			for (k = 0; k < 30; k++)
			{
				if (str[i][k] != str[j][k])
				{
					if (k >= flag)
					{
						flag = k;
					}
					break;
				}
			}
			if (str[i][flag] == '0')
			{
				break;
			}
		}
		result[i] = flag;
	}

	//begin to output
	for (i = 0; i < str_num; i++)
	{
		printf("%s ", str[i]);
		for (j = 0; j <= result[i]; j++)
		{
			if (str[i][j] == '\0')
			{
				continue;
			}
			putchar(str[i][j]);
		}
		putchar('\n');
	}
}