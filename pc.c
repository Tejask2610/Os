#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define BUFFER_SIZE 5
#define ITEMS 10

int in=0;
int out=0;
int buffer[BUFFER_SIZE];

sem_t empty;
sem_t full;

pthread_mutex_t mutex;

void*producer(void*arg)
{
	int item;
	for(int i=1;i<=ITEMS;i++)
	{
		item=i;
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		buffer[in]=item;
		printf("Producer produced:%d\n",item);
		in=(in+1)%BUFFER_SIZE;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		sleep(1);
	}
	pthread_exit(NULL);
}
void*consumer(void*arg)
{
	int item;
	for(int i=1;i<=ITEMS;i++)
	{
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		item=buffer[out];
		printf("Consumer consumed:%d\n",item);
		out=(out+1)%BUFFER_SIZE;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		sleep(2);
	}
	pthread_exit(NULL);
}
int main()
{
	pthread_t producer_thread;
	pthread_t consumer_thread;
	sem_init(&empty,0,BUFFER_SIZE);
	sem_init(&full,0,0);
	pthread_mutex_init(&mutex,NULL);
	
	pthread_create(&producer_thread,NULL,producer,NULL);
	pthread_create(&consumer_thread,NULL,consumer,NULL);
	pthread_join(producer_thread,NULL);
	pthread_join(consumer_thread,NULL);
	
	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_mutex_destroy(&mutex);
	
	printf("\nProducer-Consumer execution completed.\n");
	
	return 0;
}


~~~~~~~~~~~~~~~~~OUTPUT~~~~~~~~~~~~~~`
Producer produced:1
Consumer consumed:1
Producer produced:2
Consumer consumed:2
Producer produced:3
Producer produced:4
Consumer consumed:3
Producer produced:5
Producer produced:6
Consumer consumed:4
Producer produced:7
Producer produced:8
Consumer consumed:5
Producer produced:9
Producer produced:10
Consumer consumed:6
Consumer consumed:7
Consumer consumed:8
Consumer consumed:9
Consumer consumed:10

Producer-Consumer execution completed.

