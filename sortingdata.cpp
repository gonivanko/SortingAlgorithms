#include "sortingdata.h"

SortingData::SortingData()
{
    swaps = 0; comparisons = 0; sorting_time = 0.0;
    n = 100; sorting_method = 0;
}

int SortingData::getN()
{
    return n;
}

QString SortingData::getString()
{
    QString result;
    result += "Кількість чисел (n): " + QString::number(n) + "\n";
    result += "Метод сортування: " + getSortingMethod() + "\n";
    result += "Кількість порівнянь: " + QString::number(comparisons) + "\n";
    result += "Кількість перестановок: " + QString::number(swaps) + "\n";
    result += "Час сортування: " + QString::number(sorting_time) + " мкс";
    return result;
}

QString SortingData::getSortingMethod()
{
    switch (sorting_method) {
    case 0:
        return "злиттям (Д. фон Неймана)";
        break;
    case 1:
        return "швидке сортування";
        break;
    case 2:
        return "інтроспективне сортування";
        break;
    default:
        break;
    }
}

int SortingData::getComparisons()
{
    return comparisons;
}


int SortingData::getSwaps()
{
    return swaps;
}

int SortingData::getTime()
{
    return sorting_time;
}

int SortingData::method()
{
    return sorting_method;
}

void SortingData::SetComparisons(int new_comparisons)
{
    comparisons = new_comparisons;
}
void SortingData::setSwaps(int new_swaps)
{
    swaps = new_swaps;
}

void SortingData::SetSortingTime(double sorting_time1)
{
    sorting_time = sorting_time1;
}

void SortingData::setN(int new_number)
{
    n = new_number;
}

void SortingData::setSortingMethod(int method_number)
{
    sorting_method = method_number;
}

void SortingData::addComparisons(int number)
{
    comparisons += number;
}

void SortingData::addSwaps(int number)
{
    swaps += number;
}

void SortingData::addTime(double time_in_ms)
{
    sorting_time += time_in_ms;
}

void SortingData::reset()
{
    swaps = 0; comparisons = 0; sorting_time = 0;
}

