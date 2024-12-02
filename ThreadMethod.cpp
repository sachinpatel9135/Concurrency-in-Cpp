#include<iostream>
#include<thread>

using namespace std;

int main() {
    cout << boolalpha << endl;

    cout << "Hardware concurrency: " << thread::hardware_concurrency() << "\n";
    thread t1([] {cout << "Thread id of t1 thread: " << this_thread::get_id() << "\n";});
    thread t2([] {cout << "Thread id of t2 thread: " << this_thread::get_id() << "\n";});

    cout << "\n";

    cout << "From main thread, t1 id: " << t1.get_id() << "\n";
    cout << "From main thread, t2 id: " << t2.get_id() << "\n";
    cout << "\n";

    swap(t1, t2);
    cout << "From main thread, t1 id: " << t1.get_id() << "\n";
    cout << "From main thread, t2 id: " << t2.get_id() << "\n";
    cout << "\n";

    cout << "FROM MAIN: id of main= " << this_thread::get_id() << endl;
    cout << "\n";

    cout << "t1.joinable(): " << t1.joinable() << endl;
    cout << endl;

    t1.join();
    t2.join();
    cout << "\n";

    cout << "t2.joinable(): " << t2.joinable() << endl;
    cout << endl;
    
    return 0;
}