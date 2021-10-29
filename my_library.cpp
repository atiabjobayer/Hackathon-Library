#include "my_library.h"
#include <string>
#include <cstring>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// FlutterSensors::FlutterSensors(int forward_threshold = 2, int backward_threshold = 2, int left_threshold = 2, int right_threshold = 2)
// {
//     serviceUUID = SERVICE_UUID;
//     characteristicUUID = CHARACTERISTIC_UUID;

//     forwardThreshold = forward_threshold;
//     backwardThreshold = backward_threshold;
//     leftThreshold = left_threshold;
//     rightThreshold = right_threshold;
// }

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
    double valAra[3];
    
    void string_to_array(std::string &str){
        //static double out[3];
        std::string tmp="";
        int len=0;
        for (int i=0; i<str.length(); i++){
            if (str[i]==' '){
                valAra[len++]=stod(tmp),
                tmp="";
            }
            else
                tmp+=str[i];
        }
        valAra[len]=stod(tmp);
    }
    
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();

        if (value.length() > 0)
        {

            /*Serial.println("**********");
            Serial.print("New value: ");

            for (int i = 0; i < value.length(); i++)
            {
                Serial.print(value[i]);
            }

            Serial.println();
            Serial.println("**********");*/
            string_to_array(value);

            Serial.println(valAra[0]);
            Serial.println(valAra[1]);
            Serial.println(valAra[2]);

            Serial.println();
            Serial.println();
        }
    }
};

FlutterSensors::FlutterSensors(String networkName, int forward_threshold, int backward_threshold, int left_threshold, int right_threshold)
{
    serviceUUID = SERVICE_UUID;
    characteristicUUID = CHARACTERISTIC_UUID;

    forwardThreshold = forward_threshold;
    backwardThreshold = backward_threshold;
    leftThreshold = left_threshold;
    rightThreshold = right_threshold;

    char nname[255];

    for(int i = 0; i < networkName.length(); i++){
      nname[i] = networkName[i];
     }
    
    // Create the BLE Device
    BLEDevice::init(nname);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);

    pCharacteristic->setCallbacks(new MyCallbacks());

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting for connection...");
}


FlutterSensors::FlutterSensors(String networkName)
{
    forwardThreshold = 2,  backwardThreshold = 2,  leftThreshold = 2,  rightThreshold = 2;
    serviceUUID = SERVICE_UUID;
    characteristicUUID = CHARACTERISTIC_UUID;

    /*forwardThreshold = forward_threshold;
    backwardThreshold = backward_threshold;
    leftThreshold = left_threshold;
    rightThreshold = right_threshold;*/

    char nname[255];

    for(int i = 0; i < networkName.length(); i++){
      nname[i] = networkName[i];
     }
    
    // Create the BLE Device
    BLEDevice::init(nname);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);

    pCharacteristic->setCallbacks(new MyCallbacks());

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting for connection...");
}

void FlutterSensors::checkConnectionStatus()
{
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        //Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }

    delay(20);
}

void FlutterSensors::setThreshold(String tiltDirection, int threshold){
  tiltDirection.toLowerCase();
  /*switch(tiltDirection){
    case "right":
      rightThreshold = threshold;
      break;
     case "left":
      leftThreshold = threshold;
      break;
     case "forward":
      forwardThreshold = threshold;
      break;
     case "backward":
      backwardThreshold = threshold;
      break;
  }*/
  if(tiltDirection == "right")rightThreshold = threshold;
  else if(tiltDirection == "left")leftThreshold = threshold;
  else if(tiltDirection == "forward")forwardThreshold = threshold;
  else if(tiltDirection == "backward")backwardThreshold = threshold;
  
}

bool FlutterSensors::leftTilt(){return leftThreshold >= x;}
bool FlutterSensors::rightTilt(){return rightThreshold >= x;}
bool FlutterSensors::forwardTilt(){return forwardThreshold >= x;}
bool FlutterSensors::backwardTilt(){return backwardThreshold >= x;}
