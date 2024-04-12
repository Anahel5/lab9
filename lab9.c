#include <stdio.h>
#include <stdlib.h> // For malloc and exit

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// HashType
struct HashType
{
    struct RecordType *record;
    int occupied;
};

// Compute the hash function
int hash(int x)
{
    return x % 10; // Simple hash function, can be improved for better distribution
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
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
// index -> Records -> Records ...
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int found = 0;

    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].occupied)
        {
            if (!found) {
                printf("Records in the Hash Table:\n");
                found = 1;
            }
            printf("Index %d -> %d, %c, %d\n", i, pHashArray[i].record->id, pHashArray[i].record->name, pHashArray[i].record->order);
        }
    }

    if (!found) {
        printf("No records found in the hash table.\n");
    }
}



int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    // Parse data from file
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    // Let's create a hash table with 10 slots
    int hashSize = 10;
    struct HashType hashTable[hashSize];

    // Initialize all slots in the hash table as unoccupied
    for (int i = 0; i < hashSize; ++i)
    {
        hashTable[i].occupied = 0;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        int probes = 0;
        // Check for collision, linear probing is used here
        while (hashTable[index].occupied && probes < hashSize)
        {
            index = (index + 1) % hashSize;
            probes++;
        }
       
        if (probes == hashSize)
        {
            break;
        }
        hashTable[index].record = &pRecords[i];
        hashTable[index].occupied = 1;
    }
    
    // Display records in the hash table
    displayRecordsInHash(hashTable, hashSize);
    return 0;
}
