#include <stdio.h>
#include <string.h>

struct ParaQueue{
	int start;
	int end;
	int size;
	int num;
  char * queue[10];
};

void enqueue(struct ParaQueue paraQueue,char * para){
  paraQueue.queue[paraQueue.end] = para;
	paraQueue.end = (paraQueue.end + 1)%paraQueue.size;
	paraQueue.num++;
}

char * dequeue(struct ParaQueue paraQueue){
	char * tmp = paraQueue.queue[paraQueue.start];
	paraQueue.start = (paraQueue.start + 1)%paraQueue.size;
	paraQueue.num--;
	return tmp;
}
