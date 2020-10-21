#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int map_Row = 0, map_Col = 0;	//"Row" is the number  of rows, "Col" is the number of columns
int MR = 0, mr = 0,MC = 0,mc = 0;	//"MR" and "MC" is the max row and col, "mr" and "mc" is the min row and col
char map[2010][2010] = { '\0' };	//Array "map" contains the map

/**************************************************
 *Function Name: find_island
 *Function: find an island and mark it in the map
 *Parameter: int Row, Col - the row and colume of a land
 *Return Value: None
 **************************************************/
void find_island(int Row,int Col)
{
	int i = 0, j = 0;	//Loop variable
	if (Row > MR)
	{
		MR = Row;
	}
	if (Row < mr)
	{
		mr = Row;
	}
	if (Col > MC)
	{
		MC = Col;
	}
	if (Col < mc)
	{
		mc = Col;
	}
	map[Row][Col] = '$';
	for (i = Row - 1; i <= Row + 1; i++)
	{
		for (j = Col - 1; j <= Col + 1; j++)
		{
			if (i >= map_Row || j >= map_Col || i < 0 || j < 0)
			{
				continue;
			}
			if (map[i][j] == '*')
			{
				find_island(i, j);
			}
		}
	}
}

int main(void)
{
	int i = 0, j = 0, k = 0, l = 0;	//Loop variable
	int area = 0;	//"area" is the area of an island's territorial sea
	int max_area = 0;	//"max_area" is the max area of all territorial sea

	//input the Row, Col, and map
	scanf("%d%d", &map_Row, &map_Col);
	mr = map_Row - 1;
	mc = map_Col - 1;
	for (i = 0; i < map_Row; i++)
	{
		scanf("%s", map[i]);
	}

	//find an island and calculate its territorial sea
	for (i = 0; i < map_Row; i++)
	{
		for (j = 0; j < map_Col; j++)
		{
			if (map[i][j] == '*')
			{
				find_island(i, j);
				for (k = mr; k <= MR; k++)
				{
					for (l = mc; l <= MC; l++)
					{
						if (map[k][l] == '#')
						{
							area++;
						}
					}
				}
				if(area > max_area)
				{
					max_area = area;
				}
				MR = 0;
				mr = map_Row - 1;
				MC = 0;
				mc = map_Col - 1;
				area = 0;
			}
		}
	}

	//output the max area
	printf("%d\n", max_area);
}