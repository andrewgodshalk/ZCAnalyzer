#ifndef GUARD_logger_h
#define GUARD_logger_h

/*------------------------------------------------------------------------------
  Logger class

  Created:  2016-10-?
  Modified: 2016-03-20

  Wrapper for spdlog library: https://github.com/gabime/spdlog
  Fomatting for spdlog is drawn from fmt library: https://github.com/fmtlib/fmt
  Formatting syntax almost identical to printf syntax (http://www.cplusplus.com/reference/cstdio/printf/),
    the main exception being that the preceeding ("%4.2f") is replaced with
    curly brackets ("{4.2f}").

  To do:
  - Find some way to make debug output prefix a standard length (see
      specification in NtupleProcessor.cpp :: processCommandLineInput())

  2016-11-21 - Added option to set usage for testing (no file output).
  2017-03-20 - Implemented indentation levels.

------------------------------------------------------------------------------*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "spdlog/spdlog.h"
#include "TimeStamp.h"

class Logger
{
  public:
    Logger( const char* ln,               // Name of the logger. Used as fileName unless otherwise specified.
            const std::string& pfx = "" ,   // Prefix output at beginning of line in log file.
            const unsigned int  in = 0  ,   // Level of indentation ( (2 spaces)*in )
            const std::string&  fn = "" ,   // File name for file output.
            const std::string&  fp = ""     // Path of file output.
          ) : logName_(ln), logPrefix_(pfx), indentCount_(in), fileName_(fn), logDir_(fp), logFormat_("%v")
    { // Check for existing logger with same name.
        logger_ = spdlog::get(logName_);

      // Create logger if logger not found.
        if(!logger_)
        {   try
            { // Set up file name and location.
                if(logDir_  =="") logDir_   = "logs/";
                if(fileName_=="") fileName_ = logName_;
                if(fileName_ != "NULL") fileName_ += "_" + TimeStamp().fn_str() + ".log";

              // Create sinks (outputs) for loggers
                sinks_.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
                sinks_.push_back(std::make_shared<spdlog::sinks::stderr_sink_st>());
                if(fileName_ != "NULL") sinks_.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>((logDir_+fileName_).c_str()));

              // Create logger, to be accessed globally from all classes.
                logger_ = std::make_shared<spdlog::logger>(logName_.c_str(), begin(sinks_), end(sinks_));

              // Set logger and sink levels.
                logger_  ->set_level(spdlog::level::info);
                sinks_[0]->set_level(spdlog::level::info);
                sinks_[1]->set_level(spdlog::level::err );
                if(fileName_ != "NULL") sinks_[2]->set_level(spdlog::level::info);

              // Set attributes and register logger in global list
                setFormat(logFormat_);
                spdlog::set_async_mode(4096);
                spdlog::register_logger(logger_);
            }
            catch (const spdlog::spdlog_ex& ex)
            {
                std::cout << "Log failed: " << ex.what() << std::endl;
            }
        }

      // Set up log prefix
        setPrefix(pfx, in);
    }

    ~Logger(){}

    void setPrefix(const std::string& pfx, const size_t in = 0)
    { // Set prefix to logger with input indentation.
        indentCount_ = in;
        logPrefix_   = pfx + std::string(2*indentCount_+1, ' ');
    }

    void setFormat(const std::string& fmt)
    { // Set log format
        logFormat_ = fmt;
        logger_->set_pattern(logFormat_.c_str());
    }

    void setDebug(bool dbg = true)
    { // Set logger and sinks to print at debug levels.
        if(dbg)
        {   logger_  ->set_level(spdlog::level::trace);
            sinks_[0]->set_level(spdlog::level::debug);
            if(fileName_ != "NULL") sinks_[2]->set_level(spdlog::level::trace);
        }
        else
        {   logger_  ->set_level(spdlog::level::info);
            sinks_[1]->set_level(spdlog::level::info);
            if(fileName_ != "NULL") sinks_[2]->set_level(spdlog::level::info);
        }
    }

    void setQuiet(bool qt = true)
    { // Sets stdout sink to print only on error
        if(qt) sinks_[0]->set_level(spdlog::level::err );
        else   sinks_[0]->set_level(spdlog::level::info);
    }

    // Functions that pass along messages to logger.
    template<typename... Args>void trace   (std::string msg, const Args&... args) { logger_->trace   ((logPrefix_+msg).c_str(), args...); }
    template<typename... Args>void debug   (std::string msg, const Args&... args) { logger_->debug   ((logPrefix_+msg).c_str(), args...); }
    template<typename... Args>void info    (std::string msg, const Args&... args) { logger_->info    ((logPrefix_+msg).c_str(), args...); }
    template<typename... Args>void warn    (std::string msg, const Args&... args) { logger_->warn    ((logPrefix_+msg).c_str(), args...); }
    template<typename... Args>void error   (std::string msg, const Args&... args) { logger_->error   ((logPrefix_+msg).c_str(), args...); }
    template<typename... Args>void critical(std::string msg, const Args&... args) { logger_->critical((logPrefix_+msg).c_str(), args...); }

  private:
    std::shared_ptr<spdlog::logger> logger_;
    std::vector<spdlog::sink_ptr>   sinks_ ;
    std::string logName_     ;
    std::string logPrefix_   ;
    size_t      indentCount_ ;
    std::string fileName_    ;
    std::string logDir_      ;
    std::string logFormat_   ;
};

#endif
