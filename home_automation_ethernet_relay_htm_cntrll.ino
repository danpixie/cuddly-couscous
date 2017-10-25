#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);



String readString = String(100); //string for fetching data from address

void setup()
{
  Serial.begin(9600);

  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

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

  Serial.print(F("Garage Door Opener Control Ready at IP address "));
  Serial.println(Ethernet.localIP());
}

void loop()
{
  // command received (one character)  '1' - activate garage door button
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
                   cmd = '1';
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
                          if (cmd == '2'){
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

             // add other commands here

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
   client.println(F("<title>Home Automation Control</title>"));

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

   client.println(F("Home Auotmation Control<br/><br/>"));

   // future idea: could read a limit switch on the garage door here and tell the user if the door is currently open or closed
   /*
   if (digitalRead(DOOR_OPEN_INPUT) == HIGH)
     client.println("Door is Open");
   else 
     client.println("Door is Closed");
   client.println("<br>");
   */

   // door open / close button
   if (digitalRead(6)==LOW)
   {
   client.println(F("<button onclick=\"OnButtonClicked('1');\">1ON</button><br/><br/>"));
   }if (digitalRead(6)==HIGH){
   client.println(F("<button onclick=\"OnButtonClicked('2');\">1Off</button><br/><br/>"));
   }   if (digitalRead(5)==LOW)
   {
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


   // add more buttons here
   // button separator



   client.println(F("</div>"));

   client.println(F("</body>"));
   client.println(F("</html>"));

   client.println("");
}
