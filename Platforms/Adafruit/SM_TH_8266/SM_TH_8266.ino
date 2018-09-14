#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <SHT1x.h>
#define DHTPIN 0     // what digital pin we're connected to


// WiFi parameters
#define WLAN_SSID       "#"
#define WLAN_PASS       "#"

// Adafruit IO
#define AIO_SERVER      "#"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "#"
#define AIO_KEY         "#"

//I2C definitions
#define dataPin  2
#define clockPin 14
SHT1x sht1x(dataPin, clockPin);

bool SMdebug= true;
unsigned long debugMillis= 0;


int pwrPin = 5;
int sm = 0; 


//Create an ESP8266 WifiClient class to connect to the MQTT server
WiFiClient client;

//setup the MQTT client class by passin in the WiFi client and MQTT server and login details
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup feeds for temperature & humidity
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature_d22");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity_d22");

unsigned long previousMillis = 0;
const long postInt = 1000*30; 
const long readInt = 1000;
float h;
float f;

void setup(void) {


    Serial.begin(9600);
    pinMode(pwrPin, OUTPUT);



    delay(10);

    Serial.print(F("Connecting to... "));
    Serial.println(WLAN_SSID);

    WiFi.begin(WLAN_SSID, WLAN_PASS);


    Serial.println();

    delay(2000);

    connect();

}

void loop(void) {
 

    unsigned long currentMillis = millis();

    
    if (SMdebug & currentMillis - debugMillis >= readInt){
        int debug = analogRead(A0);
        Serial.println("***debug***");
        Serial.print("sm = ");
        Serial.println(debug);
        debugMillis = currentMillis;
    }


    if (currentMillis - previousMillis >= postInt){

            // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
         float h = sht1x.readHumidity();
        // Read temperature as Celsius (the default)
         float t = sht1x.readTemperatureC();
        // Read temperature as Fahrenheit (isFahrenheit = true)
         float f = sht1x.readTemperatureF();

         int sm = readSoil();

        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t) || isnan(f)) 
        {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        if (isnan(sm))
        {
            Serial.println("Failed to read from SM sensor!");
            return;
        }
        else 
        {    

            Serial.println(h);
            Serial.println(f);
            Serial.print("sm = ");
            Serial.println(sm);
 
                    // Publish data
            if (! temperature.publish(f))
                Serial.println(F("Failed to publish temperature"));
            else
                Serial.println(F("Temperature published!"));

            if (! humidity.publish(h))
                Serial.println(F("Failed to publish humidity"));
            else
                Serial.println(F("Humidity published!"));

            previousMillis = currentMillis;
        }
    }

   ping();

}
int readSoil(void){
    digitalWrite(pwrPin, HIGH);
    delay(1000); // wait a second before reading soil moisture values
    sm = analogRead(A0);
    delay(10);
    digitalWrite(pwrPin, LOW);
    return sm;

}

void ping(void)
{
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) 
    {
      // reconnect to adafruit io
      if(! mqtt.connected())
        connect();
    } 
}

// connect to adafruit io via MQTT
void connect() 
{

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) 
  {

    switch (ret) 
    {
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

