#include <Arduino.h>
#include <AP33772S.h>

// put function declarations here:
AP33772S usbpd;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  delay(1000); //Ensure everything got enough time to bootup
  usbpd.begin();
}

void loop() {
  usbpd.setOutput(1);
  if(usbpd.getAVSIndex() > 0)
  {
    for(int i = 15000; i <= 30000; i=i+1000)
      {
        usbpd.setAVSPDO(usbpd.getAVSIndex(), i, 3000);
        delay(600);
      }
  }
}

