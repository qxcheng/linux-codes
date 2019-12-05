#include<stdio.h>
#include<stdlib.h>

void SelectSort(int *array, int n)
{
    int k;
    for(int i=1; i<n+1; i++)
    {
        k = i;
        for(int j=i+1; j<n+1; j++)
        {
            if(array[j] < array[k])
            {
               k = j;
            }
        }
        if(k != i)
        {
            array[0] = array[k];
            array[k] = array[i];
            array[i] = array[0];
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

    SelectSort(array, n);
    printf("排序后的数据为：\n");
    for(int j=1; j<n+1; j++)
        printf("%d ", array[j]);
}
