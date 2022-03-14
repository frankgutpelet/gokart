#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "./DNSServer.h"
#include "base.hpp"
#include "Logger.hpp"
#include "EEPROM.h"

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1); 
DNSServer         dnsServer;
const char* ssid = "EmbeddedWeb";
const char* password = "1234567890";
const char* myhostname = "EmbeddedWebTemplate";

ESP8266WebServer server(80);
base indexPage(&server);
Logger* logger = Logger::instance();
int maxValue;
int minValue;
int minValueAdr = 0x00;
int maxValueAdr = 0x04;

int pwmPin = 16;
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;
os_timer_t Timer1; 

void timerCallback(void *pArg);

void handleSubmit() 
{
  if (String("calibrate") == indexPage.Get_action())
  {
    logger->Debug(String("calibrate"));
    os_timer_disarm (&Timer1);
    analogWrite(pwmPin, 0);
    calibrate();
  }
  
  Render();
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void Render()
{
  indexPage.Render();
}

void calibrate()
{
 do
  {
     maxValue = sensorValue;
     sensorValue = analogRead(sensorPin);
     delay(100);
  }while ((maxValue - 20) < sensorValue);
  minValue += 10;
  Serial.println("initialized: minValue" + String(minValue) + " maxValue " + String(maxValue));
  EEPROM.write(minValueAdr, minValue % 0xFF);
  EEPROM.write(minValueAdr + 1, minValue / 0xFF);
  EEPROM.write(maxValueAdr, maxValue % 0xFF);
  EEPROM.write(maxValueAdr + 1, maxValue % 0xFF);
  os_timer_arm(&Timer1, 100, true);
}


void setup(void) 
{
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 0);
  minValue = EEPROM.read(minValueAdr) + (EEPROM.read(minValueAdr +1) * 0xFF);
  maxValue = EEPROM.read(maxValueAdr) + (EEPROM.read(maxValueAdr +1) * 0xFF);
  os_timer_setfn(&Timer1, timerCallback, NULL);
  os_timer_arm(&Timer1, 100, true);
 
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  dnsServer.start(DNS_PORT, "*", apIP);


  server.on("/", Render);
  indexPage.SetCallback_submit(handleSubmit); 
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  
}

void loop(void) {
  dnsServer.processNextRequest();
  server.handleClient();
}

void timerCallback(void *pArg)
{ 
    if (minValue <= maxValue)
    {
      return;
    }
    sensorValue = analogRead(sensorPin);
    int trottle = (sensorValue - minValue)*255 / (maxValue - minValue);
    if (0 > trottle)
    {
      trottle = 0;
    }
    analogWrite(pwmPin, trottle);
    indexPage.Set_trottle(String(trottle));
} 
