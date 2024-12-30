#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

void safePrint(const string& message) {
    mutex mutLock;
    lock_guard<mutex> lock(mutLock);
    cout << message << "\n";
}

void threadFunction(int id) {
    safePrint("Thread with id: " + to_string(id) + " is running!");
}

int main() {
    thread t1(threadFunction, 1);
    thread t2(threadFunction, 2);
    t1.join();
    t2.join();
    return 0;
}