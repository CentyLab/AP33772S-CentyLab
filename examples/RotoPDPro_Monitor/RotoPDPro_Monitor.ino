#include <INA238.h>
#include <Arduino.h>
#include <AP33772S.h>

/* WARNING: Ensure you are using the RotoPD Pro (240W) 48V version
 * The code can detroy the standard RotoPD as it cannot handle voltage higher than 30V
 */

/* 
* This example code query INA238 for voltage, current, and temperature information of power path
*/

#define avsVotlage 28000  // Target 30V through AVS mode

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
  ina238.setOverCurrentLimit(5000); // Max out 5A threshold
  ina238.setDiagnoseAlertBit(INA238_DIAG_ALERT_LATCH); //Set to Alert latch
}


void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  delay(1000); //Ensure everything got enough time to bootup
  AP33772S_Pro_init();

  // Set desire voltage and declare max expected current to charger/powerbank
  usbpd.setAVSPDO(usbpd.getAVSIndex(), avsVotlage, 4500);
  usbpd.setOutput(1); // AP33772S set output to ENABLE

}

void loop() {
  Serial.print("Current: ");
  Serial.print(ina238.getAmpere(), 3);
  Serial.println(" A");

  Serial.print("Bus Voltage: ");
  Serial.print(ina238.getBusVoltage(), 3);
  Serial.println(" V");

  Serial.print("Shunt Voltage: ");
  // Serial.print(ina238.getShuntVoltage_mV() * 1000.0); // Convert from mV to μV
  Serial.print(ina238.getShuntMilliVolt(), 3);
  Serial.println(" mV");

  Serial.print("Power: ");
  Serial.print(ina238.getWatt(), 3);
  Serial.println(" W");

  Serial.print("Temperature: ");
  Serial.print(ina238.getTemperature(), 3);
  Serial.println(" *C");
  Serial.println();
  delay(3000);
}

