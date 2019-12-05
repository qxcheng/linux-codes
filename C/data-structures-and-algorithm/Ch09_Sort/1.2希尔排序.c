#include<stdio.h>
#include<stdlib.h>

void ShellSort(int *array, int n)
{
    int k = n / 2; //��ʼ����
    while(k>0)
    {
        int i, j;
        for(i=k+1; i<n+1; i++)
        {
            if(array[i] < array[i-k])
            {
                array[0] = array[i];
                for(j=i-k; j>0 && array[0] < array[j]; j-=k)
                {
                    array[j+k] = array[j];
                }
                array[j+k] = array[0];
            }
        }
        k = k / 2;
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

    ShellSort(array, n);
    printf("����������Ϊ��\n");
    for(int j=1; j<n+1; j++)
        printf("%d ", array[j]);
}
