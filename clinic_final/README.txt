University Clinic Patient Queue - BCS 2204 Coursework 1 (Scenario 1)

Files
  Patient.h       the Patient struct (id, name, urgency, arrivalTime)
  ClinicQueue.h   my linked queue class template (A1)
  Sorting.h       insertion sort, quicksort with random pivot, iterative binary search (A2)
  main.cpp        the tests (A3)
  timing.cpp      timing experiment for the two sorts (A4)
  bug_demo.cpp    my first buggy dequeue, used for the bug report in A3
  patients.txt    12 sample patients, one per line: id,name,urgency,arrivalTime

How to compile and run (needs g++ with C++17)
  1. Open a terminal in this folder.
  2. Tests:   g++ -std=c++17 -fsanitize=address -g main.cpp -o clinic_app
              ./clinic_app
  3. Timing:  g++ -std=c++17 -O2 timing.cpp -o timing
              ./timing
  4. patients.txt must be in the same folder you run the program from.
