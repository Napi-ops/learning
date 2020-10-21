#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int num = 0;	//"num" is the length of a permutation
int sum = 0;	//"sum" is the sum of a permutation
int number[15] = { 0 };		//array "number" contains a standard arrangement
int permutation[15] = { 0 };	//array "permutation" is a permutation

/**************************************************
 *Function Name: get_sum
 *Function: get the sum of a permutation
 *Parameter: array int arr_sum - an array which needs to get sum
			 int arr_len - the length of the array
 *Return Value: int result - the sum of a permutation
 **************************************************/
int get_sum(int arr_sum[], int arr_len)
{
	int i = 0;
	int result = 0;		//"result" is the sum of a permutation
	int level_next[15] = { 0 };	//array "level_next" is the next array which needs to get sum

	//if there is only one element in the array, return the sum
	if (arr_len == 1)
	{
		return arr_sum[0];
	}

	//add each two elements in arr_sum, and store the value in level_next 
	for (i = 0; i < arr_len - 1; i++)
	{
		level_next[i] = arr_sum[i] + arr_sum[i + 1];
	}

	//use recursive to get the total sum
	result = get_sum(level_next, arr_len - 1);
	return result;
}

/**************************************************
 *Function Name: permutation_next
 *Function: get the next permutation
 *Parameter: int len - the length of the permutation
 *Return Value: 1 or 0, return 1 means matching successfully, return 0 means unsuccessfully
 **************************************************/
int permutation_next(int len)
{
	int i = 0;
	int tag = 0;	//"tag" shows whether match successfully
	for (i = 0; i < num; i++)
	{
		//assign values to permutation
		if (number[i] == -1)
		{
			continue;
		}
		else if (number[i] != -1)
		{
			permutation[num - len] = number[i];
			number[i] = -1;
		}

		//if get the permutation, calculate its sum
		if (len == 1)
		{
			if (get_sum(permutation, num) == sum)
			{
				tag = 1;
				break;
			}
			else
			{
				number[i] = i + 1;
				permutation[num - len] = 0;
			}
		}
		//else use recursive to get the permutation
		else
		{
			if (permutation_next(len - 1))
			{
				tag = 1;
				break;
			}
			else
			{
				number[i] = i + 1;
				permutation[num - len] = 0;
			}
		}
	}
	//if match succcessfully, return 1, else return 0
	if (tag == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int main()
{
	int i = 0;

	//input num and sum
	scanf("%d%d", &num, &sum);

	//initialize array "number"
	for (i = 0; i < num; i++)
	{
		number[i] = i + 1;
	}

	//if get the right permutation, output it
	if (permutation_next(num))
	{
		for (i = 0; i < num; i++)
		{
			printf("%d", permutation[i]);
			if (i == num - 1)
			{
				printf("\n");
			}
			else
			{
				printf(" ");
			}
		}
	}
}