#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "numbersarray.h"
#include <queue>

SortingData sorting_data;
NumbersArrray unsortedArray, sortedArray, animationArray;
std::queue<int> animationQueue, valuesQueue;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->asymtoticButton->setVisible(false);

    min_number = 0, max_number = 100, animation_duration = 500;
    animation_is_on = false;

    numbers_set = new QBarSet("Числа");
    series = new QBarSeries();
    chart = new QChart();
    chartView = new QChartView(chart);
    Swaptimer = new QTimer(this);
    Swaptimer->setSingleShot(false);
    animation_window = new AnimationWindow(this);
    chart->addSeries(series);
    QObject::connect(Swaptimer, &QTimer::timeout, this, &MainWindow::updateChart1);
    QObject::connect(animation_window, &AnimationWindow::intervalDecreased, this, &MainWindow::decreaseInterval);
    QObject::connect(animation_window, &AnimationWindow::intervalIncreased, this, &MainWindow::increaseInterval);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void clear_queue( std::queue<int> &q )
{
    std::queue<int> empty;
    swap( q, empty );
}

void MainWindow::swapCalled(NumbersArrray array, int index1, int index2)
{
//    qDebug() << "swap called";
    animationQueue.push(index1);
    animationQueue.push(index2);
    valuesQueue.push(array[index1]);
    valuesQueue.push(array[index2]);
}

void MainWindow::mergeCalled(NumbersArrray array, int index1, int index2)
{
//    qDebug() << "merge called";
    int size = index2 - index1 + 1;
    animationQueue.push(index1);
    animationQueue.push(index2);
    for (int i = 0; i < size; i++)
    {
        valuesQueue.push(array[index1 + i]);
    }
}


void MainWindow::updateChart1()
{
    int i1 = animationQueue.front();
//    qDebug() << "front:" << animationQueue.front();
    animationQueue.pop();

    int i2 = animationQueue.front();
//    qDebug() << "front:" << animationQueue.front();
    animationQueue.pop();

    if (sorting_data.method() == 0)
    {
//        qDebug() << "chart merged";
//        qDebug() << "============" ;

        int size = i2 - i1 + 1;

        int v;

        for (int i = 0; i < size; i++)
        {
            v = valuesQueue.front();
            valuesQueue.pop();
            numbers_set->replace(i1 + i, v);
        }

    }
    else if (sorting_data.method() == 1 || sorting_data.method() == 2)
    {
//        qDebug() << "chart updated";
//        qDebug() << "============" ;

        int v1 = valuesQueue.front();
        valuesQueue.pop();


        int v2 = valuesQueue.front();
        valuesQueue.pop();

        numbers_set->replace(i1, v2);
        numbers_set->replace(i2, v1);
    }

    chart->update();

    if (animationQueue.empty()) Swaptimer->stop();
}

void MainWindow::decreaseInterval()
{
    if (animation_duration > 100)
    {
        animation_duration -= 100;
        Swaptimer->setInterval(animation_duration);
    }

    else if (animation_duration > 30)
    {
        animation_duration -= 10;
        Swaptimer->setInterval(animation_duration);
    }

}

void MainWindow::increaseInterval()
{
    animation_duration += 100;
    Swaptimer->setInterval(animation_duration);

}

