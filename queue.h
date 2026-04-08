#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

using namespace std;

class ThreadSafeQueue {
public:
    explicit ThreadSafeQueue(size_t capacity);

    void push(const string& message);
    string pop();

private:
    queue<string> q;
    size_t capacity;
    mutex mtx;
    condition_variable cv_not_full;
    condition_variable cv_not_empty;
};

inline ThreadSafeQueue::ThreadSafeQueue(size_t capacity) : capacity(capacity) {}

inline void ThreadSafeQueue::push(const string& message) {
    unique_lock<mutex> lock(mtx);
    cv_not_full.wait(lock, [this]() { 
        return q.size() < capacity; 
    });
    q.push(message);
    cv_not_empty.notify_one();
}

inline string ThreadSafeQueue::pop() {
    unique_lock<mutex> lock(mtx);
    cv_not_empty.wait(lock, [this]() { 
        return !q.empty(); 
    });
    string message = q.front();
    q.pop();
    cv_not_full.notify_one();
    return message;
}
