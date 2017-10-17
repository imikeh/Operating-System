#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define N 5
#define LEFT (ph_num+4)%N
#define RIGHT (ph_num+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define TRUE 1

sem_t mutex; /*資料型態，用來宣告semaphore變數*/
sem_t S[N];

void *philospher(void *num);
void take_forks(int);
void put_forks(int);
void test(int);

int state[N];
int phil_num[N]={0,1,2,3,4};

int main(){

    int i;
    pthread_t thread_id[N]; /*資料型態,用來宣告pthread變數*/
    sem_init(&mutex,0,1); /*使用sem_t變數前，需要先初始化*/

    for(i=0;i<N;i++){
        sem_init(&S[i],0,0);
    }

    for(i=0;i<N;i++){
        pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]); /*創建thread*/
        printf("Philosopher %d -> Think\n",i+1);
    }

    for(i=0;i<N;i++){
        pthread_join(thread_id[i],NULL); /*要把創建好的好的thread join進去*/
    }

}

void *philospher(void *num){

    while(TRUE){
        int *i = num;
        sleep(1);
        take_forks(*i);
        sleep(0);
        put_forks(*i);
    }

}

void take_forks(int ph_num){

    sem_wait(&mutex); /*進入臨界區down*/
    state[ph_num] = HUNGRY;
    printf("Philosopher %d -> Wait\n",ph_num+1);
    test(ph_num); /*嘗試拿兩雙fork*/
    sem_post(&mutex); /*離開臨界區up*/
    sem_wait(&S[ph_num]); /*若拿不到fork則阻塞*/
    sleep(1);

}

void put_forks(int ph_num){

    sem_wait(&mutex); /*進入臨界區down*/
    state[ph_num] = THINKING; /*吃飽了休息*/
    printf("Philosopher %d -> Think\n",ph_num+1);
    test(LEFT); /*測試左手邊人可不可以吃*/
    test(RIGHT); /*測試右手邊人可不可以吃*/
    sem_post(&mutex); /*離開臨界區up*/

}

void test(int ph_num){

    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){ /*若現在表示飢餓且左右邊人都沒在吃*/
        state[ph_num] = EATING;
        sleep(2);
        printf("Philosopher %d -> Eat\n",ph_num+1);
        sem_post(&S[ph_num]);
    }

}
