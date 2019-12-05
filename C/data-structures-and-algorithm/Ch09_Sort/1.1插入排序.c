#include<stdio.h>
#include<stdlib.h>

void InsertSort(int *array, int n)
{
    int i, j;
    for(i=2; i<n+1; i++)
    {
        if(array[i] < array[i-1])
        {
            array[0] = array[i];
            for(j=i-1; array[0]<array[j]; j--)
            {
                array[j+1] = array[j];
            }
            array[j+1] = array[0];
        }
    }
}

void main()
{
    int n;
    int *array;

    printf("请输入数据总数：\n");
    scanf("%d", &n);

    printf("请输入待排序的数据：\n");
    array = (int *) malloc(sizeof(int) * (n+1));
    for(int i=1; i<n+1; i++)
        scanf("%d", &array[i]);

    InsertSort(array, n);
    printf("排序后的数据为：\n");
    for(int j=1; j<n+1; j++)
        printf("%d ", array[j]);
}
