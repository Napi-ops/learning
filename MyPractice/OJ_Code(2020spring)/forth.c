#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char answer[1000000][100] = { 0 };

/**************************************************
 *Function Name: delete_zero
 *Function: delete leading zero and decimal point
 *Parameter: number - string which need to delete zero
 *Return Value: decimal_point - the minimum magnifications to make a decimal become an integer
 **************************************************/
int delete_zero(char number[10])
{
	int i = 0, j = 0;
	/*tag_point is the position of decimal point,
	 *and tag_zero is the position of the last zero*/
	int tag_point = 0, tag_zero = 0;
	int decimal_point = 0;
	char number_copy[10] = { 0 };
	//if the decimal point is on the first position of a string
	if (number[0] == '.')
	{
		for (i = 5; i != 0; i--)
		{
			if (number[i] != '0')
			{
				tag_point = i;
				break;
			}
		}
		for (i = 1; i != 6; i++)
		{
			if (number[i] != '0')
			{
				tag_zero = i;
				break;
			}
		}
		for (i = tag_zero; i != (tag_point + 1); i++)
			number[i - tag_zero] = number[i];
		for (i = tag_point - tag_zero + 1; i != 6; i++)
			number[i] = 0;
		decimal_point = tag_point;
	}
	//if the decimal point is on the last position of a string
	else if (number[5] == '.')
	{
		for (i = 0; i != 5; i++)
		{
			if (number[i] != '0')
			{
				decimal_point = 5 - i;
				break;
			}
		}
		for (i = 0; i != decimal_point; i++)
			number[i] = number[i + 5 - decimal_point];
		for (i = decimal_point; i != 6; i++)
			number[i] = 0;
		decimal_point = 0;
	}
	//if the decimal point is on the middle of a a string
	else
	{
		for (i = 0; i != 6; i++)
		{
			number_copy[i] = number[i];
			if (number[i] == '.')
				tag_point = i;
			number[i] = 0;
		}
		for (i = 0; i != tag_point + 1; i++)
		{
			if (number_copy[i] != '0')
			{
				tag_zero = i;
				break;
			}
		}
		for (i = tag_zero; i != tag_point; i++, j++)
			number[i - tag_zero] = number_copy[i];
		for (i = 5; i != tag_point - 1; i--)
		{
			if (number_copy[i] != '0')
			{
				tag_zero = i;
				break;
			}
		}
		for (i = 0; i != tag_zero - tag_point; i++, decimal_point++)
			number[j + i] = number_copy[tag_point + 1 + i];
		if (number[0] == '0')
		{
			for (i = 0; i != tag_zero - tag_point; i++)
			{
				if (number[i] != '0')
				{
					tag_point = i;
					break;
				}
			}
			for (i = tag_point; i != tag_zero + 1; i++)
				number[i - tag_point] = number[i];
			for (i = tag_zero - tag_point + 1; i != 6; i++)
				number[i] = 0;
		}
	}
	return decimal_point;
}

/**************************************************
 *Function Name: multiply
 *Function: mutiply two numbers and return its product
 *Parameter: number_1, number_2 - two numbers which need to multiply,
			 result - store the result of multiply
 *Return Value: result_len - the length of result number
 **************************************************/
int multiply(int number_1[], int number_2[], int result[])
{
	int i = 0, j = 0, k = 0;
	int result_len = 0;
	int number1_len = 0;
	int number2_len = 0;
	for (i = 299; i != -1; i--)
	{
		if (number_1[i] != '\0')
		{
			number1_len = i + 1;
			break;
		}
	}
	for (i = 9; i != -1; i--)
	{
		if (number_2[i] != '\0')
		{
			number2_len = i + 1;
			break;
		}
	}
	for (i = 0; i != number2_len; i++)
	{
		for (j = 0; j != number1_len; j++)
		{
			result[i + j] += number_1[j] * number_2[i];
		}
	}
	for (k = 0; k != number1_len * 2 + 10; k++)
	{
		if (result[k] >= 10)
		{
			result[k + 1] += result[k] / 10;
			result[k] = result[k] % 10;
		}
	}
	for (i = number1_len * 2 + 10; i != -1; i--)
	{
		if (result[i] != '\0')
		{
			result_len = i + 1;
			break;
		}
	}
	return result_len;
}

