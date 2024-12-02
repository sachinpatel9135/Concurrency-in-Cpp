#include<iostream>
#include<thread>
#include<chrono>

using namespace std;

class Sleeper{
public:
    Sleeper(int& i_): i{i_} {};
    void operator() (int k) {
        for(int j=0; j<=k; j++) {
            this_thread::sleep_for(chrono::microseconds(100));
            i += k;
        }
        cout << this_thread::get_id() << "\n";
    }
private:
    int& i;
};

int main() {
    int valSleeper = 1000;
    thread t1(Sleeper(valSleeper),5);
    t1.join();
    cout << valSleeper << "\n";
    return 0;
}