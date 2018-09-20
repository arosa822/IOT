//Created on 9/7/18
//tool for flashing esp devices OTA
// added function to change light output

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SHT1x.h>

//ADAFRUIT DEPENDANCIES
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


//KEYS-MUST UPLOAD WITH CORRECT KEYS FOR THIS TO WORK
const char* ssid = "#";
const char* password = "#";

// Adafruit IO
#define AIO_SERVER      "#"
#define AIO_SERVERPORT  #
#define AIO_USERNAME    "#"
#define AIO_KEY         "#"

#define DHTPIN 0     // what digital pin we're connected to
const int redLed = 12;
unsigned long previousMillis = 0;
const long postInt = 1000*30; 
const long readInt = 1000;
float h;
float f;

//Create an ESP8266 WifiClient class to connect to the MQTT server
WiFiClient client;

//ADAFRUIT
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup feeds for temperature & humidity
Adafruit_MQTT_Publish fruitPrint = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Serial_test");
//Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity_d22");
//Adafruit_MQTT_Publish soilMoisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Soil\ Moisture");

void setup() {

    Serial.begin(115200);
    Serial.println("Booting");


    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {

        Serial.println
        ("Connection Failed! Rebooting...");

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

pinMode(redLed, OUTPUT);

}

void loop() {
    //DO NOT DELETE THIS LINE
    ArduinoOTA.handle();// this line must be kept for OTA ubdates
    //DO NOT DELETE THIS LINE

    //Code Base starts below this line 
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= postInt){

         if (! fruitPrint.publish("This is another Test"))
                Serial.println(F("Failed to publish temperature"));

    }
    
    ping();

}



void ping(void){

  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {

      // reconnect to adafruit io
      if(! mqtt.connected())
        connect();
    } 
}

// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {

      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);
  }

  Serial.println(F("Adafruit IO Connected!"));
}