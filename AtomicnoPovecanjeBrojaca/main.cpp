#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> counter(0);

void incrementCounter(int id, int iterations) {
    for (int i = 0; i < iterations; i++) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    const int numThreads = 4;
    const int iterationsPerThread = 1000000;

    std::thread threads[numThreads];

    for (int i = 0; i < numThreads; i++) {
        threads[i] = std::thread(incrementCounter, i, iterationsPerThread);
    }

    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }

    std::cout << "Konačni brojač: " << counter.load(std::memory_order_relaxed) << std::endl;

    return 0;
}
