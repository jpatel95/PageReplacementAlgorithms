#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node {
	int value;
	int frequency;
	struct node *next;
	struct node *prev;
}NODE;


NODE *head = NULL;
NODE *tail = NULL;
int listSize = 0;
int maxListSize;

/*
This is an extra function that just prints out the LinkedList. It was used for testing.
*/
void printList(){
	NODE *cur = head;	
	while(cur!=NULL){
		printf("%d, %d \n", cur->value, cur->frequency);
		cur=cur->next;
	}
	printf("\n");
}

/*
This is a linear search to see if the page is already in the list
*/
NODE* lfuSearch(int number){
	NODE *current = head;
	while(current!=NULL){
		if(current->value == number){
			return current;
		}
		current=current->next;
	}
	return current;
}

/*
Finds the page that was requested the least frequently
*/
NODE * findLeastFrequent(){
	NODE *current = head;
	NODE *min = head;
	if(min==NULL){
		return NULL;
	}
	
	while(current != NULL){
		if(current->frequency < min->frequency){
			min=current;
		}
		current=current->next;
	}
	
	return min;
}

/*
Adds a new page to the end of the linked list
*/
void insertRear(int number, int freq){
	NODE* newNode = (NODE *)malloc(sizeof(NODE));
	listSize+=1;
	newNode->value=number;
	newNode->next=NULL;
	newNode->prev=tail;
	newNode->frequency=freq;

	if(head==NULL && tail==NULL){
		head=newNode;
		tail=newNode;
		return;
	}

	tail->next=newNode;
	tail=newNode;
	return;
}

/*
Removes a node from the LinkedList
*/
void removeNode(NODE *current){
	listSize-=1;
	if(current==head){
		head=head->next;
		head->prev=NULL;
	}
	else if(current==tail){
		tail=tail->prev;
		tail->next=NULL;
	}
	else{
		current->next->prev=current->prev;
		current->prev->next=current->next;
	}
	free(current);
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
	if(argc<2){
		fprintf(stderr, "Not enough arguments");
		return 0;
	}
	maxListSize = atoi(argv[1]);
	char buffer[10];
	int numberRead;
	NODE *tracker;
	
	int numberOfFaults=0;
	int numberOfRequests=0;

	while(fgets(buffer,10,stdin)!=NULL){
		if(!isdigit(buffer[0])){
			fprintf(stderr, "Number read was not a number!\n");
			continue;		
		}
		sscanf(buffer, "%d", &numberRead);		
		numberOfRequests+=1;
		if((tracker=lfuSearch(numberRead))==NULL){
			numberOfFaults+=1;	
			if(listSize<maxListSize){
				insertRear(numberRead,1);
				continue;
			}
			NODE * lfuNode = findLeastFrequent();
			insertRear(numberRead, 1);
			removeNode(lfuNode);
		}
		else{
			insertRear(tracker->value,tracker->frequency+1);
			removeNode(tracker);
		}
	}
	destroyList();
	printf("# of page requests: %d\n", numberOfRequests);
	printf("# of page faults: %d\n", numberOfFaults);
	return 0;
}