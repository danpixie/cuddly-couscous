//Include libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>


// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//int relaypin = 3;


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);


//-------variables and constants

//temp sensor variables
int temp1 = sensors.getTempCByIndex(0);
int temp2 = sensors.getTempCByIndex(1);

//webSite variable
String readString = String(100); //string for fetching data from address

//Time, millis
unsigned long BoostTimer = (0);
const long BoostInc = 1500000;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;


void setup()
{
  Serial.begin(9600); //Begin serial communication
  Serial.println("Arduino Digital Temperature // Serial Monitor Version"); //Print a message
  sensors.begin();
 // pinMode (relaypin, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  

  // start the Ethernet connection and the server:

  Serial.println(F("Initiaizing ethernet..."));

  // this uses a fixed address
  Ethernet.begin(mac, ip);

  // get an address with DHCP
  //if (Ethernet.begin(mac) == 0)
  //  Serial.println("Failed to configure Ethernet using DHCP");

  // give the card a second to initialize
  delay(1000);

  server.begin();

  Serial.print(F("RH Shower Control Ready at IP address "));
  Serial.println(Ethernet.localIP());
}

void loop()
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
  else { digitalWrite (relaypin, LOW);}*/
  delay(500);
  
  
  
  previousMillis = currentMillis;
  currentMillis = millis();
  BoostTimer = BoostTimer + previousMillis - currentMillis;
  
  
  // command received (one character)  '1' - increment BoostTimer button
  char cmd = 0;          // 1 - pulse button
  boolean done = false;  // set to indicate that response is complete

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {

    Serial.println(F("new client"));
    readString = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        //int i = c;
        //Serial.print("(");
        //Serial.print(i);
        //Serial.print(")");

        // store character received in receive string
        if (readString.length() < 100) {
          readString += (c);
        }

        // check for end of line
        if (c == '\n') {
            //Serial.print("Receved line: ");
            //Serial.print(readString); 

            // process line if its the "GET" request
            // a request looks like "GET /?1" or "GET /?2"
            if (readString.indexOf("GET") != -1) {
                if (readString.indexOf("?1") != -1)
                   BoostTimer = BoostTimer + BoostInc;
                       if (BoostTimer>6000000){
                           BoostTimer = 0;}
                // check for other commands here. ie turn on light, etc.
                if (readString.indexOf("?2") != -1)
                   cmd = '2'; 
                                  if (readString.indexOf("?3") != -1)
                   cmd = '3';
                                  if (readString.indexOf("?4") != -1)
                   cmd = '4';
                                  if (readString.indexOf("?5") != -1)
                   cmd = '5';
                                  if (readString.indexOf("?6") != -1)
                   cmd = '6';
                                  if (readString.indexOf("?7") != -1)
                   cmd = '7';
                                 if (readString.indexOf("?8") != -1)
                   cmd = '8';
   
            }             

           // if a blank line was received (just cr lf, length of 2), then its the end of the request
           if (readString.length() == 2) {
             if (cmd == '1'){
                Serial.println(F("Activate Button"));
                digitalWrite(6, HIGH);

             }
               /*           if (cmd == '2'){
                Serial.println(F("Activate Button"));
                digitalWrite(6, LOW);

             }
                          if (cmd == '3'){
                Serial.println(F("Activate Button"));
                digitalWrite(5, HIGH);

             }
                          if (cmd == '4'){
                Serial.println(F("Activate Button"));
                digitalWrite(5, LOW);

             }
                          if (cmd == '5'){
                Serial.println(F("Activate Button"));
                digitalWrite(7, HIGH);

             }
                          if (cmd == '6'){
                Serial.println(F("Activate Button"));
                digitalWrite(7, LOW);

             }
                          if (cmd == '7'){
                Serial.println(F("Activate Button"));
                digitalWrite(8, HIGH);

             }
                          if (cmd == '8'){
                Serial.println(F("Activate Button"));
                digitalWrite(8, LOW);

             }
             */

             // add other commands here
             
              Serial.println(BoostTimer);
              
             // send web page back to client
             Serial.println(F("sending web page"));
             SendWebPage(client);
             Serial.println(F("web page sent"));

             cmd = 0;

             // break out and disconnect. This will tell the browser the request is complete without having to specify content-length
             break;

           }  // end of request reached

           // start line over           
           readString = "";
        }  // end of line reached
      }  // end data is available from client
    }  // end cient is connected
    // give the web browser time to receive the data
    Serial.println(F("delay before disconnect"));
    delay(100);
    // close the connection:
    client.stop();
    Serial.println(F("client disonnected"));
  }  // end client has been created
}

