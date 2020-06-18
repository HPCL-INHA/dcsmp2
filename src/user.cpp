#include "dc_mqtt.hpp"
#include "user.hpp"

#include <iostream>

using namespace std;

USER::USER(){}
USER::USER(string userId, string deviceId){
    setUserId(userId);
    attachDevice(deviceId);
}

bool USER::isCalibrating() const{
    return calibrating;
}
void USER::startCalibration(string timestamp){
    calibrating = true;
    lastAccumulatedSteps = curSteps;
    startingDcMqttTimestamp = timestamp;

    temporalSpeed.clear();
    cumulativeDisplacement.clear();
}
void USER::progressCalibration(ACCEL accel, double totalTimeInSec){
    cumulativeDisplacement.accumulateSpeed(temporalSpeed, accel, totalTimeInSec);
    temporalSpeed.accumulateAcceleration(accel, totalTimeInSec);
}
void USER::endCalibration(string timestamp){
    calibrating = false;
    finalAccumulatedSteps = curSteps;
    endDcMqttTimestamp = timestamp;

    stepsDuringCalibration = finalAccumulatedSteps - lastAccumulatedSteps;

    DC_MQTT_DATETIME startingDatetime(startingDcMqttTimestamp);
    DC_MQTT_DATETIME endDatetime(endDcMqttTimestamp);
    durationTimeInSec = getDiffernceBetweenDcMqttDatetimes(endDatetime, startingDatetime, NULL).getSeconds();

    if(durationTimeInSec != 0.0)
        calibratedWalkingSpeed = cumulativeDisplacement.getMagnitude() / durationTimeInSec;
    else
        calibratedWalkingSpeed = 0.0;

    if(stepsDuringCalibration != 0.0)
        calibratedStride = cumulativeDisplacement.getMagnitude() / stepsDuringCalibration;
    else
        calibratedStride = 0.0;
}
void USER::printCalibrationDebugMessage(){
    cout << endl;
    cout << "Printing calibration debug message... " << endl;
    cout << "[userId]: " << userId << endl;
    for (auto iter = deviceIdToSensorIds.begin(); iter != deviceIdToSensorIds.end(); iter++){
        cout << "[deviceId]: " << iter->first << endl;
        cout << "L [sensorId]: { ";
        for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
            cout << *iter2 << " ";
        cout << "}" << endl;
    }

    cout << endl;
    cout << "[startingDcMqttTimestamp]: " << startingDcMqttTimestamp << endl;
    cout << "[endDcMqttTimestamp]: " << endDcMqttTimestamp << endl;
    cout << "[durationTimeInSec]: " << durationTimeInSec << "(s)" << endl;

    cout << endl;
    cout << "[cumulativeDisplacement]: ( " << cumulativeDisplacement.getX() << ", " << cumulativeDisplacement.getY() << ", " << cumulativeDisplacement.getZ() << " )" << endl;
    cout << "[cumulativeDisplacement.getMagnitude()]: " << cumulativeDisplacement.getMagnitude() << "(m)" << endl;

    cout << endl;
    cout << "[lastAccumulatedSteps]: " << lastAccumulatedSteps << endl;
    cout << "[finalAccumulatedSteps]: " << finalAccumulatedSteps << endl;
    cout << "[stepsDuringCalibration]: " << stepsDuringCalibration << endl;

    cout << endl;
    cout << "[calibratedWalkingSpeed]: " << calibratedWalkingSpeed << "(m/s)" << endl;
    cout << "[calibratedStride]: " << calibratedStride << "(m)" << endl;

    cout << endl;
}

void USER::attachDevice(string deviceId){
    deviceIdToSensorIds[deviceId] = set<string>();
}
void USER::attachSensor(string deviceId, string sensorId){
    deviceIdToSensorIds[deviceId].insert(sensorId);
}
set<string> USER::getDeviceList() const{
    set<string> deviceSet;
    for(auto iter = deviceIdToSensorIds.begin(); iter != deviceIdToSensorIds.end(); iter++)
        deviceSet.insert(iter->first);
    return deviceSet;
}
set<string> USER::getSensorList(string deviceId){
    return deviceIdToSensorIds[deviceId];
}

double USER::getCalibratedStride(bool *valid) const{
    if(calibratedStride != 0.0)
        *valid = true;
    else
        *valid = false;
    
    return calibratedStride;
}
double USER::getCalibratedWalkingSpeed(bool *valid) const{
    if(calibratedWalkingSpeed != 0.0)
        *valid = true;
    else
        *valid = false;
    
    return calibratedWalkingSpeed;
}

size_t USER::getCurrentSteps() const{
    return curSteps;
}
string USER::getUserId() const{
    return userId;
}

void USER::setCurrentSteps(size_t steps){
    this->curSteps = steps;
}
void USER::setUserId(string userId){
    this->userId = userId;
}