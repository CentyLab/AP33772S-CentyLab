#include <Arduino.h>
#include <AP33772S.h>
#include <INA238.h>

/* WARNING: Ensure you are using the RotoPD Pro (240W) 48V version
 * The code can detroy the standard RotoPD as it cannot handle voltage higher than 30V
 */
 
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
  ina238.setOverCurrentLimit(5100); // Allow max out 5A threshold
  ina238.setDiagnoseAlertBit(INA238_DIAG_ALERT_LATCH); //Set to Alert latch
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  delay(1000); //Ensure everything got enough time to bootup
  AP33772S_Pro_init();
  usbpd.setOutput(1); //Enable output
}

void loop() {
  if(usbpd.getAVSIndex() > 0)
  {	//Request from 15V to 48V 
    for(int i = 15000; i <= 48000; i=i+1000)
      {
        usbpd.setAVSPDO(usbpd.getAVSIndex(), i, 3000);
        delay(600);
      }
  }
}

