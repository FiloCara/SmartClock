#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <FastLED.h>
#include <fastpin.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <fastspi.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

// Import Wifi and Webserver libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Import NTP libraries
#include <NTPClient.h>
#include <WiFiUdp.h> 

// Import FastLed
//#include <fastLED.h>

// Import credentials 
#include "secrets.h"

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

/* Put IP Address details */
IPAddress local_ip(192,168,1,100);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

// Initilize mode variable to keep track of the mode selected in the Webpage
String mode = "clock"; 

// Initialize r, g, b variables
int r = 255;
int g = 255;
int b = 255;

// Initialize LED object TODO

// Initalize grid led matrix with index position --> It simplify the development
int matrix[10][11] = {{115, 114,113,112,111,110,109,108,107,106,105},
                      {94, 95, 96, 97, 98, 99,100,101,102,103,104},
                      {93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83},
                      {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82},
                      {71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61},
                      {50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60},
                      {49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39},
                      {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38},
                      {27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17},
                      { 6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16}};

// %%%%%%Function prorotypes%%%%%%
// Server
void handleColor();
void handleOnConnect();
void handleClockMode();
void handleTimerMode();
void handleWavesMode();
void handleNotFound();
// Clock

uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

// Ininitialize previous_h and previous_m to track if hour has changed
int previous_h;
int previous_m;

void setup() {
    
    // Begin Serial connection
    Serial.begin(115200);

    WiFi.begin(ssid, password);  //Connect to the WiFi network
  
    while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
      delay(500);
      Serial.println("Waiting to connect...");
    }
  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP
    
    // Initialize WebServer
    // WiFi.softAP(ssid, password);
    // WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    //Start mDNS
    //if (MDNS.begin("smartclock")) {  
    //  Serial.println("MDNS started");
    //}

    server.on("/updateColor", HTTP_POST, handleColor);
  
    server.on("/", handleOnConnect);
    server.on("/clock", handleClockMode);
    server.on("/timer", handleTimerMode);
    server.on("/waves", handleWavesMode);
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
    timeClient.setTimeOffset(-3600 * 4);

    // Get current time
    timeClient.update();
    previous_h = timeClient.getHours();
    previous_m = timeClient.getMinutes();
}

void loop() {
  
  server.handleClient();
  
  if (mode == "clock") {
    timeClient.update(); 
    
    int currentHour = timeClient.getHours();
    // Serial.print("Hour: ");
    // Serial.println(currentHour);  

    int currentMinute = timeClient.getMinutes();
    // Serial.print("Minutes: ");
    // Serial.println(currentMinute);

    // Serial.print("Colors: ");
    // Serial.println(r);
    // Serial.println(g);
    // Serial.println(b);

    delay(0);

  } 

  else if (mode == "timer") {
    delay(0);
  }

  else if (mode = "waves") {
    delay(0);    
  }
}


void handleOnConnect() {
  server.send(200, "text/html", HTML(r, g, b, mode)); 
}

void handleClockMode() {
    mode = "clock";
    server.send(200, "text/html", HTML(r, g, b, mode));
}

void handleTimerMode() {
    mode = "timer";
    server.send(200, "text/html", HTML(r, g, b, mode));
}

void handleWavesMode() {
    mode = "waves";
    server.send(200, "text/html", HTML(r, g, b, mode));
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void handleColor() {
    // Handle invalid data
    
    String data = server.arg("plain");
    Serial.println(data);
    r = getValue(data, ',', 0);
    g = getValue(data, ',', 1);
    b = getValue(data, ',', 2);
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
}

int getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]).toInt() : 255;
}

