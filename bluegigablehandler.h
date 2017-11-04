#ifndef BLUEGIGABLEHANDLER_H
#define BLUEGIGABLEHANDLER_H

#include <QObject>
#include <QString>
#include <QList>

class BluegigaBLEHandler : public QObject
{
    Q_OBJECT
public:
    explicit BluegigaBLEHandler(QObject *parent = nullptr);

public slots:
    void find_dongles();
    void find_device(QString _com);

signals:
    /** QList with found com ports */
    void found_dongles( QList<int> );


public slots:
};

#endif // BLUEGIGABLEHANDLER_H
