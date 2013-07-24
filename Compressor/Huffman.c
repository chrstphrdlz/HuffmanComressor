#include <stdio.h>
#include <string.h>
#include <stdlib.h>





typedef struct huffmanTree
{
	struct huffmanTree* left;

	struct huffmanTree* right;

	char character;

	unsigned int count;

}  huffmanTree;

typedef struct characterTranslation
{
	char data[4];

	int bitLength;

} characterTranslation;
unsigned int numCharacters(unsigned int* chracters);
unsigned int* count_Characters(char* buf, unsigned int* characters, int numChars);
size_t sizeOfFile(FILE* fp);
int isLeaf(huffmanTree* node);
huffmanTree* combineTree(huffmanTree* one, huffmanTree* two);
huffmanTree* makeTree(char character, unsigned int count);
unsigned int* count_From_File(FILE* fp);
void printNode(huffmanTree* leaf);


int main(int argc, char** argv)
{
	FILE* fp = fopen("MOBY DICK OR THE WHALE.txt","r");

	if(fp == NULL)
	{
		printf("The file is not found\n");
		return 0;
	}

	unsigned int* counted_Chars = count_From_File(fp);
	int charactersUsed = 0;
	int i;
	for(i=0;i<256;i++)
	{
		if(counted_Chars[i]>0)
		{
			//printf("%u %c\n", counted_Chars[i],i);

			charactersUsed++;
		}
		
	}

	huffmanTree** allTheNodes = malloc(sizeof(huffmanTree*)* charactersUsed);
	int nodeIndex = 0,j;
	for(i=0;i<256;i++)
	{
		if(counted_Chars[i]>0)
		{
			allTheNodes[nodeIndex] = makeTree(i, counted_Chars[i]);

			nodeIndex++;
		}		
	}

	intiialize();

	huffmanTree* temp;

	for(i=0;i<charactersUsed-1;i++)
	{
		temp = combineMins(allTheNodes, charactersUsed-i);

		insert(allTheNodes, temp, charactersUsed-i);
	}

	characterTranslation* translated;

	characterTranslation = translate();

}

void sort(huffmanTree** nodes, int size)
{

}

void  shiftLeft(huffmanTree** nodes, int length)
{
	int i;

	for(i=length;i>0;i--)
	{
		nodes[i-1] = nodes[i];
	}

	return;
}

huffmanTree* combineMins(huffmanTree** nodes, int length)
{
	huffmanTree* new = malloc(sizeof(huffmanTree));

	new->left = nodes[0];

	new->right = nodes[1];

	nodes[1] = new;

	shiftLeft(nodes, length);

	return new;
}



void insert(huffmanTree** nodes, huffmanTree* inserter, int length)
{

}

void swapTree(huffmanTree** nodes, int i, int j)
{
	huffmanTree* swapper = nodes[i];

	nodes[i] = nodes[j];

	nodes[j] = swapper;

	return;
}

//For debugging
void printNode(huffmanTree* leaf)
{
	printf("The char is %c, with %u occurences\n", leaf->character, leaf->count);
}
//One is the lesser, two is the greater
huffmanTree* combineTree(huffmanTree* one, huffmanTree* two)
{
	huffmanTree* returner = malloc(sizeof(huffmanTree));

	returner->left = one;

	returner->right = two;
 	
 	returner->count = returner->left->count + returner->right->count;

 	return returner;
}

huffmanTree* makeTree(char character, unsigned int count)
{
	huffmanTree* returningNode = malloc(sizeof(huffmanTree));

	returningNode->count = count;

	returningNode->character = character;

	returningNode->left=NULL;

	returningNode->right=NULL;

	return returningNode;
}
int isLeaf(huffmanTree* node)
{
	return node->left == NULL && node->right == NULL;
}

unsigned int numCharacters(unsigned int* characters)
{
	int i;
	unsigned int tot =0;
	for(i=0;i<256;i++)
	{
		tot+=characters[i];
	}
}

unsigned int* count_From_File(FILE* fp)
{
	
	char buf[1000];

	//Make an array to hold all the charcters in the string
	unsigned int i;
	unsigned int* characters = malloc(sizeof(unsigned int)*256);
	//Initialize the array and count the number of characters
	memset(characters,0,256);

	int j;

	//Get the file size in bytes
	int fileSizeLeft = sizeOfFile(fp);

	int numReads = 0;

	if(fileSizeLeft%1000 != 0)
	{
		numReads = fileSizeLeft/1000 + 1;
	}
	else
	{
		numReads = fileSizeLeft/1000;
	}

	for(j=0;j<numReads;j++)
	{
		if(fileSizeLeft < 1000)
		{
			fread(buf, sizeof(char),fileSizeLeft, fp);
			count_Characters(buf, characters, fileSizeLeft);
		}

		else
		{
			fread(buf, sizeof(char),1000, fp);
			count_Characters(buf, characters, 1000);
		}

		fileSizeLeft -= 1000;
	}


	return characters;
}

unsigned int* count_Characters(char* buf, unsigned int* characters, int numChars)
{
	int i;

	for(i=0;i<numChars;i++)
	{
		characters[buf[i]]++;
	}

	return characters;
}

size_t sizeOfFile(FILE* fp)
{
	fseek(fp, 0, SEEK_END);

	size_t size = ftell(fp); 

	fseek(fp, 0, SEEK_SET); 	

	printf("size is %ld\n", size);

	return size;
}