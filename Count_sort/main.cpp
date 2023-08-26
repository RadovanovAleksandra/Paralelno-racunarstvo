#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

void count_sort(std::vector<int>& arr, std::vector<int>& out) {
    const int max_value = 100;
    std::vector<int> count(max_value + 1, 0);

    for (int i = 0; i < arr.size(); i++) {
        count[arr[i]]++;
    }

    for (int i = 1; i <= max_value; i++) {
        count[i] += count[i - 1];
    }

    for (int i = arr.size() - 1; i >= 0; i--) {
        out[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
}

void count_sort_openmp(std::vector<int>& arr, std::vector<int>& out) {
    const int max_value = 100;
    std::vector<int> count(max_value + 1, 0);

    #pragma omp parallel for
    for (int i = 0; i < arr.size(); i++) {
        #pragma omp atomic
        count[arr[i]]++;
    }

    #pragma omp parallel for
    for (int i = 1; i <= max_value; i++) {
        #pragma omp atomic
        count[i] += count[i - 1];
    }

    #pragma omp parallel for
    for (int i = arr.size() - 1; i >= 0; i--) {
        int position;
        #pragma omp atomic capture
        {
            position = count[arr[i]] - 1;
            count[arr[i]]--;
        }
        out[position] = arr[i];
    }
}

int main() {
    std::vector<int> arr = {4, 2, 2, 8, 3, 3, 1};
    std::vector<int> out(arr.size());

    count_sort(arr, out);
    std::cout << "Sekvencijalno sortiran niz: ";
    for (int num : out) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::fill(out.begin(), out.end(), 0);

    count_sort_openmp(arr, out);
    std::cout << "Paralelno sortiran niz: ";
    for (int num : out) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
