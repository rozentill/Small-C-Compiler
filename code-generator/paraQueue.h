#include <stdio.h>
#include <string.h>

typedef struct Queue{
	int start;
	int end;
	int size;
	int num;
  char ** queue;
} ParaQueue;

void enqueue(ParaQueue * paraQueue,char * para){
  	paraQueue->queue[paraQueue->end] = para;
	paraQueue->end = (paraQueue->end + 1)%paraQueue->size;
	paraQueue->num++;
}

char * dequeue(ParaQueue * paraQueue){
	char * tmp = paraQueue->queue[paraQueue->start];
	paraQueue->start = (paraQueue->start + 1)%paraQueue->size;
	paraQueue->num--;
	return tmp;
}
