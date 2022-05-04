/*
 * @Author: Limer
 * @Date: 2022-04-16 21:57:03
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-24 19:21:06
 * @Description:
 */
#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false) {
    // start 'size' threads
    for (int i = 0; i < size; ++i) {
        threads.emplace_back(std::thread([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    // Wait for the condition variable if the task queue is not
                    // empty or the thread pool has stopped
                    // ! if the stop or 'tasks' queue empty, the thread will be
                    // ! hang up.
                    cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                    // If the task queue is empty and the thread poll has
                    // stopped, the thread will be end.
                    if (stop && tasks.empty()) return;
                    task = tasks.front();
                    tasks.pop();
                }  // Variable 'lock' will be destructed, the tasks_mtx will be
                   // unlocked.
                task();  // implement task
            }
        }));
    }
}
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all();
    for (auto& th : threads) {
        if (th.joinable()) th.join();
    }
}

// void ThreadPoll::add(std::function<void()> func) {
//     {
//         std::unique_lock<std::mutex> lock(tasks_mtx);
//         if (stop)
//             throw std::runtime_error(
//                 "ThreadPoll already stop, can't add task any more!");
//         tasks.emplace(func);
//     }  // Variable 'lock' will be destructed, the tasks_mtx will be
//        // unlocked.
//     cv.notify_one();  // notify condition variable one time.
// }