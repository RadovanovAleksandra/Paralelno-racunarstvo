#include <iostream>
#include <atomic>

std::atomic<int> max_value(0);

void correct_max(int value) {
    int current_max = max_value.load();
    while (value > current_max) {
        if (max_value.compare_exchange_weak(current_max, value)) {
            break;
        }
    }
}

int main() {
    std::thread t1([&] {
        for (int i = 1; i <= 1000; ++i) {
            correct_max(i);
        }
    });

    std::thread t2([&] {
        for (int i = 1001; i <= 2000; ++i) {
            correct_max(i);
        }
    });

    t1.join();
    t2.join();

    std::cout << "Maksimalna vrednost: " << max_value.load() << std::endl;

    return 0;
}
