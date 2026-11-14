#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "ClinicQueue.h"
#include "Patient.h"
#include "Sorting.h"

using namespace std;

bool byId(const Patient& a, const Patient& b) {
    return a.id < b.id;
}

bool byUrgency(const Patient& a, const Patient& b) {
    if (a.urgency != b.urgency) {
        return a.urgency > b.urgency;
    }
    return a.arrivalTime < b.arrivalTime;
}

int queueToArray(LinkedQueue<Patient>& q, Patient arr[]) {
    int size = q.size();
    for (int i = 0; i < size; i++) {
        arr[i] = q.dequeue();
    }

    for (int i = 0; i < size; i++) {
        q.enqueue(arr[i]);
    }
    return size;
}

void report(string id, string what, bool ok, int &passCount, int &failCount) {
    if (ok) {
        cout << id << ": PASS (" << what << ")" << endl;
        passCount++;
    } else {
        cout << id << ": FAIL (" << what << ")" << endl;
        failCount++;
    }
}

bool byUrgencyOnly(const Patient& a, const Patient& b) {
    return a.urgency > b.urgency;
}

Patient makePatient(int id, string name, int urgency, string time) {
    Patient p;
    p.id = id;
    p.name = name;
    p.urgency = urgency;
    p.arrivalTime = time;
    return p;
}

