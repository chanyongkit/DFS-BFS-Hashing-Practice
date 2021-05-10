#include <stdio.h>
#include <stdlib.h>

typedef struct _listnode
{
    int vertex;
    int T;
	struct _listnode *next;
} ListNode;
typedef ListNode StackNode;

typedef struct _graph{
    int V;
    int E;
    char T;
    ListNode **list;
}Graph;

typedef ListNode QueueNode;

typedef struct _queue{
   int size;
   QueueNode *head;
   QueueNode *tail;
} Queue;

typedef struct _stack
{
	int size;
	StackNode *head;
} Stack;

void insertAdjVertex(ListNode** AdjList,int vertex);
void removeAdjVertex(ListNode** AdjList,int vertex);
int matching(Graph g);

void enqueue(Queue *qPtr, int item);
int dequeue(Queue *qPtr);
int getFront(Queue q);
int isEmptyQueue(Queue q);
void removeAllItemsFromQueue(Queue *qPtr);
void printQ(QueueNode *cur);
//////STACK///////////////////////////////////////////
void push(Stack *sPtr, int vertex);
int pop(Stack *sPtr);
int peek(Stack s);
int isEmptyStack(Stack s);
void removeAllItemsFromStack(Stack *sPtr);
Stack S_Path(int vertex, int total, int prj, int std, int mtr ,int aMatrix[][total], int fMatrix[total]);
int does_path_exists(int start, int end,int total, int aMatrix[][total],int fMatrix[total]);
//////////////////////////////////

int main()
{
    //Let index of project = project
    //Index of student = total_proj_count + student
    //Index of mentor = total_proj_count + total_student_count + proj

    int Prj, Std, Mtr, i, x,y; //Project, Student and Mentor;
    int maxMatch;
    scanf("%d %d %d", &Std, &Prj, &Mtr);
    int total = Std + Prj + Mtr + 2;
    int aMatrix[total][total];
    int rMatrix[total][total];
    int fMatrix[total];

    int t_hist_Matrix[total];
    int np,nm; //number of projects and number of mentors

    //build graph
	Graph g;
    g.V = total;

	g.list = (ListNode **) malloc(g.V*sizeof(ListNode *));

    //Write your code
    //Assign Null Values
    for(i = 0 ; i < total; i++){
        for(x = 0; x<total;x++)
        {
            aMatrix[i][x] = 0;
        }
        fMatrix[i] = 0;
        t_hist_Matrix[i] = 0;
    }

    //Connect Nodes
    int sProj, sMent;
    ListNode *temp;
    for(i=0;i<Std;i++)
    {
        scanf("%d", &np);
        scanf("%d", &nm);

        //Link Project
        for(x=0;x<np;x++)
        {
            scanf("%d",&sProj);
            aMatrix[1+sProj-1][1+i+Prj]= 1;
        }

        //Link Mentor
        for(y=0;y<nm;y++)
        {
            scanf("%d",&sMent);
            aMatrix[1+Prj+i][1+Std+Prj+sMent-1]= 1;
        }
    }
    for(i = 0 ; i < total; i++){
        for(x = 0; x<total;x++)
        {
            rMatrix[i][x] = aMatrix[i][x];
        }
    }

    //Connect sink and source
    for(i = 1; i < Prj+1; i++)
    {
        rMatrix[0][i] = 1;
        aMatrix[0][i] = 1;
    }
    for(i=Std+Prj+1;i<total-1;i++)
    {
        rMatrix[i][total-1] = 1;
        aMatrix[i][total-1] = 1;
    }


/*
    for(i = 0 ; i < total; i++){
        for(x = 0; x<total;x++)
        {
            printf("%d",rMatrix[i][x]);
        }
        printf("\n");
    }*/

    //apply Ford Fulkerson algorithm
    int path_cost = 0;
    Queue queue;
    queue.head=NULL;
    queue.size=0;
    queue.tail=NULL;

    Stack stack;

    int index_1 = 0,index_2 = 0;

    maxMatch = 0;
    while(does_path_exists(0, total-1, total, rMatrix,fMatrix)!=0)
    {

        stack = S_Path(0,total,Prj,Std,Mtr,rMatrix, fMatrix);


        //Edit rMatrix
        if(isEmptyStack(stack) == 1);
        else
        {
            ListNode * temp = stack.head;
            while(1)
            {
                if(temp == NULL)
                    break;

                printf("%i ",temp->vertex);
                temp=temp->next;
            }
            printf("\n");

            index_1 = peek(stack);
            pop(&stack);
            fMatrix[index_1] = 1;
            index_2 = peek(stack);
            pop(&stack);
            fMatrix[index_2] = 1;

            rMatrix[index_1][index_2] = 1;
            if(isEmptyStack(stack)!= 1)
            {
                while(isEmptyStack(stack)!=1)
                {
                    rMatrix[index_2][index_1] = 0;
                    index_1 = index_2;
                    index_2 = peek(stack);
                    pop(&stack);
                    rMatrix[index_1][index_2] = 1;

                    fMatrix[index_1] = 1;
                    fMatrix[index_2] = 1;
                }
            }
            else
            {
                rMatrix[index_2][index_1] = 0;
                rMatrix[index_2][i] = 1;
                index_1 = index_2;
            }
            rMatrix[i][index_1] = 0;
            maxMatch++;
        }
    }
/*
    for(int k = 0 ; k < total; k++){
        for(x = 0; x<total;x++)
        {
            printf("%d",rMatrix[k][x]);
        }
        printf("\n");
    }
*/
   /* maxMatch = 0;

    for(i=0;i<total;i++)
    {
        fMatrix[i]=0;
    }

    for(i = 0; i < Mtr; i++)
    {
        if(does_path_exists(i+Std+Prj+1, 0, total, rMatrix,fMatrix)!=0)
        {
            fMatrix[does_path_exists(i+Std+Prj+1, 0, total, rMatrix,fMatrix)]=1;

            maxMatch++;
        }
    }*/
    printf("%d\n", maxMatch);
    return 0;
}

