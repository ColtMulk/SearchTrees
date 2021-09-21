#include <iostream>
#include "MapTree.h"
#include "MapTree2_4.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

struct lexiographicOrder {
    bool operator()(const string a, const string b) const {
        return a < b;
    }
};

void treeMapTest() {
    MapTree<string, string, lexiographicOrder> treeTest;
    treeTest.insert("HAHAHA", "string");
    treeTest.insert("A", "A");
    treeTest.insert("B", "B");
    treeTest.insert("Z", "Z");
    treeTest.insert("X", "X");
    treeTest.insert("Y", "Y");
    cout << "Size: " << treeTest.size() << " IsEmpty: " << treeTest.empty() << endl;
    pair<string,string> *val = treeTest.search("HAHAHA");
    cout << "Key: " << val->first << " Value: " << val->second << endl;

    treeTest.erase("HAHAHA");
    val = treeTest.search("HAHAHA");
    cout << "Val: " << val << endl;
    val = treeTest.search("Z");
    cout << "Key: " << val->first << " Value: " << val->second << endl;
    treeTest.erase("A");
    treeTest.erase("B");
    treeTest.erase("Z");
    treeTest.erase("X");
    treeTest.erase("Y");
    //cout << "Size: " << treeTest.size() << " IsEmpty: " << treeTest.empty() << endl;
};

void MapTree2_4Test(){
    MapTree2_4<string,string,lexiographicOrder> test;
    test.insert("HAHAH", "HAHAH");
    pair<string, string>* value = test.search("HAHAH");
    cout << value->first << " Value: " << value->second << endl;
    test.insert("A", "A");
    test.insert("Z", "Z");
    test.insert("B", "B");
    test.insert("Y", "Y");
    test.print();
    test.insert("D", "D");
    test.insert("T", "T");
    test.insert("L", "L");
    test.print();
    test.insert("X", "X");
    test.insert("M", "M");
    test.print();
    value = test.search("Y");
    cout << value->first << " Value: " << value->second << endl;
    value = test.search("D");
    cout << value->first << " Value: " << value->second << endl;
    value = test.search("Z");
    cout << value->first << " Value: " << value->second << endl;
    cout << test.size() << endl;
    test.erase("X");
    cout << "Erase 1...complete" << endl;
    test.print();
    test.erase("Z");
    cout << "Erase 2...complete" << endl;
    test.print();
    test.erase("M");
    cout << "Erase 3...complete" << endl;
    test.print();
    test.erase("Y");
    test.erase("T");
    test.print();
    test.erase("D");
    test.erase("HAHAH");
    test.print();
    test.erase("L");
    test.erase("A");
    test.erase("B");
    test.print();
    cout << "Erase 4...complete" << endl;
    cout << test.size() << endl;

}

void random2_4(int n){
    int randomNums[n];
    MapTree2_4<int,int> insertions24;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < n; i++){
        randomNums[i] = rand()%(n*100);
        while(insertions24.search(randomNums[i]) != nullptr){
            randomNums[i] = rand()%(n*100);
        }
        insertions24.insert(randomNums[i],randomNums[i]);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time = duration_cast<duration<double>>(t2 - t1);
    cout << time.count()*1000 << ",";
    for(int i = 0; i < n; i++){
        insertions24.erase(randomNums[i]);
    }
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    time = duration_cast<duration<double>>(t3-t2);
    cout << time.count()*1000 << endl;
}

void forward2_4(int n){
    MapTree2_4<int,int> insertions24;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < n; i++){
        insertions24.insert(i,i);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time = duration_cast<duration<double>>(t2 - t1);
    cout << time.count()*1000 << ",";
    for(int i = 0; i < n; i++){
        insertions24.erase(i);
    }
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    time = duration_cast<duration<double>>(t3-t2);
    cout << time.count()*1000 << endl;
}

void backwards2_4(int n){
    MapTree2_4<int,int> insertions24;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < n; i++){
        insertions24.insert(n-i,n-i);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time = duration_cast<duration<double>>(t2 - t1);
    cout << time.count()*1000 << ",";
    for(int i = 0; i < n; i++){
        insertions24.erase(n-i);
    }
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    time = duration_cast<duration<double>>(t3-t2);
    cout << time.count()*1000 << endl;
}

void randomMap(int n){
    MapTree<int, int> insertions24;
    int randomNums[n];
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < n; i++){
        randomNums[i] = rand()%(n*100);
        while(insertions24.search(randomNums[i]) != nullptr){
            randomNums[i] = rand()%(n*100);
        }
        insertions24.insert(randomNums[i],randomNums[i]);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time = duration_cast<duration<double>>(t2 - t1);
    cout << time.count()*1000 << ",";
    for(int i = 0; i < n; i++){
        insertions24.erase(randomNums[i]);
    }
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    time = duration_cast<duration<double>>(t3-t2);
    cout << time.count()*1000 << endl;
}

void forwardMap(int n){
    MapTree<int, int> insertions24;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < n; i++){
        insertions24.insert(i,i);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time = duration_cast<duration<double>>(t2 - t1);
    cout << time.count()*1000 << ",";
    for(int i = 0; i < n; i++){
        insertions24.erase(i);
    }
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    time = duration_cast<duration<double>>(t3-t2);
    cout << time.count()*1000 << endl;
}

void backwardsMap(int n){
    MapTree<int, int> insertions24;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < n; i++){
        insertions24.insert(n-i,n-i);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time = duration_cast<duration<double>>(t2 - t1);
    cout << time.count()*1000 << ",";
    for(int i = 0; i < n; i++){
        insertions24.erase(n-i);
    }
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    time = duration_cast<duration<double>>(t3-t2);
    cout << time.count()*1000 << endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    //treeMapTest();
    //MapTree2_4Test();


    //n= 100000 takes awhile
    for(int n = 10; n<= 10000; n*=10) {
        cout << n << " ";
        cout << "Two Four Tree" << endl;
        random2_4(n);
        forward2_4(n);
        backwards2_4(n);
        cout << "Map Tree" << endl;
        randomMap(n);
        forwardMap(n);
        backwardsMap(n);
        cout << endl;
    }

    return 0;
}