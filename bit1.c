//name: paz blutman id: 206000119

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ID_SIZE 8
#define PHONE_NO_SIZE 11
#define LEFT 0
#define RIGHT 1

typedef struct _client
{
    char id[9];     // “12345678”	
    char phone[12]; // “054-1234567” 	
} Client;

typedef struct _short_client
{
    unsigned char short_id[4];
    unsigned char short_phone[5];
} Short_client;

typedef unsigned char BYTE;

BYTE bitsToValue(BYTE bitsNum, int flag);
bool findID(char* str1, char* str2);
char* searchClientByID(Short_client* arr, int size, char* id);
void insertDataToCompactArrPhoneNo(Short_client* arr, char num, int i, int placeToInsert);
void insertDataToCompactArrID(Short_client* arr, char num, int i, int placeToInsert);
Client* makeClientArr(int size);
void getDataOfClient(Client* clientArr, int size);
Short_client* makeCompactArray(Client* clientArr, int size);
Short_client* createShortClientArr(int size);
void checkAlloc(void* ptr, char* failedMessage);

void main()
{
    int n;
    Short_client* arr;
    char id[9];
    char* phone;

    printf("Please enter the number of clients: ");
    scanf("%d", &n);

    arr = createShortClientArr(n);

    scanf("%s", id);

    phone = searchClientByID(arr, n, id);

    if (phone == NULL)
        printf("Can not find client with i.d. %s\n", id);
    else
    {
        printf("The phone number of client i.d. %s is %s\n", id, phone);
        free(phone);
    }

    free(arr);
}

// Searches for a client in an array of Short_client structures by their ID
char* searchClientByID(Short_client* arr, int size, char* id)
{
    Short_client client[1] = { 0 };
    int index, i;
    BYTE* phone = (BYTE*)calloc(PHONE_NO_SIZE + 1, sizeof(BYTE));
    
    // Convert the ID string into a compact representn in the client struct
    for (i = 0; i < ID_SIZE; i++)
        insertDataToCompactArrID(client, id[i], 0, i);

    // Search for the client with matching ID in the array
    for (i = 0; i < size; i++)
    {
        if (findID(arr[i].short_id, client->short_id))
            break;
    }

    // If the client is not found, return NULL
    if (i == size)
        return NULL;
    else
    {
        // Convert the compact phone number to the phone number string
        for (int j = 0, k = 0; j < PHONE_NO_SIZE + 1; j++, k++)
        {
            if (j == 3)
            {
                phone[j] = '-';
                k--;
                continue;
            }
            phone[j] = bitsToValue(arr[i].short_phone[k / 2], k % 2);
        }
        phone[PHONE_NO_SIZE] = '\0';
    }
    return phone;
}

// Converts 4 bit to its corresponding char
BYTE bitsToValue(BYTE bitsNum, int flag)
{
    BYTE mask = 0xf; // Mask to extract the right 4 bits

    if (flag == LEFT)
    {
        bitsNum = bitsNum >> 4;
    }
    else
    {
        bitsNum &= mask;
    }
    return bitsNum + '0';
}

// Compares two ID strings to check if they are equal
bool findID(char* str1, char* str2)
{
    for (int i = 0; i < ID_SIZE / 2; i++)
    {
        if (str1[i] != str2[i])
            return false;
    }

    return true;
}

// Creates a Short_client array from an existing Client array
Short_client* createShortClientArr(int size)
{
    Client* clientArr;

    clientArr = makeClientArr(size);

    getDataOfClient(clientArr, size);

    return makeCompactArray(clientArr, size);
}

Short_client* makeCompactArray(Client* clientArr, int size)
{
    Short_client* shortClientArr = (Short_client*)calloc(size, sizeof(Short_client));
    checkAlloc(shortClientArr, "error");

    // Add the compact array ID data
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < ID_SIZE; j++)
        {
            insertDataToCompactArrID(shortClientArr, clientArr[i].id[j], i, j);
        }
    }

    // Add the compact array phone number data
    for (int i = 0; i < size; i++)
    {
        for (int j = 0, k = 0; j < PHONE_NO_SIZE; j++, k++)
        {
            if (clientArr[i].phone[j] == '-')
            {
                k--;
                continue;
            }
            insertDataToCompactArrPhoneNo(shortClientArr, clientArr[i].phone[j], i, k);
        }
    }
    return shortClientArr;
}

// Inserts data into the short_phone of the Short_client array
void insertDataToCompactArrPhoneNo(Short_client* arr, char num, int i, int placeToInsert)
{
    BYTE ch = num - '0';

    if (placeToInsert % 2 == 0)
    {
        ch = ch << 4;
        arr[i].short_phone[placeToInsert / 2] = ch;
    }
    else
    {
        arr[i].short_phone[placeToInsert / 2] |= ch;
    }
}

// Inserts data into the short_id of the Short_client array
void insertDataToCompactArrID(Short_client* arr, char num, int i, int placeToInsert)
{
    BYTE ch = num - '0';

    if (placeToInsert % 2 == 0)
    {
        ch = ch << 4;
        arr[i].short_id[placeToInsert / 2] = ch;
    }
    else
    {
        arr[i].short_id[placeToInsert / 2] |= ch;
    }
}

Client* makeClientArr(int size)
{
    Client* clientArr = (Client*)malloc(sizeof(Client) * size);
    checkAlloc(clientArr, "error");
    return clientArr;
}

void getDataOfClient(Client* clientArr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Please enter ID: ");
        scanf("%s", clientArr[i].id);

        printf("Please enter phone no.: ");
        scanf("%s", clientArr[i].phone);
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