//
// Created by Molybdenum on 8/6/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//

#include "Core/Log.h"
#include <unistd.h>
#include <pthread.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include "Core/Notification.hpp"


namespace WN {

Log &Log::GetSharedLog() {
    static Log log;
    return log;
}


const NotificationName &Log::GetDidLogNotificationName() noexcept {
    static const NotificationName name("WNLogDidLog");
    return name;
}

void Log::__log(std::string msg) {
    NotificationCenter::GetDefault().postNotification({GetDidLogNotificationName(), msg});
    logs.push_back(std::move(msg));
}

void Log::setFile(FILE * aFile) noexcept {
    file = aFile;
}

const std::vector<std::string> &Log::getLogs() const {
    return logs;
}

FILE *Log::getFile() const {
    return file;
}

Log::Log() noexcept : file(stderr), mutex(PTHREAD_MUTEX_INITIALIZER) {

}
Log::Log(const char *path) noexcept : file(fopen(path, "w+")), mutex(PTHREAD_MUTEX_INITIALIZER){

}

Log::~Log() {
    if (file != stderr && file != stdout && file != stdin) {
        fclose(file);
    }
}

void Log::setPath(const char *path) noexcept {
    file = fopen(path, "w+");
}

void Log::log(const char *format, ...) {
    va_list list;
    va_start(list, format);
    logv(format, list);
    va_end(list);
}

void Log::logv(const char *format, va_list args) {
    static int pid = 0;

    if (0 == pid) {
        pid = static_cast<int>(getpid());
    }

    char threadName[256];
    pthread_getname_np(pthread_self(), threadName, sizeof(threadName));

    __uint64_t tid;
    pthread_threadid_np(pthread_self(), &tid);


    //    std::time_t t = std::time(nullptr);
    //    char calStr[32];
    //    std::strftime(calStr, sizeof(calStr), "%Y-%m-%d", std::localtime(&t));

    std::timespec ts;
    std::timespec_get(&ts, TIME_UTC);
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %T", std::localtime(&ts.tv_sec));
    //    std::cout << "Current time: " << timeStr << '.' << ts.tv_nsec << " UTC\n";

    const char * appname = getprogname();

    std::ostringstream oss;
    auto nsecStr = std::to_string(ts.tv_nsec);

    oss << timeStr << '.' << nsecStr.substr(0, 3) << ' ' << appname << " [" << pid << '|' ;

    if (*threadName) {
        oss << threadName << '|';
    }

    oss << tid << "] ";

    char message[256];
    vsnprintf(message, sizeof(message), format, args);

    oss << message << std::endl;

    // lock
    pthread_mutex_lock(&mutex);

    this->__log(oss.str());

    fputs(oss.str().c_str(), this->getFile());

    pthread_mutex_unlock(&mutex);

}

const std::string &Log::getLastLog() const {
    if (logs.empty()) {
        static const std::string noLogMsg("no logs available");
        return noLogMsg;
    }
    return logs.back();
}




}

void WNLog(int val) {
    WNLog("%d", val);
}
void WNLog(long val) {
    WNLog("%ld", val);
}
void WNLog(char val) {
    WNLog("%c", val);
}

void WNLog(unsigned int val) {
    WNLog("%u", val);
}

void WNLog(unsigned long val) {
    WNLog("%lu", val);
}

void WNLog(unsigned char val) {
    WNLog("%uc", val);
}

void WNLog(float val) {
    WNLog("%f", val);
}

void WNLog(double val) {
    WNLog("%lf", val);
}


const char * WNLogGetLast(void) {
    return WN::Log::GetSharedLog().getLastLog().c_str();
}




void WNLog(const char * format, ...) {
    va_list args;
    va_start(args, format);
    WN::Log::GetSharedLog().logv(format, args);
    va_end(args);
}


void WNLogSetFile(FILE * file) {
    WN::Log::GetSharedLog().setFile(file);
}

void WNLogSetPath(const char * path) {
    WN::Log::GetSharedLog().setPath(path);
}

void WNLogv(const char * format, va_list args) {
    WN::Log::GetSharedLog().logv(format, args);
}
