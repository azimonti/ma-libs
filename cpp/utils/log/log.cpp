/************************/
/*       log.cpp        */
/*    Version 2.4       */
/*     2023/01/29       */
/************************/

#include "log.h"

#if defined(LOGGING)

#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdexcept>

size_t logging::Logger::_level      = INFO;
int logging::Logger::_line          = 0;
const char* logging::Logger::_fname = nullptr;
std::mutex logging::Logger::_mutex;

logging::Logger::Logger()
    : _fileName("out.log"), _log_level_max(INFO), _stdout(true), _fileout(false), _log_line(false), _log_time(false)
{
}

logging::Logger::~Logger()
{
    if (_fileStream.is_open()) _fileStream.close();
}

logging::Logger& logging::Logger::instance()
{
    static Logger _i;
    return _i;
}

logging::Logger& logging::Logger::instance(const size_t l, const char* fn, const int n)
{
    _level = l;
    _line  = n;
    _fname = fn;
    return instance();
}

void logging::Logger::params(size_t p, bool t)
{
    switch (p)
    {
    case STDOUT: _stdout = t; break;
    case FILEOUT:
        _fileout = t;
        if (_fileout)
        {
            if (_fileStream.is_open()) _fileStream.close();
            _fileStream.open(_fileName, std::ofstream::out | std::ofstream::app);
        }
        else
        {
            if (_fileStream.is_open()) _fileStream.close();
        }
        break;
    case LOGLINE: _log_line = t; break;
    case LOGTIME: _log_time = t; break;
    default: throw std::invalid_argument("Unknown parameter");
    }
};

void logging::Logger::params(size_t p, size_t t)
{
    switch (p)
    {
    case LEVELMAX: _log_level_max = t; break;
    default: throw std::invalid_argument("Unknown parameter");
    }
};

void logging::Logger::params(size_t p, const char* t)
{
    switch (p)
    {
    case FILENAME: _fileName = t; break;
    default: throw std::invalid_argument("Unknown parameter");
    }
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
static std::tm* localtime_r(const std::time_t* timer, std::tm* buf)
{
    *buf = *std::localtime(timer);
    return buf;
}
#endif

std::string logging::Logger::NowTime()
{
    std::chrono::time_point<std::chrono::system_clock> _now = std::chrono::system_clock::now();
    std::time_t _t                                          = std::chrono::system_clock::to_time_t(_now);
    std::chrono::milliseconds _m =
        std::chrono::duration_cast<std::chrono::milliseconds>(_now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    std::tm _tm;
    localtime_r(&_t, &_tm);
    oss << std::put_time(&_tm, "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << _m.count();

    return oss.str();
}

#endif // LOGGING
