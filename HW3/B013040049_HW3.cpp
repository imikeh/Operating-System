#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>

#define N 5
#define LEFT (phil_num+4)%N
#define RIGHT (phil_num+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define TRUE 1

void *philospher(void *num);

int state[N];
int phil_num[N]= {0,1,2,3,4};

class moniter{
private:
    int state[N];
    pthread_mutex_t mutex; /*thread互斥鎖,pthread_mutex_t結構為不公開的數據類型*/
    pthread_cond_t S[N]; /*thread條件鎖,條件變量的結構*/

public:
    moniter(){
        int i;
        pthread_mutex_init(&mutex,NULL); /*初始化,生成一個互斥鎖*/

        for(i=0;i<N;i++){
            state[i]=0;
        }

        for(i=0; i<N; i++){
            pthread_cond_init(&S[i],NULL); /*初始化條件變量*/
        }
};

void take_fork(int phil_num);
void put_fork(int phil_num);
void test(int phil_num);
};

class moniter phil;

int main(){

    int i;
    pthread_t thread_id[N]; /*資料型態,用來宣告pthread變數*/

    for(i=0;i<N;i++){
        pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]); /*創建thread*/
        printf("Philosopher %d -> Think\n",i+1);
    }

    for(i=0;i<N;i++){
        pthread_join(thread_id[i],NULL); /*要把創建好的好的thread join進去*/
    }

return 0;
}

void *philospher(void *num){
    while(TRUE){
        int *i = (int*)num;
        sleep(1);
        phil.take_fork(*i);
        sleep(0);
        phil.put_fork(*i);
    }
}

void moniter::take_fork(int phil_num){
    pthread_mutex_lock(&mutex); /*鎖定互斥鎖,直到直到unlock以前同一時間只能被一個thread使用*/ /*當一個thread執行到pthread_mlock處時，如果該鎖此時被另一個線程使用，那此線程被阻塞，即程序將等待到另一個線程釋放此互斥鎖*/
    state[phil_num] = HUNGRY;
    printf("Philosopher %d -> Wait\n",phil_num+1);
    test(phil_num);
    while (state[phil_num] == HUNGRY){
        pthread_cond_wait (&S[phil_num],&mutex); /*使thread阻塞在一個條件變量上,thread解開mutex指向的鎖並被條件變量cond阻塞*/
    }
    pthread_mutex_unlock(&mutex); /*打開互斥鎖*/
    sleep(1);
};

void moniter::put_fork(int phil_num){
    pthread_mutex_lock(&mutex);
    state[phil_num] = THINKING;
    printf("Philosopher %d -> Think\n",phil_num+1);
    test(LEFT);
    test(RIGHT);
    pthread_mutex_unlock(&mutex);
};

void moniter::test(int phil_num){
    if (state[phil_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
        state[phil_num] = EATING;
        sleep(2);
        printf("Philosopher %d -> Eat\n",phil_num+1);
        pthread_cond_signal(&S[phil_num]); /*用來釋放被阻塞在條件變量cond上的一個thread*/
    }
};
