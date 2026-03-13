#ifndef MYSMARTPOINTER_H
#define MYSMARTPOINTER_H
#pragma once

#include <cstddef>
#include "RefCounter.h"

template<typename T>
class MySmartPointer {
private:
    RefCounter* counter_;
    T* pointer_;

public:
    MySmartPointer();
    MySmartPointer(T* ptr);
    MySmartPointer(const MySmartPointer& other);
    ~MySmartPointer();

    T& operator*();
    T* operator->();
    MySmartPointer& operator=(const MySmartPointer& other);

    int getCounter() const;
    T* getPtr() const;
};

template<typename T>
MySmartPointer<T>::MySmartPointer()
    : pointer_(nullptr),
      counter_(nullptr) {}

template<typename T>
MySmartPointer<T>::MySmartPointer(T* ptr)
    : pointer_(ptr),
      counter_(nullptr) {
    if (pointer_ != nullptr) {
        counter_ = new RefCounter();
        counter_->add();
    }
}

template<typename T>
MySmartPointer<T>::MySmartPointer(const MySmartPointer& other)
    : pointer_(other.pointer_),
      counter_(other.counter_) {
    if (counter_ != nullptr) {
        counter_->add();
    }
}

template<typename T>
MySmartPointer<T>::~MySmartPointer() {
    if (counter_ != nullptr && counter_->sub() == 0) {
        delete pointer_;
        delete counter_;
        pointer_ = nullptr;
        counter_ = nullptr;
    }
}

template<typename T>
T& MySmartPointer<T>::operator*() {
    return (*pointer_);
}

template<typename T>
T* MySmartPointer<T>::operator->() {
    return pointer_;
}

template<typename T>
MySmartPointer<T>& MySmartPointer<T>::operator=(const MySmartPointer& other) {
    if (this == &other) {
        return *this;
    }

    if (counter_ != nullptr && counter_->sub() == 0) {
        delete pointer_;
        delete counter_;
    }

    pointer_ = other.pointer_;
    counter_ = other.counter_;

    if (counter_ != nullptr) {
        counter_->add();
    }

    return *this;
}

template<typename T>
int MySmartPointer<T>::getCounter() const {
    if (counter_ == nullptr) {
        return 0;
    }
    return counter_->get();
}

template<typename T>
T* MySmartPointer<T>::getPtr() const {
    return pointer_;
}

#endif //MYSMARTPOINTER_H