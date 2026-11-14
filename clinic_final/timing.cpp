#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "Patient.h"
#include "Sorting.h"

using namespace std;
using namespace std::chrono;

bool byId(const Patient& a, const Patient& b) {
    return a.id < b.id;
}

int main() {
    srand(time(0));
    cout << "n,Insertion_ms,Quick_ms" << endl;

    for (int n = 1000; n <= 64000; n *= 2) {
        double totalInsertion = 0;
        double totalQuick = 0;

        for (int run = 0; run < 3; run++) {
            Patient* arr1 = new Patient[n];
            Patient* arr2 = new Patient[n];

            for (int i = 0; i < n; i++) {
                arr1[i].id = rand() % 100000;
                arr2[i].id = arr1[i].id;
            }

            auto start1 = high_resolution_clock::now();
            insertionSort(arr1, n, byId);
            auto stop1 = high_resolution_clock::now();
            totalInsertion += duration<double, milli>(stop1 - start1).count();

            auto start2 = high_resolution_clock::now();
            quickSort(arr2, n, byId);
            auto stop2 = high_resolution_clock::now();
            totalQuick += duration<double, milli>(stop2 - start2).count();

            delete[] arr1;
            delete[] arr2;
        }

        cout << n << "," << totalInsertion / 3 << "," << totalQuick / 3 << endl;
    }
    return 0;
}
