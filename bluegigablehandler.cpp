#include "bluegigablehandler.h"
#include <BluegigaUART.h>
#include <vector>
#include <logger.h>

/** Locals */
BluegigaUARTHandler uart_;

BluegigaBLEHandler::BluegigaBLEHandler(QObject *parent) : QObject(parent)
{
    uart_.close();
}

void BluegigaBLEHandler::find_dongles()
{
    LOG_FUNC_BEGIN
    std::list< int > coms = uart_.list_devices();
    QList<int> ql_coms;
    for( std::list<int>::const_iterator cit = coms.begin(); cit != coms.end(); ++cit)
        ql_coms.append(*cit);
    emit found_dongles(ql_coms);
    LOG_FUNC_BEGIN
}

void BluegigaBLEHandler::find_device(QString _com)
{

}
