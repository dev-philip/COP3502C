
#include <stdio.h>

int main()
{
    int array[] = {97, 16, 45, 63, 13, 22, 7, 58, 72};
    int n = sizeof(array) / sizeof(array[0]);

    // BUBBLE SORT
    int numOfIteration, temp, numOfSwap;
    numOfIteration = 0;
    for (int i = n; i > 0; i--)
    {
        numOfIteration++;
        numOfSwap = 0;
        for (int j = 0; j < n - 1; j++)
        {
            temp = array[j];
            if (array[j] > array[j + 1])
            {
                array[j] = array[j + 1];
                array[j + 1] = temp;
                numOfSwap++;
            }
        }
        printf("Iteration %d - %d SWAP\n", numOfIteration, numOfSwap);
    }

    return 0;
}
