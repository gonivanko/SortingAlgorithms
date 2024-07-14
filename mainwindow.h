#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "animationwindow.h"
class NumbersArrray;
#include "sortingdata.h"
#include <cstdlib>
#include <chrono>

#include <QApplication>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QValueAxis>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using std::swap;
using namespace std::chrono;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void quickSort(NumbersArrray& arr, int low, int high);
    int partition(NumbersArrray& arr, int low, int high);

    void merge(NumbersArrray& array1, int left, int mid, int right);
    void mergeSort(NumbersArrray& array, int const begin, int const end);

    void InsertionSort(NumbersArrray& arr, int begin, int end);

    void IntrosortUtil(NumbersArrray& arr, int begin, int end, int depthLimit);
    void Introsort(NumbersArrray& arr, int begin, int end);

    void heapify(NumbersArrray& arr, int low, int high, int i);
    void heapSort(NumbersArrray& arr, int low, int high);

    ~MainWindow();

signals:

    void swapCalled(NumbersArrray array, int index1, int index2);
    void mergeCalled(NumbersArrray array, int index1, int index2);

public slots:
    void updateChart1();

    void decreaseInterval();
    void increaseInterval();

private slots:

    void on_SizeBox_valueChanged(int arg1);
    void on_MaxValueBox_valueChanged(int arg1);
    void on_MinValueBox_valueChanged(int arg1);

    void on_Generate_clicked();

    void on_MethodBox_activated(int index);
    void on_Sort_clicked();

    void on_Unsorted_clicked();
    void on_Sorted_clicked();
    void on_AdditionalData_clicked();

    void on_Animation_clicked();

    void on_Save_clicked();

    void on_asymtoticButton_clicked();

    void on_DescendingButton_clicked();

    void on_AscendingButton_clicked();

private:
    Ui::MainWindow *ui;
    AnimationWindow *animation_window;
    QString currentFile = "";
    QTimer *Swaptimer;
    QBarSet *numbers_set;
    QBarSeries *series;
    QChart *chart;
    QChartView *chartView;
    int min_number, max_number, animation_duration;
    int ind1, ind2;
    bool animation_is_on;

};
#endif // MAINWINDOW_H
