#include<iostream>
#include<thread>

using namespace std;

void helloFunction() {
    cout << "Hello from a function" << "\n";
}

class HelloFunctionObject{
public:
    void operator ()() const {
        cout << "Hello from a fuction object" << "\n";
    }
};

int main() {
    thread t1(helloFunction);
    HelloFunctionObject HelloFunctionObject;
    thread t2(HelloFunctionObject);
    thread t3([] {
        cout << "Hello from a lambda function" << "\n";
    });
    t1.join();
    t2.join();
    t3.join();
    return 0;
}