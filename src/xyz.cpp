#include "xyz.hpp"

#include <cmath>

using namespace std;

XYZ::XYZ(){
    clear();
}
XYZ::XYZ(double x, double y, double z){
    setVector(x, y, z);
}

double XYZ::getMagnitude() const{
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}
void XYZ::multiplyByScalar(double s){
    setX(x * s);
    setY(y * s);
    setZ(z * s);
}

double XYZ::getX() const{
    return x;
}
double XYZ::getY() const{
    return y;
}
double XYZ::getZ() const{
    return z;
}

void XYZ::setX(double x){
    this->x = x;
}
void XYZ::setY(double y){
    this->y = y;
}
void XYZ::setZ(double z){
    this->z = z;
}
void XYZ::setVector(double x, double y, double z){
    setX(x);
    setY(y);
    setZ(z);
}
void XYZ::clear(){
    setX(0);
    setY(0);
    setZ(0);
}

ACCEL::ACCEL(){}
ACCEL::ACCEL(XYZ xyz, double unitDistInMeter, double unitTimeInSec){
    setAcceleration(xyz, unitDistInMeter, unitTimeInSec);
}

void ACCEL::setAcceleration(XYZ xyz, double unitDistInMeter, double unitTimeInSec){
    xyz.multiplyByScalar(unitDistInMeter / unitTimeInSec);
    setVector(xyz.getX(), xyz.getY(), xyz.getZ());
}

void SPEED::accumulateAcceleration(ACCEL accel, double totalTimeInSec){
    setVector(getX() + accel.getX() * totalTimeInSec, getY() + accel.getY() * totalTimeInSec, getZ() + accel.getZ() * totalTimeInSec);
}

void DISPL::accumulateSpeed(SPEED speed, double totalTimeInSec){
    setVector(getX() + speed.getX() * totalTimeInSec, getY() + speed.getY() * totalTimeInSec, getZ() + speed.getZ() * totalTimeInSec);
}
void DISPL::accumulateSpeed(SPEED speed, ACCEL accel, double totalTimeInSec){
    setVector(getX() + speed.getX() * totalTimeInSec + 0.5 * accel.getX() * pow(totalTimeInSec, 2),
        getY() + speed.getY() * totalTimeInSec + 0.5 * accel.getY() * pow(totalTimeInSec, 2),
        getZ() + speed.getZ() * totalTimeInSec + 0.5 * accel.getZ() * pow(totalTimeInSec, 2));
}
    
SPEED DISPL::getAverageSpeed(double totalTimeInSec){
    SPEED speed;
    speed.setVector(getX(), getY(), getZ());
    speed.multiplyByScalar(1.0 / totalTimeInSec);
    return speed;
}