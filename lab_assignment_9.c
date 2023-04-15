#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Fill out this structure
struct HashType
{
    struct RecordType record;
    struct HashType *next;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
    struct HashType *pHash;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        pHash = pHashArray + i;
        while (pHash->next != NULL)
        {
            printf("%d %c %d -> ", pHash->record.id, pHash->record.name, pHash->record.order);
            pHash = pHash->next;
        }
        printf("\n");
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashSz = 10;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    // Your hash implementation

    // Allocate memory for the hash table
    struct HashType *pHashArray = (struct HashType *)malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }

    for (int i = 0; i < recordSz; ++i)
    {
        // Create a new hash entry for the record
        struct HashType *tempHash = (struct HashType *)malloc(sizeof(struct HashType));
        if (tempHash == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        tempHash->record = *(pRecords + i);
        tempHash->next = NULL;

        // Compute the hash index for the record
        int hashIndex = hash(pRecords[i].id, hashSz);

        // Check if the hash index is empty
        struct HashType *pHash = pHashArray + hashIndex;

        pHash = pHashArray + hashIndex;
        if (pHash->next == NULL)
        {
            pHash->next = tempHash;
        }
        else
        {
            // Append the new hash entry to the end of the linked list
            while (pHash->next != NULL)
            {
                pHash = pHash->next;
            }
            pHash->next = tempHash;
        }
    }

    // Display the records in the hash table
    displayRecordsInHash(pHashArray, hashSz);

    free(pHashArray);

    return 0;
}