#ifndef DC_MQTT_HPP
#define DC_MQTT_HPP

#include <deque>
#include <string>

const auto DC_MQTT_KEEPALIVE_VAL = 20;
const auto DC_MQTT_QOS_LEVEL = 1;

const char DC_MQTT_ACTION_TOPIC[] = "buddybot/action/";
const char DC_MQTT_SENSOR_TOPIC[] = "buddybot/sensordata/";
const char DC_MQTT_EAM_TOPIC[] = "buddybot/eam/";

std::string getMqttRecursiveTopic(std::string rootTopic);
std::string getMqttSubtopic(std::string rootTopic, std::deque<std::string> subtopics);

const auto DC_MQTT_TIMESTAMP_DEC_PRECISION = 3;
class DC_MQTT_DATETIME{
private:
    unsigned long long intTime; // yyyyMMddHHmmss
    double decTime; // milliseconds
public:
    DC_MQTT_DATETIME();
    DC_MQTT_DATETIME(unsigned long long intTime, double decTime);
    DC_MQTT_DATETIME(std::string dcMqttTimestamp);

    void setToCurrentDatetime();
    void translateAndStoreDcMqttTimestamp(std::string dcMqttTimestamp);

    std::string getDcMqttTimestamp() const;

    double getSeconds() const;

    unsigned long long getIntTime() const;
    double getDecTime() const;

    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int getHour() const;
    int getMinute() const;
    int getIntegerSeconds() const;
    double getMilliSeconds() const;

    void setIntTime(unsigned long long intTime);
    void setDecTime(double decTime);
    void setDatetime(unsigned long long intTime, double decTime);
    void setTime(int year, int mon, int day, int hour, int min, double sec);
};
DC_MQTT_DATETIME getDiffernceBetweenDcMqttDatetimes(DC_MQTT_DATETIME destDatetime, DC_MQTT_DATETIME srcDatetime, bool *negative);

#endif