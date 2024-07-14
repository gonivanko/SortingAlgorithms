#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class AnimationWindow;
}

class AnimationWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void intervalIncreased();
    void intervalDecreased();

public:
    explicit AnimationWindow(QWidget *parent = nullptr);
    ~AnimationWindow();

private slots:
    void on_slower_triggered();

    void on_faster_triggered();

private:
    Ui::AnimationWindow *ui;

};

#endif // ANIMATIONWINDOW_H
