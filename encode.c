//
// Created by arctic on 9/22/21.
//

#include <stdio.h>
#include <stdlib.h>

const int charSetSize = 256;

struct ScoredNode {
    char letter;
    int timesUsed;
};

void initializeArrayWithEmptyNodes(struct ScoredNode nodes[charSetSize]){
    for (int i = 0;i<charSetSize;i++){
        struct ScoredNode temp;
        temp.timesUsed = 0;
        temp.letter = (char) i;
        nodes[i] = temp;
    }
}

void printScoresNodesArray(struct ScoredNode nodes[charSetSize]){
    for (int i = 0;i<charSetSize;i++){
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

void quicksort(struct ScoredNode nodes[charSetSize],int low, int high){
    if (low < high) {
        int pi = partition(nodes,low,high);
        quicksort(nodes,low,pi - 1);
        quicksort(nodes,pi +1,high);
    }
}

void readFromFile(char * filename){

}


int main(){
    struct ScoredNode lst [charSetSize];
    initializeArrayWithEmptyNodes(lst);
    struct ScoredNode a;
    a.timesUsed = 13;
    a.letter = 'a';
    lst[0] = a;
    struct ScoredNode b;
    b.timesUsed = 4;
    b.letter = 'b';
    lst[1] = b;
    struct ScoredNode c;
    c.timesUsed = 6;
    c.letter = 'c';
    lst[2] = c;
    struct ScoredNode d;
    d.timesUsed = 9;
    d.letter = 'd';
    lst[3] = d;
    struct ScoredNode e;
    e.timesUsed = 21;
    e.letter = 'e';
    lst[4] = e;
    quicksort(lst,0,charSetSize - 1);
    printScoresNodesArray(lst);
    return 1;
}

