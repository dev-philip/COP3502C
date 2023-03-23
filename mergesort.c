// Online C compiler to run C program online
#include <stdio.h>

void mergeSort(int a[], int lb, int ub)
{
    if (lb < ub)
    {
        int mid = (lb + ub) / 2;
        mergeSort(a, lb, mid);
        mergeSort(a, mid + 1, ub);
        merge(a, lb, mid, ub);
    }
}

void merge(int a[], int lb, int mid, int ub)
{
    int i, j, k;

    i = lb;
    j = mid + 1;
    k = lb;
    int b[ub];

    while (i <= mid && j <= ub)
    {
        if (a[i] <= a[j])
        {
            b[k] = a[i];
            i++;
        }
        else
        {
            b[k] = a[j];
            j++;
        }
        k++;
    }
    if (i > mid)
    {
        while (j <= ub)
        {
            b[k] = a[j];
            j++;
            k++;
        }
    }
    else
    {
        while (i <= mid)
        {
            b[k] = a[i];
            i++;
            k++;
        }
    }

    for (k = lb; k <= ub; k++)
    {
        a[k] = b[k];
    }
}

int main()
{
    int array[] = {9, 4, 8, 1, 7, 0, 3, 2, 5, 6};
    int length = sizeof(array) / sizeof(array[0]);

    mergeSort(array, 0, length - 1);

    for (int i = 0; i < length; i++)
    {
        printf("%d ", array[i]);
        printf("\n");
    }

    return 0;
}
