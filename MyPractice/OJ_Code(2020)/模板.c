#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void solve(void);

int main()
{
	const int DEBUGGING = 1;
	if (!DEBUGGING) {
		solve();
		return 0;
	}
	const int NUMBER_OF_TESTCASES = 10;
	for (int _index = 1; _index <= NUMBER_OF_TESTCASES; ++_index) {
		char file_name[20];
		sprintf(file_name, "testcases/%d.in", _index);
		freopen(file_name, "r", stdin);
		sprintf(file_name, "testcases/%d.ans", _index);
		freopen(file_name, "w", stdout);
		int t1 = clock();
		solve();
		t1 = clock() - t1;
		fprintf(stderr, "Testcase %2d: time consuming %8lf seconds\n", _index, (double)t1 / CLOCKS_PER_SEC);
		fclose(stdin);
		fclose(stdout);
	}
	return 0;
}

void solve() 
{
}