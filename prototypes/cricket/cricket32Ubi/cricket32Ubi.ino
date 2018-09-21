// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// SI7020-A20
// This code is designed to work with the SI7020-A20_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Humidity?sku=SI7020-A20_I2CS#tabs-0-product_tabset-2

#include "config.h"
#include <ESP8266WiFi.h>

const char* DEVICE_LABEL = "feather-huzzah"; // Your device label
const char* VARIABLE_LABEL = "temperature"; // Your variable label
const char* USER_AGENT = "ESP8266";
const char* VERSION = "1.0";
const char* HTTPSERVER = "industrial.api.ubidots.com";
int HTTPPORT = 80;

WiFiClient clientUbi;

/********************************
 * Auxiliar Functions
 *******************************/
/**
 * Gets the length of the body
 * @arg variable the body of type char
 * @return dataLen the length of the variable
 */

int dataLen(char* variable) {
  uint8_t dataLen = 0;
  for (int i = 0; i <= 250; i++) {
    if (variable[i] != '\0') {
      dataLen++;
    } else {
      break;
    }
  }
  return dataLen;

}


    float temp;
    float ctemp;
    float ftemp;
    float humidity;
    float ppm;
    unsigned int data[2];

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  WiFi.begin(SSID_NAME, SSID_PASS);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
             Serial.print(".");
        }

        WiFi.setAutoReconnect(true);
        Serial.println(F("WiFi connected"));
        Serial.println(F("IP address: "));
        Serial.println(WiFi.localIP());

}

void loop()
{

    GetTempHumid();

        // Output data to serial monitor
        Serial.print("Relative humidity : ");
        Serial.print(humidity);
        Serial.println(" % RH");
        Serial.print("Temperature in Celsius : ");
        Serial.print(ctemp);
        Serial.println(" C");
        Serial.print("Temperature in Fahrenheit : ");
        Serial.print(ftemp);
        Serial.println(" F");
        delay(500);
    
    GetGas();

        // Output data to serial monitor
        Serial.print("Gas Concentration : ");
        Serial.print(ppm);
        Serial.println(" ppm");
        delay(500);
        
}

void GetTempHumid(){

    // Start I2C transmission
    Wire.beginTransmission(TH_Addr);
    // Send humidity measurement command, NO HOLD MASTER
    Wire.write(0xF5);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(500);
    
    // Request 2 bytes of data
    Wire.requestFrom(TH_Addr, 2);

    // Read 2 bytes of data
    // humidity msb, humidity lsb 
    if(Wire.available() == 2)
    {
      data[0] = Wire.read();
      data[1] = Wire.read();
    }
    
    // Convert the data
    humidity  = ((data[0] * 256.0) + data[1]);
    humidity = ((125 * humidity) / 65536.0) - 6;

    // Start I2C transmission
    Wire.beginTransmission(TH_Addr);
    // Send temperature measurement command, NO HOLD MASTER
    Wire.write(0xF3);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(500);
    
    // Request 2 bytes of data
    Wire.requestFrom(TH_Addr, 2);

    // Read 2 bytes of data
    // temp msb, temp lsb
    if(Wire.available() == 2)
    {
      data[0] = Wire.read();
      data[1] = Wire.read();
    }

    // Convert the data
    temp  = ((data[0] * 256.0) + data[1]);
    ctemp = ((175.72 * temp) / 65536.0) - 46.85;
    ftemp = ctemp * 1.8 + 32;

}

void GetGas(){
      // Start I2C Transmission
    Wire.beginTransmission(GAS_Addr);
    // Select data register
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();

    // Request 2 bytes of data
    Wire.requestFrom(GAS_Addr, 2);

    // Read 2 bytes of data
    // raw_adc msb, raw_adc lsb
    if (Wire.available() == 2)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }

    // Convert the data to 12-bits
    int raw_adc = ((data[0] & 0x0F) * 256) + data[1];
    ppm = (1.99 * raw_adc) / 4095.0 + 0.01;


}