#include<stdio.h>
#include<stdlib.h>

int MidIndex(int *array, int low, int high)
{
    int mid_value = array[low];
    array[0] = array[low];
    while(low < high)
    {
        while(low<high && array[high]>=mid_value)
        {
            high--;
        }
        array[low] = array[high];
        while(low<high && array[low]<=mid_value)
        {
            low++;
        }
        array[high] = array[low];
    }
    array[low] = array[0];
    return low;
}

void QuickSort(int *array, int low, int high)
{
    if(low < high)
    {
        int mid_index = MidIndex(array, low, high);
        QuickSort(array, low, mid_index-1);
        QuickSort(array, mid_index+1, high);
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

    QuickSort(array, 1, n);
    printf("排序后的数据为：\n");
    for(int j=1; j<n+1; j++)
        printf("%d ", array[j]);
}
