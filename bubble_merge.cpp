#include <iostream>     // For input and output
#include <vector>       // For vector
#include <omp.h>        // OpenMP for parallel programming
#include <time.h>       // For execution time

using namespace std;


// ================= BUBBLE SORT =================

// Sequential Bubble Sort
void bubble_sort(vector<int>& arr, int n) {

    // Outer loop for passes
    for (int i = 0; i < n - 1; i++) {

        // Inner loop for comparison
        for (int j = 0; j < n - i - 1; j++) {

            // Swap if left element is greater
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


// Parallel Bubble Sort
void parallel_bubble_sort(vector<int>& arr, int n) {

    // Flag to check sorted array
    bool sorted = false;

    // Repeat until sorted
    while (!sorted) {

        sorted = true;

        // ===== EVEN PHASE =====
        // Compare 0-1, 2-3, 4-5 ...

        #pragma omp parallel for shared(arr, sorted)
        for (int i = 0; i < n - 1; i += 2) {

            // Swap if needed
            if (arr[i] > arr[i + 1]) {

                swap(arr[i], arr[i + 1]);

                sorted = false;
            }
        }

        // ===== ODD PHASE =====
        // Compare 1-2, 3-4, 5-6 ...

        #pragma omp parallel for shared(arr, sorted)
        for (int i = 1; i < n - 1; i += 2) {

            // Swap if needed
            if (arr[i] > arr[i + 1]) {

                swap(arr[i], arr[i + 1]);

                sorted = false;
            }
        }
    }
}



// ================= MERGE SORT =================

// Merge Function
void merge(vector<int>& arr, int start, int mid, int end) {

    // Left half array
    vector<int> left(arr.begin() + start,
                     arr.begin() + mid + 1);

    // Right half array
    vector<int> right(arr.begin() + mid + 1,
                      arr.begin() + end + 1);

    int i = 0, j = 0, k = start;

    // Compare elements from left and right
    while (i < left.size() && j < right.size()) {

        // Smaller element goes into original array
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        }

        else {
            arr[k++] = right[j++];
        }
    }

    // Copy remaining left elements
    while (i < left.size()) {
        arr[k++] = left[i++];
    }

    // Copy remaining right elements
    while (j < right.size()) {
        arr[k++] = right[j++];
    }
}


// Sequential Merge Sort
void merge_sort(vector<int>& arr, int start, int end) {

    // Continue if more than one element
    if (start < end) {

        // Find middle
        int mid = (start + end) / 2;

        // Sort left half
        merge_sort(arr, start, mid);

        // Sort right half
        merge_sort(arr, mid + 1, end);

        // Merge sorted halves
        merge(arr, start, mid, end);
    }
}


// Parallel Merge Sort
void parallel_merge_sort(vector<int>& arr,
                         int start,
                         int end) {

    // Continue if valid range
    if (start < end) {

        // Find middle index
        int mid = (start + end) / 2;

        // Run left and right recursively in parallel
        #pragma omp parallel sections
        {

            // Left section
            #pragma omp section
            parallel_merge_sort(arr, start, mid);

            // Right section
            #pragma omp section
            parallel_merge_sort(arr, mid + 1, end);
        }

        // Merge sorted halves
        merge(arr, start, mid, end);
    }
}


int main() {

    // ===== RANDOM INPUT =====

    int n = 100;

    // Create array
    vector<int> arr(n);

    // Generate random numbers
    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }



    /*
    ===== MANUAL INPUT =====

    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";

    for(int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    */


    // Copy original array
    vector<int> arr_copy = arr;

    // Variables for timing
    clock_t start, end;



    // ===== PRINT ORIGINAL ARRAY =====

    cout << "Original Array:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\n\n";



    // ================= BUBBLE SORT =================

    // Start timer
    start = clock();

    // Sequential Bubble Sort
    bubble_sort(arr, n);

    // Stop timer
    end = clock();

    // Calculate execution time
    double bubble_seq =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    // Print sorted array
    cout << "Sequential Bubble Sort Output:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    // Print time
    cout << "\nTime Taken : "
         << bubble_seq << " ms\n\n";


    // ===== Parallel Bubble Sort =====

    // Restore original array
    arr = arr_copy;

    start = clock();

    parallel_bubble_sort(arr, n);

    end = clock();

    double bubble_par =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel Bubble Sort Output:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << bubble_par << " ms\n";

    // Speedup formula
    cout << "Bubble Speedup : "
         << bubble_seq / bubble_par << "\n\n";



    // ================= MERGE SORT =================

    // Restore original array
    arr = arr_copy;

    // Sequential Merge Sort
    start = clock();

    merge_sort(arr, 0, n - 1);

    end = clock();

    double merge_seq =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential Merge Sort Output:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << merge_seq << " ms\n\n";


    // ===== Parallel Merge Sort =====

    // Restore original array
    arr = arr_copy;

    start = clock();

    parallel_merge_sort(arr, 0, n - 1);

    end = clock();

    double merge_par =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel Merge Sort Output:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << merge_par << " ms\n";

    // Print speedup
    cout << "Merge Speedup : "
         << merge_seq / merge_par << endl;

    return 0;
}