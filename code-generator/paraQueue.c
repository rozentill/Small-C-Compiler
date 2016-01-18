#include <stdio.h>
#include <string.h>

struct ParaQueue{
	int start=0;
	int end=0;
	int size = 10;
	int num=0;
  char * queue[10];
  void enqueue(char * para);
  char * dequeue();
};

void paraQueue::enqueue(char * para){
  queue[end] = para;
	end = (end + 1)%size;
	num++;
}

char * paraQueue::dequeue(){
	char * tmp = queue[start];
	start = (start + 1)%size;
	num--;
	return tmp;
}
