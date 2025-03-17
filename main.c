#include <stdio.h>
#include <stdlib.h>

/* COP 3502C Programming Assignment 4
   This program is written by: Jalil Fountain */
typedef struct {
    int x, y;
} Point;

// global variables
int userX, userY;

// function to compare two points based on the distance from the userd position
int compareTo(Point *ptrPt1, Point *ptrPt2) {
    // calculate the distance of the first point using formula 
    int d1 = (ptrPt1->x - userX) * (ptrPt1->x - userX) + (ptrPt1->y - userY) * (ptrPt1->y - userY);
    // calculate the distance of the second point using formula
    int d2 = (ptrPt2->x - userX) * (ptrPt2->x - userX) + (ptrPt2->y - userY) * (ptrPt2->y - userY);
    
    // compare the distances
    if (d1 < d2) {
        return -1;  // ptrPt1 is closer than ptrPt2
    } else if (d1 > d2) {
        return 1;   // ptrPt1 is farther than ptrPt2
    }

    // if the distances are the same compare the x coordinates
    if (ptrPt1->x < ptrPt2->x) {
        return -1;  // ptrPt1 x is smaller than ptrPt2 x
    } else if (ptrPt1->x > ptrPt2->x) {
        return 1;   // ptrPt1 x is larger than ptrPt2 x
    }

    // if both x coordinates are the same compare the y coordinates
    if (ptrPt1->y < ptrPt2->y) {
        return -1;  // ptrPt1 y is smaller than ptrPt2 y
    } else if (ptrPt1->y > ptrPt2->y) {
        return 1;   // ptrPt1 y is larger than ptrPt2 y
    }

    // if the points are the same
    return 0;  
}

// function to perform insertion sort on an array of points between left and right indiciies
void insertionSort(Point arr[], int left, int right) {
    int i, j;
    Point item;
    // loop through the array 
    for (i = left + 1; i <= right; i++) {
        item = arr[i];  // store the current point to be inserted
        // loop backward to find the correct position for the item
        for (j = i - 1; j >= left; j--) {
            // if the current item is closer shift the array element
            if (compareTo(&arr[j], &item) > 0)
                arr[j + 1] = arr[j];
            else
                break;  
        }
        arr[j + 1] = item;  // insert the item at its correct position
    }
}

// merge function used in merge sort to combine two sorted halves of an array
void merge(Point arr[], int left, int mid, int right) {
    // calculate the sizes of the two subarrays
    int n1 = mid - left + 1, n2 = right - mid;
    // allocate memory for left and right subarrays
    Point *L = malloc(n1 * sizeof(Point));
    Point *R = malloc(n2 * sizeof(Point));
    
    // copy data into the left and right subarrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];  
    for (int i = 0; i < n2; i++) 
        R[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    
    // merge the two sorted subarrays back into the original array
    while (i < n1 && j < n2) {
        // compare the current elements of the two subarrays and add the smaller one to arr
        if (compareTo(&L[i], &R[j]) <= 0) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    // copy the remaining elements of L
    while (i < n1) 
    arr[k++] = L[i++];
    // copy the remaining elements of R
    while (j < n2) 
    arr[k++] = R[j++];
    
    free(L);
    free(R);
}

// function to recursively perform merge sort on an array of points
void mergeSort(Point arr[], int left, int right, int t) {
    // if the number of elements is smaller than or equal to t, use insertion sort
    if (right - left + 1 <= t) {
        insertionSort(arr, left, right);
        return;
    }
    // find the midpoint of the array
    int mid = left + (right - left) / 2;
    // recursively call mergeSort on the left and right halves
    mergeSort(arr, left, mid, t);
    mergeSort(arr, mid + 1, right, t);
    // merge the two sorted halves
    merge(arr, left, mid, right);
}

// function to sort an array of points using merge sort 
void sortPoints(Point arr[], int n, int t) {
    mergeSort(arr, 0, n - 1, t);  
}

// function to perform binary search on an array of points to find a point
int binarySearch(Point arr[], int n, int x, int y) {
    int left = 0, right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        Point key = {x, y};  // for comparison
        int cmp = compareTo(&arr[mid], &key);  // compare the key with the middle point

        if (cmp == 0) {
            return mid+1;  // found the point
        }
        else if (cmp < 0) {
            left = mid + 1;  // key is greater move right
        } else {
            right = mid - 1;  // key is smaller move left
        }
    }
    return -1;  // point not found
}

// function to read the input data
Point* readData(int *n, int *s, int *t) {
    scanf("%d %d %d %d %d", &userX, &userY, n, s, t);

    // allocate memory for the stadiums array
    Point *stadiums = malloc(*n * sizeof(Point));

    // read the coordinates for each stadium
    for (int i = 0; i < *n; i++) {
        scanf("%d %d", &(stadiums[i].x), &(stadiums[i].y));
    }

    return stadiums;  
}

int main() {
    int n, s, t; 
    int queryX, queryY;
    // read data and get the array of stadiums
    Point *stadiums = readData(&n, &s, &t);
    
    printf("\n");
    printf("\n");

    // sort the stadiums 
    sortPoints(stadiums, n, t);  

    // print the sorted stadiums
    for (int i = 0; i < n; i++) {
        printf("%d %d\n", stadiums[i].x, stadiums[i].y);
    }

    // answer the queries
    for (int i = 0; i < s; i++) {
        // read the coordinates for each query
        scanf("%d %d", &queryX, &queryY);

        // binary search to find the query point
        int rank = binarySearch(stadiums, n, queryX, queryY);
        if (rank != -1) {
            printf("%d %d: stadium found at rank %d in the sorted order\n", queryX, queryY, rank);
        } else {
            printf("%d %d: no stadium found at this location\n", queryX, queryY);
        }
    }

    
    free(stadiums);

    return 0;  
}


