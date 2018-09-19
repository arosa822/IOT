//Created on 9/7/18
//tool for flashing esp devices OTA
// added function to change light output

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "CenturyLink0029";
const char* password = "fd663ua66347d7";

const int LED = 12;


void setup() {
 Serial.begin(115200);
 Serial.println("Booting");
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 while (WiFi.waitForConnectResult() != WL_CONNECTED) {
 Serial.println("Connection Failed! Rebooting...");
 delay(5000);
 ESP.restart();
 }

// Port defaults to 8266
 // ArduinoOTA.setPort(8266);

// Hostname defaults to esp8266-[ChipID]
 // ArduinoOTA.setHostname("myesp8266");

// No authentication by default
 // ArduinoOTA.setPassword((const char *)"123");

ArduinoOTA.onStart([]() {
 Serial.println("Start");
 });
 ArduinoOTA.onEnd([]() {
 Serial.println("\nEnd");
 });
 ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
 Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
 });
 ArduinoOTA.onError([](ota_error_t error) {
 Serial.printf("Error[%u]: ", error);
 if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
 else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
 else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
 else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
 else if (error == OTA_END_ERROR) Serial.println("End Failed");
 });
 ArduinoOTA.begin();
 Serial.println("Ready");
 Serial.print("IPess: ");
 Serial.println(WiFi.localIP());
//DO NOT CHANGE ANYTHING ABOVE THIS LINE OR OTA WILL NOT WORK!!!

pinMode(LED, OUTPUT);

}

void loop() {

 ArduinoOTA.handle();// this line must be kept for OTA ubdates


 digitalWrite(LED,HIGH);
 
 
}