#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm> 
#include <iostream>


class  huffmanTree
{
	public : huffmanTree* left;

	public : huffmanTree* right;

	public : char character;

	public : unsigned int count;

	public : int depth;

	public : huffmanTree(char charToBeCompressed, unsigned int numTimesRepeated)
	{
		left = NULL;
		right = NULL;

		count  = numTimesRepeated;
		character = charToBeCompressed;
	}

	public : huffmanTree(huffmanTree* one, huffmanTree* two)
	{
		count = one->count + two->count;
		character = NULL;
		left = one;
		right = two;
	}
	

};
bool compareHuffman(huffmanTree* one, huffmanTree* two)
{
	return one->count > two->count ?true:false;
};

typedef struct charTranslation
{
	char letter;
	std::vector<bool>* v;
} charTranslation;

void printTranslation(charTranslation translated);
int maxDepth(huffmanTree* complete);
unsigned int numCharacters(unsigned int* chracters);
unsigned int* count_Characters(char* buf, unsigned int* characters, int numChars);
size_t sizeOfFile(FILE* fp);
int isLeaf(huffmanTree* node);
huffmanTree* combineTree(huffmanTree* one, huffmanTree* two);
huffmanTree* makeTree(char character, unsigned int count);
unsigned int* count_From_File(FILE* fp);
void printNode(huffmanTree* leaf);
void putDepth(huffmanTree* completed, int depth);
void assignDepth(huffmanTree* completed);
void printdepthOfNode(huffmanTree* leaf);
void printTree(huffmanTree* completed);
void translateTreeRecurs(huffmanTree* completed, charTranslation* translation, std::vector<bool> b);
void  printBoolVector(std::vector <bool> v);
charTranslation* translateTree(huffmanTree* completed);
void inputToOutputFile(FILE* input, FILE* output, charTranslation* characters);
FILE* writeBoolVectorToFile(std::vector <bool> v, FILE* output);
std::vector <huffmanTree*> createHuffmanHeap(unsigned int* counted_Chars);


int main(int argc, char** argv)
{
	FILE* fp = fopen("MOBY DICK.txt","r");

	if(fp == NULL)
	{
		std::cout<<"The file is not found"<<argv[1]<<"\n";
		return 0;
	}

	unsigned int* counted_Chars = count_From_File(fp);
	int charactersUsed = 0;
	int i;
	for(i=0;i<256;i++)
	{
		if(counted_Chars[i]>0)
		{
			charactersUsed++;
		}
		
	}

	std::vector <huffmanTree*> huffmanHeap = createHuffmanHeap(counted_Chars);


	std::make_heap (huffmanHeap.begin(),huffmanHeap.end(),&compareHuffman );
	for(i=0;i<charactersUsed;i++)
	{
		printNode(huffmanHeap[i]);
	}

 	std::make_heap (huffmanHeap.begin(),huffmanHeap.end(),&compareHuffman );

	huffmanTree* temp1, *temp2, *combinedTree;
	for(i=0;i<charactersUsed-1;i++)
	{
		temp1 = huffmanHeap.front();
		std::pop_heap (huffmanHeap.begin(),huffmanHeap.end(), compareHuffman) ;
		huffmanHeap.pop_back();

		temp2 = (huffmanHeap.front());
		std::pop_heap (huffmanHeap.begin(),huffmanHeap.end(), compareHuffman) ;
		huffmanHeap.pop_back();

		combinedTree = new huffmanTree (temp1, temp2);
		

		huffmanHeap.push_back(combinedTree); 
		std::push_heap (huffmanHeap.begin(),huffmanHeap.end(), compareHuffman);		
	}

	//might not need

	assignDepth(combinedTree);

	charTranslation *translation;

	//memset(translation, 256, sizeof(charTranslation));

	translation = translateTree(combinedTree);

	FILE*output = fopen("compressed.txt", "w");

	inputToOutputFile(fp, output, translation);

	fclose(fp);

 	return 0;

}

std::vector <huffmanTree*> createHuffmanHeap(unsigned int* counted_Chars)
{
	std::vector <huffmanTree*> huffmanHeap = std::vector <huffmanTree*>();

	huffmanTree* temp;
	int nodeIndex = 0,i;
	for(i=0;i<256;i++)
	{
		if(counted_Chars[i]>0)
		{
			temp = new huffmanTree(i, counted_Chars[i]);

			huffmanHeap.push_back(temp);
		}		
	}

	return huffmanHeap;
}

