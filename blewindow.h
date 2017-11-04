#ifndef BLEWINDOW_H
#define BLEWINDOW_H

#include <QMainWindow>

namespace Ui {
class BLEWindow;
}

class BLEWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BLEWindow(QWidget *parent = 0);
    ~BLEWindow();

private:
    Ui::BLEWindow *ui;
};

#endif // BLEWINDOW_H
