#include <Arduino.h>
#include <AP33772S.h>

/** To modify for higher voltage battery:
 *	Change BATT_START_VOLTAGE to your dead battery voltage
 * 	Change BATT_END_VOLTAGE to the full charge of your battery
 *  Change BATT_MAX_CURRENT to the fastest charging current desire
 *	Change BATT_END_CURRENT to the desire cut of current, consider complete charging
 *	Change PPS_PROFILE_LOCATION to the location of the detected PPD profile on your charger
 */

#define BATT_START_VOLTAGE 3300	//3.3V
#define BATT_END_VOLTAGE 4200 	//4.2V
#define BATT_MAX_CURRENT 900 	  //900mA
#define BATT_END_CURRENT 150 	  //150mA
#define PPS_PROFILE_LOCATION	6 //Get from profile read. See SerialProfileOnOff example.

// put function declarations here:
AP33772S usbpd;

int reqvoltage; //Request voltage send to the charger
int done_charging = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  delay(1000); 
  usbpd.begin();
  usbpd.setPPSPDO(6,3300,1000);
}

void loop() {
  while (done_charging == 0)
  {
    Serial.print("V:");
    Serial.println(usbpd.readVoltage());
    Serial.print("I:");
    Serial.print(usbpd.readCurrent());
    
    if (usbpd.readCurrent() < BATT_MAX_CURRENT)
    {
      reqvoltage = reqvoltage + 100; //Increasing voltage to hit current target
      if (reqvoltage == BATT_END_VOLTAGE + 100) //if overstep by 100mV
        reqvoltage = BATT_END_VOLTAGE; //Cap back to limit
        
      usbpd.setPPSPDO(PPS_PROFILE_LOCATION, reqvoltage, 3000);
    }	
	
    if(usbpd.readCurrent() < 150) //Charge is now completed
    {
      usbpd.setOutput(0); //Turn of output
      done_charging = 1;
    }
    delay(1000);
    }
}

