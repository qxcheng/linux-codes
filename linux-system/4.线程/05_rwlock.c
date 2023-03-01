/* 3个线程不定时 "写" 全局资源，5个线程不定时 "读" 同一全局资源 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int counter;                 //全局资源
pthread_rwlock_t rwlock;

void *th_write(void *arg)    //写方式请求锁，请求写锁
{
    int t;
    int i = (int)arg;

    while (1) {
        t = counter;
        usleep(1000);

        pthread_rwlock_wrlock(&rwlock);
        printf("=======write %d: %lu: counter=%d ++counter=%d\n", i, pthread_self(), t, ++counter);
        pthread_rwlock_unlock(&rwlock);

        usleep(5000);
    }
    return NULL;
}

void *th_read(void *arg)    //读方式请求锁，请求读锁
{
    int i = (int)arg;

    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("----------------------------read %d: %lu: %d\n", i, pthread_self(), counter);
        pthread_rwlock_unlock(&rwlock);

        usleep(900);
    }
    return NULL;
}

int main(void)
{
    int i;
    pthread_t tid[8];

    pthread_rwlock_init(&rwlock, NULL);   //初始化读写锁

    for (i = 0; i < 3; i++)
        pthread_create(&tid[i], NULL, th_write, (void *)i);

    for (i = 0; i < 5; i++)
        pthread_create(&tid[i+3], NULL, th_read, (void *)i);

    for (i = 0; i < 8; i++)
        pthread_join(tid[i], NULL);

    pthread_rwlock_destroy(&rwlock);            //释放读写琐

    return 0;
}
