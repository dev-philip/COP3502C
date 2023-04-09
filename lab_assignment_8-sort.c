#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void swap(int arr[], int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// heapifies a subtree with root at index i
void heapify(int arr[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // if left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // if right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // if largest is not root
    if (largest != i)
    {
        swap(arr, i, largest);

        // recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
    int i;

    // build heap (rearrange array)
    for (i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // one by one extract an element from heap
    for (i = n - 1; i > 0; i--)
    {
        // move current root to end
        swap(arr, 0, i);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);
        int i, j, k;
        int n1 = m - l + 1;
        int n2 = r - m;
        int *L = (int *)malloc(n1 * sizeof(int));
        int *R = (int *)malloc(n2 * sizeof(int));
        extraMemoryAllocated += n1 * sizeof(int) + n2 * sizeof(int);
        for (i = 0; i < n1; i++)
            L[i] = pData[l + i];
        for (j = 0; j < n2; j++)
            R[j] = pData[m + 1 + j];
        i = 0;
        j = 0;
        k = l;
        while (i < n1 && j < n2)
        {
            if (L[i] <= R[j])
            {
                pData[k] = L[i];
                i++;
            }
            else
            {
                pData[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1)
        {
            pData[k] = L[i];
            i++;
            k++;
        }
        while (j < n2)
        {
            pData[k] = R[j];
            j++;
            k++;
        }
        free(L);
        free(R);
        extraMemoryAllocated -= n1 * sizeof(int) + n2 * sizeof(int);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n, *data;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (int *)malloc(sizeof(int) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            fscanf(inFile, "%d ", &n);
            data = *ppData + i;
            *data = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");
    for (i = 0; i < 100; ++i)
    {
        printf("%d ", pData[i]);
    }
    printf("\n\t");

    for (i = sz; i < dataSz; ++i)
    {
        printf("%d ", pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
    clock_t start, end;
    int i;
    double cpu_time_used;
    char *fileNames[] = {"input1.txt", "input2.txt", "input3.txt", "input4.txt"};

    for (i = 0; i < 4; ++i)
    {
        int *pDataSrc, *pDataCopy;
        int dataSz = parseData(fileNames[i], &pDataSrc);

        if (dataSz <= 0)
            continue;

        pDataCopy = (int *)malloc(sizeof(int) * dataSz);

        printf("---------------------------\n");
        printf("Dataset Size : %d\n", dataSz);
        printf("---------------------------\n");

        printf("Heap Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        heapSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        printf("Merge Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        mergeSort(pDataCopy, 0, dataSz - 1);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        free(pDataCopy);
        free(pDataSrc);
    }
}