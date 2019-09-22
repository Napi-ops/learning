#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int mine[100][100], s[100][100], f[100][100];
char p[100][100];
int n, m, x;

char search(int row, int column)
{
	int i, j;
	f[row][column] = 1;
	if (s[row][column] != 0) return (char)s[row][column] + 48;
	for (i = row - 1;i <= row + 1;i++)
		for (j = column - 1;j <= column + 1;j++)
			if (f[i][j] == 0 && i <= n && i >= 1 && j <= m && j >= 1)
				p[i][j] = search(i, j);
	return ' ';
}

int win()
{
	int i, j, sum;
	sum = 0;
	for (i = 1;i <= n;i++)
		for (j = 1;j <= m;j++)
			if (p[i][j] == '#') sum++;
	if (sum == x) return 1; else return 0;
}

void answer()
{
	int i, j;
	for (i = 1;i <= n;i++)
		for (j = 1;j <= m;j++)
			if (mine[i][j] == 1) p[i][j] = '*';
	for (i = 1;i <= n;i++)
	{
		for (j = 1;j <= m;j++) printf("%c ", p[i][j]);
		printf("\n");
	}
	printf("\n");
}
int main()
{
	int i, j, r1, r2, row, column;
	printf("please input row and column\n");
	printf("row=");
	scanf_s("%d", &n);
	printf("column=");
	scanf_s("%d", &m);
	printf("mine=");
	scanf_s("%d", &x);

	srand(1);
	for (i = 0;i <= n + 1;i++)
		for (j = 0;j <= m + 1;j++)
		{
			mine[i][j] = 0;
			p[i][j] = '#';
			s[i][j] = 0;
			f[i][j] = 0;
		}
	for (i = 1;i <= x;i++)
	{
		while (0 == 0)
		{
			r1 = rand() % n + 1;
			r2 = rand() % m + 1;
			if (mine[r1][r2] == 1) continue;
			else
			{
				mine[r1][r2] = 1;//ÓÐÀ×
				break;
			}
		}
	}
	for (row = 1;row <= n;row++)
		for (column = 1;column <= m;column++)
			for (i = row - 1;i <= row + 1;i++)
				for (j = column - 1;j <= column + 1;j++)
					if (mine[i][j] == 1) s[row][column]++;
	/*for (i=1;i<=n;i++)
	{
		for (j=1;j<=m;j++) printf("%d ",mine[i][j]);
		printf("\n");
	}*/
	printf("Begin!\n");
	for (i = 1;i <= n;i++)
	{
		for (j = 1;j <= m;j++) printf("%c ", p[i][j]);
		printf("\n");
	}
	printf("\n");

	while (0 == 0)
	{
		printf("row and column:");
		scanf_s("%d %d", &row, &column);
		if (mine[row][column] == 1)
		{
			printf("you lose!\n");
			answer();
			break;
		}

		p[row][column] = search(row, column);

		if (win() == 1)
		{
			printf("you win!\n");
			answer();
			break;
		}

		for (i = 1;i <= n;i++)
		{
			for (j = 1;j <= m;j++) printf("%c ", p[i][j]);
			printf("\n");
		}
		printf("\n");
	}
	return 0;
}