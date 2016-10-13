#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node {
	int value;
	struct node *next;
	struct node *prev;
}NODE;

NODE *head = NULL;
int listSize = 0;
int maxListSize;


/*
This is a linear search to see if the page is already in the list
*/
NODE* search(int number, int *count){
	NODE *current = head;
	while(current!=NULL){
		if(current->value==number){
			break;
		}
		current=current->next;
		*count+=1;
	}
	//returns null if not found, otherwise returns the node
	return current;
}

/*
Adds a new page to the head of the linked list
*/
void insert(int number){
	NODE* newNode = (NODE *)malloc(sizeof(NODE));
	++listSize;
	newNode->value=number;
	newNode->next=head;
	newNode->prev=NULL;
	if(head!=NULL)
		head->prev=newNode;
	head=newNode;
	return;
}

void lruSwap(NODE *current){
	if(current==head){
		return;
	}
	current->prev->next=current->next;
	current->next->prev=current->prev;
	current->prev=head->prev;
	head->prev=current;
	current->next=head;
	head=current;
	return;
}

/*
This is an extra function that just prints out the LinkedList. It was used for testing.
*/
void printList(){
	NODE *cur = head;	
	while(cur!=NULL){
		printf("%d, ", cur->value);
		cur=cur->next;
	}
	printf("\n");
}

/*
Frees up all the memory
*/
void destroyList(){
	NODE *cur = head;
	while(cur!=NULL){
		cur=cur->next;
		free(head);
		head=cur;
	}
}

/*
This is the main function, it takes in an argument for cache size and it reads a file from stdin and parses
thorough it looking for only numbers and does the page replacement.
*/
int main(int argc, char **argv){
	if(argc<2)
		fprintf(stderr, "Not enough arguments");

	maxListSize = atoi(argv[1]);
	
	char buffer[10];
	int numberRead;
	NODE *tracker;
	int count;
	
	int numberOfFaults=0;
	int numberOfRequests=0;

	while(fgets(buffer,10,stdin)!=NULL){
		if(!isdigit(buffer[0])){
			fprintf(stderr, "Number read was not a number!\n");
			continue;		
		}

		sscanf(buffer, "%d", &numberRead);		
		numberOfRequests+=1;		
		count = 0;
		if((tracker=search(numberRead, &count))==NULL){
			insert(numberRead);
			//PAGE FAULT HERE B/C it was not FOUND
			numberOfFaults+=1;	
		}
		else{
			if(count>=maxListSize){
				//PAGEFAULT HERE
				numberOfFaults+=1;
			}
			lruSwap(tracker);
		}
	}
	
	destroyList();
	printf("# of page requests: %d\n", numberOfRequests);
	printf("# of page faults: %d\n", numberOfFaults);
	return 0;
}