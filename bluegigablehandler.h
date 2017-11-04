#ifndef BLUEGIGABLEHANDLER_H
#define BLUEGIGABLEHANDLER_H

#include <QObject>

class BluegigaBLEHandler : public QObject
{
    Q_OBJECT
public:
    explicit BluegigaBLEHandler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // BLUEGIGABLEHANDLER_H