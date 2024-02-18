#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>  // pthread series
#include<unistd.h>  // sleep()

#define TRUE 1
#define FALSE 0
#define N 5  // how many philosophers
#define LEFT (i+N-1)%N  // who seats the left side
#define RIGHT (i+1)%N  // who seats the right side
#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef pthread_mutex_t semaphore;
int state[N];
semaphore mutex;  // critical regions
semaphore s[N];  // one semaphore per philosopher. if can eat singal
pthread_t p_thread[N];  // control pthread

void* philosopher(void* i); 
void take_forks(int i);
void put_forks(int i);
void test(int i);
void output();
void think();
void eat();

int main(){

	int* philosopher_s[N];
	pthread_mutex_init(&mutex, 0);
	printf("%d philosophers sitting around a round table.\n\n", N);
	printf("<< THINKING: 0 HUNGRY: 1 EATING: 2 >>\n\n", N);
	for (int i = 0; i < N; i++){
		philosopher_s[i] = malloc(sizeof(int));
		*philosopher_s[i] = i;
		pthread_mutex_init(&s[i], NULL);
		p_thread[i] = i;
		pthread_create(&p_thread[i], NULL, philosopher, (void *)philosopher_s[i]);  // create a new pthread
	}
	while (TRUE){
		pthread_mutex_lock(&mutex);  // enter critical region
		output();
		pthread_mutex_unlock(&mutex);		 
		sleep(1);
	}
	for (int i = 0; i < N; i++){
		free(philosopher_s[i]);
		pthread_join(p_thread[i], NULL);  // wait for pthread's return
		pthread_mutex_destroy(&s[i]);  // destroy pthread
	}

	return 0;
}

void* philosopher(void* i){
	int num = *((int*)i);
	// (int*) is a type cast so that compiler reinterprets the bits as pointer-to-int instead of pointer-to-void
	// *( ) use to get the number it points to
	while (TRUE){
		think();
		take_forks(num);
		eat();
		put_forks(num);
	}
}
void take_forks(int i){
	
	pthread_mutex_lock(&mutex);  // down(&mutex);
	state[i] = HUNGRY;
	test(i);  // if can take the forks
	pthread_mutex_unlock(&mutex);  // up(&mutex);
	pthread_mutex_lock(&s[i]);  // down(&s[i]);
}
void put_forks(int i){
	
	pthread_mutex_lock(&mutex);  // down(&mutex);
	state[i]=THINKING;
	test(LEFT);
	test(RIGHT);
	pthread_mutex_unlock(&mutex);  // up(&mutex);
}
void test(int i){
	if (state[i] == HUNGRY  &&  state[LEFT] != EATING  &&  state[RIGHT] != EATING){
		state[i] = EATING;
		pthread_mutex_unlock(&s[i]);  // up(&s[i]);
	}
}
void output(){
    for (int i = 0;  i < N;  i++){
        printf("%d ", state[i]);
    }
    printf("\n");
}
void think(){
	sleep(1);
}
void eat(){
	sleep(1);
}