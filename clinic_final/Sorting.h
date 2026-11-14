#ifndef SORTING_H
#define SORTING_H

#include <iostream>
#include <cstdlib>

using namespace std;

template <typename T>
void insertionSort(T arr[], int n, bool (*compare)(const T&, const T&)) {
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;

        while (j >= 0 && compare(key, arr[j])) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

template <typename T>
int partition(T arr[], int low, int high, bool (*compare)(const T&, const T&)) {
    long bigRandom = (long)rand() * (RAND_MAX + 1L) + rand();
    int randomIndex = low + bigRandom % (high - low + 1);

    T temp = arr[randomIndex];
    arr[randomIndex] = arr[high];
    arr[high] = temp;

    T pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (compare(arr[j], pivot)) {
            i++;
            T swapTemp = arr[i];
            arr[i] = arr[j];
            arr[j] = swapTemp;
        }
    }

    T swapTemp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = swapTemp;

    return (i + 1);
}

template <typename T>
void quickSortHelper(T arr[], int low, int high, bool (*compare)(const T&, const T&)) {
    if (low < high) {
        int pi = partition(arr, low, high, compare);
        quickSortHelper(arr, low, pi - 1, compare);
        quickSortHelper(arr, pi + 1, high, compare);
    }
}

template <typename T>
void quickSort(T arr[], int n, bool (*compare)(const T&, const T&)) {
    if (n > 1) {
        quickSortHelper(arr, 0, n - 1, compare);
    }
}

template <typename T>
int binarySearch(T arr[], int n, int targetId) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid].id == targetId) {
            return mid;
        }
        if (arr[mid].id < targetId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

#endif
