#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int cmpfunc (const void *a, const void *b);

void merge(int* a, int* endA, int *b, int* endB, int *res);

void multisort(int* arr, int* space, int n);


int main(int argc, char **argv) {
    int *arr, *space; 

    int i;

    int n; 

    int numOfThreads; 

    
    printf("Give size of array: ");
    scanf("%d", &n);

    
    printf("Give number of threads: ");
    scanf("%d", &numOfThreads);

    
    omp_set_num_threads(numOfThreads);

    
    arr = (int *) malloc(sizeof(int)*n);
    space = (int *) malloc(sizeof(int)*n);


   
    printf("Give the data of the array\n");
    for(i = 0; i < n; i++){
        printf("A[%d] = ", i+1);
        scanf("%d", &arr[i]);
    }


    
    #pragma omp parallel
        #pragma omp single
            multisort(arr, space, n);

    
    printf("Array: [ ");
    for(i = 0; i < n-1; i++){
        printf("%d, ", arr[i]);
    }
    printf("%d]\n", arr[n-1]);

    return 0;
}


int cmpfunc (const void * a, const void * b){
    return (*(int *)a - *(int *)b);
}

void merge(int* a, int* endA, int *b, int* endB, int *res) {
    int i = 0;

    while (a <= endA && b <= endB) {
        if ((*a) < (*b)) {
            res[i++] = *a;
            a++;
        }
        else {
            res[i++] = *b;
            b++;
        }
    }

    while (a <= endA) {
        res[i++] = *a;
        a++;
    }

    while (b <= endB) {
        res[i++] = *b;
        b++;
    }
}

void multisort(int *array, int *space, int n)
{
    int quarter = n / 4;

    if (quarter < 4)
    {
        qsort(array, n, sizeof(int), cmpfunc);
        return;
    }

    int *startA = array;            int *spaceA = space;
    int *startB = startA + quarter; int *spaceB = spaceA + quarter;
    int *startC = startB + quarter; int *spaceC = spaceB + quarter;
    int *startD = startC + quarter; int *spaceD = spaceC + quarter;

    #pragma omp task
    multisort(startA, spaceA, quarter);

    #pragma omp task
    multisort(startB, spaceB, quarter);

    #pragma omp task
    multisort(startC, spaceC, quarter);
    
    #pragma omp task
    multisort(startD, spaceD, n - 3 * quarter);

    #pragma omp taskwait

    #pragma omp task
    merge(startA, startA + quarter - 1, startB, startB + quarter - 1, spaceA);

    #pragma omp task
    merge(startC, startC + quarter - 1, startD, array + n - 1, spaceC);

    #pragma omp taskwait

    merge(spaceA, spaceC - 1, spaceC, space + n - 1, array);
}
