#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <forward_list>

constexpr int kMax = 30;
std::forward_list<int> g_list;
std::mutex g_lock;

void WriterThread()
{
    for (int i = 0; i <= kMax; ++i) {
        {
            std::lock_guard<std::mutex> lock(g_lock);

            g_list.push_front(i);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void ReaderThread()
{
    while (true) {
        int number = 0;

        {
            std::lock_guard<std::mutex> lock(g_lock);

            if (g_list.empty()) continue;

            number = g_list.front();

            g_list.pop_front();
        }

        if (number == kMax) return;

        std::cout << number << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

int main()
{
    std::thread writer(WriterThread);
    std::thread reader(ReaderThread);

    writer.join();
    reader.join();
}