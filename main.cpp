#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "queue.h"

static const size_t QUEUE_CAPACITY = 64;
static const size_t NUM_PRODUCERS = 4;
// static const size_t NUM_CONSUMERS = 4;
static const size_t MESSAGES_PER_PRODUCER = 1000;


void producer(ThreadSafeQueue& queue, int producer_id) {
    for (size_t i = 0; i < MESSAGES_PER_PRODUCER; i++) {
        string message = "Producer " + to_string(producer_id) + " - Message " + to_string(i);
        queue.push(message);
    }
}

void consumer(ThreadSafeQueue& queue) {
    int total = NUM_PRODUCERS * MESSAGES_PER_PRODUCER;
    for (size_t i = 0; i < total; ++i) {
        string message = queue.pop();
        cout << message << "\n";
    }
}

int main() {
    ThreadSafeQueue queue(QUEUE_CAPACITY);
    vector<thread> producers;
    thread consumer_thread(consumer, ref(queue));

    // Start producer threads
    for (size_t i = 0; i < NUM_PRODUCERS; i++) {
        producers.emplace_back(producer, ref(queue), i);
    }


    for (auto& p : producers) p.join();
    consumer_thread.join();

    std::cout << "Done. Total messages: " << NUM_PRODUCERS * MESSAGES_PER_PRODUCER << "\n";
    
    return 0;
}