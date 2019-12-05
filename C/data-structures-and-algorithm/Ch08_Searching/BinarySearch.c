#include<stdio.h>
#include<stdlib.h>

int BinarySearch(int *array, int key, int low, int high)
{
    int mid;
    while(low <= high)
    {
        mid = (low + high) / 2;
        if(key == array[mid])
        {
            return mid;
        }
        else if(key < array[mid])
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return 0;
}

int main()
{
    int n, key, position;
    int *array;

    printf("请输入元素的总个数：\n");
    scanf("%d", &n);

    printf("请按升序输入所有元素：\n");
    array = (int *) malloc(sizeof(int) * n);
    for(int i=0; i<n; i++)
        scanf("%d", &array[i]);

    printf("请输入需要查找的元素：\n");
    scanf("%d", &key);

    position = BinarySearch(array, key, 0, n-1);
    if(position)
    {
        printf("%d所在的位置为第%d个！\n", key, position+1);
    }
    else
    {
        printf("未查找到该元素！\n");
    }
    return 0;
}
