#include "blewindow.h"
#include "ui_blewindow.h"

#include <QThread>
#include <QScrollBar>

#include <bluegigablehandler.h>
#include <logger.h>

/* Logging */

BLEWindow::BLEWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BLEWindow)
{
    ui->setupUi(this);

    /* Logger background gray*/
    QPalette log_palette = ui->te_log->palette();
    log_palette.setColor(QPalette::Base, QColor(100,100,100));
    ui->te_log->setPalette(log_palette);

    thread_ = new QThread;
    worker_ = new BluegigaBLEHandler;
    worker_->moveToThread(thread_);

    std::function<void(int, std::string)> callback = std::bind(&BLEWindow::logger_callback, this, std::placeholders::_1, std::placeholders::_2);
    Logger::get_instance().set_callback(callback);

    setup_connects();
}

BLEWindow::~BLEWindow()
{
    delete ui;
}

void BLEWindow::find_dongles()
{
    LOG_FUNC_BEGIN
            worker_->find_dongles();
    LOG_FUNC_END
}

void BLEWindow::dongles_found(QList<int> _coms)
{
    LOG_FUNC_BEGIN
            ui->cb_coms->clear();
    for( QList<int>::iterator it = _coms.begin(); it != _coms.end(); ++it)
         {
        LOG_INFO(*it)
        ui->cb_coms->addItem(QString("COM").append(QString::number(*it)));
    }
    ui->cb_coms->update();
    LOG_FUNC_END
}

void BLEWindow::setup_connects()
{
    LOG_FUNC_BEGIN
    connect(ui->tb_clearlog, &QToolButton::clicked, ui->te_log, &QTextEdit::clear);
    connect(ui->pb_find_dongle, &QPushButton::clicked, this, &BLEWindow::find_dongles);
    connect( worker_, &BluegigaBLEHandler::found_dongles, this, &BLEWindow::dongles_found);

    LOG_FUNC_END
}

void BLEWindow::logger_callback(int _level, std::string _msg)
{
    QColor old_color = ui->te_log->textColor();

    QTextCursor cursor(ui->te_log->textCursor());
    QTextCharFormat format;

    QString logtext;//Logger::LogLevelTEXT[_level]);
    switch( _level)
    {
    case Logger::INFO:
        format.setForeground(QBrush(QColor(Qt::green)));
        break;
    case Logger::FUNCTIONCALL:
        format.setForeground(QBrush(QColor(Qt::lightGray)));
        break;
    case Logger::WARNING:
        format.setForeground(QBrush(QColor(Qt::yellow)));
        break;
    case Logger::ERROR:
        format.setForeground(QBrush(QColor(Qt::red)));
        break;
    default:
        break;
    }
    cursor.setCharFormat(format);
    cursor.insertText(Logger::get_instance().level2text(_level));
    format.setForeground(QBrush(old_color));
    cursor.setCharFormat(format);
    logtext.append(" : ");
    logtext.append(_msg.c_str());
    cursor.insertText(logtext + "\n");

    ui->te_log->verticalScrollBar()->setValue(ui->te_log->verticalScrollBar()->maximum());
}
