#ifndef REFCOUNTER_H
#define REFCOUNTER_H
#pragma once

class RefCounter {
private:
    int count_;

public:
    RefCounter();
    int add();
    int sub();
    int get() const;
};

inline RefCounter::RefCounter()
    : count_(0) {}

inline int RefCounter::add() {
    return (++count_);
}

inline int RefCounter::sub() {
    return (--count_);
}

inline int RefCounter::get() const {
    return count_;
}

#endif //REFCOUNTER_H