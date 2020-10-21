#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MARK 109	//in array "mos_like", position "MARK" contains the serial number of an array
#define LENGTH 108	//in array "mos_like", position "LENGTH" contains the length of an array 
#define RESULT 107	//in array "mos_like", position "RESULT" contains the result of a match 

int num = 0;	//"num" is the number of mosquitos
int people[110] = { 0 };	//array "people" contains people's number
int mos_like[110][110] = { 0 };	//array "mos_like" contains mosquitos' like

/**************************************************
 *Function Name: cmp_len
 *Function: tell "qsort" function how to sort the array
 *Parameter: const void* _a,_b - array pointer
 *Return Value: int result - tell the "qsort" function sort from small to large by array length
 **************************************************/
int cmp_len(const void* a, const void* b)
{
	int p = ((int*)a)[LENGTH];
	int q = ((int*)b)[LENGTH];
	return p - q;
}

/**************************************************
 *Function Name: cmp_mark
 *Function: tell "qsort" function how to sort the array
 *Parameter: const void* _a,_b - array pointer
 *Return Value: int result - tell the "qsort" function sort from small to large by array mark
 **************************************************/
int cmp_mark(const void* a, const void* b)
{
	int p = ((int*)a)[MARK];
	int q = ((int*)b)[MARK];
	return p - q;
}

/**************************************************
 *Function Name: match
 *Function: match mosquitos' like
 *Parameter: int number - the number of mosquitos
 *Return Value: 1 or 0, return 1 means matching successfully, return 0 means failure
 **************************************************/
int match(int number)
{
	int i = 0;
	for (i = 0; i < mos_like[num - number][LENGTH]; i++)
	{
		if (people[mos_like[num - number][i] - 1] == -1)
		{
			continue;
		}
		else if (people[mos_like[num - number][i] - 1] >= 1)
		{
			if (number == 1)
			{
				mos_like[num - number][RESULT] = mos_like[num - number][i];
				return 1;
			}
			else if (number > 1)
			{
				mos_like[num - number][RESULT] = mos_like[num - number][i];
				people[mos_like[num - number][i] - 1] = -1;
				if (match(number - 1))
				{
					return 1;
				}
				else
				{
					mos_like[num - number][RESULT] = 0;
					people[mos_like[num - number][i] - 1] = mos_like[num - number][i];
				}
			}
		}
	}
	return 0;
}

int main(void)
{
	int i = 0, j = 0;	//loop variable
	char temp = 0;	//"temp" receive the input

	//input mosquitos' like
	scanf("%d", &num);
	getchar();
	for (i = 0; i < num; i++)
	{
		while (1)
		{
			scanf("%d", &mos_like[i][j]);
			mos_like[i][j++]++;
			if ((temp = getchar()) == '\n')
			{
				j = 0;
				break;
			}
		}
	}

	//assign values to array "people" and "mos_like"
	for (i = 0; i < num; i++)
	{
		people[i] = i + 1;
		mos_like[i][MARK] = i + 1;
		for (j = 0; j < num; j++)
		{
			if (mos_like[i][j] != 0)
			{
				mos_like[i][LENGTH]++;
			}
			else if (mos_like[i][j] == 0)
			{
				break;
			}
		}
	}

	//sort from small to large by array length
	qsort(mos_like, num, sizeof(mos_like[0]), cmp_len);

	//match mosquitos' like
	match(num);

	//sort from small to large by array mark
	qsort(mos_like, num, sizeof(mos_like[0]), cmp_mark);

	//output the result
	for (i = 0; i < num; i++)
	{
		printf("%d\n", mos_like[i][RESULT] - 1);
	}
}