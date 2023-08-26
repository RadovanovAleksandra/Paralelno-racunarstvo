#include <iostream>
#include <future>

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }

    std::future<int> fib1 = std::async(fibonacci, n - 1);

    int fib2 = fibonacci(n - 2);

    return fib1.get() + fib2;
}

int main() {
    int n;
    std::cout << "Unesite broj elemenata Fibonačijevog niza: ";
    std::cin >> n;

    int result = fibonacci(n);

    std::cout << "Fibonačijev broj #" << n << " je " << result << std::endl;

    return 0;
}
