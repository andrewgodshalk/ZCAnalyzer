#ifndef GUARD_timestamp_h
#define GUARD_timestamp_h

/*------------------------------------------------------------------------------
TimeStamp class

Class adapted from old timestamp functions.

2016-10-14 - Reworked to rely on std instead of ROOT.
2016-08-18 - Reworked into a class.
2015-02-01 - taken from 2013 Lorentz Angle work. Functions that spit out time
  stamps formatted for file names.


------------------------------------------------------------------------------*/

#include <cstdio>   // For sprintf function
#include <ctime>    // For time functions
#include <string>


class TimeStamp
{
  public:
    TimeStamp(char d = '-', char b = ' ', char t = ':') : dDlm_(d), bDlm_(b), tDlm_(t) {update();}
   ~TimeStamp(){};

    std::string log_str();
    std::string fn_str();
    void update();   // Update to the current time.
    void setDelimiters(char d = '-', char b = ' ', char t = ':') { dDlm_=d; bDlm_=b; tDlm_=t; }

  private:
    std::tm* now_;
    char dDlm_;   // Delimeter between date numbers,
    char bDlm_;   //   between date and time,
    char tDlm_;   //   and between the numbers in the time.
    int  year_, month_, day_, hour_, min_, sec_;
};

// Outputs a timestamp string suitable for text logs.
//   Deliminated by the input values. Default example: 2016-08-17 01:56:24
inline std::string TimeStamp::log_str()
{
    char str[20];   // Length of string shown in example above, plus one character for termination.
    std::snprintf(str, 20, "%04i%c%02i%c%02i%c%02i%c%02i%c%02i", year_, dDlm_, month_, dDlm_, day_, bDlm_, hour_, tDlm_, min_, tDlm_, sec_);
    return str;
}

// Spits out date and time, by default in YYYY-MM-DD_HHMMSS, for use in filenames
inline std::string TimeStamp::fn_str()
{
    char str[18];   // Length of string shown in example above, plus one character for termination.
    std::snprintf(str, 18, "%04i-%02i-%02i_%02i%02i%02i", year_, month_, day_, hour_, min_, sec_);
    return str;
}

// Set a new date and time
inline void TimeStamp::update()
{
    std::time_t t = std::time(nullptr);
    now_   = std::localtime(&t);
    year_  = now_->tm_year+1900;
    month_ = now_->tm_mon+1    ;
    day_   = now_->tm_mday     ;
    hour_  = now_->tm_hour     ;
    min_   = now_->tm_min      ;
    sec_   = now_->tm_sec      ;
}

#endif
