// Import Wifi and Webserver libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Import NTP libraries
#include <NTPClient.h>
#include <WiFiUdp.h> 

// Include FastLed
#include <fastLED.h>

// Include WebPage variable
#include "html_page.ino"

// Declare WI-FI credentials 
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

// Initilize mode variable to keep track of the mode selected in the Webpage
String mode = "clock"; 

uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

void setup() {
    
    Serial.begin(115200);
    
    // Initialize WebServer
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    server.on("/updateColor", HTTP_POST, handleColor)
  
    server.on("/", handle_OnConnect);
    server.on("/clock", handleClockMode);
    server.on("/timer", handleTimerMode);
    server.on("/waves", handleWavesMode);
    // server.on("/led1on", handle_led1on);
    // server.on("/led1off", handle_led1off);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.println("HTTP server started");

    // Initialize a NTPClient to get time
    timeClient.begin();
    // Set offset time in seconds to adjust for your timezone, for example:
    // GMT +1 = 3600
    // GMT +8 = 28800
    // GMT -1 = -3600
    // GMT 0 = 0
    timeClient.setTimeOffset(0);
}

void loop() {
    server.handleClient();
  
  if (mode == "clock") {
    timeClient.update(); 
    
    int currentHour = timeClient.getHours();
    Serial.print("Hour: ");
    Serial.println(currentHour);  

    int currentMinute = timeClient.getMinutes();
    Serial.print("Minutes: ");
    Serial.println(currentMinute);
  } 

  else if (mode == "timer") {

  }

  else if (mode = "waves") {
      
  }
//   if(LED1status)
//   {digitalWrite(LED1pin, HIGH);}
//   else
//   {digitalWrite(LED1pin, LOW);}
  
//   if(LED2status)
//   {digitalWrite(LED2pin, HIGH);}
//   else
//   {digitalWrite(LED2pin, LOW);}
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handleClockMode() {
    mode = "clock";
    // server.send(200, "text/html")
}

void handleTimerMode() {
    mode = "timer";
    // server.send(200, "text/html")
}

void handleWavesMode() {
    mode = "waves";
    // server.send(200, "text/html")
}

// void handle_led1on() {
//   LED1status = HIGH;
//   Serial.println("GPIO7 Status: ON");
//   server.send(200, "text/html", SendHTML(true,LED2status)); 
// }

// void handle_led1off() {
//   LED1status = LOW;
//   Serial.println("GPIO7 Status: OFF");
//   server.send(200, "text/html", SendHTML(false,LED2status)); 
// }

void handleNotFound(){
  server.send(404, "text/plain", "Not found");
}

void handleColor() {
    // Handle invalid data
    if(!server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b") 
      || server.arg("r") == NULL || server.arg("g") == NULL || server.arg("b") == NULL) { 
    server.send(400, "text/plain", "400: Invalid Request");
    return;
    }
    // Update color here
}
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}