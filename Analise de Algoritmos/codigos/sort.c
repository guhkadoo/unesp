#include <stdio.h>
#include <stdlib.h>
#define ARRAY_SIZE(v) sizeof(v)/sizeof(v[0])

static inline void swap(int *a, int *b) { if(a!=b) { (*a^=*b), (*b^=*a), (*a^=*b); } }

// BUBBLE SORT BEGIN
void bubble_sort(int *v, int length)
{
    register int i, j;
    for(i=0; i<length-1; i++)
    {
        int is_sorted = 1;
        for(j=0; j<length-i-1; j++)
        {
            if(v[j+1] < v[j])
            {
                swap(&v[j+1], &v[j]);
                is_sorted = 0;
            }
        }
        if(is_sorted)
            break;
    }
}
// BUBBLE SORT END

// INSERTION SORT BEGIN
void insertion_sort(int *v, int len)
{
    register int i, j;
    for(i=1; i<len; i++)
    {
        int key = v[i];
        for(j=i-1; j>=0 && v[j]>key; j--)
            v[j+1] = v[j];
        v[j+1] = key;
    }
}
// INSERTION SORT END

// HEAP SORT BEGIN 
void max_heapify(int v[], int n, int i)
{
    int l = 2*i+1;
    int r = 2*i+2;
    int larg = i;
    if(l < n && v[l] > v[larg])
        larg = l;
    if(r < n && v[r] > v[larg])
        larg = r;
    if(larg != i) {
        swap(&v[larg], &v[i]);
        max_heapify(v, n, larg);
    }
}

void heap_sort(int *v, int n)
{
    for(int i=n/2-1; i>=0; i--)
        max_heapify(v, n, i);
    
    n--;
    while(n) {
        swap(&v[0], &v[n--]);
        max_heapify(v, n, 0);
    }
}
// HEAP SORT END

// MERGE SORT BEGIN
void merge(int v[], int low, int mid, int high)
{
    int n1 = mid-low+1;
    int n2 = high-mid;
    int v1[n1], v2[n2];

    for(int i=0; i<n1; i++)
        v1[i] = v[low+i];
    for(int i=0; i<n2; i++)
        v2[i] = v[mid+1+i];

    int i=0,j=0,k=low;
    while(i<n1 && j<n2)
    {
        if(v1[i] < v2[j])
            v[k++] = v1[i++];
        else
            v[k++] = v2[j++];
    }
    while(i<n1)
        v[k++] = v1[i++];
    while(j<n2)
        v[k++] = v2[j++];
}

void merge_sort(int *v, int low, int high)
{
    if(low < high) {
        int mid = (low+high)/2;
        merge_sort(v, low, mid);
        merge_sort(v, mid+1, high);

        merge(v, low, mid, high);
    }
}
// MERGE SORT END

// QUICK BEGIN
int lomuto(int *v, int low, int high)
{
    int *pivot = &v[high];
    int boundary = low;
    for(int i=low; i<high; i++) {
        if(v[i] <= *pivot)
            swap(&v[i], &v[boundary++]);
    }
    swap(pivot, &v[boundary]);

    return boundary;
}

int hoare(int *v, int low, int high)
{
    int i=low-1, j=high+1;
    int pivot = v[low];
    while(1) {
        do {
            i++;
        } while(v[i] < pivot);
        do {
            j--;
        } while(v[j] > pivot);
        if(i>=j)
            return j;
        swap(&v[i], &v[j]);
    }
}

void quick_sort(int *v, int low, int high)
{
    if(low < high) {
        int boundary = hoare(v, low, high);
        quick_sort(v, low, boundary);
        quick_sort(v, boundary+1, high);
    }
}
// QUICK SORT END

// RADIX SORT BEGIN
int getmax(int v[], int len)
{
    int max = v[0];
    for(int i=1; i<len; i++) 
        if(v[i]>max) max = v[i];
    return max;
}

void counting_sort(int v[], int len, int exp)
{
    int *answer = malloc(len * sizeof(int));
    int count[10] = {0};

    int i;
    for(i=0; i<len; i++)
        count[(v[i]/exp)%10]++;
    for(i=1; i<10; i++)
        count[i] += count[i-1];
    for(i=len-1; i>=0; i--) {
        answer[count[(v[i]/exp)%10]-1] = v[i];
        count[(v[i]/exp)%10]--;
    }
    for(i=0; i<len; i++)
        v[i] = answer[i];

    free(answer);
}

void radix_sort(int *v, int len)
{
    int max = getmax(v, len);

    for(int i=1; max/i>0; i*=10)
        counting_sort(v, len, i);
}
// RADIX SORT END 
