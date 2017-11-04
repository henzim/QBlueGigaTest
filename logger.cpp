#include "logger.h"
#include <iostream>
#include <sstream>

Logger::Logger() : callback_(std::bind(&Logger::local_callback, this, std::placeholders::_1, std::placeholders::_2))
{

}

Logger::~Logger()
{

}

void Logger::log(std::string _msg, int _level)
{
    callback_(_level, _msg);
}

void Logger::set_callback(std::function<void (int, std::string)> &_func)
{
    callback_ = _func;
}

const char *Logger::level2text(int _level) const
{
    return LogLevelTEXT[_level];
}

void Logger::local_callback(int _level, const std::string &_msg) const
{
    std::stringstream ss;
    ss << LogLevelTEXT[_level] << " : " << _msg << std::endl;
    std::cerr << ss.str();
}
