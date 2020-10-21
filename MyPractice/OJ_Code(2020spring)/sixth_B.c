#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char str[1000][1000][30] = { '\0' };	//Array "str" contains the file strings

/**************************************************
 *Function Name: cmp
 *Function: tell "qsort" function how to sort the strings
 *Parameter: const void* _a,_b - array pointer 
 *Return Value: int result - tell the "qsort" function using descending sort
 **************************************************/
int cmp(const void* _a, const void* _b)
{
	char* a = (char*)_a;
	char* b = (char*)_b;
	int result=strcmp(a, b);
	return result;
}

/**************************************************
 *Function Name: sort_files
 *Function: sort the files and dirs, and also print them
 *Parameter: int level - show which level the dir is 
			 int file_num - the number of files in a dir
			 char array dir - contain the dir which needs to be sorted
 *Return Value: None
 **************************************************/
void sort_files(int level, int file_num, char dir[][30])
{
	int i = 0, j = 0, k = 0, l = 0; //loop variable
	int tag = 0;	//"tag" is a judgment variable to decide copying the files
	int flag = 0;	//"flag" is a judgment variable to decide coping the dirs
	char files[1000][30] = { '\0' };	//Array "files" contains the files
	char folders[1000][30] = { '\0' };	//Array "files" contains the folders

	//output the ROOT
	if (level == 0)
	{
		printf("ROOT\n");
	}

	//get the files and dirs to recursive
	for (i = 0; i < file_num; i++)
	{
		if (dir[i][0] == 'f' && tag == 0)
		{
			strcpy(files[j++], dir[i]);
		}
		else if (dir[i][0] == 'd')
		{
			
			if (tag == 0)
			{
				flag = 1;
				//out put the dir
				for (l = 0; l < level+1; l++)
				{
					printf("|     ");
				}
				printf("%s\n", dir[i]);
			}
			else
			{
				strcpy(folders[k++], dir[i]);
			}
			tag++;
		}
		else if (dir[i][0] == ']')
		{
			tag--;
			if (tag == 0)
			{
				sort_files(level+1, k, folders);
				memset(folders,'\0',sizeof(folders));
				flag = 0;
				k = 0;
			}
			else
			{
				strcpy(folders[k++], dir[i]);
			}
		}
		else if (dir[i][0] == 'f' && flag == 1)
		{
			strcpy(folders[k++], dir[i]);
		}
	}

	//sort the files
	qsort(files, j, sizeof(files[0]), cmp);

	//output the files
	for (i = 0; i < j; i++)
	{
		for (l = 0; l < level; l++)
		{
			printf("|     ");
		}
		printf("%s\n", files[i]);
	}
}

int main(void)
{
	int i = 0, j = 0, k = 0;
	char ch;	//"ch" receive the input
	int dataset_num = 0;	//"str_num" is the number of datasets
	int file_num[1000] = { 0 };	//Array "file_num" is the number of files;

	//input the files
	while ((ch = getchar()) != '#')
	{
		if (ch == '\n')
		{
			if (str[i][j][k - 1] == '*')
			{
				j = 0;
				k = 0;
				i++;
				dataset_num++;
			}
			else
			{
				k = 0;
				j++;
				file_num[i]++;
			}
			continue;
		}
		str[i][j][k++] = ch;
	}

	//use recursive function to print the sorted files
	for (i = 0; i < dataset_num; i++)
	{
		if (i != 0)
		{
			printf("\n");
		}
		printf("DATA SET %d:\n", i + 1);
		sort_files(0, file_num[i], str[i]);
	}
}