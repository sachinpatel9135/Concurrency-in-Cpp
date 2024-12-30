#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>

using namespace std;

class MessageQueue {
private:
    queue<string> q;
    mutex mtx;
    condition_variable conditionVariable;
public:
    void enqueue(string message) {
        lock_guard<mutex> lock(mtx);
        q.push(message);
        conditionVariable.notify_one(); //notify a consumer
    }

    string dequeue() {
        unique_lock<mutex> lock(mtx);
        conditionVariable.wait(lock, [this] {return !q.empty();}); //wait for queue to have messages
        string message = q.front();
        q.pop();
        return message;
    }
};

class Consumer {
private:
    MessageQueue& queue;
    atomic<bool>& done;
public:
    Consumer(MessageQueue& q, atomic<bool>& flag): queue(q),done(flag) {}

    void operator()() {
        while (!done) {
            string message = queue.dequeue();
            cout << "Processing message: " << message << std::endl;
        }
    }
};

class Producer {
private:
    MessageQueue& queue;
public:
    Producer(MessageQueue& q): queue(q) {}

    void produce(const string& message) {
        queue.enqueue(message);
        std::cout << "Produced message: " << message << std::endl;
    }
};

int main() {
    MessageQueue queue;
    atomic<bool> done(false);

    //create a thread pool of consumers
    vector<thread> consumerThreads;
    int numConsumers = 3;
    for(int i=0; i<3; i++) {
        consumerThreads.push_back(thread(Consumer(queue, done)));
    }

    //create producer thread
    Producer producer(queue);
    thread producerThread([&producer]() {
        for(int i=0; i<10; i++) {
            producer.produce("Message " + to_string(i));
            this_thread::sleep_for(chrono::microseconds(100));
        }
    });

    producerThread.join(); //wait for producer to finish producing
    done = true; //signal consumers to start consuming

    for(auto& t: consumerThreads) {
        t.join(); // Wait for consumers to finish processing
    }
    return 0;
}