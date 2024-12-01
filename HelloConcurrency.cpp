#include<iostream>
#include<thread>

using namespace std;

void hello() {
    cout << "Hello concurrency world!";
}

int32_t main()
{
    thread t(hello);
    t.join();
    return 0;
}