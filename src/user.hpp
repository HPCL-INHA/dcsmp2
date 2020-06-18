#ifndef USER_HPP
#define USER_HPP

#include "xyz.hpp"

#include <set>
#include <map>
#include <string>

class USER{
private:
    std::string userId;
    std::map<std::string, std::set<std::string>> deviceIdToSensorIds;
    
    size_t curSteps;
    
    bool calibrating = false;
    std::string startingDcMqttTimestamp; // Starting Time of Calibration
    size_t lastAccumulatedSteps; // Last Accumulated Steps Before Calibration

    SPEED temporalSpeed; // Temporal Speed Accumulating Temporal Acceleartion
    DISPL cumulativeDisplacement; // Temporal Displacement Accumulating Temporal Speed

    std::string endDcMqttTimestamp; // End Time of Calibration
    size_t finalAccumulatedSteps; // Final Accumulated Steps After Calibration

    size_t stepsDuringCalibration; // (finalAccumulatedSteps) - (lastAccumulatedSteps)
    double calibratedStride = 0.0; // (cumulativeDisplacement) / (stepsDuringCalibration)

    double durationTimeInSec; // (endDcMqttTimestamp) - (startingDcMqttTimestamp)
    double calibratedWalkingSpeed = 0.0; // (cumulativeDisplacement) / (durationTimeInSec)
public:
    USER();
    USER(std::string userId, std::string deviceId);

    bool isCalibrating() const;
    void startCalibration(std::string timestamp);
    void progressCalibration(ACCEL accel, double totalTimeInSec);
    void endCalibration(std::string timestamp);
    void printCalibrationDebugMessage();

    void attachDevice(std::string deviceId);
    void attachSensor(std::string deviceId, std::string sensorId);
    std::set<std::string> getDeviceList() const;
    std::set<std::string> getSensorList(std::string deviceId);

    double getCalibratedStride(bool *valid) const;
    double getCalibratedWalkingSpeed(bool *valid) const;

    size_t getCurrentSteps() const;
    std::string getUserId() const;

    void setCurrentSteps(size_t steps);
    void setUserId(std::string userId);
};

#endif