void inputToOutputFile(FILE* input, FILE* output, charTranslation* characters)
{
	char buffer[1000];
	size_t fileSize = sizeOfFile(input);

	unsigned int num_chars = fileSize,i,j, numReads = fileSize/1000 +(fileSize%1000==0?0:1), charsToRead;

	std::vector <bool> writeToFile;
	for(i=0;i<numReads;i++)
	{
		if(fileSize>=1000)
		{
			charsToRead = 1000;
		}
		
		else
		{
			charsToRead = fileSize;
		}

		for(j=0;j<charsToRead;j++)
		{
			fread(buffer, sizeof(char),charsToRead, input );	

			writeToFile.insert(writeToFile.end(), characters[buffer[j]].v->begin(), characters[buffer[j]].v->end());
	
		}

		fileSize-=1000;
		
	}

	writeBoolVectorToFile(writeToFile,output);

	return;
}

FILE* writeBoolVectorToFile(std::vector <bool> v, FILE* output)
{
	unsigned int numBits = v.size(),i;
	int numWrites = numBits/8 + (numBits%8 == 0 ? 0:1);
	int bitsRemaining = numBits%4;
	int j;

	for(i=0;i<bitsRemaining;i++)
	{
		v.push_back(false);
	}

	char buf;

	for(i=0;i<numWrites;i++)
	{
		buf = 0x00;

		for(j=0;j<8;j++)
		{
			if(v[i])
			{
				buf = buf| (0x01 << 7-i); 
			}
		}
		fwrite (&buf , sizeof(char), sizeof(buf), output);

	}

	fclose(output);

	return output;

}

void printTranslation(charTranslation translated)
{
	int i;
	printBoolVector(*(translated.v));
	
}

void  printBoolVector(std::vector <bool> v)
{
	int i;
	for(i=0;i<v.size();i++)
	{
		std::cout<<v[i];
	}
	std::cout<<"\n";
}
charTranslation* translateTree(huffmanTree* completed)
{
	charTranslation *translation = new charTranslation [256];

	std::vector<bool> b;

	translateTreeRecurs(completed, translation, b);

	return translation;
}
void translateTreeRecurs(huffmanTree* completed, charTranslation* translation, std::vector<bool> b)
{
	if(isLeaf(completed))
	{
		std::vector <bool> *insertingVecor = new std::vector <bool>();

		insertingVecor->insert(insertingVecor->end(), b.begin(), b.end());

		translation[completed->character].v = insertingVecor;
		translation[completed->character].letter = completed->character;
	}

//	Will have to free
	else
	{
		std::vector <bool> copyleft = b;
		std::vector <bool> copyright = b;
		if(completed->left != NULL)
		{
			copyleft.push_back (false);
			translateTreeRecurs(completed->left, translation, copyleft);
		}

		if(completed->right != NULL)
		{
			copyright.push_back (true);
			translateTreeRecurs(completed->right, translation, copyright);		
		}
	}

	return;

}

int maxDepth(huffmanTree* complete)
{
	if(complete == NULL)
	{
		return -1;
	}

	int left = maxDepth(complete->left) + 1;
	int right = maxDepth(complete->right) + 1;

	if(left>right)
	{
		return left;
	}
	return right;
}

void assignDepth(huffmanTree* completed)
{
	putDepth(completed, 0);
}

void putDepth(huffmanTree* completed, int depth)
{
	completed->depth = depth;

	if(completed->left != NULL)
	{
		putDepth(completed->left, depth+1);
	}

	if(completed->right != NULL)
	{
		putDepth(completed->right, depth+1);
	}
}
void printTree(huffmanTree* completed)
{
	if(isLeaf(completed))
	{
		printdepthOfNode(completed);
		return;
	}

	if(completed->left!=NULL)
	{
		printTree(completed->left);
	}

	if(completed->right!=NULL)
	{
		printTree(completed->right);
	}

	return;
}

//For debugging
void printNode(huffmanTree* leaf)
{
	std::cout << "The char is "<<leaf->character<<", with "<<leaf->count<<" occurences\n";
}

void printdepthOfNode(huffmanTree* leaf)
{
	std::cout << "The char is "<<leaf->character<<", depth is "<<leaf->depth<<" \n";
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
	unsigned int* characters = (unsigned int*)malloc(sizeof(unsigned int)*256);
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

	return size;
}