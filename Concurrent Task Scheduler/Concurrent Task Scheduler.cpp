#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <atomic>
#include <semaphore>

std::binary_semaphore semaphore(3); // Limit the number of concurrent tasks to 3

class TaskScheduler {
public:
    TaskScheduler(size_t numThreads) : stop(false) {
        // Initialize worker threads
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    // Acquire semaphore before processing
                    semaphore.acquire();
                    task();
                    semaphore.release();
                }
            });
        }
    }

    // Add new task to the queue
    void enqueueTask(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

    // Stop the scheduler and join threads
    void stopScheduler() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            if (worker.joinable()) worker.join();
        }
    }

    ~TaskScheduler() {
        stopScheduler();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

int main() {
    // Create a scheduler with 5 threads
    TaskScheduler scheduler(5);

    // Simulate 10 tasks
    for (int i = 0; i < 10; ++i) {
        scheduler.enqueueTask([i] {
            std::cout << "Processing task " << i << " on thread " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate task duration
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(10)); // Wait for tasks to complete
    scheduler.stopScheduler();
    return 0;
}
