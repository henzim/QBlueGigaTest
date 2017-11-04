#ifndef BLEWINDOW_H
#define BLEWINDOW_H

#include <QMainWindow>
class QThread;
class BluegigaBLEHandler;

namespace Ui {
class BLEWindow;
}

class BLEWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BLEWindow(QWidget *parent = 0);
    ~BLEWindow();

    void logger_callback( int _level, std::string _msg);
public slots:
    void find_dongles();
    void dongles_found(QList<int> _coms);

private:
    void setup_connects();

private:
    Ui::BLEWindow *ui;

    QThread* thread_;
    BluegigaBLEHandler * worker_;
};

#endif // BLEWINDOW_H
