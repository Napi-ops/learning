#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**************************************************
 *Function Name: divide
 *Function: divide the number and return the amount of different tpyes of division
 *Parameter: int integer - a number which needs to be divided
			 int part - decides the number should be divided into how many parts
			 int min_num - the minimum number of loop variable
 *Return Value: int result - the amount of different tpyes of division
 **************************************************/
int divide(int integer, int part,int min_num)
{
	int i = 0;
	int max_num = 0;	//the maximum number of loop variable
	int result = 0;		//the amount of different tpyes of division
	if (part == 1)
	{
		return 1;
	}
	else
	{
		max_num = integer / part + 1;
		for (i = min_num; i < max_num; i++)
		{
			result = result + divide(integer - i, part - 1, i);
		}
		return result;
	}
}

int main(void)
{
	int i = 0;
	int num_len = 0; //"num_len" is the amount of all the numbers which need to divide
	int result = 0;	 //"result" is the number of different tpyes of division
	int number[2000][2] = { 0 };	//array "number" contains all the numbers which need to divide

	//input the numbers
	while (scanf("%d%d", &number[i][0], &number[i][1]) == 2)
	{
		i++;
		num_len++;
	}

	//divide the numbers and print the result
	for (i = 0; i < num_len; i++)
	{
		result = divide(number[i][0], number[i][1], 1);
		printf("%d\n", result);
	}
}