int does_path_exists(int start, int end,int total, int aMatrix[][total],int fMatrix[total])
{
    Stack d_stack;
    d_stack.head = NULL;
    d_stack.size = 0;
    int i,x,devisit;
    int t_hist_Matrix[total];

    for(x=0;x<total;x++)
    {
        t_hist_Matrix[x] = 0;
    }

    enqueue(&d_stack, start);
    while(isEmptyStack(d_stack)==0)
    {
        i = peek(d_stack);

        devisit = 0;
        for(x=0;x<total;x++)
        {
            //Not seen
            if(t_hist_Matrix[x]==0 && aMatrix[i][x]==1)
            {
                t_hist_Matrix[x] = 1;
                devisit = 1;

                //Reached goal?
                if(x == end)
                {
                    if(fMatrix[peek(d_stack)]==0)
                        return peek(d_stack);
                }

                push(&d_stack,x);
                break;
            }
        }
        if(devisit == 0)
        {
            pop(&d_stack);
        }
    }
    return 0;
}

Stack S_Path(int vertex, int total, int prj, int std, int mtr ,int aMatrix[][total], int fMatrix[total])
{
    int i =0, x=0;

    Stack stack;
    stack.head = NULL;
    stack.size = 0;

    Stack d_stack;
    d_stack.head = NULL;
    d_stack.size = 0;

    int t_hist_Matrix[total];
    for(i=0;i<total;i++)
    {
        t_hist_Matrix[i] = 0;
    }

    push(&stack,vertex);
    push(&d_stack,vertex);

    int std_count,student_taken =0;

    ListNode * cNode;
    int devisit = 0;
    int count = 0;
    while(isEmptyStack(d_stack)==0)
    {
        i = peek(d_stack);

        devisit = 0;
        for(x=0;x<total;x++)
        {
            //Not seen
            if(t_hist_Matrix[x]==0 && aMatrix[i][x]==1)
            {
                t_hist_Matrix[x] = 1;
                devisit = 1;

                //Reached goal?
                if(x == total-1)
                {
                    if(fMatrix[peek(d_stack)]==0){
                        push(&stack,x);
                        return stack;
                    }
                }

                push(&d_stack,x);
                push(&stack,x);
                break;
            }
        }
        if(devisit == 0)
        {
            pop(&stack);
            pop(&d_stack);
        }

    }
    removeAllItemsFromStack(&stack);
    return stack;
}
/*
Stack S_Path(int vertex, int total, int prj, int std, int mtr ,int aMatrix[][total], int fMatrix[total])
{
    int i =0, x=0;

    Stack stack;
    stack.head = NULL;
    stack.size = 0;

    Stack d_stack;
    d_stack.head = NULL;
    d_stack.size = 0;

    int t_cost = 0;
    Queue queue;
    queue.head=NULL;
    queue.size=0;
    queue.tail=NULL;

    int t_hist_Matrix[total];

    for(i=0;i<total;i++)
    {
        t_hist_Matrix[i] = 0;
    }

    push(&stack,vertex);
    push(&d_stack,vertex);
    int std_count ,counter, check_1, check_2,student_taken =0;
    ListNode * cNode;
    int devisit = 0;
    int count = 0;
    while(isEmptyStack(d_stack)==0)
    {
        i = peek(d_stack);

        devisit = 0;
        for(x=0;x<total;x++)
        {
            //Not seen
            if(t_hist_Matrix[x]==0 && aMatrix[i][x]==1)
            {
                t_hist_Matrix[x] = 1;
                devisit = 1;

                //Reached goal?
                if(x == total-1)
                {
                    if(fMatrix[peek(stack)]==0)
                    {
                        cNode = stack.head;
                        count = 0;
                        while(1)
                        {
                            if(cNode == NULL)
                                break;

                            if(fMatrix[cNode->vertex]==0)
                                count++;

                            cNode = cNode->next;
                        }
                        if(count >= 3)
                        {
                            push(&stack,x);
                            return stack;
                        }
                    }

                    devisit = 0;
                    break;
                }

                push(&d_stack,x);
                push(&stack,x);
                break;
            }
        }
        if(devisit == 0)
        {
            pop(&stack);
            pop(&d_stack);
        }

    }
    removeAllItemsFromStack(&stack);
    return stack;
}*/





