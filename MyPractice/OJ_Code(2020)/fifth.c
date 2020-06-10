#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Array "Clocks" stores all the pending clock status 
int Clocks[300000][9] = { 0 };
//Array "Clocks_Status" stores all possible clock status and the step to this status
int Clocks_Status[262144][18] = { 0 };

int main()
{
	int i = 0, j = 0, k = 0, l = 0, m = 0;	//Loop variables
	int tag = 0;	//tag is a clock status's position in a array
	int str_num = 0;	//str_num is the effective length of array "Clocks"
	int temp[18] = { 0 };	//Array "temp" stores a temporary clock status
	int count[9] = { 0 };	//Array "count" stores number which is power of 4 
	int operatings[9][9] = { 0 };	//Array "operatings" stores 9 kinds of operatings
	int Loop_variable[9] = { 0 };	//Array "Loop_variable" is a group of loop variables

	//get the power of 4(from 4^0 to 4^8)
	for (i = 8; i >=0; i--)
	{
		count[i] = pow(4, 8 - i);
	}

	//input all the pending clock status
	for (i = 0; i < 300000; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (scanf("%d", &Clocks[i][j]) == EOF)
			{
				tag = 1;
				break;
			}
		}
		if (tag)
		{
			break;
		}
		str_num++;
	}

	//initializing array "Clocks_Status"
	for (i = 0; i < 262144; i++)
	{
		for (j = 0; j < 9; j++)
		{
			Clocks_Status[i][j] = 12;
		}
	}

	//input 9 kinds of operatings 
	operatings[0][0] = 3, operatings[0][1] = 3, operatings[0][2] = 3;
	operatings[1][0] = 3, operatings[1][1] = 3, operatings[1][3] = 3, operatings[1][4] = 3;
	operatings[2][0] = 3, operatings[2][3] = 3, operatings[2][6] = 3;
	operatings[3][1] = 3, operatings[3][2] = 3, operatings[3][4] = 3, operatings[3][5] = 3;
	operatings[4][1] = 3, operatings[4][3] = 3, operatings[4][4] = 3, operatings[4][5] = 3, operatings[4][7] = 3;
	operatings[5][2] = 3, operatings[5][5] = 3, operatings[5][8] = 3;
	operatings[6][3] = 3, operatings[6][4] = 3, operatings[6][6] = 3, operatings[6][7] = 3;
	operatings[7][4] = 3, operatings[7][5] = 3, operatings[7][7] = 3, operatings[7][8] = 3;
	operatings[8][6] = 3, operatings[8][7] = 3, operatings[8][8] = 3;

	//get all possible clock status and the step to this status
	for (Loop_variable[0] = 0; Loop_variable[0] < 4; Loop_variable[0]++)
		for (Loop_variable[1] = 0; Loop_variable[1] < 4; Loop_variable[1]++)
			for (Loop_variable[2] = 0; Loop_variable[2] < 4; Loop_variable[2]++)
				for (Loop_variable[3] = 0; Loop_variable[3] < 4; Loop_variable[3]++)
					for (Loop_variable[4] = 0; Loop_variable[4] < 4; Loop_variable[4]++)
						for (Loop_variable[5] = 0; Loop_variable[5] < 4; Loop_variable[5]++)
							for (Loop_variable[6] = 0; Loop_variable[6] < 4; Loop_variable[6]++)
								for (Loop_variable[7] = 0; Loop_variable[7] < 4; Loop_variable[7]++)
									for (Loop_variable[8] = 0; Loop_variable[8] < 4; Loop_variable[8]++)
									{
										tag = 0;
										memset(temp, 0, sizeof(temp));
										for (i = 0; i < 9; i++)
										{
											for (j = 0; j < 9; j++)
											{
												temp[i] = temp[i] - Loop_variable[j] * operatings[j][i];
												if (temp[i] <= 0)
												{
													temp[i] += 12;
												}
											}
											temp[i + 9] = Loop_variable[i];
										}
										for (i = 0; i < 9; i++)
										{
											tag += ((12 - temp[i]) / 3) * count[i];
										}
										for (i = 0; i < 18; i++)
										{
											Clocks_Status[tag][i] = temp[i];
										}
									}
	
	//start to output
	for (i = 0; i < str_num; i++)
	{
		tag = 0;
		for (j = 0; j < 9; j++)
		{
			tag += ((12 - Clocks[i][j]) / 3) * count[j];
		}
		for (k = 17; k >= 9; k--)
		{
			if (Clocks_Status[tag][k] != 0)
			{
				break;
			}
		}
		for (l = 9; l <= k; l++)
		{
			for (m = 0; m < Clocks_Status[tag][l]; m++)
			{
				printf("%d", l-9);
				if (l == k && (m + 1) == Clocks_Status[tag][l])
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
}