// send web page
void SendWebPage(EthernetClient client)
{
   client.println(F("HTTP/1.1 200 OK"));
   client.println(F("Content-Type: text/html"));
   // to specify the length, wooul have to construct the entire string and then get its length
   //client.println("Content-Length: 1234");
   client.println(F("Connnection: close"));
   client.println();

   client.println(F("<!DOCTYPE HTML>"));
   client.println(F("<html>"));
   client.println(F("<head>"));
   client.println(F("<title>RH HotWaterControl</title>"));

   client.println(F("<style type='text/css'>"));
   client.println(F(".label {font-size: 30px; text-align:center;}"));
   client.println(F("button {width: 160px; height: 70px; font-size: 30px; -webkit-appearance: none; background-color:#dfe3ee; }"));
   client.println(F("</style>"));

   client.println(F("<script type='text/javascript'>"));

   client.println(F("function OnButtonClicked(parm) { window.location.href=\"X?\" + parm; }"));

   client.println(F("</script>"));

   client.println(F("</head>"));

   client.println(F("<body style=\"background-color:#3b5998\">"));

   client.println(F("<div class=\"label\">"));

   client.println(F("RH HotWaterControl<br/><br/>"));
   client.println(F("TopOfTank"));
   client.print(temp1);
   client.print("C</br>");
   
   client.println(F("BottomOfTank"));
   client.print(temp2);
   client.print("C</br>");

   // future idea: could read a limit switch on the garage door here and tell the user if the door is currently open or closed
   /*
   if (digitalRead(DOOR_OPEN_INPUT) == HIGH)
     client.println("Door is Open");
   else 
     client.println("Door is Closed");
   client.println("<br>");
   */

   // door open / close button
  // if (digitalRead(6)==LOW)
   //{
   client.println(F("<button onclick=\"OnButtonClicked('1');\">Boost25m</button><br/><br/>"));
   client.println(F("<a href=192.168.1.177><button>reload variables</button></a><br/><br/>"));
   client.println("Boosted for");
   client.print (BoostTimer / 60000);
   client.print ("min");
   client.print ("<br/><br/>");

   //}if (digitalRead(6)==HIGH){
   //client.println(F("<button onclick=\"OnButtonClicked('2');\">1Off</button><br/><br/>"));
   //}   if (digitalRead(5)==LOW)
   /*{
   client.println(F("<button onclick=\"OnButtonClicked('3');\">2ON</button><br/><br/>"));
   }if (digitalRead(5)==HIGH){
   client.println(F("<button onclick=\"OnButtonClicked('4');\">2Off</button><br/><br/>"));
   }  if (digitalRead(7)==LOW)
   {
   client.println(F("<button onclick=\"OnButtonClicked('5');\">3ON</button><br/><br/>"));
   }if (digitalRead(7)==HIGH){
   client.println(F("<button onclick=\"OnButtonClicked('6');\">3Off</button><br/><br/>"));
     }   if (digitalRead(8)==LOW)
   {
   client.println(F("<button onclick=\"OnButtonClicked('7');\">4ON</button><br/><br/>"));
   }if (digitalRead(8)==HIGH){
     
   client.println(F("<button onclick=\"OnButtonClicked('8');\">4Off</button><br/><br/>"));
   }
   */

   // add more buttons here
   // button separator



   client.println(F("</div>"));

   client.println(F("</body>"));
   client.println(F("</html>"));

   client.println("");
}
