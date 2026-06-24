#include "../../include/utils/thread_pool.hpp"
#include<iostream>
#include<stdexcept>
#include<utility>


//workers_ -> vector of thread. tasks_ ->queue of tasks and each task is a function

ThreadPool:: ThreadPool(std::size_t thread_count){
    if(thread_count==0){
        thread_count=1;
    }
    for(int i=0;i<thread_count;++i){
        workers_.emplace_back([this](){
            worker_loop();
        });
    }
};

ThreadPool::~ThreadPool(){
    {
        //creadting a small block
        std::lock_guard<std::mutex>lock(mutex_);
        //its scope is only till this block
        stopping_=true;
    }
    condition_.notify_all();
    for(std::thread &worker:workers_){
        if(worker.joinable()){
            worker.join();
        }
    }
}

void ThreadPool::submit(std::function<void()>task){
    {
        std::lock_guard<std::mutex>lock(mutex_);
        if(stopping_){
            throw std::runtime_error("cannot submit task to stopped ThreadPool");
            //this might crash the program? if so what's the solution.
        }
        tasks_.push(std::move(task));
    }
    condition_.notify_one();
}

void ThreadPool::worker_loop(){
    while(true){
        std::function<void()>task;
        {
            std::unique_lock<std::mutex>lock(mutex_);
            
            condition_.wait(lock,[this](){
                return stopping_ || !tasks_.empty();
            });
            if(stopping_){
                return;
            }
            task=std::move(tasks_.front());
            tasks_.pop();
        }
        try{
            task();
        }catch(const std::exception& e){
            std::cerr<<"Worker task failed: "<<e.what()<<"\n";
            //learn exception handling properly in c++.
            //learn about throw and runtime errors.
            //learn bit more properly how move works
            //learn more about conditional variable.
        }
    }
}