//Created on 9/7/18
//tool for flashing esp devices OTA
// added function to change light output

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SHT1x.h>
#include "DHT.h"

//ADAFRUIT DEPENDANCIES
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//For outputs
#include "AdafruitIO_WiFi.h"

//KEYS-MUST UPLOAD WITH CORRECT KEYS FOR THIS TO WORK
const char* ssid = "#";
const char* password = "#";

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "#"
#define AIO_KEY         "#"

#define WIFI_SSID       "#"
#define WIFI_PASS       "#"

#define DHTPIN 0     // what digital pin we're connected to
#define DHTTYPE DHT22 

//for output section
#define IO_USERNAME    "#"
#define IO_KEY         "#"
DHT dht(DHTPIN, DHTTYPE);

const int redLed = 12;
unsigned long previousMillis = 0;
const long postInt = 1000*5; 
const long readInt = 1000;
float h;
float f;
float t;

//pinouts
int dhtPwr = 14;

//LED pins power to violet and white led's
int vioPwr = 15;
int whtPwr = 5;

//Create an ESP8266 WifiClient class to connect to the MQTT server
WiFiClient client;

//ADAFRUIT
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
//need for control on the adafruit side of things

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
// Setup feeds for temperature & humidity

Adafruit_MQTT_Publish fruitPrint = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Serial_test");
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");

//DigitalOut

AdafruitIO_Feed *digital = io.feed("digital");

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
    
    io.connect();

    pinMode(dhtPwr, OUTPUT);
    pinMode(whtPwr, OUTPUT);
    pinMode(vioPwr, OUTPUT);   

    digital->onMessage(handleMessage);

    while(io.status() < AIO_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    digital->get();
}

void loop() {
    //DO NOT DELETE THIS LINE
    ArduinoOTA.handle();// this line must be kept for OTA ubdates
    //DO NOT DELETE THIS LINE

    // io.run(); is required for all sketches.
    // it should always be present at the top of your loop
    // function. it keeps the client connected to
    // io.adafruit.com, and processes any incoming data.
    io.run();

    digitalWrite(dhtPwr,HIGH);
    //digitalWrite(whtPwr, HIGH);
    digitalWrite(vioPwr, LOW);

    //Code Base starts below this line 
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= postInt){
 
        digitalWrite(vioPwr, HIGH);
        

        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        h = dht.readHumidity();

        // Read temperature as Celsius (the default)
        t = dht.readTemperature();

        // Read temperature as Fahrenheit (isFahrenheit = true)
        f = dht.readTemperature(true);

        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t) || isnan(f)) 
        {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        else{
                
            Serial.println(h);
            Serial.println(f);
        }

        if (! fruitPrint.publish("This is another Test"))
            Serial.println(F("Failed to publish Serial"));
        else 
            Serial.println(F("Serial Published!"));

        if (! temperature.publish(f))
             Serial.println(F("Failed to publish temperature"));
        else 
            Serial.println(F("temperature published"));

        if (! humidity.publish(h))
            Serial.println(F("Failed to publish humidity!"));
        else
            Serial.println(F("Humidity Published!"));

        previousMillis = currentMillis;   
    }
    
    ping();


    //TEST FOR FEEDBACK 
    if (h > 50){
        digitalWrite(whtPwr, HIGH);
    }
    else {
        digitalWrite(whtPwr,LOW);
    }

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

void handleMessage(AdafruitIO_Data *data) {
 
  Serial.print("received <- ");
 
  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");
 
  // write the current state to the led
  digitalWrite(whtPwr, data->toPinLevel());
 
}