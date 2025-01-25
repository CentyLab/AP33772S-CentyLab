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
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();  // Read the incoming character
    if (receivedChar == 'p') 	  // Print out profile
      usbpd.displayProfiles();	  
    else if (receivedChar == 'o'){ // On
      Serial.println("Output is ON");
      usbpd.setOutput(1);
    }
    else if(receivedChar == 'f'){ // Off
      Serial.println("Output is OFF");
      usbpd.setOutput(0);
    }
    else if(receivedChar == 't'){ // Print temperature
      Serial.print("Board temperature: ");
      Serial.print(usbpd.readTemp());
      Serial.println(" C");
    }
    else
      Serial.println("Command not found");
  }
}

