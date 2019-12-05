#include <stdio.h>
#include <stdlib.h>

int n;    //元素个数
int bit_num;    //最大数字位数

/*
 * 获取相应位置上的数（从右到左）
 */
int GetNumInPos(int num, int pos)
{
    int i, temp = 1;
    for (i = 0; i < pos - 1; i++)
    {
        temp *= 10;
    }
    return (num / temp) % 10;
}

/*
 * 基数排序（LSD）
 */
void RadixSort(int *array)
{
    int radix = 10;
    int *count, *bucket, i, j, k;
    count = (int*) malloc(sizeof(int) * radix);
    bucket = (int*) malloc(sizeof(int) * n);
    for (k = 1; k <= bit_num; k++)
    {
        for (i = 0; i < radix; i++)
        {
            count[i] = 0;
        }
        //统计各个桶中所盛数据个数
        for (i = 0; i < n; i++)
        {
            count[GetNumInPos(array[i], k)]++;
        }
        //count[i]表示第i个桶的右边界索引
        for (i = 1; i < radix; i++)
        {
            count[i] = count[i] + count[i - 1];
        }
        //分配
        for (i = n - 1; i >= 0; i--)
        {
            j = GetNumInPos(array[i], k);
            bucket[count[j] - 1] = array[i];
            count[j]--;
        }
        //收集
        for (i = 0, j = 0; i < n; i++, j++)
        {
            array[i] = bucket[j];
        }
    }
}

int main()
{
    int i;
    int *array;
    printf("请输入最大数字的位数：");
    scanf("%d", &bit_num);
    printf("请输入数组的大小：");
    scanf("%d", &n);
    array = (int*) malloc(sizeof(int) * n);
    printf("请输入数据（用空格分隔）：");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &array[i]);
    }
    RadixSort(array);
    printf("排序后为：");
    for (i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}