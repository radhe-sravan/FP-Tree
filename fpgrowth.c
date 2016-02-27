#include "header.h"


char transactionsFile[50] = "transactions.txt"; 					// transaction file
char inputFile[50] = "input.txt";							// custom input file 
char fpTreeOutputFile[50] = "fptree.txt";						// output file for the tree

int numItems; 										// number of items
int numTransactions; 									// number of transactions

float minSup = 10; 									// minimum support for a frequent itemset
float minConf = 50;									// minimum confidence
		
char *seperator = ",";									// item seperator in transactions file

char **items;										// variable to hold all the items

int *frequencyCount;									// counter variable to hold the frequencies of items

int *frequent1ItemSets;
int frequent1ItemsCount;

struct fptree										// structure for a node of FP tree
{
	int data;									// data 
	int count;									// support count
	int numChild;									// no. of children for the node
	struct fptree **children;							// array of children of the node
};
struct fptree *root;

FILE *fpTreeFile, *frequentItemsFile, *rulesFile;

int main()
{
	int i,j;

	root = malloc(sizeof(struct fptree));						// initializing the root of tree
	root -> data = -1;								// data is -1
	root -> numChild = 0;								// initial number of children
	root -> children = malloc(sizeof(struct fptree**)*10000);			// initializing the children array of root

	fpgrowth();	

	// writing the fptree output

	if(fopen(fpTreeOutputFile,"r") != NULL)
	{
		remove(fpTreeOutputFile);
	}

	fpTreeFile = fopen(fpTreeOutputFile,"a");
	fprintf(fpTreeFile,"\n Pre-order traversal of the FP Tree generated is as shown below.\n\n Numbers in () indicate the support count of the node.\n\n");
	printTree(root);
	fclose(fpTreeFile);
								
	return 0;
}

// method to initiate the fp growth process

void fpgrowth()										
{	
	frequencyCount = malloc(sizeof(int)*100000);					// instantiating frequency counter
	frequent1ItemSets = malloc(sizeof(int)*100000);					// instantiating the frequent1Itemsets array

	readTransactions();								// reading the transactions file 

	readInput();									// reading custom input file and generating tree     							
}

// method to read the transactions file provided

void readTransactions()									
{
	FILE *file1;									
	file1 = fopen(transactionsFile, "r");

	FILE *file2;
	file2 = fopen(inputFile,"w");

	char *string;									// variable to hold each line of the transaction file
	string = malloc(sizeof(char)*1000000);

	items = malloc(sizeof(char*)*100000);						// instantiating items array

	char *tokens;									// variable to hold the tokens

	while((fscanf(file1, "%[^\n]\n", string)) != EOF)
	{
		numTransactions++;							// increasing numTransactions for every line

  		tokens = strtok (string,seperator);					// tokenizing the string
 			
		while (tokens != NULL)
  		{
    			if(getStringIndex(tokens) == -1)				
			{								// if the token does not exist in the items array
				items[numItems] = malloc(strlen(tokens));		// adding it to the array
				strcpy(items[numItems],tokens);
				frequencyCount[numItems] = 1;				// setting the frequency count of the item to 1
				numItems++;						// increasing the items count
			}
			else
				frequencyCount[getStringIndex(tokens)] += 1;		// increasing the frequency count

			fprintf(file2,"%d ",getStringIndex(tokens));			// writing the index of token to custom input file

    			tokens = strtok (NULL,seperator);
  		}
		fprintf(file2,"\n");
	}
	
	free(string);									// freeing the allocated memory and closing file
	free(tokens);									// pointers
	fclose(file1);
	fclose(file2);

	printf("\n  Number of transactions : %d\n",numTransactions);
	printItems();
	
}

// method to return the index of an item in the items array

int getStringIndex(char *string)	
{						
	int i,j=-1;
	
	if(numItems != 0)
	{	
		for(i=0;i<numItems;i++)
		{
			if(strcmp(items[i],string)==0)					// returning the index incase the string matches
			{								// else returning -1
				j = i;			
				break;
			}
		}
	}
	return j;
}

// method to read the input file and generate frequencies file

