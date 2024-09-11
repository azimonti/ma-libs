#ifndef _LOG_H_431CFDB186F64BFBB9F6BFC972784272_
#define _LOG_H_431CFDB186F64BFBB9F6BFC972784272_

/************************/
/*       log.h          */
/*    Version 2.4       */
/*     2023/01/29       */
/************************/

#include <cstring>
#include <iostream>
#include <mutex>
#include "std/cout.h"
#include "shared_libraries_export.hpp"

namespace logging
{
    enum LOG_LEVEL : size_t { NONE = 0, INFO, WARNING, ERROR, FATAL };

    enum LOG_PARAMS : size_t { STDOUT = 0, FILEOUT, FILENAME, LOGLINE, LOGTIME, LEVELMAX };
} // namespace logging
#if !defined(LOGGING)
#define LOGGER(x) 0 && std::cout << x
#define LOGGER_PARAM(x, y)
#else
#define LOGGER(x)          logging::Logger::instance(x, __FILE__, __LINE__)
#define LOGGER_PARAM(x, y) logging::Logger::instance().params(x, y)
#include <fstream>
#include <sstream>
#include <string>

namespace logging
{
    class DLL_PUBLIC Logger
    {
      public:
        static Logger& instance();
        static Logger& instance(const size_t l, const char* fn, const int n);
        ~Logger();
        Logger(const Logger&)         = delete;
        void operator=(const Logger&) = delete;

        template <typename T> Logger& operator<<(T t)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_level >= _log_level_max)
            {
                std::stringstream ss;
                if (_log_time) ss << "[" << NowTime() << "]";
                ss << "[" << _level_text[_level] << "]";
                if (_log_line)
                    ss << "["
                       << (strrchr(_fname, '/') ? strrchr(_fname, '/') + 1
                                                : (strrchr(_fname, '\\') ? strrchr(_fname, '\\') + 1 : _fname))
                       << ":" << _line << "]";
                ss << " " << t;
                if (_stdout) std::cout << ss.str() << std::endl;
                if (_fileout && _fileStream.is_open()) _fileStream << ss.str() << std::endl;
                return *this;
            }
            else return *this;
        };

        void params(size_t p, bool b);
        void params(size_t p, size_t n);
        void params(size_t p, const char*);

      private:
        Logger();
        std::string NowTime();

        static size_t _level;
        static int _line;
        static const char* _fname;
        static std::mutex _mutex;
        std::string _fileName;
        std::ofstream _fileStream;
        size_t _log_level_max;
        bool _stdout;
        bool _fileout;
        bool _log_line;
        bool _log_time;
        const char* _level_text[10] = {"NONE", "INFO", "WARNING", "ERROR", "FATAL"};
    };

} // namespace logging
#endif // LOGGING
#endif