void MainWindow::merge(NumbersArrray& array1, int left, int mid, int right)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;

    NumbersArrray leftArray(subArrayOne), rightArray(subArrayTwo);

    for (int i = 0; i < subArrayOne; i++)
        leftArray[i] = array1[left + i];
    for (int j = 0; j < subArrayTwo; j++)
        rightArray[j] = array1[mid + 1 + j];

    int indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    sorting_data.addComparisons(1);
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo)
    {
        sorting_data.addComparisons(1);
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo])
        {
            array1[indexOfMergedArray] = leftArray[indexOfSubArrayOne];

            indexOfSubArrayOne++;
            sorting_data.addSwaps(1);
        }
        else
        {
            array1[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
            sorting_data.addSwaps(1);
        }
        indexOfMergedArray++;
    }

    sorting_data.addComparisons(1);
    while (indexOfSubArrayOne < subArrayOne) {
        array1[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
        sorting_data.addSwaps(1);
    }

    sorting_data.addComparisons(1);
    while (indexOfSubArrayTwo < subArrayTwo) {
        array1[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
        sorting_data.addSwaps(1);
    }

    if (animation_is_on) mergeCalled(array1, left, right);
}

void MainWindow::mergeSort(NumbersArrray& array, int const begin, int const end)
{
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

int MainWindow::partition(NumbersArrray& arr, int low, int high)
{
    int pivot = arr[high];

    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        sorting_data.addComparisons(1);
        if (arr[j] < pivot) {

            i++;
            if (animation_is_on) swapCalled(arr, i, j);
            swap(arr[i], arr[j]);
            sorting_data.addSwaps(1);
        }
    }
    if (animation_is_on) swapCalled(arr, i + 1, high);
    swap(arr[i + 1], arr[high]);
    sorting_data.addSwaps(1);
    return (i + 1);
}


void MainWindow::quickSort(NumbersArrray& arr, int low, int high)
{
    if (low < high) {

        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void MainWindow::heapify(NumbersArrray& arr, int low, int high, int i)
{
    int largest = i;

    int l = 2 * i  + 1 - low;
    int r = 2 * i  + 2 - low;

    sorting_data.addComparisons(3);

    if (l <= high && arr[l] > arr[largest])
        largest = l;

    if (r <= high && arr[r] > arr[largest])
        largest = r;

    if (largest != i)
    {
        sorting_data.addSwaps(1);
        if (animation_is_on) swapCalled(arr, i, largest);
        swap(arr[i], arr[largest]);

        heapify(arr, low, high, largest);
    }
}

void MainWindow::heapSort(NumbersArrray& arr, int low, int high)
{
    sorting_data.addComparisons(1);
    for (int i = (high - low + 1) / 2 - 1 + low; i >= low; i--)
    {
        sorting_data.addComparisons(1);

        heapify(arr, low, high, i);
    }

    sorting_data.addComparisons(1);
    for (int i = high; i > low; i--)
    {
        sorting_data.addSwaps(1);
        sorting_data.addComparisons(1);

        if (animation_is_on) swapCalled(arr, low, i);
        swap(arr[low], arr[i]);

        heapify(arr, low, i - 1, low);
    }
}

void MainWindow::InsertionSort(NumbersArrray& arr, int begin, int end)
{
    int i, j;

    sorting_data.addComparisons(1);
    for (i = begin + 1; i <= end; i++)
    {
        j = i;
        sorting_data.addComparisons(1);
        while (j > begin && arr[j - 1] > arr[j])
        {
            sorting_data.addComparisons(1);
            sorting_data.addSwaps(1);
            if (animation_is_on) swapCalled(arr, j - 1, j);
            swap(arr[j], arr[j - 1]);
            j--;
        }
    }
}

int MedianOfThree(NumbersArrray& arr, const int& low, const int& high)
{
    int mid = (low + high) / 2;
    if (arr[low] < arr[mid] && arr[mid] < arr[high])
        return mid;

    if (arr[low] < arr[high] && arr[high] <= arr[mid])
        return high;

    if (arr[mid] <= arr[low] && arr[low] < arr[high])
        return low;

    if (arr[mid] < arr[high] && arr[high] <= arr[low])
        return high;

    if (arr[high] <= arr[low] && arr[low] < arr[mid])
        return low;

    if (arr[high] <= arr[mid] && arr[mid] <= arr[low])
        return mid;
}

void MainWindow::IntrosortUtil(NumbersArrray& arr, int begin, int end, int depthLimit)
{
//    qDebug() << "depthLimit:" << depthLimit;

    int size = end - begin + 1;

    if (size < 16)
    {
        sorting_data.addComparisons(1);
        qDebug() << "Insertion sort called" << "begin:" << begin << "end:" << end;
        InsertionSort(arr, begin, end);
        return;
    }

    sorting_data.addComparisons(2);

    if (depthLimit == 0)
    {
        qDebug() << "---Heapsort called" << "begin:" << begin << "end:" << end;
        heapSort(arr, begin, end);
        return;
    }

//    int pivot = MedianOfThree(arr, begin, end);

//    if (animation_is_on) swapCalled(arr, pivot, end);
//    swap(arr[pivot], arr[end]);

    sorting_data.addSwaps(1);
    qDebug() << "Partition called" << "begin:" << begin << "end:" << end;
    int partitionPoint = partition(arr, begin, end);
    IntrosortUtil(arr, begin, partitionPoint-1, depthLimit - 1);
    IntrosortUtil(arr, partitionPoint + 1, end, depthLimit - 1);

    return;

}

/* Implementation of introsort*/
void MainWindow::Introsort(NumbersArrray& arr, int begin, int end)
{
    int depthLimit = 2 * log(end - begin + 1);

//    qDebug() << "end - begin + 1 =" << end - begin + 1;
//    qDebug() << "log(end - begin + 1) =" << log(end - begin + 1);
//    qDebug() << "depthLimit:" << depthLimit;

    IntrosortUtil(arr, begin, end, depthLimit);

    return;
}

void MainWindow::on_MinValueBox_valueChanged(int arg1)
{
    min_number = arg1;
    ui->MaxValueBox->setMinimum(arg1);
}

void MainWindow::on_MaxValueBox_valueChanged(int arg1)
{
    max_number = arg1;
}

void MainWindow::on_SizeBox_valueChanged(int arg1)
{
    sorting_data.setN(arg1);
    ui->textBrowser->setPlainText("Кількість чисел для генерації: " + QString::number(arg1));
}

void MainWindow::on_Generate_clicked()
{
    srand((unsigned) time(nullptr));
    NumbersArrray Generated(sorting_data.getN());
    for (int i = 0; i < sorting_data.getN(); i++)
    {
        Generated.pushBack( min_number + rand() % (max_number - min_number + 1) );
    }
    ui->Unsorted->setChecked(true);
    sortedArray = Generated;
    unsortedArray = Generated;
    ui->textBrowser->setPlainText( unsortedArray.getString() );
}

void MainWindow::on_MethodBox_activated(int index)
{
    sorting_data.setSortingMethod(index);
}

void MainWindow::on_Sort_clicked()
{
    if (!unsortedArray.isEmpty())
    {
        sortedArray = unsortedArray;
        sorting_data.reset();
        animation_is_on = false;

        auto start = high_resolution_clock::now();

        switch(sorting_data.method()) {
        case 0:
            mergeSort(sortedArray, 0, sortedArray.length() - 1);
            break;

        case 1:
            quickSort(sortedArray, 0, sortedArray.length() - 1);
            break;

        case 2:
            Introsort(sortedArray, 0, sortedArray.length() - 1);
            break;
        }
        auto end = high_resolution_clock::now();
        auto time_taken = duration_cast<microseconds>(end - start);
        sorting_data.addTime(time_taken.count());
        ui->Sorted->setChecked(true);
        ui->textBrowser->setPlainText(sortedArray.getString());
        qDebug() << "=========================================";
    }
    else
    {
        QMessageBox::warning(this, "Помилка", "Масив для сортування не згенерований");
    }
}


void MainWindow::on_Unsorted_clicked()
{
    ui->textBrowser->setPlainText(unsortedArray.getString());
}

void MainWindow::on_Sorted_clicked()
{
    ui->textBrowser->setPlainText(sortedArray.getString());
}

void MainWindow::on_AdditionalData_clicked()
{
    ui->textBrowser->setPlainText(sorting_data.getString());
}

void MainWindow::on_Animation_clicked()
{
    if (unsortedArray.length() <= 300)
    {
        animation_is_on = true;
        numbers_set->remove(0, numbers_set->count());

        for (int i = 0; i < unsortedArray.length(); i++)
        {
            *numbers_set << unsortedArray[i];
        }

        clear_queue(animationQueue);
        clear_queue(valuesQueue);

        animationArray = unsortedArray;
        chart->removeSeries(series);

        series->append(numbers_set);

        chart->addSeries(series);
        chart->setTitle("Анімація сортування");
        chart->setAnimationOptions(QChart::AllAnimations);

        QValueAxis *axisY = new QValueAxis;
        axisY->setRange(min_number, max_number);

        chart->createDefaultAxes();
        chart->setAxisY(axisY, series);

        chartView->setRenderHint(QPainter::Antialiasing);
        QPalette pal = qApp->palette();
        pal.setColor(QPalette::Window, QRgb(0xffffff));
        pal.setColor(QPalette::WindowText, QRgb(0x404040));

        animation_window->setCentralWidget(chartView);
        animation_window->show();

        ui->Animation->setEnabled(false);

        Swaptimer->start(animation_duration);

        switch (sorting_data.method()) {
        case 0:
            mergeSort(animationArray, 0, animationArray.length() - 1);
            break;
        case 1:
            quickSort(animationArray, 0, animationArray.length() - 1);
            break;
        case 2:
//            InsertionSort(animationArray, 0, animationArray.length() - 1);
            Introsort(animationArray, 0, animationArray.length() - 1);
//            heapSort(animationArray, 0, animationArray.length() - 1);
            break;
        default:
            break;
        }
        ui->Animation->setEnabled(true);
    }
    else
    {
        animation_is_on = false;
        QMessageBox::warning(this, "Помилка", "Розмірність масиву занадто велика і перевищує 300");
    }
}


void MainWindow::on_Save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Помилка", "Неможливо зберегти файл");
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    out << sorting_data.getString() << "\n"
        << "Початковий масив: " << unsortedArray.getString() << "\n"
        << "Відсортований масив: " << sortedArray.getString();
    file.close();
}

void MainWindow::on_asymtoticButton_clicked()
{
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    QLineSeries *series3 = new QLineSeries();
    QLineSeries *series4 = new QLineSeries();

    for (int i = 1; i < 10000; i+=100)
    {
        sorting_data.reset();
        srand((unsigned) time(nullptr));
        NumbersArrray Generated(i);

        for (int j = 0; j < i; j++)
        {
            Generated.pushBack( min_number + rand() % (max_number - min_number + 1) );
        }
        NumbersArrray CopyArr = Generated;
        NumbersArrray Copy2Arr = Generated;


//        auto start = high_resolution_clock::now();
        mergeSort(Generated, 0, Generated.length() - 1);
//        auto end = high_resolution_clock::now();
//        auto time_taken = duration_cast<microseconds>(end - start);
        series1->append(i, sorting_data.getComparisons() + sorting_data.getSwaps());
//        series1->append(i, sorting_data.getSwaps());
//        series1->append(i, sorting_data.getComparisons());
//        series1->append(i, time_taken.count());
        sorting_data.reset();
//        start = high_resolution_clock::now();
        quickSort(CopyArr, 0, CopyArr.length() - 1);
//        end = high_resolution_clock::now();
//        time_taken = duration_cast<microseconds>(end - start);
        qDebug() << sorting_data.getComparisons();
        series2->append(i, sorting_data.getSwaps() + sorting_data.getComparisons());
//        series2->append(i, sorting_data.getSwaps());
//        series2->append(i, sorting_data.getComparisons());
//        series2->append(i, time_taken.count());
        sorting_data.reset();
//        start = high_resolution_clock::now();
        Introsort(Copy2Arr, 0, Copy2Arr.length() - 1);
//        end = high_resolution_clock::now();
//        time_taken = duration_cast<microseconds>(end - start);
        series3->append(i, sorting_data.getSwaps() + sorting_data.getComparisons());
//        series3->append(i, sorting_data.getSwaps());
//        series3->append(i, sorting_data.getComparisons());
//        series3->append(i, time_taken.count());
        series4->append(i, (int) (i * log2(i)));
    }

    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->addSeries(series3);
    chart->addSeries(series4);
    series1->setName("Сортування злиттям");
    series2->setName("Швидке сортування");
    series3->setName("Інтроспективне сортування");
    series4->setName("Асимптотична оцінка");

//    chart->setTitle("Кількість перестановок");
//    chart->setTitle("Кількість порівнянь");
    chart->setTitle("Кількість операцій");

    chart->createDefaultAxes();

    chartView->setRenderHint(QPainter::Antialiasing);

    animation_window->setCentralWidget(chartView);
    animation_window->show();

}

void MainWindow::on_DescendingButton_clicked()
{
    NumbersArrray Generated(sorting_data.getN());
    float step = (max_number - min_number) / (float) sorting_data.getN();
//    qDebug() << "Step: " << step;
    float current_value = max_number;
    for (int i = sorting_data.getN(); i > 0; i--)
    {
        Generated.pushBack((int) current_value);
        current_value -= step;
    }
    ui->Unsorted->setChecked(true);
    sortedArray = Generated;
    unsortedArray = Generated;
    ui->textBrowser->setPlainText( unsortedArray.getString() );
}


void MainWindow::on_AscendingButton_clicked()
{
    NumbersArrray Generated(sorting_data.getN());
    float step = (max_number - min_number) / (float) sorting_data.getN();
//    qDebug() << "Step: " << step;
    float current_value = min_number;
    for (int i = 0; i < sorting_data.getN(); i++)
    {
        Generated.pushBack( (int) current_value);
//        Generated.pushBack(10);
        current_value += step;
    }
    ui->Unsorted->setChecked(true);
    sortedArray = Generated;
    unsortedArray = Generated;
    ui->textBrowser->setPlainText( unsortedArray.getString() );
}

