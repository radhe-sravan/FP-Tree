#ifndef _header_h
#define _header_h

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


// prototypes for the methods in fpgrowth.c

void fpgrowth();
void readTransactions();
int getStringIndex(char *);
void readInput();
void sortTransactionsByFrequency(int *,int);
void generateFPTree(int *, int);
int isNodeCreated();
struct fptree* createNode();
struct fptree* getNode();
int searchFrequent1ItemSets(int);
void generateFrequentItemSets();
void generateRules();


void printItems();
void printTree(struct fptree *);




// prototypes for the methods in utils.c

void mergeSort(int *array,int *frequency,int low,int mid,int high);
void partition(int *array,int *frequency,int low,int high);


#endif
