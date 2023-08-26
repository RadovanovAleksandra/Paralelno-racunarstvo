#include <iostream>
#include <vector>
#include <future>
#include <algorithm>
#include <atomic>

int main() {
    int n;
    std::cout << "Unesite broj elemenata niza: ";
    std::cin >> n;

    if (n <= 4) {
        std::cerr << "Niz mora imati više od 4 elementa." << std::endl;
        return 1;
    }

    // Unos niza
    std::vector<int> arr(n);
    std::cout << "Unesite elemente niza:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    // Deljenje niza na četiri dela
    int quarter_size = n / 4;

    // Promenljive za čuvanje lokalnih minimuma i sume
    std::vector<std::future<int>> min_futures;
    std::atomic<int> sum(0);

    // Asinhrone niti za pronalaženje minimuma i računanje sume
    for (int i = 0; i < 4; i++) {
        min_futures.push_back(std::async([&arr, i, quarter_size, &sum] {
            int local_min = arr[i * quarter_size];
            int local_sum = 0;

            for (int j = i * quarter_size; j < (i + 1) * quarter_size; j++) {
                local_sum += arr[j];
                if (arr[j] < local_min) {
                    local_min = arr[j];
                }
            }

            // Atomically dodajemo lokalnu sumu na glavnu sumu
            sum += local_sum;

            return local_min;
        }));
    }

    // Asinhrone niti za ispisivanje lokalnih minimuma u obrnutom redosledu
    for (int i = 3; i >= 0; i--) {
        std::async([i, &min_futures] {
            int local_min = min_futures[i].get();
            std::cout << "Lokalni minimum #" << (3 - i) << ": " << local_min << std::endl;
        });
    }

    // Čekamo sve niti da završe
    for (auto& future : min_futures) {
        future.wait();
    }

    // Ispisivanje ukupne sume
    std::cout << "Suma svih elemenata niza: " << sum << std::endl;

    return 0;
}
