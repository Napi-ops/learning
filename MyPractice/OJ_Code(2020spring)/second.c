#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(void)
{
	int n = 0;	//Variable "n" is the number of cities
	int k = 0;	//Variable "k" is the number of capitals
	int temp = 0;   //Variable "temp" is a temporary variable to receive the input
	long long sum_1 = 0;	//Variable "sum_1" is the total charm of building roads which link two capital city 
	long long sum_2 = 0;	//Variable "sum_2" is the total charm of building roads which link a city and its next city
	long long total_charm = 0;	//Variable "total_charm" is the total charm of every cities
	long long normal_charm = 0;	//Variable "normal_charm" is the total charm of the normal city
	unsigned long long int result = 0; //Variable "result" is the final result
	int charm[100100] = { 0 };	//Array "charm" contains the charm of each city
	int capital[100100] = { 0 };	//Array "capital" contains the subscript of each city
	int index[100100] = { 0 };	//Array "index" decide whether this city should build a road to its next city
	long long capital_charm_add[100100] = { 0 };	//Array "capital_charm_add" contains the total charm of the capital city whose subscript is under i

	//input the value of n, k, and the charm of every city
	scanf("%d%d", &n, &k);
	for (int i = 0; i != n; ++i)
	{
		scanf("%d", &charm[i]);
		total_charm += charm[i];
	}

	//get the subscript of every capital city
	for (int i = 0; i != k; ++i)
	{
		scanf("%d", &temp);
		capital[i] = temp - 1;
		if (i == 0)
			capital_charm_add[0] = charm[capital[0]];
		else if (i != 0)
			capital_charm_add[i] = capital_charm_add[i - 1] + charm[capital[i]];
		if (temp - 1 == 0)
		{
			index[0] = 1;
			index[n - 1] = 1;
		}
		else
		{
			index[temp - 1] = 1;
			index[temp - 2] = 1;
		}
	}

	/*capital_charm_add[k - 1] is the total charm of every capital city
	 *so normal_charm is total_charm minus capital_charm_add[k - 1]*/
	normal_charm = total_charm - capital_charm_add[k - 1];

	//get the value of sum_1 and sum_2
	for (int i = k - 1; i != 0; --i)
		sum_1 = sum_1 + charm[capital[i]] * capital_charm_add[i - 1];
	for (int i = 0; i != n; ++i)
	{
		if (index[i] == 1)
			continue;
		else
		{
			if (i != n - 1)
				sum_2 = sum_2 + charm[i] * charm[i + 1];
			else if (i == n - 1)
				sum_2 = sum_2 + charm[n - 1] * charm[0];
		}
	}

	//output the result
	result = sum_1 + sum_2 + normal_charm * capital_charm_add[k - 1];
	printf("%llu\n", result);
}