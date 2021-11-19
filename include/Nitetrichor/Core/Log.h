//
// Created by Molybdenum on 8/6/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//

#ifndef LEARNKIT_LOG_H
#define LEARNKIT_LOG_H

#include <stdarg.h>
#include <stdio.h>


#ifdef __cplusplus

#include <vector>
#include <string>
#include "Notification.hpp"



extern "C" {
#endif

#if (__GNUC__*10+__GNUC_MINOR__ >= 42)
	#define WN_FORMAT_FUNCTION(F,A) __attribute__((format(printf, F, A)))
    #else
	#define WN_FORMAT_FUNCTION(F,A)
#endif


void WNLog(const char * format, ...) WN_FORMAT_FUNCTION(1,2);

void WNLogv(const char * format, va_list args);

void WNLogSetFile(FILE * file);

void WNLogSetPath(const char * path);

const char * WNLogGetLast(void);


#ifdef __cplusplus
}

void WNLog(int val);
void WNLog(unsigned int val);
void WNLog(long val);
void WNLog(unsigned long val);
void WNLog(char val);
void WNLog(unsigned char val);
void WNLog(float val);
void WNLog(double val);



namespace WN {
class Log {
    std::vector<std::string> logs;
    FILE * file;
    pthread_mutex_t mutex;


public:
    Log(const Log &) = delete;
    Log &operator=(const Log &) = delete;


    Log() noexcept;
    explicit Log(const char * path) noexcept;


    ~Log();

    static Log &GetSharedLog();

    /*!
     * @brief should not call this, this is used with global WNLog function.
     */
    void __log(std::string msg);

    void log(const char * format, ...) WN_FORMAT_FUNCTION(2,3);

    void logv(const char * format,va_list list);

    [[nodiscard]] const std::string &getLastLog() const ;

    void setFile(FILE * file) noexcept;

    void setPath(const char * path) noexcept;

    [[nodiscard]] const std::vector<std::string> &getLogs() const;

    [[nodiscard]] FILE *getFile() const;


    static const NotificationName &GetDidLogNotificationName() noexcept ;

};


}

#endif






#endif //LEARNKIT_LOG_H
