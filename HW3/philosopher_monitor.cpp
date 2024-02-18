#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<pthread.h>	//pthread series
#include<unistd.h>	//sleep()

#define TRUE 1
#define FALSE 0
#define N 5  // how many philosophers
#define LEFT (i+N-1)%N  // who seats the left side
#define RIGHT (i+1)%N  // who seats the right side
#define THINKING 0
#define HUNGRY 1
#define EATING 2

using namespace std;

typedef pthread_mutex_t semaphore;
void* philosopher(void* i);
pthread_t p_thread[N];
                        /*monitor*/
class monitor
{
	private:

		int state[N];
		pthread_mutex_t mutex;  // mutual exclusion for critical regions
		pthread_cond_t s[N];  // condition variable

	public:

		void ini();					/*initialize*/ 
		void take_forks(int i);
		void put_forks(int i);
		void test(int i);
		void output();
		void think();
		void eat();

};
						/*declare a monitor*/ 
monitor m;

int main()
{
	int* philosopher_s[N];
	printf("%d philosophers are sitting around a round table.\n\n", N);
	printf("<< (T)hinking / (H)ungry / (E)ating >>\n\n");
	for (int i = 0; i < N; i++){
		philosopher_s[i]= (int *)malloc(sizeof(int));
		*philosopher_s[i]=i;
		p_thread[i]=i;
		pthread_create(&p_thread[i], NULL, philosopher, (void *)philosopher_s[i]);
	}

	m.ini();

	while (TRUE){
		m.output();
		sleep(1);
	}

	for (int i = 0; i < N; i++){
		free(philosopher_s[i]);
		pthread_join(p_thread[i], NULL);
	}

	return 0;
}

void* philosopher(void* i)
{
	int num = *((int*)i);
	while (TRUE){
		m.think();
		m.take_forks(num);
		m.eat();
		m.put_forks(num);
	}
}

void monitor::ini()  // initialize the private variables
{
	pthread_mutex_init(&mutex, NULL);		
	for (int i = 0; i < N; i++)
		pthread_cond_init(&s[i], NULL);
}

void monitor::take_forks(int i)
{
	pthread_mutex_lock(&mutex);
	state[i] = HUNGRY;
	test(i);
	pthread_mutex_unlock(&mutex);
    sleep(1);  // !!!!!!!!!!!!!!!!!!!!!!!
}

void monitor::put_forks(int i)
{
	pthread_mutex_lock(&mutex);
	state[i] = THINKING;
	pthread_cond_signal(&s[LEFT]);
	pthread_cond_signal(&s[RIGHT]);
	pthread_mutex_unlock(&mutex);
    sleep(1);  // !!!!!!!!!!!!!!!!!!!!!!!
}

void monitor::test(int i)
{
	while ( ! (state[i] == HUNGRY  &&  state[LEFT] != EATING  &&  state[RIGHT] != EATING)){
        pthread_cond_wait(&s[i],&mutex);
    }
	state[i] = EATING;
		
}

void monitor::output()
{
	for (int i = 0; i < 5; i++){
		if(state[i] == THINKING)
			cout << "T ";
		else if(state[i] == HUNGRY)
			cout << "H ";
		else if(state[i] == EATING)
			cout << "E ";
	}
    printf("\n\n");
}

void monitor::think()
{
	sleep(1);
}

void monitor::eat()
{
	sleep(1);
}