String HTML(int r, int g, int b, String mode) {
    
    String ptr = "<!DOCTYPE html>\n";
        ptr += "<!DOCTYPE html>\n";
        ptr += "<html lang=\"en\">\n";
        ptr += "<head>\n";
        ptr += "<meta charset=\"UTF-8\">\n";
        ptr += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
        ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
        ptr += "<!-- Bootstrap CSS -->\n";
        ptr += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3\" crossorigin=\"anonymous\">\n";
        ptr += "<!-- JavaScript Bundle with Popper -->\n";
        ptr += "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p\" crossorigin=\"anonymous\"></script>\n";
        ptr += "<!-- Color picker -->\n";
        ptr += "<script src=\"https://cdn.jsdelivr.net/npm/@jaames/iro\"></script>\n";
        ptr += "<title>SmartClock</title>\n";
        ptr += "</head>\n";
        ptr += "<body class=\"bg-secondary text-white\">\n";
        ptr += "<div class=\"container\">\n";
        ptr += "<div class=\"row justify-content-center\">\n";
        ptr += "<div class=\"col-sm\"></div>\n";
        ptr += "<div class=\"col-sm\">\n";
        ptr += "<h1 class=\"text-center\">Smart Clock</h1>\n";
        ptr += "<br>\n";
        ptr += "<div class=\"wheel mx-auto\" id=\"colorWheelDemo\"></div>\n";
        ptr += "</div>\n";
        ptr += "<div class=\"col-sm\"></div>\n";
        ptr += "</div>\n";
        ptr += "<br>\n";
        ptr += "<div class=\"row justify-content-center\">\n";
        ptr += "<div class=\"col-sm\"></div>\n";
        ptr += "<div class=\"col-sm text-center\">\n";
        ptr += "<div class=\"dropdown\">\n";
        ptr += "<button class=\"btn btn-primary dropdown-toggle\" type=\"button\" id=\"modeSelection\" data-bs-toggle=\"dropdown\" aria-expanded=\"false\">\n";
        ptr += "Select the mode\n";
        ptr += "</button>\n";
        ptr += "<ul class=\"dropdown-menu\" aria-labelledby=\"modeSelection\">\n";
        ptr += "<li><a class=\"dropdown-item\" href=\"/clock\">Clock</a></li>\n";
        ptr += "<li><a class=\"dropdown-item\" href=\"/timer\">Timer</a></li>\n";
        ptr += "<li><a class=\"dropdown-item\" href=\"/waves\">Waves</a></li>\n";
        ptr += "</ul>\n";
        ptr += "</div>\n";
        ptr += "</div>\n";
        ptr += "<div class=\"col-sm\"></div>\n";
        ptr += "</div>\n";
        ptr += "</div>\n";
        ptr += "<script>\n";
        ptr += "var colorPicker = new iro.ColorPicker(\"#colorWheelDemo\", {\n";
        ptr += "// Set the size of the color picker\n";
        ptr += "width: 320,\n";
        ptr += "// Set the initial color to pure red\n";
        ptr += "color: {r: " + String(r) + ", g: " + String(g) + ", b:" + String(b) + "},\n";
        ptr += "borderWidth: 1,\n";
        ptr += "borderColor: \"#fff\",\n";
        ptr += "});\n";
        ptr += "function updateColor(color) {\n";
        ptr += "let xhr = new XMLHttpRequest();\n";
        ptr += "xhr.open(\"POST\", \"/updateColor\");\n";
        ptr += "xhr.setRequestHeader(\"Accept\", \"application/json\");\n";
        ptr += "xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded\");\n";
        ptr += "xhr.onreadystatechange = function () {\n";
        ptr += "if (xhr.readyState === 4) {\n";
        ptr += "console.log(xhr.status);\n";
        ptr += "console.log(xhr.responseText);\n";
        ptr += "}};\n";
        ptr += "let data = JSON.stringify(color.rgb);\n";
        ptr += "console.log(data);\n";
        ptr += "xhr.send(data);\n";
        ptr += "}\n";
        ptr += "// Start listening to the color change event\n";
        ptr += "colorPicker.on(\"color:change\", updateColor);\n";
        ptr += "</script>\n";
        ptr += "</body>\n";
        ptr += "</html>\n";
    
    return ptr;

}