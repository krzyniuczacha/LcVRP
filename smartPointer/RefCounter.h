#pragma once
class RefCounter {
private:
    int count;
public:
    RefCounter() {count = 0;}
    int add() {return (++count);}
    int sub() {return (--count);}
    int get() const {return count;}
};

