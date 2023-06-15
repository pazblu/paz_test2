//name: paz blutman id: 206000119

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "List.h"

#define RANGE 100

char* getFileName(char* str);
void getDataFromFile(FILE* studentFile, STUDENT* studentArr, int* dataLocationsInFile, int studArrSize);
STUDENT* makeStudentArr(FILE* studentFile, int** dataLocationsInFile, int* studArrSize);
void checkAlloc(void* ptr, char* failedMessage);
void makeIndexFile(FILE* indexFile, int* dataLocationsInFile, int size);
void freeAll(STUDENT* studentArr, char* fileName, int* dataLocationsInFile, int size);
void countingSort(STUDENT arr[], int location[], int n, int k);


void main(int argc, char** argv)
{
	FILE* studentData;
	FILE* indexFile;
	STUDENT* studentArr;
	int studArrSize;
	char* fileName;
	int* dataLocationsInFile;

	fileName = getFileName(argv[1]);
	studentData = fopen(fileName, "rb");

	fileName = strcat(fileName, ".ind");
	indexFile = fopen(fileName, "wb");

	checkAlloc(studentData, (char*)"file error");
	checkAlloc(indexFile, (char*)"file error");

	studentArr = makeStudentArr(studentData, &dataLocationsInFile, &studArrSize);

	getDataFromFile(studentData, studentArr, dataLocationsInFile, studArrSize);

	countingSort(studentArr, dataLocationsInFile, studArrSize, RANGE);

	makeIndexFile(indexFile, dataLocationsInFile, studArrSize);

	fclose(studentData);
	fclose(indexFile);

	freeAll(studentArr, fileName, dataLocationsInFile, studArrSize);
}

// Free all dynamically allocated memory
void freeAll(STUDENT* studentArr, char* fileName, int* dataLocationsInFile, int size)
{
	free(dataLocationsInFile);

	for (int i = 0; i < size; i++)
	{
		free(studentArr[i].name);
	}
	free(studentArr);
	free(fileName);
}

// Extracts the file name from the command-line argument
char* getFileName(char* str)
{
	char* fileName = (char*)malloc(sizeof(char) * (strlen(str) + strlen(".ind") + 1));
	checkAlloc(fileName, (char*)"error");
	strcpy(fileName, str);

	return fileName;
}

// Write the data locations to the index file
void makeIndexFile(FILE* indexFile, int* dataLocationsInFile, int size)
{
	fwrite(dataLocationsInFile, sizeof(int), size, indexFile);
}

// Create an array to hold the student data and data locations
STUDENT* makeStudentArr(FILE* studentFile, int** dataLocationsInFile, int* studArrSize)
{
	short int size;

	fread(&size, sizeof(short int), 1, studentFile);

	STUDENT* studentArr = (STUDENT*)malloc(sizeof(STUDENT) * size);
	checkAlloc(studentArr, (char*)"error");

	*dataLocationsInFile = (int*)malloc(sizeof(int) * size);
	checkAlloc(*dataLocationsInFile, (char*)"error");

	*studArrSize = size;
	return studentArr;
}

// Read data from the student file and store it in the array
void getDataFromFile(FILE* studentFile, STUDENT* studentArr,int* dataLocationsInFile, int studArrSize)
{
	short int nameLen;

	for (int i = 0;i < studArrSize;i++)
	{
		dataLocationsInFile[i] = ftell(studentFile);

		fread(&nameLen, sizeof(short int), 1, studentFile);

		studentArr[i].name = (char*)malloc(sizeof(char) * nameLen);
		checkAlloc(studentArr[i].name, (char*)"error");

		fread(studentArr[i].name, sizeof(char), nameLen, studentFile);
		fread(&studentArr[i].average, sizeof(int), 1, studentFile);
	}
}

void checkAlloc(void* ptr, char* failedMessage)
{
	if (!ptr)
	{
		printf("\n%s\n", failedMessage);
		exit(1);
	}
}

// Counting sort for sorting the student data
void countingSort(STUDENT arr[], int location[], int n, int k)
{
	List* counters = (List*)malloc((k + 1) * sizeof(List));
	ListNode* currListNode;
	int i, j = 0;

	checkAlloc(counters, (char*)"error");

	// initialize counters
	for (i = 0; i < k + 1; i++)
		makeEmptyList(counters + i);


	for (i = 0; i < n; i++)
		insertDataToEndList(counters + (arr[i].average), arr[i], location[i]);

	// Write sorted values back to arr and location array
	for (i = 0; i < k + 1; i++)
	{	
		currListNode = counters[i].head;
		while (currListNode != NULL)
		{
			arr[j] = currListNode->student;
			location[j] = currListNode->locationInFile;
			j++;
			currListNode = currListNode->next;
		}
	}

	for (i = 0; i < k + 1; i++)
	{
		if (counters[i].head != NULL)
			freeList(&counters[i]);
	}
	free(counters);
}
