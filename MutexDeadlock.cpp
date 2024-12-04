#include<iostream>
#include<mutex>
#include<chrono>
#include<thread>

using namespace std;

class CriticalData {
public:
    mutex mutexLock;
};

void deadlock(CriticalData& a, CriticalData& b) {
    a.mutexLock.lock();
    cout << "get the first mutex\n";
    this_thread::sleep_for(chrono::milliseconds(1));
    b.mutexLock.lock();
    cout << "get the second mutex\n";
    a.mutexLock.unlock();
    b.mutexLock.unlock();
}

int main() {
    CriticalData c1;
    CriticalData c2;
    thread t1([&]{deadlock(c1,c2);});
    thread t2([&]{deadlock(c2,c1);});
    t1.join();
    t2.join();
    return 0;
}