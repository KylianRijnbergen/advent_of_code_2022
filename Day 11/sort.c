#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"

static void swap_ll(long long* x, long long* y);
static void quicksort_ll_rec(long long arr[], long long low, long long high);
static long long partition_ll(long long arr[], long long low, long long high);

static void swap_ll(long long* x, long long* y) {
    long long temp = *x;
    *x = *y;
    *y = temp;
}

void quicksort_ll(long long arr[], long long dim) {
    srand(time(NULL));
    quicksort_ll_rec(arr, 0, dim - 1);
}

static void quicksort_ll_rec(long long arr[], long long low, long long high) {
    if (low < high) {
        long long pivot = partition_ll(arr, low, high);
        /* Sort left */
        quicksort_ll_rec(arr, low, pivot - 1);

        /* Sort right */
        quicksort_ll_rec(arr, pivot + 1, high);
    }
}

static long long partition_ll(long long arr[], long long low, long long high) {
    long long pivot = low + (rand() % (high - low)); /* NOTE THAT THIS IS NOT A PROPER IMPLEMENTATION FOR LONG LONG AS RAND_MAX IS NOT SUFFICIENTLY LARGE. */

    if (pivot != high) {
        swap_ll(&arr[pivot], &arr[high]);
    }
    long long pivot_val = arr[high];
    long long i = low;
    for (long long j = low; j < high; j++) {
        if (arr[j] < pivot_val) {
            swap_ll(&arr[i], &arr[j]);
            i++;
        }
    }
    swap_ll(&arr[i], &arr[high]);

    return i;
}