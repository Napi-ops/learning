#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int QuickSort(int *arr, int low, int high)
{
    int count=0;
    if (low < high)
    {
        int i = low;
        int j = high;
        int k = arr[low];
        while (i < j)
        {
            while(i < j && arr[j] >= k)     // 从右向左找第一个小于k的数
            {
                j--;
                count++;
            }
 
            if(i < j)
            {
                arr[i++] = arr[j];
            }
 
            while(i < j && arr[i] < k)      // 从左向右找第一个大于等于k的数
            {
                i++;
                count++;
            }
 
            if(i < j)
            {
                arr[j--] = arr[i];
            }
        }
 
        arr[i] = k;
 
        // 递归调用
        QuickSort(arr, low, i - 1);     // 排序k左边
        QuickSort(arr, i + 1, high);    // 排序k右边
    }
    return(count);
}
//快排函数


int main()
{
    int i,j,t,tag=0;
    int nsort1[10000]={0},nsort2[10000]={0};
    long long count1=0,count2=0;
    int len=10000;


    srand(time(0));
    for(i=0;i<=9999;i++)
        nsort1[i]=rand()%10000001; 
    //得到随机数组


    for(i=0;i<=9999;i++)
        nsort2[i]=nsort1[i];
    for (j=10000;j >= 2;j--)
	{
		for (i=0;i < j - 1;i++)
		{
			if (nsort1[i] < nsort1[i + 1])
			{
				t = nsort1[i];
				nsort1[i] = nsort1[i + 1];
				nsort1[i + 1] = t;
                count1++;
			}
		}
	}
	for (i = 0;i < 10000;i++)
	{
		if (i % 5 == 0) printf("\n");
		printf("%d\t", nsort1[i]);      
	}   
     //冒泡排序


    count2=QuickSort(nsort2,0,len-1);
    for(i=10000;i>0;i--)
    {
        if(i%5==0) printf("\n");
        printf("%d\t",nsort2[i]);
    }
    printf("\n");
    //快排


    for(i=0;i<10000;i++)
    {
        if(nsort1[i]!=nsort2[9999-i])
            tag=1;
    }

    if(tag==0)
        printf("The result of these two kinds of sort is the same\n");
    else if(tag==1)
        printf("The result of these two kind of sort is different\n");
    
    printf("Bubble Sort needs to exchange %d times\n",count1);
    printf("QuickSort needs to exchange %d times\n",count2);
    return 0;
}
