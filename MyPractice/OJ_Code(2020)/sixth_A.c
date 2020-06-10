#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int flag = 0;	//"flag" is a loop variable of Array "ans_str"
int str_tag = 0;	//"str_tag" is a loop variable of Array "str"
char ch = '\0';	//"ch" refer to a char in Array "str"
char str[50] = { '\0' };	//Array "str" is the string which need to deal with
char ans_str[50] = { '\0' };	//Array "ans_str" contains the answer string


/**************************************************
 *Function Name: rev_RPN
 *Function: reverse the Reverse Polish notation
 *Parameter: tag - a judgment variable to decide the brackets in answer string
 *Return Value: None
 **************************************************/
void rev_RPN(int tag)
{
	int brack_tag = 0;	//"brack_tag" a judgment variable to decide the brackets in answer string
	str_tag--;

	//start to reverse
	if (str_tag >= 0)
	{
		ch = str[str_tag];
		//decide the brackets in answer string
		if (tag == 1 && ch != '*' && ch != '/')
		{
			brack_tag = 1;
		}
		if (ch == '+' || ch == '-')
		{
			if (!isdigit(str[str_tag + 1]) && str[str_tag + 1] != '\0')
			{
				brack_tag = 1;
			}
		}
		if (ch == '/' && tag == 1)
		{
			if (str[str_tag + 1] == '*' && str[str_tag + 1] != '\0')
			{
				brack_tag = 1;
			}
		}
		if (ch == '*' && tag == 1)
		{
			if (str[str_tag - 1] == '/' && str_tag != 0)
			{
				brack_tag = 1;
			}
		}
		//start to recursive
		switch (ch)
		{
		case'+':
		{
			if (brack_tag == 1)
			{
				ans_str[flag++] = ')';
			}
			rev_RPN(0);
			ans_str[flag++] = '+';
			rev_RPN(0);
			if (brack_tag == 1)
			{
				ans_str[flag++] = '(';
			}
			break;
		}
		case'-':
		{
			if (brack_tag == 1)
			{
				ans_str[flag++] = ')';
			}
			rev_RPN(0);
			ans_str[flag++] = '-';
			rev_RPN(0);
			if (brack_tag == 1)
			{
				ans_str[flag++] = '(';
			}
			break;
		}
		case'*':
		{
			if (brack_tag == 1)
			{
				ans_str[flag++] = ')';
			}
			rev_RPN(1);
			ans_str[flag++] = '*';
			rev_RPN(1);
			if (brack_tag == 1)
			{
				ans_str[flag++] = '(';
			}
			break;
		}
		case'/':
		{
			if (brack_tag == 1)
			{
				ans_str[flag++] = ')';
			}
			rev_RPN(1);
			ans_str[flag++] = '/';
			rev_RPN(1);
			if (brack_tag == 1)
			{
				ans_str[flag++] = '(';
			}
			break;
		}
		default:
		{
			ans_str[flag++] = ch;
			break;
		}
		}
	}
}

int main(void)
{
	int i = 0;
	//input the RPN
	scanf("%s", str);
	str_tag = strlen(str);
	//use recursive function to reverse the RPN
	rev_RPN(0);
	//output the answer string
	for (i = flag - 1; i >= 0; i--)
	{
		putchar(ans_str[i]);
	}
}