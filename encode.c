//
// Created by arctic on 9/22/21.
//

#include <stdio.h>
#include <stdlib.h>

const int charSetSize = 256;
const int bufferSize = 3;

struct ScoredNode {
    char letter;
    int timesUsed;
    struct ScoredNode * left;
    struct ScoredNode * right;
};

struct ScoredNode *  copyAndFree(struct ScoredNode toCopy){
    struct ScoredNode * newNode = malloc(sizeof (struct ScoredNode));
    newNode->timesUsed = toCopy.timesUsed;
    newNode->letter = toCopy.letter;
    newNode->left = toCopy.left;
    newNode->right = toCopy.right;
    free(&toCopy);
    return newNode;
}

void initializeArrayWithEmptyNodes(struct ScoredNode nodes[charSetSize]){
    for (int i = 0;i<charSetSize;i++){
        struct ScoredNode temp;
        temp.timesUsed = 0;
        temp.letter = (char) i;
        nodes[i] = temp;
    }
}

int getUniqueLetterCount(struct ScoredNode nodes[charSetSize]){
    int count = 0;
    for (int i = charSetSize - 1;i>=0;i--){
        if (nodes[i].timesUsed > 0){
            count++;
        }else{
            break;
        }
    }
    return count;
}

//doesn't wipe old nodes memory, if it does, take care to deep copy nodes
void feedUniqueLetters(struct ScoredNode nodes[charSetSize],struct ScoredNode * toFeed,int feedSize){
    for (int i = 0;i<feedSize;i++){
        toFeed[feedSize - i - 1] = nodes[charSetSize - i - 1];
    }
}

void printScoresNodesArray(struct ScoredNode nodes[charSetSize]){
    for (int i = 0;i<charSetSize;i++){
        printf("character: %c, occurrences: %d\n",nodes[i].letter,nodes[i].timesUsed);
    }
}

void printScoresNodesArrayVar(struct ScoredNode * nodes, int size){
    for (int i = 0;i<size;i++){
        printf("character: %c, occurrences: %d\n",nodes[i].letter,nodes[i].timesUsed);
    }
}

// I stole the following quicksort implementation from people on the internet
void swap(struct ScoredNode * a,struct ScoredNode * b){
    struct ScoredNode t = *a;
    *a = *b;
    *b = t;
}

int partition(struct ScoredNode nodes[charSetSize],int low, int high){
    int pivot = nodes[high].timesUsed;
    int i = (low - 1);

    for (int j = low; j < high ; j++) {
        if (nodes[j].timesUsed <= pivot){
            i++;
            swap(&nodes[i],&nodes[j]);
        }
    }
    swap(&nodes[i+1],&nodes[high]);
    return (i + 1);
}

//only sort after empties have been eliminated
void quicksort(struct ScoredNode nodes[charSetSize],int low, int high){
    if (low < high) {
        int pi = partition(nodes,low,high);
        quicksort(nodes,low,pi - 1);
        quicksort(nodes,pi +1,high);
    }
}

void readFileToArray(char * filename,struct ScoredNode toFeed[charSetSize]){
    FILE *fp;
    fp = fopen(filename,"r");
    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }
    char buffer[bufferSize];
    size_t bytesRead = bufferSize;
    while (bytesRead == bufferSize){
        bytesRead = fread(buffer,1,bufferSize,fp);
        for (int i = 0;i<bytesRead;i++){
            toFeed[buffer[i]].timesUsed++;
        }
    }
}


void insertAtIndex(struct ScoredNode * nodes,struct ScoredNode * toInsert,int idx,int nodesSize,struct ScoredNode * newLst){
    int foundIt = 0;
    for (int i = 0;i<nodesSize;i++){
        newLst[i + foundIt] = nodes[i + 2];
        if (i == idx){
            newLst[i + 1] = *toInsert;
            foundIt ++;
        }
    }
}

int highestIndexLessThan(struct ScoredNode * nodes,int lower,int upper,int currentGreatest,int greatestIdx, int nodeTimesUsed){
    if ((upper - lower <= 1) || nodes[lower].timesUsed > nodeTimesUsed){
        return greatestIdx + 1;
    }
    int mid = (upper - lower)/2 + lower;
    int val = nodes[mid].timesUsed;
    if (val <= nodeTimesUsed){
        if (val > currentGreatest){
            currentGreatest = val;
            greatestIdx = mid;
        }
        lower = mid;
    }else{
        upper = mid;
    }
    return highestIndexLessThan(nodes,lower,upper,currentGreatest,greatestIdx,nodeTimesUsed);
}

struct ScoredNode *  insertLastTwoNodes(struct ScoredNode * nodes, int size){
    struct ScoredNode * new = malloc(sizeof (struct ScoredNode));
    new->timesUsed = nodes[0].timesUsed + nodes[1].timesUsed;
    new->left = &nodes[0];
    new->right = &nodes[1];
    struct ScoredNode * newNodes = malloc(size * sizeof(struct ScoredNode) - 1);
    int insertIdx = highestIndexLessThan(nodes,0,size,0,0,new->timesUsed);
    insertAtIndex(nodes,new,insertIdx,size,newNodes);
    return newNodes;
}

struct ScoredNode * buildTree(struct ScoredNode * nodes,int size){
    if (size == 0){
        struct ScoredNode * empty = malloc(sizeof (struct ScoredNode));
        return empty;
    }
    if (size == 1){
        return &nodes[0];
    }
    size --;
    return buildTree(insertLastTwoNodes(nodes,size),size);


}


int main(){
    struct ScoredNode lst [charSetSize];
    initializeArrayWithEmptyNodes(lst);
    readFileToArray("sample.txt",lst);
    quicksort(lst,0,charSetSize-1);
    int uniqueLetterCount = getUniqueLetterCount(lst);
    struct ScoredNode uniqueLst [uniqueLetterCount];
    feedUniqueLetters(lst,uniqueLst,uniqueLetterCount);
    printScoresNodesArrayVar(uniqueLst,uniqueLetterCount);
    buildTree(uniqueLst,uniqueLetterCount);

    return 1;
}