int main() {
    srand(time(0));

    cout << "=== A1: Queue Tests ===" << endl;
    LinkedQueue<Patient> q;
    int passCount = 0;
    int failCount = 0;

    try {
        q.dequeue();
        cout << "T1: FAIL (Should have crashed)" << endl;
        failCount++;
    } catch (...) {
        cout << "T1: PASS (Empty queue throws error)" << endl;
        passCount++;
    }

    try {
        int loaded = q.loadFromFile("patients.txt");
        if (loaded == 12) {
            cout << "T2: PASS (Loaded 12 patients)" << endl;
            passCount++;
        } else {
            cout << "T2: FAIL (Loaded wrong amount)" << endl;
            failCount++;
        }
    } catch (...) {
        cout << "T2: FAIL (File error)" << endl;
        failCount++;
    }

    LinkedQueue<Patient> copyQ = q;
    q.dequeue();
    if (q.size() == 11 && copyQ.size() == 12) {
        cout << "T3: PASS (Deep copy works)" << endl;
        passCount++;
    } else {
        cout << "T3: FAIL (Copy messed up)" << endl;
        failCount++;
    }

    cout << "\n=== A2: Sorting Tests ===" << endl;

    Patient arr[100];
    int n = queueToArray(q, arr);

    insertionSort(arr, n, byId);
    cout << "Array sorted by ID (First 3): " << endl;
    for (int i = 0; i < 3; i++) {
        cout << "  ID: " << arr[i].id << " Name: " << arr[i].name << endl;
    }

    quickSort(arr, n, byUrgency);
    cout << "Array sorted by Urgency (First 3): " << endl;
    for (int i = 0; i < 3; i++) {
        cout << "  Urgency: " << arr[i].urgency << " Name: " << arr[i].name << endl;
    }

    insertionSort(arr, n, byId);
    int searchId = 1054;
    int index = binarySearch(arr, n, searchId);
    if (index != -1) {
        cout << "T4: PASS (Binary search found ID " << searchId << " at index " << index << ")" << endl;
        passCount++;
    } else {
        cout << "T4: FAIL (Binary search failed)" << endl;
        failCount++;
    }

    cout << "\n=== A3: More boundary tests ===" << endl;

    {
        LinkedQueue<Patient> e;
        bool threw = false;
        try { e.front(); } catch (...) { threw = true; }
        report("T5", "front on empty queue throws error", threw, passCount, failCount);
    }

    {
        LinkedQueue<Patient> one;
        one.enqueue(makePatient(1, "Solo", 2, "08:00"));
        bool ok = (one.size() == 1 && one.front().id == 1);
        one.dequeue();
        ok = ok && one.isEmpty();
        report("T6", "one record: enqueue, front, dequeue", ok, passCount, failCount);
    }

    {
        LinkedQueue<Patient> e;
        e.enqueue(makePatient(1, "A", 1, "08:00"));
        e.dequeue();
        e.enqueue(makePatient(2, "B", 1, "08:05"));
        e.enqueue(makePatient(3, "C", 1, "08:10"));
        bool ok = (e.size() == 2 && e.front().id == 2);
        report("T7", "enqueue after queue was emptied", ok, passCount, failCount);
    }

    {
        LinkedQueue<Patient> d;
        d.enqueue(makePatient(7, "DupA", 1, "08:00"));
        d.enqueue(makePatient(7, "DupB", 2, "08:10"));
        bool ok = (d.size() == 2 && d.dequeue().name == "DupA" && d.dequeue().name == "DupB");
        report("T8", "duplicate id: both stored in order", ok, passCount, failCount);
    }

    {
        LinkedQueue<Patient> f;
        f.loadFromFile("patients.txt");
        Patient first = f.dequeue();
        report("T9", "remove first record (id 1042), 11 left", first.id == 1042 && f.size() == 11, passCount, failCount);
    }

    {
        LinkedQueue<Patient> l;
        l.enqueue(makePatient(1, "A", 1, "08:00"));
        l.enqueue(makePatient(2, "B", 1, "08:05"));
        l.enqueue(makePatient(3, "C", 1, "08:10"));
        l.dequeue();
        l.dequeue();
        Patient last = l.dequeue();
        report("T10", "remove last record (id 3), queue empty", last.id == 3 && l.isEmpty(), passCount, failCount);
    }

    {
        LinkedQueue<Patient> a, b;
        a.enqueue(makePatient(1, "A", 1, "08:00"));
        b.enqueue(makePatient(5, "X", 1, "08:00"));
        b.enqueue(makePatient(6, "Y", 1, "08:05"));
        b = a;
        a.dequeue();
        report("T11", "assignment operator copies the queue", b.size() == 1 && b.front().id == 1, passCount, failCount);
    }

    {
        LinkedQueue<Patient> s;
        s.loadFromFile("patients.txt");
        s.enqueue(makePatient(2000, "New Patient", 4, "09:30"));
        s.saveToFile("patients_test_copy.txt");
        LinkedQueue<Patient> r;
        int loaded = r.loadFromFile("patients_test_copy.txt");
        report("T12", "save 13 records then load them again", loaded == 13 && r.size() == 13, passCount, failCount);
    }

    {
        LinkedQueue<Patient> m;
        bool threw = false;
        try { m.loadFromFile("no_such_file.txt"); } catch (...) { threw = true; }
        report("T13", "missing file is rejected safely", threw, passCount, failCount);
    }

    {
        Patient none[1];
        insertionSort(none, 0, byId);
        quickSort(none, 0, byId);
        report("T14", "both sorts on empty array do nothing", true, passCount, failCount);
    }

    {
        Patient one[1];
        one[0] = makePatient(9, "Solo", 3, "08:00");
        insertionSort(one, 1, byId);
        quickSort(one, 1, byId);
        report("T15", "both sorts on one record", one[0].id == 9, passCount, failCount);
    }

    {
        Patient a[5], b[5];
        for (int i = 0; i < 5; i++) {
            a[i] = makePatient(i + 1, "P", 1, "08:00");
            b[i] = a[i];
        }
        insertionSort(a, 5, byId);
        quickSort(b, 5, byId);
        bool ok = true;
        for (int i = 0; i < 5; i++) {
            if (a[i].id != i + 1 || b[i].id != i + 1) ok = false;
        }
        report("T16", "already sorted input stays sorted", ok, passCount, failCount);
    }

    {
        Patient a[5], b[5];
        for (int i = 0; i < 5; i++) {
            a[i] = makePatient(5 - i, "P", 1, "08:00");
            b[i] = a[i];
        }
        insertionSort(a, 5, byId);
        quickSort(b, 5, byId);
        bool ok = true;
        for (int i = 0; i < 5; i++) {
            if (a[i].id != i + 1 || b[i].id != i + 1) ok = false;
        }
        report("T17", "reversed input gets sorted", ok, passCount, failCount);
    }

    {
        Patient s[6];
        s[0] = makePatient(1, "A", 3, "08:00");
        s[1] = makePatient(2, "B", 5, "08:05");
        s[2] = makePatient(3, "C", 3, "08:10");
        s[3] = makePatient(4, "D", 5, "08:15");
        s[4] = makePatient(5, "E", 3, "08:20");
        s[5] = makePatient(6, "F", 5, "08:25");
        insertionSort(s, 6, byUrgencyOnly);

        bool ok = (s[0].id == 2 && s[1].id == 4 && s[2].id == 6 &&
                   s[3].id == 1 && s[4].id == 3 && s[5].id == 5);
        report("T18", "insertion sort keeps arrival order for equal urgency", ok, passCount, failCount);
    }

    {
        Patient many[60];
        for (int i = 0; i < 60; i++) {
            many[i] = makePatient(i, "P", 1 + i % 3, "08:00");
        }
        quickSort(many, 60, byUrgencyOnly);
        bool ok = true;
        for (int i = 1; i < 60; i++) {
            if (many[i - 1].urgency < many[i].urgency) ok = false;
        }
        report("T19", "quicksort with many equal urgency values", ok, passCount, failCount);
    }

    {
        report("T20", "binary search for absent id 999 gives -1", binarySearch(arr, n, 999) == -1, passCount, failCount);
    }

    {
        bool ok = (binarySearch(arr, n, arr[0].id) == 0 && binarySearch(arr, n, arr[n - 1].id) == n - 1);
        report("T21", "binary search finds first and last record", ok, passCount, failCount);
    }

    {
        Patient none[1];
        report("T22", "binary search on empty array gives -1", binarySearch(none, 0, 5) == -1, passCount, failCount);
    }

    cout << "\nPassed: " << passCount << " | Failed: " << failCount << endl;
    cout << "=======================" << endl;

    q.saveToFile("patients_updated.txt");
    cout << "Saved to patients_updated.txt" << endl;

    return 0;
}
