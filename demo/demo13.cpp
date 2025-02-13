#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// 共享的整数值
int sharedNumber = 0;
// 用于保护共享资源的互斥锁
std::mutex numberMutex;
// 条件变量，用于线程间的同步
std::condition_variable cv;
// 计数器，记录已完成加法操作的线程数量
int threadsCompleted = 0;
// 总共需要完成加法操作的线程数量
const int totalThreadsToComplete = 2;

// 负责加值的线程函数
void incrementNumber() {
    for (int i = 0; i < 1000; ++i) {
        // 加锁，保护共享资源
        std::lock_guard<std::mutex> lock(numberMutex);
        sharedNumber++;
    }

    // 加锁，准备修改共享的完成线程计数器并通知条件变量
    std::unique_lock<std::mutex> lock(numberMutex);
    threadsCompleted++;
    std::cout << "Thread " << std::this_thread::get_id() << " completed increment operation, threads completed: " << threadsCompleted << std::endl;

    // 当完成加值操作的线程数量达到总数时，通知等待的线程
    if (threadsCompleted == totalThreadsToComplete) {
        cv.notify_one(); // 唤醒一个等待在条件变量 cv 上的线程 (即线程 3)
    }
    // unique_lock 在此作用域结束时自动释放锁
}

// 负责输出数值的线程函数
void outputNumber() {
    // 加锁，准备等待条件变量被通知
    std::unique_lock<std::mutex> lock(numberMutex);
    std::cout << "Thread " << std::this_thread::get_id() << " waiting for other threads to complete..." << std::endl;

    // 等待条件变量被通知，只有当 threadsCompleted == totalThreadsToComplete 时才会被唤醒
    cv.wait(lock, []{ return threadsCompleted == totalThreadsToComplete; });

    // 当被唤醒时，说明两个加值线程已完成，可以安全地访问 sharedNumber
    std::cout << "Thread " << std::this_thread::get_id() << " woken up, outputting final value: " << sharedNumber << std::endl;
    // unique_lock 在此作用域结束时自动释放锁
}

int main() {
    std::cout << "Main thread " << std::this_thread::get_id() << " started" << std::endl;

    // 创建两个负责加值的线程
    std::thread thread1(incrementNumber);
    std::thread thread2(incrementNumber);

    // 创建负责输出数值的线程
    std::thread thread3(outputNumber);

    // 等待所有线程完成
    thread1.join();
    thread2.join();
    thread3.join();

    std::cout << "Main thread " << std::this_thread::get_id() << " finished" << std::endl;
    return 0;
}