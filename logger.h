#ifndef LOGGER_H
#define LOGGER_H

#include <functional>
#include <sstream>

class ToString
{
public:
    template<typename T>
    ToString& operator<<(const T& _t)
    {
        ss << _t;
        return *this;
    }
    operator std::string()
    {
        return ss.str();
    }

private:
    std::stringstream ss;
};

class Logger
{
public:
    enum LogLevelID {INFO = 0, FUNCTIONCALL, WARNING, ERROR, NUMLEVELS };
    const char* LogLevelTEXT[NUMLEVELS]
    {
      "INF", "FNC", "WRN", "ERR"
    };

    void log( std::string _msg, int _level);

    static Logger& get_instance()
    {
        static Logger logger;
        return logger;
    }

    void set_callback( std::function<void(int,std::string)>& _func);
    const char* level2text( int _level) const;



protected:
    Logger();
    ~Logger();
    Logger(const Logger&){}
    Logger& operator=(const Logger&){return *this;}

    void local_callback( int _level, const std::string& _msg) const;

    std::function<void(int, std::string&)> callback_;
};

#define LOG_INFO( msg ) \
    Logger::get_instance().log(ToString() << __FUNCTION__ << "@" << __LINE__ << " : " << msg, Logger::INFO);
#define LOG_WARN( msg ) \
    Logger::get_instance().log(ToString() << __FUNCTION__ << "@" << __LINE__ << " : " << msg, Logger::WARNING);
#define LOG_ERROR( msg ) \
    Logger::get_instance().log(ToString() << __FUNCTION__ << "@" << __LINE__ << " : " << msg, Logger::ERROR);
#ifdef LOG_FUNCTIONCALLS
#define LOG_FUNC_BEGIN \
    Logger::get_instance().log(ToString() << __FUNCTION__ << "@" << __LINE__ << " : " << "BEGIN", Logger::FUNCTIONCALL);
#else
#define LOG_FUNC_BEGIN
#endif
#ifdef LOG_FUNCTIONCALLS
#define LOG_FUNC_END \
    Logger::get_instance().log(ToString() << __FUNCTION__ << "@" << __LINE__ << " : " << "END", Logger::FUNCTIONCALL);
#else
#define LOG_FUNC_END
#endif

#endif // LOGGER_H
