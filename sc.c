#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node {
	int value;
	int referenceBit;
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
		printf("%d:%d, ", cur->value, cur->referenceBit);
		cur=cur->next;
	}
	printf("\n");
}

/*
This is a linear search to see if the page is already in the list
*/
NODE* secondChanceSearch(int number){
	NODE *current = head;
	while(current!=NULL){
		if(current->value==number){
			current->referenceBit=1;
			break;
		}
		current=current->next;
	}
	return current;
}

/*
Adds a new page to the end of the linked list
*/
void insertRear(int number, int bit){
	NODE* newNode = (NODE *)malloc(sizeof(NODE));
	listSize+=1;
	newNode->value=number;
	newNode->next=NULL;
	newNode->prev=tail;
	newNode->referenceBit=bit;

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
	if(current==head){
		head=head->next;
	}
	else{
		current->next->prev=current->prev;
		current->prev->next=current->next;
	}
	free(current);
}

/*
Inserts a node at the end and removes a node
*/
void secondChanceSwap(NODE* current, int numberRead){
	insertRear(numberRead, 1);
	removeNode(current);
	return;
}

/*
Moves nodes to end of the list using a function defined above or deletes a node and inserts the new page requested 
*/
void updateAndInsert(numberRead){
	NODE *current = head;
	while(current!=NULL){
		if(current->referenceBit==0){
			secondChanceSwap(current,numberRead);
			return;
		}
		else{
			insertRear(current->value,0);
		}
		current=current->next;
		removeNode(current->prev);
	}
	return;
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
	
	int numberOfFaults=0;
	int numberOfRequests=0;

	while(fgets(buffer,10,stdin)!=NULL){
		if(!isdigit(buffer[0])){
			fprintf(stderr, "Number read was not a number!\n");
			continue;		
		}
		sscanf(buffer, "%d", &numberRead);		
		numberOfRequests+=1;
		if((tracker=secondChanceSearch(numberRead))==NULL){
			numberOfFaults+=1;	
			if(listSize<maxListSize){
				insertRear(numberRead,1);
				continue;
			}
			updateAndInsert(numberRead);
		}
	}
	destroyList();
	printf("# of page requests: %d\n", numberOfRequests);
	printf("# of page faults: %d\n", numberOfFaults);
	return 0;
}