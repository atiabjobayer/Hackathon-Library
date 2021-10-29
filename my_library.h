#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H
#include <Arduino.h>
#include <string>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
class FlutterSensors
{
public:
    std::string characteristicUUID, serviceUUID, deviceName;
    int forwardThreshold, backwardThreshold, leftThreshold, rightThreshold;
    String networkName;

    // FlutterSensors(int forwardThreshold, int backwardThreshold, int leftThreshold, int rightThreshold);

    FlutterSensors(String network_name, int forward_threshold, int backward_threshold, int left_threshold, int right_threshold);
    FlutterSensors(String network_name);
    //FlutterSensors(String networkName);

    void setThreshold(String tiltDirection, int value);
    
    bool leftTilt();
    bool rightTilt();
    bool fowardTilt();
    bool backwardTilt();

    double degreeOfRotation();
    
    
    void checkConnectionStatus();

private:
    int x, y, z;
};
#endif
