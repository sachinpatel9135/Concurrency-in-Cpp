#include<iostream>
#include<mutex>
#include<thread>

using namespace std;

int sharedCounter = 0;
mutex mutexLock;

void increamentCounter(int iterations) {
    for(int i=0; i<iterations; i++) {
        lock_guard<mutex> lock(mutexLock);
        sharedCounter++;
    }
}

int main() {
    const int iterations = 1000;
    thread t1(increamentCounter,iterations);
    thread t2(increamentCounter, iterations);
    t1.join();
    t2.join();
    cout << sharedCounter;
    return 0;
}