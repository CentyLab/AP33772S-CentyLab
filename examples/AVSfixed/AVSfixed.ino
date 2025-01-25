#include <Arduino.h>
#include <AP33772S.h>
#include <arduino-timer.h>

#define avsVotlage 16000 //16V 

// put function declarations here:
AP33772S usbpd;
auto timer = timer_create_default(); // create a timer with default settings

/**
  * Some charger will disconnect with sink if no refresh request is sent within 1s
  * Spamming request to keep power on
*/
bool keepalive_AVS(void *) {
  usbpd.setAVSPDO(usbpd.getAVSIndex(), avsVotlage, 3000);
  return true; // keep timer active? true
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  delay(1000); //Ensure everything got enough time to bootup
  usbpd.begin();

  timer.every(1000, keepalive_AVS); // Call keepalive_AVS() every 1 second
  usbpd.setOutput(1);
}

void loop() {
  timer.tick(); // tick the timer
}

