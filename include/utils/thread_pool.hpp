#pragma once

#include<condition_variable>
#include<mutex>
#include<thread>
#include<functional>
#include<queue>
#include<vector>


class ThreadPool{
    public:
    explicit ThreadPool(std::size_t thread_count);
    ~ThreadPool();

    ThreadPool(const ThreadPool&)=delete;
    ThreadPool& operator=(const ThreadPool&)=delete;
    void submit(std::function<void()> task);
    private:
    void worker_loop();
    std::vector<std::thread>workers_;
    std::queue<std::function<void()>>tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stopping_ =false;
};