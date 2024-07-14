#ifndef NUMBERSARRAY_H
#define NUMBERSARRAY_H
#include "mainwindow.h"

class NumbersArrray
{
    int n, max_capacity;
    int* numbers;
public:

    NumbersArrray();
    NumbersArrray(int n1);
    NumbersArrray(NumbersArrray& other);
    int length();
    QString getString();
    void pushBack(int value);
    void popBack();
    void doubleCapacity();
    bool isEmpty();

    int& operator[] (int index);
    NumbersArrray operator=(const NumbersArrray& other);
};

#endif // NUMBERSARRAY_H
