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

    printf("������Ԫ�ص��ܸ�����\n");
    scanf("%d", &n);

    printf("�밴������������Ԫ�أ�\n");
    array = (int *) malloc(sizeof(int) * n);
    for(int i=0; i<n; i++)
        scanf("%d", &array[i]);

    printf("��������Ҫ���ҵ�Ԫ�أ�\n");
    scanf("%d", &key);

    position = BinarySearch(array, key, 0, n-1);
    if(position)
    {
        printf("%d���ڵ�λ��Ϊ��%d����\n", key, position+1);
    }
    else
    {
        printf("δ���ҵ���Ԫ�أ�\n");
    }
    return 0;
}
