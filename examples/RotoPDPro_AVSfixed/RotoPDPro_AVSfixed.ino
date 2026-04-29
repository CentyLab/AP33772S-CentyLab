#include <Arduino.h>
#include <AP33772S.h>
#include <INA238.h>
#include <arduino-timer.h>

/* WARNING: Ensure you are using the RotoPD Pro (240W) 48V version
 * The code can detroy the standard RotoPD as it cannot handle voltage higher than 30V
 */

#define avsVotlage 35000 //35V 

// put function declarations here:
AP33772S usbpd;
INA238 ina238(0x40);
auto timer = timer_create_default(); // create a timer with default settings

void AP33772S_Pro_init(){
  usbpd.clearConfig(AP33772_CONFIG::UVP_EN); // Required for RotoPD Pro. Prevent UVP from issuing hard reset.
  usbpd.begin(); // Load in profiles
  usbpd.setOutput(0); // Turn off output

  if(!ina238.begin())
  {
    Serial.println("Cannot find INA");
  }

  ina238.setADCRange(1);
  ina238.setMaxCurrentShunt(7, 0.005); // Based on RotoPD Pro schematic
  ina238.setShuntVoltageConversionTime(INA238_150_us);
  ina238.setAverage(INA238_16_SAMPLES); 
  ina238.setOverCurrentLimit(5100); // Allow max out 5A threshold
  ina238.setDiagnoseAlertBit(INA238_DIAG_ALERT_LATCH); //Set to Alert latch
}

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
  AP33772S_Pro_init();

  timer.every(1000, keepalive_AVS); // Call keepalive_AVS() every 1 second
  usbpd.setOutput(1);
}

void loop() {
  timer.tick(); // tick the timer
}

