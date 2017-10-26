// -----LIBRARIES

// ----CONSTANTS (won't change)

//------- VARIABLES (will change)

//=======

void setup() {

 Serial.begin(9600);
Serial.println("Starting DraftSketchForHotWaterController.ino"); // so we know what sketch is running

//=======

void loop() {

// Notice that none of the action happens in loop() apart from reading millis()
     //   it just calls the functions that have the action code

 currentMillis = millis();   // capture the latest value of millis()
                             //   this is equivalent to noting the time from a clock
                             //   use the same time for all LED flashes to keep them synchronized
 
 readButton();               // call the functions that do the work
 readWebsite();
 BoostCountDown();
 ReadTankTemperatures();
 ReadRoomTempHumi();
 updateLedState();
 updateWebPage();
 BoilerControl();
 TowelRailControl();


}

//========

void readButton() {

     // this only reads the button state after the button interval has elapsed
     //  this avoids multiple flashes if the button bounces
     // every time the button is pressed it changes buttonLed_State causing the Led to go on or off
     // Notice that there is no need to synchronize this use of millis() with the flashing Leds
 
 if (millis() - previousButtonMillis >= buttonInterval) {
   //if (digitalRead(buttonPin) == LOW) {
   //buttonLed_State = ! buttonLed_State; // this changes it to LOW if it was HIGH
                                          //   and to HIGH if it was LOW
    // previousButtonMillis += buttonInterval;
       //how do I add boostInterval to boostCountdown when button is pressed

   }
 }

}

//========
