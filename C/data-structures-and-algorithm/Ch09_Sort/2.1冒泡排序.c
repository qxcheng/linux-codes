#include<stdio.h>
#include<stdlib.h>

void BubbleSort(int *array, int n)
{
    for(int i=n; i>1 ; i--)
    {
        for(int j=2; j<=i; j++)
        {
            if(array[j-1] > array[j])
            {
                array[0] = array[j];
                array[j] = array[j-1];
                array[j-1] = array[0];
            }
        }
    }
}

void main()
{
    int n;
    int *array;

    printf("����������������\n");
    scanf("%d", &n);

    printf("���������������ݣ�\n");
    array = (int *) malloc(sizeof(int) * (n+1));
    for(int i=1; i<n+1; i++)
        scanf("%d", &array[i]);

    BubbleSort(array, n);
    printf("����������Ϊ��\n");
    for(int j=1; j<n+1; j++)
        printf("%d ", array[j]);
}
