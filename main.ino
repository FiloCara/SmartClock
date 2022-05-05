// Import Wifi and Webserver libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Import NTP libraries
#include <NTPClient.h>
#include <WiFiUdp.h> 

// Import FastLed
#include <fastLED.h>

// Import WebPage variable
#include "html_page.ino"

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
int matrix[10][11];
matrix[0] = {115, 114,113,112,111,110,109,108,107,106,105};
matrix[1] = {94, 95, 96, 97, 98, 99,100,101,102,103,104};
matrix[2] = {93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83};
matrix[3] = {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82};
matrix[4] = {71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61};
matrix[5] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60};
matrix[6] = {49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39};
matrix[7] = {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38};
matrix[8] = {27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17};
matrix[9] = { 6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16};

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
    
    // Initialize WebServer
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    //Start mDNS
    if (MDNS.begin("smartclock")) {  
      Serial.println(“MDNS started”);
    }

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
    Serial.print("Hour: ");
    Serial.println(currentHour);  

    int currentMinute = timeClient.getMinutes();
    Serial.print("Minutes: ");
    Serial.println(currentMinute);

    Serial.print("Colors: ");
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);

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
    else {
      r = server.arg("r").toInt();
      g = server.arg("g").toInt();
      b = server.arg("b").toInt();
    }
}

String HTML(uint8_t r, uint8_t g, uint8_t b, String mode) {
    
    String ptr += "<!DOCTYPE html>";
        ptr += "<!DOCTYPE html>";
        ptr += "<html lang=\"en\">";
        ptr += "<head>";
        ptr += "<meta charset=\"UTF-8\">";
        ptr += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
        ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
        ptr += "<!-- Bootstrap CSS -->";
        ptr += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3\" crossorigin=\"anonymous\">";
        ptr += "<!-- JavaScript Bundle with Popper -->";
        ptr += "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p\" crossorigin=\"anonymous\"></script>";
        ptr += "<!-- Color picker -->";
        ptr += "<script src=\"https://cdn.jsdelivr.net/npm/@jaames/iro\"></script>";
        ptr += "<title>SmartClock</title>";
        ptr += "</head>";
        ptr += "<body class=\"bg-secondary text-white\">";
        ptr += "<div class=\"container\">";
        ptr += "<div class=\"row justify-content-center\">";
        ptr += "<div class=\"col-sm\"></div>";
        ptr += "<div class=\"col-sm\">";
        ptr += "<h1 class=\"text-center\">Smart Clock</h1>";
        ptr += "<br>";
        ptr += "<div class=\"wheel mx-auto\" id=\"colorWheelDemo\"></div>";
        ptr += "</div>";
        ptr += "<div class=\"col-sm\"></div>";
        ptr += "</div>";
        ptr += "<br>";
        ptr += "<div class=\"row justify-content-center\">";
        ptr += "<div class=\"col-sm\"></div>";
        ptr += "<div class=\"col-sm text-center\">";
        ptr += "<div class=\"dropdown\">";
        ptr += "<button class=\"btn btn-primary dropdown-toggle\" type=\"button\" id=\"modeSelection\" data-bs-toggle=\"dropdown\" aria-expanded=\"false\">";
        ptr += "Select the mode";
        ptr += "</button>";
        ptr += "<ul class=\"dropdown-menu\" aria-labelledby=\"modeSelection\">";
        ptr += "<li><a class=\"dropdown-item\" href=\"/clock\">Clock</a></li>";
        ptr += "<li><a class=\"dropdown-item\" href=\"/timer\">Timer</a></li>";
        ptr += "<li><a class=\"dropdown-item\" href=\"/waves\">Waves</a></li>";
        ptr += "</ul>";
        ptr += "</div>";
        ptr += "</div>";
        ptr += "<div class=\"col-sm\"></div>";
        ptr += "</div>";
        ptr += "</div>";
        ptr += "<script>";
        ptr += "var colorPicker = new iro.ColorPicker(\"#colorWheelDemo\", {";
        ptr += "// Set the size of the color picker";
        ptr += "width: 320,";
        ptr += "// Set the initial color to pure red";
        ptr += "color: {r: " + r + ", g: " + g + ", b:" + b + "},";
        ptr += "borderWidth: 1,";
        ptr += "borderColor: \"#fff\",";
        ptr += "});";
        ptr += "function updateColor(color) {";
        ptr += "let xhr = new XMLHttpRequest();";
        ptr += "xhr.open(\"POST\", \"/updateColor\");";
        ptr += "xhr.setRequestHeader(\"Accept\", \"application/json\");";
        ptr += "xhr.setRequestHeader(\"Content-Type\", \"application/json\");";
        ptr += "xhr.onreadystatechange = function () {";
        ptr += "if (xhr.readyState === 4) {";
        ptr += "console.log(xhr.status);";
        ptr += "console.log(xhr.responseText);";
        ptr += "}};";
        ptr += "let data = JSON.stringify(color.rgb)";
        ptr += "xhr.send(data);";
        ptr += "}";
        ptr += "// Start listening to the color change event";
        ptr += "colorPicker.on(\"color:change\", updateColor);";
        ptr += "</script>";
        ptr += "</body>";
        ptr += "</html>";

}