int main(void)
{
	int i = 0, j = 0, k = 0;
	char ch = '\0';
	int power = 0;
	int tag = 1;	//tag controls which two number need to multiply and which string stores the result
	int str_num = 0;	//str_num is the total number of strings
	int num_len = 0;	//num_len is the length of a number
	int decimal = 0;	//decimal is the minimum magnifications to make a decimal become an integer
	char number[10] = { 0 };
	int number_reverse[10] = { 0 };
	int number_1[300] = { 0 };
	int number_2[300] = { 0 };
	while (scanf("%s%d", number, &power) == 2)
	{
		getchar();
		j = 0;
		num_len = 0;
		//delete zero
		decimal = delete_zero(number);
		for (i = 0; number[i] != '\0'; i++, num_len++);
		for (i = num_len - 1; i != -1; i--, j++)
			number_reverse[j] = number[i] - '0';
		for (i = 0; i != 10; i++)
			number_1[i] = number_reverse[i];
		//begin to multiply
		for (tag = 1; tag != power; tag++)
		{
			if (tag % 2 != 0)
			{
				num_len = multiply(number_1, number_reverse, number_2);
				memset(number_1, 0, sizeof(number_1));
			}
			else if (tag % 2 == 0)
			{
				num_len = multiply(number_2, number_reverse, number_1);
				memset(number_2, 0, sizeof(number_2));
			}
		}
		//get the result string and add decimal point to this string
		if (tag % 2 == 0)
		{
			j = 0;
			if (decimal == 0)
			{
				for (i = num_len - 1; i != -1; i--)
				{
					answer[str_num][j++] = number_2[i] + 48;
				}
			}
			else
			{
				if (num_len < decimal * power)
				{
					answer[str_num][j++] = '.';
					for (i = 0; i != decimal * power - num_len; i++)
					{
						answer[str_num][j++] = '0';
					}
					for (i = num_len - 1; i != -1; i--)
					{
						answer[str_num][j++] = number_2[i] + 48;
					}
				}
				else
				{
					for (i = num_len - 1; i != -1; i--)
					{
						if (i == decimal * power - 1)
							break;
						answer[str_num][j++] = number_2[i] + 48;
					}
					answer[str_num][j++] = '.';
					for (k = i; k != -1; k--)
						answer[str_num][j++] = number_2[k] + 48;
				}

			}
		}
		else if (tag % 2 != 0)
		{
			j = 0;
			if (decimal == 0)
			{
				for (i = num_len - 1; i != -1; i--)
				{
					answer[str_num][j++] = number_1[i] + 48;
				}
			}
			else
			{
				if (num_len < decimal * power)
				{
					answer[str_num][j++] = '.';
					for (i = 0; i != decimal * power - num_len; i++)
					{
						answer[str_num][j++] = '0';
					}
					for (i = num_len - 1; i != -1; i--)
					{
						answer[str_num][j++] = number_1[i] + 48;
					}
				}
				else
				{
					for (i = num_len - 1; i != -1; i--)
					{
						if (i == decimal * power - 1)
							break;
						answer[str_num][j++] = number_1[i] + 48;
					}
					answer[str_num][j++] = '.';
					for (k = i; k != -1; k--)
						answer[str_num][j++] = number_1[k] + 48;
				}

			}
		}
		str_num++;
		memset(number, 0, sizeof(number));
		memset(number_1, 0, sizeof(number_1));
		memset(number_2, 0, sizeof(number_2));
		memset(number_reverse, 0, sizeof(number_reverse));
	}
	for (i = 0; i != str_num; i++)
	{
		printf("%s", answer[i]);
		putchar('\n');
	}
}