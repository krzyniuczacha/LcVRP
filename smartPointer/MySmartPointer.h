#pragma once
#include <cstddef>
#include "RefCounter.h"

template<typename T>
class MySmartPointer {
private:
    RefCounter* counter;
    T* pointer;
public:
    MySmartPointer(T* ptr) {
        pointer = ptr;
        counter = new RefCounter();
        counter->add();
    }
    MySmartPointer(const MySmartPointer& other) {
        pointer = other.pointer;
        counter = other.counter;
        counter->add();
    }
    ~MySmartPointer() {
        if (counter->sub() == 0) {
            delete pointer;
            delete counter;
        }
    }
    T& operator*() { return (*pointer); }
    T* operator->() { return (pointer); }

    MySmartPointer& operator=(const MySmartPointer& other) {
        if (this == &other) return *this;

        if (counter != nullptr && counter->sub() == 0) {
            delete pointer;
            delete counter;
        }

        pointer = other.pointer;
        counter = other.counter;
        if (counter != nullptr) counter->add();

        return *this;
    }

    int getCounter() {
        return counter->get();
    }

    T* getPtr() {return pointer; }

};
