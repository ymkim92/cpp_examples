#include <iostream>
#include <chrono>
#include <thread>

void f()
{
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    static std::chrono::time_point<std::chrono::system_clock> prev;

    auto diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now-prev);
    std::cout << "diff(ms):" << diff_ms.count() << "\n";
    prev = now;
}

int main()
{
    for (int i=0; i<10; i++)
    {
        f();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}