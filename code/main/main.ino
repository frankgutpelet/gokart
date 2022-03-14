#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "./DNSServer.h"
#include "base.hpp"
#include "Logger.hpp"

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1); 
DNSServer         dnsServer;
const char* ssid = "EmbeddedWeb";
const char* password = "1234567890";
const char* myhostname = "EmbeddedWebTemplate";

ESP8266WebServer server(80);
base indexPage(&server);
Logger* logger = Logger::instance();

void handleSubmit() 
{
  logger->Debug(String("Value dynamicVariable2: ") + indexPage.Get_dynamicVariable2());
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
  indexPage.Set_dynamicVariable("from c++");
  indexPage.Render();
}

void setup(void) {
 
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
