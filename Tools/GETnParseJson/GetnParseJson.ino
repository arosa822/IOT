#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11

#define WLAN_SSID       ""
#define WLAN_PASS       ""
// Adafruit IO

/*
  U8glib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
*/

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);
  
    /* typical format for output
    for (int index=0; index<10; index++){
        u8g2.clearBuffer();					// clear the internal memory
        u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);	// choose a suitable font
        u8g2.setCursor(0,10);	
        u8g2.print("Index: ");
        u8g2.setCursor(55,10);
        u8g2.print(index);
        u8g2.sendBuffer();
        delay(1000);
    }*/

WiFiClient client;

unsigned long previousMillis = 0;
const long postInt = 1000*30; 
const long readInt = 1000;

void setup(void) {

    u8g2.begin();

    Serial.begin(9600);

    dht.begin();

    delay(10);

    Serial.print(F("Connecting to... "));
    Serial.println(WLAN_SSID);

    WiFi.begin(WLAN_SSID, WLAN_PASS);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        olMessage("Connecting to ");
        olMessage2(WLAN_SSID);
    }

    Serial.println();

    olMessage("Connected!");

    u8g2.sendBuffer();

    delay(2000);

    connect();

    olMessage("Initializing!");
}

void loop(void) {

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= postInt){
  

            previousMillis = currentMillis;
        }
    }
}

void displayData(float f, float h ){

            u8g2.clearBuffer();					// clear the internal memory
            u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);

            u8g2.setCursor(0,10);
            u8g2.print("Temp: ");

                u8g2.setCursor(55,10);
                u8g2.print(f);

            u8g2.setCursor(0,20);
            u8g2.print("R/H: ");

                u8g2.setCursor(55,20);
                u8g2.print(h);


            u8g2.setCursor(0,30);
            u8g2.print(WiFi.localIP());

            u8g2.sendBuffer();
}
  

void olMessage(char* theString){
        u8g2.clearBuffer();					// clear the internal memory
        u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);
        u8g2.setCursor(0,10);
        u8g2.print(theString);
        u8g2.sendBuffer();
        delay(2000);
        u8g2.clearBuffer();
    
}

void olMessage2(char* theString){
        //u8g2.clearBuffer();					// clear the internal memory
        u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);
        u8g2.setCursor(0,20);
        u8g2.print(theString);
        u8g2.sendBuffer();
        delay(2000);
        u8g2.clearBuffer();
    
}

void olValue(float thefloat){
        u8g2.clearBuffer();					// clear the internal memory
        u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);
        u8g2.setCursor(0,10);
        u8g2.print(thefloat);
        u8g2.sendBuffer();
        delay(1000);
}



