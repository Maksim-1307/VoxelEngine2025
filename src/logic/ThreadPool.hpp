#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <iostream>

class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;

    ThreadPool() {

        size_t num_threads = std::max(1u, std::thread::hardware_concurrency() - 1);

        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock lock(mtx);
                        cv.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (auto& w : workers)
            if (w.joinable()) w.join();
    }

public:

    static ThreadPool& get_instance() {
        static ThreadPool instance;
        return instance;
    }

    void enqueue(std::function<void()> f) {
        {
            std::lock_guard lock(mtx);
            tasks.push(std::move(f));
        }
        cv.notify_one();
    }
};