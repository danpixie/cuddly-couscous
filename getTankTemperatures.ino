//

//Include libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
int temp1 = sensors.getTempCByIndex(0);
int temp2 = sensors.getTempCByIndex(1);
//int relaypin = 3;

void setup(void)
{
  Serial.begin(9600); //Begin serial communication
  Serial.println("Arduino Digital Temperature // Serial Monitor Version"); //Print a message
  sensors.begin();
 // pinMode (relaypin, OUTPUT);
}

void loop(void)
{
  // Send the command to get temperatures
  sensors.requestTemperatures(); 
  Serial.print("Temperature1 is: ");
  Serial.println(temp1); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print("Temperature2 is: ");
  Serial.println(temp2);
   //Update value every 1 sec.
   temp1 = sensors.getTempCByIndex(0);
   temp2 = sensors.getTempCByIndex(1);
  Serial.println(temp1) ;
  Serial.println(temp2);
 /* if (temp1 > temp2)
  {digitalWrite (relaypin, HIGH); }
  else { digitalWrite (relaypin, LOW);}
  delay(1000);*/ 
}

