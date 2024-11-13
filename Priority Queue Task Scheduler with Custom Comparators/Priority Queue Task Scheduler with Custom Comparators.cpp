#include <iostream>
#include <queue>
#include <vector>
#include <functional>

struct Task {
    int id;
    int priority;

    Task(int i, int p) : id(i), priority(p) {}
};

// Custom comparator to prioritize higher priority tasks
struct TaskComparator {
    bool operator()(const Task& t1, const Task& t2) {
        return t1.priority < t2.priority; // Higher priority comes first
    }
};

class TaskScheduler {
public:
    void addTask(int id, int priority) {
        taskQueue.emplace(id, priority);
    }

    void processTasks() {
        while (!taskQueue.empty()) {
            Task currentTask = taskQueue.top();
            taskQueue.pop();
            std::cout << "Processing Task ID: " << currentTask.id 
                      << " with priority: " << currentTask.priority << std::endl;
        }
    }

private:
    std::priority_queue<Task, std::vector<Task>, TaskComparator> taskQueue;
};

int main() {
    TaskScheduler scheduler;
    scheduler.addTask(1, 5);  // Task ID 1, priority 5
    scheduler.addTask(2, 2);  // Task ID 2, priority 2
    scheduler.addTask(3, 8);  // Task ID 3, priority 8
    scheduler.addTask(4, 1);  // Task ID 4, priority 1

    std::cout << "Processing tasks in order of priority:\n";
    scheduler.processTasks();

    return 0;
}
