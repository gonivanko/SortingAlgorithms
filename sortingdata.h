#ifndef SORTINGDATA_H
#define SORTINGDATA_H
#include "mainwindow.h"
#include <QString>
#include "numbersarray.h"

class SortingData
{
    int swaps, comparisons, n, sorting_method;
    double sorting_time;

public:
    SortingData();
    QString getString();
    QString getSortingMethod();
    int method();
    int getN();
    int getComparisons();
    int getSwaps();
    int getTime();

    void setSwaps(int new_swaps);
    void SetComparisons(int new_comparisons);
    void SetSortingTime(double sorting_time1);
    void setN(int new_number);
    void setSortingMethod(int method_number);

    void addSwaps(int number);
    void addComparisons(int number);
    void addTime(double time_in_ms);
    void reset();
};

#endif // SORTINGDATA_H
