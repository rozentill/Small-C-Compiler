/*
this file used to check reserved words.
*/
#include <stdio.h>
#include <string.h>

void checkWord(char * identifier){
	if (!strcmp(identifier,"if"))
	{
		fprintf(stderr, "Error:reserved word %s can not be declared\n",identifier);
		exit(-1);
	}
	else if (!strcmp(identifier,"for"))
	{
		fprintf(stderr, "Error:reserved word %s can not be declared\n",identifier);
		exit(-1);
	}
	else if (!strcmp(identifier,"break"))
	{
		fprintf(stderr, "Error:reserved word %s can not be declared\n",identifier);
		exit(-1);
	}
	else if (!strcmp(identifier,"continue"))
	{
		fprintf(stderr, "Error:reserved word %s can not be declared\n",identifier);
		exit(-1);
	}
	else if (!strcmp(identifier,"return"))
	{
		fprintf(stderr, "Error:reserved word %s can not be declared\n",identifier);
		exit(-1);
	}
	else if (!strcmp(identifier,"void"))
	{
		fprintf(stderr, "Error:reserved word %s can not be declared\n",identifier);
		exit(-1);
	}
	else if (!strcmp(identifier,"read"))
	{
		fprintf(stderr, "Error:reserved word %s can not be declared\n",identifier);
		exit(-1);
	}
	else if (!strcmp(identifier,"write"))
	{
		fprintf(stderr, "Error:reserved word %s can not be declared\n",identifier);
		exit(-1);
	}

}




