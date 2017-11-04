#include "blewindow.h"
#include "ui_blewindow.h"

BLEWindow::BLEWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BLEWindow)
{
    ui->setupUi(this);
}

BLEWindow::~BLEWindow()
{
    delete ui;
}
