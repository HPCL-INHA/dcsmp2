#include "dc_mqtt.hpp"

#include <iomanip>
#include <sstream>

#include <sys/time.h>

using namespace std;

string getMqttRecursiveTopic(string rootTopic){
    stringstream ss;
    if(!rootTopic.size())
        return rootTopic;
    ss << rootTopic;
    if(rootTopic[rootTopic.size() - 1] != '/')
        ss << '/';
    ss << "#";
    return ss.str();
}
string getMqttSubtopic(string rootTopic, deque<string> subtopics){
    stringstream ss;
    if(!rootTopic.size())
        return rootTopic;
    ss << rootTopic;
    if(rootTopic[rootTopic.size() - 1] != '/')
        ss << '/';
    for (auto iter = subtopics.begin(); iter != subtopics.end(); iter++){
        ss << *iter;
        if (!iter->compare("#"))
            break;
        ss << "/";
    }
    return ss.str();
}

DC_MQTT_DATETIME::DC_MQTT_DATETIME(){
    setToCurrentDatetime();
}
DC_MQTT_DATETIME::DC_MQTT_DATETIME(unsigned long long intTime, double decTime){
    setIntTime(intTime);
    setDecTime(decTime);
}
DC_MQTT_DATETIME::DC_MQTT_DATETIME(string dcMqttTimestamp){
    translateAndStoreDcMqttTimestamp(dcMqttTimestamp);
}

void DC_MQTT_DATETIME::setToCurrentDatetime(){
    time_t cur_time = time(NULL);
    struct tm *cur_tm = localtime(&cur_time);
    struct timeval cur_tv;
    gettimeofday(&cur_tv, NULL);
    int year = cur_tm->tm_year + 1900;
    int mon = cur_tm->tm_mon + 1;
    int day = cur_tm->tm_mday;
    int hour = cur_tm->tm_hour;
    int min = cur_tm->tm_min;
    double sec = cur_tm->tm_sec + cur_tv.tv_usec * 0.000001;

    setTime(year, mon, day, hour, min, sec);
}
void DC_MQTT_DATETIME::translateAndStoreDcMqttTimestamp(string dcMqttTimestamp){
    stringstream ss;
    ss << dcMqttTimestamp;
    string intTimeStr;
    getline(ss, intTimeStr, '.');
    string decTimeStr;
    getline(ss, decTimeStr);
    decTimeStr.insert(0, "0.");

    setDatetime(strtoull(intTimeStr.c_str(), NULL, 10), atof(decTimeStr.c_str()));
}

string DC_MQTT_DATETIME::getDcMqttTimestamp() const{
    stringstream decTimeSs;
    decTimeSs << fixed << setprecision(DC_MQTT_TIMESTAMP_DEC_PRECISION) << decTime;
    decTimeSs.seekg(1, decTimeSs.beg);
    string decTimeStr;
    decTimeSs >> decTimeStr;

    stringstream timestampSs;
    timestampSs << intTime;
    timestampSs << decTimeStr;
    return timestampSs.str();
}

double DC_MQTT_DATETIME::getSeconds() const{
    // this->intTime 은 yyyyMMddHHmmss
    // this->decTime 은 밀리세컨드
    // intTime, decTime 을 합쳐서 초 단위(double) 로 변환
    double sec = (double)(this->intTime % 100);
    sec += this->decTime / 1000.0;
    return sec;
}

unsigned long long DC_MQTT_DATETIME::getIntTime() const{
    return intTime;
}
double DC_MQTT_DATETIME::getDecTime() const{
    return decTime;
}

void DC_MQTT_DATETIME::setIntTime(unsigned long long intTime){
    this->intTime = intTime;
}
void DC_MQTT_DATETIME::setDecTime(double decTime){
    this->decTime = decTime;
}
void DC_MQTT_DATETIME::setDatetime(unsigned long long intTime, double decTime){
    setIntTime(intTime);
    setDecTime(decTime);
}
void DC_MQTT_DATETIME::setTime(int year, int mon, int day, int hour, int min, double sec){
    if (year < 0 || mon < 0 || day < 0 || hour < 0 || min < 0 || sec < 0){
        intTime = 0;
        decTime = 0.0;
        return;
    }
    stringstream intTimeSs;
    intTimeSs.fill('0');
    intTimeSs << year << setw(2) << mon << setw(2) << day << setw(2) << hour << setw(2) << min << setw(2) << (int)sec;

    intTime = strtoull(intTimeSs.str().c_str(), NULL, 10);
    decTime = sec - (int)sec;
}

