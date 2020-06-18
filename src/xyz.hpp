#ifndef XYZ_HPP
#define XYZ_HPP

class XYZ{
private:
    double x;
    double y;
    double z;
public:
    XYZ();
    XYZ(double x, double y, double z);

    double getMagnitude() const;
    void multiplyByScalar(double s);

    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setVector(double x, double y, double z);
    void clear();
};

class ACCEL : public XYZ{ // Unit: m/s^2
private:
    
public:
    ACCEL();
    ACCEL(XYZ xyz, double unitDistInMeter, double unitTimeInSec);

    void setAcceleration(XYZ xyz, double unitDistInMeter, double unitTimeInSec);
};

class SPEED : public XYZ{ // Unit: m/s
private:

public:
    void accumulateAcceleration(ACCEL accel, double totalTimeInSec);
};

class DISPL : public XYZ{ // Unit: m
private:
    
public:
    void accumulateSpeed(SPEED speed, double totalTimeInSec);
    void accumulateSpeed(SPEED speed, ACCEL accel, double totalTimeInSec);
    
    SPEED getAverageSpeed(double totalTimeInsec);
};

#endif