#include <INA238.h>
#include <Arduino.h>
#include <AP33772S.h>

/* WARNING: Ensure you are using the RotoPD Pro (240W) 48V version
 * The code can detroy the standard RotoPD as it cannot handle voltage higher than 30V
 */

/* 
* This example code request [avsVotlage] and set the OCP threshold at [overCurrentThreshold]
* When current exceed this OCP limit, output will shutoff, and stay off (latched)
* Calling getDiagnoseAlert() function will give control back to AP33772S if the overcurrent even no longer exist
*/

#define avsVotlage 35000  // Target 35V through AVS mode
#define overCurrentThreshold 2750  // Circuit breaker activate at 2750 mA

// put function declarations here:
AP33772S usbpd;
INA238 ina238(0x40);

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
  ina238.setOverCurrentLimit(overCurrentThreshold); // Set circuit breaker threshold
  // ina238.setShuntOvervoltageTH(8000); // 8000 = 2A, 12000 = 3A
  ina238.setDiagnoseAlertBit(INA238_DIAG_ALERT_LATCH); //Set to Alert latch
}


void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  delay(1000); //Ensure everything got enough time to bootup
  AP33772S_Pro_init();

  // Set desire voltage and declare max expected current to SOURCE
  usbpd.setAVSPDO(usbpd.getAVSIndex(), avsVotlage, 4500);
  usbpd.setOutput(1); // AP33772S set output to ENABLE

}

void loop() {
  // ina238.getDiagnoseAlert(); // Call this fuction to clear over current protection, give control back to AP33772S IC
}

