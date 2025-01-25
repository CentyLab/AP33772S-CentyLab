#include <Arduino.h>
#include <AP33772S.h>

// put function declarations here:
AP33772S usbpd;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  delay(1000); //Ensure everything got enough time to bootup
  usbpd.begin();
}

void loop() {
  usbpd.setOutput(1);
  if(usbpd.getPPSIndex() > 0)
  {
    for(int i = 3600; i <= 20000; i=i+100) // Cycle between 3.3V to 20V, 600ms step
      {
        usbpd.setPPSPDO(usbpd.getPPSIndex(), i, 3000);
        delay(600);
      }
  }
}

