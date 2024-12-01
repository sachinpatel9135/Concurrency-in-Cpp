#include<iostream>
#include<thread>

using namespace std;

int main() {
    string s = "C++11";
    thread t1([=] {cout << s << "\n";});
    t1.join();
    thread t2([&] {cout << s << "\n";});
    t2.join();
    return 0;
}