//IN THE NAME OF GOD
//Temprture Control & Monitoring System
//Ali Forghani Elah Abadi : 93412130
//Sadjad University of Technology

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP23008.h> 

#define OLED_RESET 100
#define SSD1306_LCDHEIGHT 64
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
  
// Replace with your network credentials
const char* ssid = "BP37";
const char* password = "";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String page = ""; //Webpage portal frontEnd code(by HTML)
int Heater_sig = 13;  //pin for control heater ON/OFF
int Sensor_sig= A0;   //pin for read sensor analog signal
int analogValue = 0;  //sensor analog value
float millivolts = 0; //sensor value(milivolts)
float celsius = 0;    //sensor tempreture(celsius)
int Degree = 0;   //Heater Degree Level
//int WIFI_STATE = 2;
Adafruit_MCP23008 mcp; 

void setup(void){
  Wire.begin(D2,D1);
  mcp.begin(); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  // Intro Text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Temprture");
  display.display();
  display.display();  
  //the HTML of the web page
  page = "<h1>" + String(celsius) + "</h1>";

  //make the Heater pin output and initially turned off
  pinMode(Heater_sig, OUTPUT);
//  pinMode(WIFI_STATE, OUTPUT);
  digitalWrite(Heater_sig, LOW);
   
  delay(1000);
  Serial.begin(115200); //intialize the Circuit Serial Interface Protocol Speed(bps)
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println(""); //function to print checking parameter and text on Serial Monitor
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     //Wifi local Ip to Access the Control Portal
   
  server.on("/", [](){         //load page root
    server.send(200, "text/html", page);
  });
  server.on("/getTemp", [](){       //run Heater On Task
    server.send ( 200, "application/json", "{\"temp\":" + String(celsius) + " }" );
    delay(1000);
  });
  server.on("/HeaterLOW", [](){       //run Heater On Task
    server.send(200, "text/html", page);
    analogWrite(Heater_sig, 50);
    delay(1000);
  });
  server.on("/HeaterMID", [](){      //run Heater Off Task
    server.send(200, "text/html", page);
    analogWrite(Heater_sig, 150);
    delay(1000); 
  });
  server.on("/HeaterHIGH", [](){      //run Heater Off Task
    server.send(200, "text/html", page);
    analogWrite(Heater_sig, 250);
    delay(1000); 
  });
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  server.handleClient();

//  if(WiFi.status() != WL_CONNECTED)
//  {
//    digitalWrite(WIFI_STATE, HIGH);
//  }
  page = "<h1>" + String(celsius) + "</h1>";
  analogValue = analogRead(Sensor_sig);
  millivolts = (analogValue/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
  celsius = millivolts/10;
  
  // Intro Text
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 1);
  display.println("Temp:         " + String(celsius));
  display.display();
  display.display(); 
  delay(500);

}