// yyyyMMddHHmmss
inline int DC_MQTT_DATETIME::getYear() const {
    return (int)(this->intTime / 10000000000);
}

inline int DC_MQTT_DATETIME::getMonth() const {
    int month = (int)(this->intTime / 100000000);
    month %= 100;
    return month;
}

inline int DC_MQTT_DATETIME::getDay() const {
    int day = (int)(this->intTime / 1000000);
    day %= 100;
    return day;
}

inline int DC_MQTT_DATETIME::getHour() const {
    int hour = (int)(this->intTime / 10000);
    hour %= 100;
    return hour;
}

inline int DC_MQTT_DATETIME::getMinute() const {
    int minute = (int)(this->intTime / 100);
    minute %= 100;
    return minute;
}

inline int DC_MQTT_DATETIME::getIntegerSeconds() const {
    return (int)(this->intTime % 100);
}

inline double DC_MQTT_DATETIME::getMilliSeconds() const {
    return this->decTime;
}

int getLastDay(int year, int month) {
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
            return 29;
        else
            return 28;
    }
    return -1;
}

DC_MQTT_DATETIME getDiffernceBetweenDcMqttDatetimes(DC_MQTT_DATETIME destDatetime, DC_MQTT_DATETIME srcDatetime, bool *negative){
    // destDatetime 에서 srcDatetime을 뺀것을 반환
    // DC_MQTT_DATETIME 형식은 dc_mqtt.hpp 참고
    // intTime 부분은 yyyyMMddHHmmss, decTime 부분은 밀리세컨드 -> 뺄 때 각 부분의 형식에 맞게 빼야함
    // (if) dest-src 가 음수이면, negative = true

    // 임시
    if (!negative)
        negative = new bool;

    if (destDatetime.getIntTime() < srcDatetime.getIntTime())
        *negative = true;
    else if ((destDatetime.getIntTime() == srcDatetime.getIntTime()) && destDatetime.getMilliSeconds() < srcDatetime.getMilliSeconds())
        *negative = true;
    else
        *negative = false;
    
    int dYear = destDatetime.getYear() - srcDatetime.getYear();
    int dMonth = destDatetime.getMonth() - srcDatetime.getMonth();
    int dDay = destDatetime.getDay() - srcDatetime.getDay();
    int dHour = destDatetime.getHour() - srcDatetime.getHour();
    int dMinute = destDatetime.getMinute() - srcDatetime.getMinute();
    int dSeconds = destDatetime.getIntegerSeconds() - srcDatetime.getIntegerSeconds();
    double dMs = destDatetime.getMilliSeconds() - srcDatetime.getMilliSeconds();

    if (*negative) {
        dYear = -dYear;
        dMonth = -dMonth;
        dDay = -dDay;
        dHour = -dHour;
        dMinute = -dMinute;
        dSeconds = -dSeconds;
        dMs = -dMs;
    }

    if (dMs < 0.0) {
        dSeconds--;
        dMs += 1000.0;
    }
    if (dSeconds < 0) {
        dMinute--;
        dSeconds += 60;
    }
    if (dMinute < 0) {
        dHour--;
        dMinute += 60;
    }
    if (dHour < 0) {
        dDay--;
        dHour += 24;
    }
    if (dDay < 0) {
        dMonth--;
        if (*negative)
            dDay += getLastDay(srcDatetime.getYear(), srcDatetime.getMonth() - 1);
        else
            dDay += getLastDay(destDatetime.getYear(), destDatetime.getMonth() - 1);
    }
    if (dMonth < 0) {
        dYear--;
        dMonth += 12;
    }
    double dSec = (double)dSeconds + dMs / 1000.0;
    DC_MQTT_DATETIME datetime;
    datetime.setTime(dYear, dMonth, dDay, dHour, dMinute, dSec);

    // 임시
    delete negative;

    return datetime;
}