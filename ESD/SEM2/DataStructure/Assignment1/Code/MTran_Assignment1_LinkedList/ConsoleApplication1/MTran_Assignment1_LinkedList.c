/*******************************************************************************************
* File Name          : PROG8130Assign1SkeletonV2.c
* Description        : Prog8130 - Assignment 1: Linked Lists (revised June 9, 2022)
*					   This program uses linked lists to store and print a list of words that are
*					   organized by an "index number" assosciated with each word. Each word can
*					   consist of maximum MAX_STRING_LEN-1 characters.
*
* Author:              Minh Quan Tran
* Date:                September 24th, 2024
********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_DATA_ITEMS   10						// the number of data items to be used in list
#define MAX_STRING_LEN		   30	

// structure/typedef used for holding data
typedef struct {
	char name[MAX_STRING_LEN];							// "string" data to be stored in the linked list.
	int index;											// position in linked list to put the data into
	
														// the "index" is used to determine where to put the data.
														// For example an indes of 0 or 1 would always put the data
														// at the start of the current linked list at that moment of time.
														// An "index" number larger than the number of linked list nodes
														// would have the effect of putting the new node on the end of the list.
													    // Any other "index" value would place it at the location
} inputData;

// definition of linked list data nodes
typedef struct LinkedListNode {
	struct LinkedListNode*  ptrNextNode;				// pointer to next node in list (NULL if the end)
	char					NameData[MAX_STRING_LEN];	// the name to be stored 
} LinkedListNodeDef;									// DO NOT STORE THE "index" from inputData because it
														// has no meaning after the insertion into linked list


// function prototypes
LinkedListNode*    CreateNode(char* NameData);
LinkedListNodeDef* AddToLinkedList(LinkedListNodeDef* ptrHead, char* NameData, int DataIndex, int currentLength);
LinkedListNodeDef* DeleteFromList(LinkedListNodeDef* ptrHead, int DataIndex);
void			   PrintLinkedList(LinkedListNodeDef* ptrHead);
void			   DeleteNode(LinkedListNodeDef* ptrHead, int nodeToBeDelete);

// make the data positions the last 4 digits of student number
// for example if my student number ended in 94330 the data will be as supplied

// Used the following data in your assignment #1
inputData iData[10] = {
		{ "milk", 3 },
		{ "kiwi", 4 },
		{ "butter", 1 },
		{ "fish", 1 },
		{ "car", 7 },
		{ "water", 1 },
		{ "orange", 7 },
		{ "airplane", 7 },
		{ "allan", 4 },
		{ "chocolate", 2 }
};
//end of data for assignment

// data used for test
// paste the data here that was created by Prog8130Assign1DataCreator.exe


int main(void) {
	int i;
	int listLength = 0;

	// define linked list start/head
	struct LinkedListNode* ptrHead = NULL;

	// Print out the input data
	printf("Data before inserting into the Linked List is:\n");
	for (i = 0; i < NUMBER_OF_DATA_ITEMS; i++)
	{
		printf("%s, %d\n", iData[i].name, iData[i].index);
	}
		

	// insert the data into Linked List
	for (i = 0; i < NUMBER_OF_DATA_ITEMS; i++) 
	{
		ptrHead = AddToLinkedList(ptrHead, iData[i].name, iData[i].index, listLength);
		listLength++; // keep track of how many node have been inserted to the list
	}

	// Print out the list in order it is stored starting at head
	PrintLinkedList(ptrHead);

	
	// Deleting a node at specific indices for testing (node from 1 -> 10)
	DeleteNode(ptrHead, 10); // Delete end of list
	DeleteNode(ptrHead, 5);  // Delete node 5
	DeleteNode(ptrHead, 1);  // Delete head

	return 0;
}

// FUNCTION      : CreateNode()
// 
// DESCRIPTION   : This function is used to create a new node and insert the data
//				   to that node
// 
// PARAMETERS    : char* NameData - data to insert to the node
// RETURNS       : ptrNode		  - the node created
LinkedListNode* CreateNode(char* NameData)
{
	LinkedListNodeDef* ptrNode = (LinkedListNodeDef*) malloc(sizeof(LinkedListNodeDef));

	// Check for Error
	if (ptrNode == NULL)
	{
		printf("Malloc failed! \n");
		exit(-1);
	}

	// Insert data to the node
	strncpy_s(ptrNode->NameData, MAX_STRING_LEN - 1, NameData, MAX_STRING_LEN - 1);
	ptrNode->ptrNextNode = NULL;

	return ptrNode;

}

// FUNCTION      : AddToLinkedList()
// 
// DESCRIPTION   : This function takes an input of Data of 30 Char word and an index and 
//				   places them in a linked list organized by priority of index number. 
// 
// PARAMETERS    : LinkedListNodeDef *ptrHead	- current head of the linked list
//				   char *NameData				- Array of Characters with data (a "string")
//		           int DataIndex				- Index of data (postion to put the data in the list)
// RETURNS       : possibly updated pointer to the head of the list 
//				   (updated ptrHead if insertion was to start of list)


LinkedListNodeDef* AddToLinkedList(LinkedListNodeDef* ptrHead, char* NameData, int DataIndex, int currentLength) 
{
	LinkedListNode* ptrNewNode = CreateNode(NameData);

	// If the index is 0, insert at the head
	if (DataIndex == 0 || DataIndex == 1 || ptrHead == NULL) 
	{
		ptrNewNode->ptrNextNode = ptrHead;
		ptrHead = ptrNewNode;
		return ptrHead;
	}

	LinkedListNode* currentNode = ptrHead;
	int count = 1;

	// If the input index is greater than the current list length then insert node at the end
	if (DataIndex > currentLength)
	{
		// Traverse the list until reach the last node of current list
		while (currentNode->ptrNextNode != NULL)
		{
			currentNode = currentNode->ptrNextNode;
		}

		currentNode->ptrNextNode = ptrNewNode;
		return ptrHead;
	}

	// Traverse the list to find the position to insert
	while (currentNode != NULL && count < DataIndex - 1)
	{
		currentNode = currentNode->ptrNextNode;
		count++;
	}

	// If the index is out of bounds
	if (currentNode == NULL) {
		printf("Index %s out of bounds.\n", NameData);
		free(ptrNewNode);
		return ptrHead;
	}

	// Insert the node at the index position
	ptrNewNode->ptrNextNode = currentNode->ptrNextNode;
	currentNode->ptrNextNode = ptrNewNode;

	return ptrHead;
}

// FUNCTION      : DeleteFromList()
// 
// DESCRIPTION   : This function takes an input of an index and delete that node
//				   in a linked list and linked the previous node to the next of a
//				   node to be deleted
// 
// PARAMETERS    : LinkedListNodeDef *ptrHead	- current head of the linked list
//		           int DataIndex				- Index of data (postion to delete the data in the list)
// RETURNS       : possibly updated pointer to the head of the list 
//				   (updated ptrHead if insertion was to start of list)
LinkedListNodeDef* DeleteFromList(LinkedListNodeDef* ptrHead, int DataIndex)
{
	if (ptrHead == NULL)
	{
		return NULL;  // List is empty, nothing to delete
	}

	LinkedListNodeDef* temp = ptrHead;

	// If the node to be deleted is the head node
	if (DataIndex == 0) {
		ptrHead = temp->ptrNextNode;  // Change head
		free(temp);					  // Free old head
		return ptrHead;
	}

	// Find the previous node of the node to be deleted
	for (int i = 0; i < DataIndex - 1; i++)
	{
		temp = temp->ptrNextNode;
		if (temp == NULL)
		{
			printf("Error no previous node from list and is not head\n");
			printf("Returning current List\n");
			return ptrHead;
		}
	}

	// Get the next node of the node to be deleted
	LinkedListNodeDef* nodeToBeDeleted = temp->ptrNextNode;  // Store pointer to the next of the node to be deleted
	temp->ptrNextNode = nodeToBeDeleted->ptrNextNode;        // Unlink the node to be deleted
	free(nodeToBeDeleted);									 // Free memory of the node to be deleted

	return ptrHead;  // Return the updated head
}

// FUNCTION      : DeleteNode()
// 
// DESCRIPTION   : This function takes an input of int nodeToBeDeleted and use the DeleteFromList()
//				   to delete that node and print out the new list
// 
// PARAMETERS    : LinkedListNodeDef *ptrHead	- current head of the linked list
//				   int nodeToBeDelete           - position node want to be deleted
// RETURNS       : VOID
void DeleteNode(LinkedListNodeDef* ptrHead, int nodeToBeDelete)
{
	// Deleting a node at specific indices for testing
	ptrHead = DeleteFromList(ptrHead, nodeToBeDelete - 1);
	printf("\nAfter deleting index %d:\n", nodeToBeDelete);
	PrintLinkedList(ptrHead);
}

// FUNCTION      : PrintLinkedList()
// 
// DESCRIPTION   : This function takes an input of the head of the linked list, then loops through
//				   and prints each word in the linked list until the ptrNextNode is NULL (end of list).
// 
// PARAMETERS    : 
//				   LinkedListNodeDef *ptrHead - A pointer to the start of the linked list
// RETURNS       : void (nothing)
void PrintLinkedList(LinkedListNodeDef* ptrHead)
{
	printf("\nLinked List is:\n");

	// put code in here to print the linked list out

	LinkedListNodeDef* currentNode;
	int index = 1;
	// go through the linked list from the start (ptrHead) and use ptrNextNode to find the next node to be
	// printed until you reach a ptrNextNode == NULL which signifies the end of the list
	
	currentNode = ptrHead;

	while (currentNode != NULL)
	{
		printf("%d. %s\n", index, currentNode->NameData);
		index++;
		currentNode = currentNode->ptrNextNode;
	}

	return;
}
