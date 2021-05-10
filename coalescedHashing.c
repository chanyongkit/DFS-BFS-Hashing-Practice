#include <stdio.h>
#include <stdlib.h>

#define TABLESIZE 37
#define PRIME     13

enum Marker {EMPTY,USED};

typedef struct _slot{
    int key;
    enum Marker indicator;
    int next;
} HashSlot;

int HashInsert(int key, HashSlot hashTable[]);
int HashFind(int key, HashSlot hashTable[]);

int hash(int key)
{
    return (key % TABLESIZE);
}

int main()
{
    int opt;
    int i;
    int key;
    int index;
    HashSlot hashTable[TABLESIZE];

    for(i=0;i<TABLESIZE;i++){
        hashTable[i].next = -1;
        hashTable[i].key = 0;
        hashTable[i].indicator = EMPTY;
    }

    printf("============= Hash Table ============\n");
    printf("|1. Insert a key to the hash table  |\n");
    printf("|2. Search a key in the hash table  |\n");
    printf("|3. Print the hash table            |\n");
    printf("|4. Quit                            |\n");
    printf("=====================================\n");

    printf("Enter selection: ");
    scanf("%d",&opt);
    while(opt>=1 && opt <=3){
        switch(opt){
        case 1:
            printf("Enter a key to be inserted:\n");
            scanf("%d",&key);
            index = HashInsert(key,hashTable);
            if(index <0)
                printf("Duplicate key\n");
            else if(index < TABLESIZE)
                printf("Insert %d at index %d\n",key, index);
            else
                printf("Table is full.\n");
            break;
        case 2:
            printf("Enter a key for searching in the HashTable:\n");
            scanf("%d",&key);
            index = HashFind(key, hashTable);

            if(index!=-1)
                printf("%d is found at index %d.\n",key,index);
            else
                printf("%d is not found.\n",key);
            break;

        case 3:
            printf("index:\t key \t next\n");
            for(i=0;i<TABLESIZE;i++) printf("%d\t%d\t%d\n",i, hashTable[i].key,hashTable[i].next);
            break;
        }
        printf("Enter selection: ");
        scanf("%d",&opt);
    }
    return 0;
}

H(int key, int i)
{
    return hash(key+i);
}

int HashInsert(int key, HashSlot hashTable[])
{
 // Write your code here
    int index,i=0,prevIndex=-1,first_search=-1;

    index = H(key,i);
    while(1){

        //Not used
        if(hashTable[index].indicator == EMPTY)
        {
            //If linked then connect
            if(prevIndex != -1)
            {
                hashTable[prevIndex].next = index;
            }
            //Store new key
            hashTable[index].key = key;
            hashTable[index].indicator = USED;
            hashTable[index].next = -1;
            break;
        }
        //If used
        else if (hashTable[index].indicator == USED)
        {
            //if duplicate exist
            if(hashTable[index].key == key)
                return -1;
            if(first_search == -1)
            {
                //keep a copy
                prevIndex = index;
                //Set to 0 to check if linked
                first_search = 0;
            }
        }
        i++;
        //Increment index
        if(hashTable[index].indicator == USED && hashTable[index].next != -1)
        {
            index = hashTable[index].next;

            //If linked
            if(first_search == 0)
            {
                //Set to -1 to check again if linked
                first_search = -1;
            }
        }
        else
        {
            index = H(key,i);
            first_search = 1;
        }
        //Every slot has been searched
        if(i == TABLESIZE)
            return i;
    }
    return index;

}

int HashFind(int key, HashSlot hashTable[])
{
 // Write your code here
    int index, i=0, counter;
    index = H(key,i);

    while(1)
    {
        //When occupied compare keys
        if(hashTable[index].indicator == USED && hashTable[index].key == key)
            return index;

        //Increment index
        if(hashTable[index].indicator == USED && hashTable[index].next != -1)
        {
            i++;
            index = hashTable[index].next;
        }
        else
        {
            i++;
            index = H(key,i);
        }

        //Every slot has been searched
        if(i == TABLESIZE)
            return -1;
    }

}
