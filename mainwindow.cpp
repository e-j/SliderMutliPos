#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //TODO : add a label display interval boundaries, for test of signals
    //TODO : add a combo for select detection mode
    //TOOD : add box for select nb intervals, interval color

    ui->horizontalSlider->setNbValues(6);
    ui->horizontalSlider->setRange(0,100);
    ui->horizontalSlider->setValue(0,0);
    ui->horizontalSlider->setValue(1,25);
    ui->horizontalSlider->setValue(2,50);
    ui->horizontalSlider->setValue(3,75);
    ui->horizontalSlider->setValue(4,87);
    ui->horizontalSlider->setValue(5,100);
    ui->horizontalSlider->setSpanColor(0,QColor(0,0,255));
    ui->horizontalSlider->setSpanColor(1,QColor(0,255,0));
    ui->horizontalSlider->setSpanColor(2,QColor(255,255,0));
    ui->horizontalSlider->setSpanColor(3,QColor(255,127,0));
    ui->horizontalSlider->setSpanColor(4,QColor(255,0,0));

    ui->horizontalSlider->valueIndication(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}
