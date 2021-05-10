#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct _arraynode
{
    int *itemArray;
    int sizeArray;
    struct _arraynode *next;
}ArrayNode;

typedef struct _arraylist{
   int size; //the size of a possible sequence
   ArrayNode *head;
} ArrayList;

typedef struct _listnode
{
    int item;
	struct _listnode *next;
} ListNode;

typedef struct _linkedlist{
   int sum; //the number of possible sequences
   int size;
   ListNode *head;
} LinkedList;

// You should not change the prototypes of these functions
//////LinkedList///////////////////////////////////////////
int insertNode(LinkedList *ll, int index, int coin);
int removeNode(LinkedList *ll, int index);
ListNode *findNode(LinkedList ll, int index);
void removeAllItems(LinkedList *ll);
int calculate_sum(int n, int arr[n]);
void dfs_sum(LinkedList ll, int C, int cursor, int mainGoal, ArrayList* al);
///////////////////////////////////////////////////

void sumToC(LinkedList* ll, int C, ArrayList* al);
int main()
{
    int C;
    printf("Enter a number:\n");
    scanf("%d",&C);

    LinkedList ll;
    ll.head = NULL;
    ll.sum = 0;
    ll.size = 0;
    ArrayList al;
    al.head = NULL;
    al.size = 0;

    sumToC(&ll,C,&al);
    ArrayNode* temp;
    int i,j;
    temp = al.head;
    for(i=0;i<al.size;i++){
        for(j=0;j<temp->sizeArray;j++)
            printf(" %d ",temp->itemArray[j]);
        printf("\n");
        temp = temp->next;
    }

    return 0;
}

void sumToC(LinkedList* ll, int C, ArrayList* al)
{
   // Write your code here
   dfs_sum(*ll,C,0,C,al);
}

void dfs_sum(LinkedList ll, int C, int cursor, int mainGoal, ArrayList* al)
{
    if(C == 0)
    {
        ListNode *checkll = ll.head;
        ArrayNode *checkal = al->head;
        int similar = 0;
        while(1)
        {
            if(checkal == NULL)
                break;

            checkll = ll.head;
            similar = 0;
            while(1)
            {
                if(checkll == NULL)
                    break;

                for(int i = 0; i<checkal->sizeArray;i++)
                {
                    if(checkal->itemArray[i] == checkll->item)
                        similar++;
                }
                checkll = checkll->next;
                if(similar == checkal->sizeArray)
                    return;
            }


            checkal = checkal->next;
        }



        int * arr = (int*)malloc(ll.size * sizeof(int));
        ListNode *ln = ll.head;

        for(int x =0;x<ll.size;x++)
        {
            arr[x] = ln->item;
            ln = ln->next;
        }

        if(al->head==NULL){
            ArrayNode *temp = malloc(sizeof(ArrayNode));
            temp->itemArray = arr;
            temp->sizeArray = ll.size;
            temp->next = al->head;
            al->head = temp;
            al->size++;
        }
        else
        {
            ArrayNode *temp = malloc(sizeof(ArrayNode));
            temp->itemArray = arr;
            temp->sizeArray = ll.size;
            temp->next = NULL;

            ArrayNode * aNode = al->head;
            while(1)
            {
                if(aNode->next == NULL)
                {
                    aNode->next = temp;
                    break;
                }
                aNode=aNode->next;
            }
            al->size++;

        }
        return;
    }

    if(C < 0)
        return;

    ListNode *ln = ll.head;
    for(int i=cursor ; i< mainGoal+1 ;i++)
    {

        ln = ll.head;
        while(1)
        {
            //check if it's in node
            if(ln == NULL)
            {
                //Link Node
                if(ll.size==0)
                    insertNode(&ll,0,(i+1));
                else
                    insertNode(&ll,ll.size,(i+1));

                //DFS
                dfs_sum(ll, C - (i+1),cursor+1,mainGoal,al);
                removeNode(&ll,ll.size-1);
                break;
            }

            if(ln->item == (i+1))
                break;

            ln = ln->next;
        }

    }

}



///////////////////////////////////////////////////////
int insertNode(LinkedList *ll, int index, int value){

	ListNode *pre, *cur;

	if (ll == NULL || index < 0 || index > ll->size)
		return 0;

        if (index == 0){
		cur = ll->head;
		ll->head = (ListNode*) malloc(sizeof(ListNode));
		ll->head->item = value;
		ll->head->next = cur;
		ll->sum += value;
		ll->size++;
		return 1;
	}


	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(*ll, index - 1)) != NULL){
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		pre->next->item = value;
		pre->next->next = cur;
		ll->sum += value;
		ll->size++;
		return 1;
	}

	return 0;
}


int removeNode(LinkedList *ll, int index){

	ListNode *pre, *cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index > ll->size)
		return 0;

	// If removing first node, need to update head pointer
	if (index == 0){
		cur = ll->head->next;
		ll->sum -= ll->head->item;
		free(ll->head);
		ll->head = cur;
		ll->size--;
		return 1;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(*ll, index - 1)) != NULL){

		if (pre->next == NULL)
			return 0;

		cur = pre->next;
		ll->sum -= cur->item;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 1;
	}

	return 0;
}

ListNode *findNode(LinkedList ll, int index){

	ListNode *temp;

	if (index < 0 || index >= ll.size)
		return NULL;

	temp = ll.head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0){
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

void removeAllItems(LinkedList *ll)
{
	ListNode *cur = ll->head;
	ListNode *tmp;

	while (cur != NULL){
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	ll->head = NULL;
	ll->size = 0;
	ll->sum =0;
}
