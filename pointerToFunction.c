#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZE 100

int* getIntArr(int* size);
int* scrambleInt(int* intArr, int intSize, int* indArr);
void* scramble(void* arr, int ElemSize, int n, int* indArr);
void printIntArr(int* IntArr, int intSize);
char** getStringArr(int* stringSize);
char** scrambleString(char** stringArr, int stringSize, int* indArr);
void printStringArr(char** strArr, int strSize);
void freeMemory(int* intArr, int* scrambleIntArr, int intSize, char** stringArr, char** scrambleStringArr, int stringSize);
void checkAlloc(void* ptr, char* failedMessage);

void main()
{
    int* intArr, * scrambleIntArr;
    int intSize;
    char** stringArr, ** scrambleStringArr;
    int stringSize, i;
    int indArr[SIZE];

    // The user will enter the number of integers followed by the integers.
    intArr = getIntArr(&intSize);

    // The user will enter the indices
    for (i = 0; i < intSize; i++)
        scanf("%d", &indArr[i]);

    //The function scrambles the array using scramble()
    scrambleIntArr = scrambleInt(intArr, intSize, indArr);

    printIntArr(scrambleIntArr, intSize);

    // The user will enter the number of strings (lines) followed by the strings.
    // You may assume that each line contains up to 99 characters.
    stringArr = getStringArr(&stringSize);

    // The user will enter the indices
    for (i = 0; i < stringSize; i++)
        scanf("%d", &indArr[i]);

    //The function scrambles the array using scramble()
    scrambleStringArr = scrambleString(stringArr, stringSize, indArr);

    printStringArr(scrambleStringArr, stringSize);

    freeMemory(intArr, scrambleIntArr, intSize, stringArr, scrambleStringArr, stringSize);
}

void freeMemory(int* intArr, int* scrambleIntArr, int intSize, char** stringArr, char** scrambleStringArr, int stringSize)
{
    free(intArr);
    
    for (int i = 0; i < stringSize; i++)
    {
        free(stringArr[i]);
    }
    free(stringArr);
}


int* getIntArr(int* size)
{
    int* intArr;

    scanf("%d", size);

    intArr = (int*)malloc(sizeof(int) * (*size));
    checkAlloc(intArr, "error");

    for (int i = 0; i < *size; i++)
        scanf("%d", &intArr[i]);

    return intArr;
}

int* scrambleInt(int* intArr, int intSize, int* indArr)
{
    return (int*)scramble(intArr, sizeof(int), intSize, indArr);
}

void* scramble(void* arr, int ElemSize, int n, int* indArr)
{
    // Create a temporary array to hold the shuffled elements
    char* tmpArr = (char*)malloc(ElemSize * n);
    checkAlloc(tmpArr, "error");
    
    // Copy the original array elements to the temporary array
    memcpy(tmpArr, arr, n * ElemSize);

    // Shuffle the elements based on the provided index array
    for (int i = 0; i < n; i++)
        memcpy((char*)arr + (i * ElemSize), tmpArr + (indArr[i] * ElemSize), ElemSize);

    free(tmpArr);
    
    return arr;
}

void printIntArr(int* IntArr, int intSize)
{
    for (int i = 0; i < intSize; i++)
        printf("%d ", IntArr[i]);
}

char** getStringArr(int* stringSize)
{

    char** stringArr;
    char tmpStr[SIZE];
    int logSise;

    scanf("%d", stringSize);

    stringArr = (char**)malloc(sizeof(char*) * (*stringSize));
    checkAlloc(stringArr, "error");

    getchar();
    gets(tmpStr);

    logSise = strlen(tmpStr) + 1;
    stringArr[0] = (char*)malloc(sizeof(char) * logSise);
    checkAlloc(stringArr[0], "error");
    memcpy(stringArr[0], tmpStr, logSise);

    for (int i = 1; i < *stringSize; i++)
    {
        gets(tmpStr);
        logSise = strlen(tmpStr) + 1;
        stringArr[i] = (char*)malloc(sizeof(char) * logSise);
        checkAlloc(stringArr[i], "error");
        memcpy(stringArr[i], tmpStr, logSise);
    }
    return stringArr;
}

char** scrambleString(char** stringArr, int stringSize, int* indArr)
{
    return (char**)scramble(stringArr, sizeof(char*), stringSize, indArr);
}

void printStringArr(char** strArr, int strSize)
{
    for (int i = 0; i < strSize; i++)
        printf("%s\n", strArr[i]);
}

void checkAlloc(void* ptr, char* failedMessage)
{
    if (!ptr)
    {
        printf("\n%s\n", failedMessage);
        exit(1);
    }
}