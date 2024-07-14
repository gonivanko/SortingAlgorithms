#include "animationwindow.h"
#include "ui_animationwindow.h"

AnimationWindow::AnimationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnimationWindow)
{
    ui->setupUi(this);
}

AnimationWindow::~AnimationWindow()
{
    delete ui;
}

void AnimationWindow::on_slower_triggered()
{
    emit intervalIncreased();
}


void AnimationWindow::on_faster_triggered()
{
    emit intervalDecreased();
}