int matching(Graph g)
{

    //Write your code
    return 0;

}

void removeAdjVertex(ListNode** AdjList,int vertex)
{
    ListNode *temp, *preTemp;
    if(*AdjList != NULL)
    {
        if((*AdjList)->vertex ==vertex){//first node
            temp = *AdjList;
            *AdjList = (*AdjList)->next;
            free(temp);
            return;
        }
        preTemp = *AdjList;
        temp = (*AdjList)->next;
        while(temp!=NULL && temp->vertex != vertex)
        {
            preTemp= temp;
            temp = temp->next;
        }
        preTemp->next = temp->next;
        free(temp);
    }

}

int does_itExist_inStack(Stack s, int vertex)
{
    ListNode * cNode = s.head;
    while(1)
    {
        if(cNode == NULL)
            break;
        if(cNode->vertex == vertex)
            return 1;

        cNode = cNode->next;
    }
    return 0;
}

void insertAdjVertex(ListNode** AdjList,int vertex)
{
    ListNode *temp;
    if(*AdjList==NULL)
    {
        *AdjList = (ListNode *)malloc(sizeof(ListNode));
        (*AdjList)->vertex = vertex;
        (*AdjList)->next = NULL;
    }
    else{
        temp = (ListNode *)malloc(sizeof(ListNode));
        temp->vertex = vertex;
        temp->next = *AdjList;
        *AdjList = temp;
    }
}
void enqueue(Queue *qPtr, int vertex) {
    QueueNode *newNode;
    newNode = malloc(sizeof(QueueNode));
    if(newNode==NULL) exit(0);

    newNode->vertex = vertex;
    newNode->next = NULL;

    if(isEmptyQueue(*qPtr))
        qPtr->head=newNode;
    else
        qPtr->tail->next = newNode;

    qPtr->tail = newNode;
    qPtr->size++;
}

int dequeue(Queue *qPtr) {
    if(qPtr==NULL || qPtr->head==NULL){ //Queue is empty or NULL pointer
        return 0;
    }
    else{
       QueueNode *temp = qPtr->head;
       qPtr->head = qPtr->head->next;
       if(qPtr->head == NULL) //Queue is emptied
           qPtr->tail = NULL;

       free(temp);
       qPtr->size--;
       return 1;
    }
}

int getFront(Queue q){
    return q.head->vertex;
}

int isEmptyQueue(Queue q) {
    if(q.size==0) return 1;
    else return 0;
}

void removeAllItemsFromQueue(Queue *qPtr)
{
	while(dequeue(qPtr));
}

void printQ(QueueNode *cur){
    if(cur==NULL) printf("Empty");

    while (cur != NULL){
        printf("%d ", cur->vertex);
        cur = cur->next;
    }
    printf("\n");
}

void push(Stack *sPtr, int vertex)
{
	StackNode *newNode;
    newNode= malloc(sizeof(StackNode));
    newNode->vertex = vertex;
    newNode->next = sPtr->head;
    sPtr->head = newNode;
    sPtr->size++;
}

int pop(Stack *sPtr)
{
    if(sPtr==NULL || sPtr->head==NULL){
        return 0;
    }
    else{
       StackNode *temp = sPtr->head;
       sPtr->head = sPtr->head->next;
       free(temp);
       sPtr->size--;
       return 1;
    }
}

int isEmptyStack(Stack s)
{
     if(s.size==0) return 1;
     else return 0;
}

int peek(Stack s){
    return s.head->vertex;
}

void removeAllItemsFromStack(Stack *sPtr)
{
	while(pop(sPtr));
}

