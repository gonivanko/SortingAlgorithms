#include "numbersarray.h"

NumbersArrray::NumbersArrray()
{
    n = 0; max_capacity = 1;
    numbers = new int[1];
}

NumbersArrray::NumbersArrray(int n1)
{
    n = 0;
    max_capacity = n1;
    numbers = new int[n1];
}

NumbersArrray::NumbersArrray(NumbersArrray& other)
{
    n = other.n;
    max_capacity = other.max_capacity;
    numbers = new int[other.n];
    for (int i = 0; i < n; i++)
    {
        numbers[i] = other[i];
    }
}

void NumbersArrray::doubleCapacity()
{
    max_capacity *= 2;
    int* temporary_array = new int[max_capacity];
    for (int i = 0; i < n; i++)
    {
        temporary_array[i] = numbers[i];
    }
    delete[] numbers;
    numbers = temporary_array;
}

int NumbersArrray::length()
{
    return n;
}

bool NumbersArrray::isEmpty()
{
    return (n == 0);
}

void NumbersArrray::pushBack(int value)
{
    if (n == max_capacity)
    {
        doubleCapacity();
    }

    numbers[n] = value;
    n++;
}


int& NumbersArrray::operator[](int index)
{
    return numbers[index];
}

NumbersArrray NumbersArrray::operator=(const NumbersArrray& other)
{
    if (other.max_capacity > max_capacity)
    {
        max_capacity = other.max_capacity;
        delete[] numbers;
        numbers = new int[max_capacity];
    }
    n = other.n;
    for (int i = 0; i < n; i++)
    {
        numbers[i] = other.numbers[i];
    }
    return *this;
}

QString NumbersArrray::getString()
{
    QString s;
    for (int i = 0; i < n; i++) s += QString::number(numbers[i]) + " ";
    return s;
}


