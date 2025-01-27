#include<bits/stdc++.h>
#include<mutex>

using namespace std;

template<typename K, typename V>
class KeyValueStore {
private:
    unordered_map<K, V> keyValue;
    mutex mtx;
public:
    KeyValueStore() {}

    optional<V> get(const K& key) {
        unique_lock<mutex> lock(mtx);
        if(keyValue.find(key) != keyValue.end()) return keyValue[key];
        return nullopt;
    }

    void put(const K& key, const V& value) {
        unique_lock<mutex> lock(mtx);
        keyValue[key] = value;
    }

    void remove(const K& key) {
        unique_lock<mutex> lock(mtx);
        keyValue.erase(key);
    }
};

int main() {
    KeyValueStore<int, string> kv;
    kv.put(1,"Hello");
    if (auto value = kv.get(1)) {
        std::cout << "Value for key 1: " << *value << std::endl;
    } else {
        std::cout << "'key1' not found!" << std::endl;
    }

    if (auto value = kv.get(2)) {
        std::cout << "Value for key 2: " << *value << std::endl;
    } else {
        std::cout << "key 2 is not found!" << std::endl;
    }

    kv.remove(1);

    KeyValueStore<int, string> kv2;
    kv2.put(1, "value1");
    if (auto value = kv2.get(1)) {
        std::cout << "Value for key 1: " << *value << std::endl;
    } else {
        std::cout << "Key 1 not found!" << std::endl;
    }
    return 0;
}