#include <iostream>
#include <vector>
#include <future>

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int n;
    std::cout << "Unesite broj elemenata Fibonačijevog niza: ";
    std::cin >> n;

    std::vector<std::future<int>> futures;

    for (int i = 0; i < n; ++i) {
        futures.push_back(std::async(std::launch::async, fibonacci, i));
    }

    std::cout << "Prvih " << n << " elemenata Fibonačijevog niza: ";
    for (int i = 0; i < n; ++i) {
        std::cout << futures[i].get() << " ";
    }
    std::cout << std::endl;

    return 0;
}