void readInput()
{
	FILE *file1;									
	file1 = fopen(inputFile, "r");

	char *string;									
	string = malloc(sizeof(char)*10000);

	char *tokens;	

	int *tokensArray;								// variable to hold the indices of each transaction
	tokensArray = malloc(sizeof(int)*1000);

	int i,j;

	while((fscanf(file1, "%[^\n]\n", string)) != EOF)
	{
		i = 0;
		tokens = strtok (string," ");	
							
		while (tokens != NULL)							
  		{									
				
			int token = atoi(tokens);					// tokenizing each string from the custom input file
			tokensArray[i] = token;						// and passing the tokens to the tokensarray
						
			i++;
			tokens = strtok (NULL," ");
				  		
		}
		sortTransactionsByFrequency(tokensArray,i-1);				// sorting each transaction by the frequency of items
		generateFPTree(tokensArray,i);						// generating the tree by passing the array and length
	}

	free(string);
	free(tokensArray);
	fclose(file1);
	remove(inputFile);
} 

// method to sort the transactions

void sortTransactionsByFrequency(int *transaction,int length)
{
	partition(transaction,frequencyCount,0,length);
}

// method to generate FP tree

void generateFPTree(int *array, int length)
{
	int i,index;

	struct fptree *parent,*node;
	parent = malloc(sizeof(struct fptree));
	node = malloc(sizeof(struct fptree));

	parent = root;									// making the root as the parnt for the first element

	for(i=0;i<length;i++)
	{
		if(frequencyCount[array[i]] >= minSup)					// inserting nodes with frequency greater than minSup
		{
			if(searchFrequent1ItemSets(array[i]) == -1)			// searching the frequent1Items and inserting if req
			{
				frequent1ItemSets[frequent1ItemsCount] == array[i];
				frequent1ItemsCount++;
			}

			index = isNodeCreated(parent,array[i]);				// finding if a node has been created

			if(index==-1)
			{
				node = createNode(parent,array[i]);			// if the node does not exist creating a new node
			}
			else
			{
				node = getNode(parent,index);				// else making the existing node as node and 
				node->count = node->count + 1;				// increasing the count
			}	

			parent = node;							// making the node as parent
		}
	}
}

// method to check if a node exists for a given parent

int isNodeCreated(struct fptree *parent,int index)
{
	int i;

	if(parent->numChild == 0)							// if the parent's child count is 0 returning -1
		return -1;	
	for(i=0;i<parent->numChild;i++)
	{
		if(parent->children[i]->data == index)					// if the node is already created for the parent 
			return i;							// returning it's  index
	}

	return -1;
}

// method to create a child node for a parent

struct fptree* createNode(struct fptree *parent, int index)
{
	struct fptree *child;
	child = malloc(sizeof(struct fptree));
	child -> data = index;								// creating a new child for the index
	child -> count = 1;								// setting the count to 1
	child -> numChild = 0;								// number of childs for newly created child
	child -> children = malloc(sizeof(struct fptree*)*100000);

	parent-> children[parent->numChild] = malloc(sizeof(struct fptree));		// making the newly created node as a child of 
	parent-> children[parent->numChild] = child;					// parent node
	parent->numChild++;								// increasing the child count of parent

	return child;									// returning the newly created child
}

// method to return a node of the parent at specified index

struct fptree* getNode(struct fptree *parent,int i)
{
	if(parent == NULL || i < 0)
		return NULL;

	return parent->children[i];	
}


// method to print all the items along with corrseponding frequencies

void printItems()
{									
	int i;
	
	printf("\n  Items and their frequencies\n\n");
	for(i=0;i<numItems;i++)
	{
		printf("  %d\t%s\t\t%d\n",i,items[i],frequencyCount[i]);
	}
	printf("\n");
}

// method to print the tree 

void printTree(struct fptree *parent)
{
	int i,j;

	if(parent == NULL)
		return;

	if(parent->numChild == 0)
	{
		if(parent->data == -1)
			fprintf(fpTreeFile,"%s"," null");
		else
			fprintf(fpTreeFile," %s (%d)",items[parent->data],parent->count);
	}
	for(i=0;i<parent->numChild;i++)
	{
		if(parent->data == -1)
			fprintf(fpTreeFile,"%s"," null ->");
		else
			fprintf(fpTreeFile," %s (%d) ->",items[parent->data],parent->count);
		printTree(parent->children[i]);
	}
	fprintf(fpTreeFile,"\n");
}

int searchFrequent1ItemSets(int index)
{
	int i;
	
	for(i=0;i<frequent1ItemsCount;i++)
	{
		if(frequent1ItemSets[i] == index)
			return i;
	}

	return -1;

}
