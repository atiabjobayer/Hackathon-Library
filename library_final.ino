#include "my_library.h"
String networkName = "Amader JRC";
FlutterSensors fs(networkName,2,2,2,2);

void setup()
{
  Serial.begin(115200);
  
}
void loop()
{
  fs.checkConnectionStatus();
}
