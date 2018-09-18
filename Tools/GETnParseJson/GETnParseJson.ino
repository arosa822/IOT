#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>



#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//test case for parsing json
bool testJson = false; 
char JsonTest[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

//KEYS - YOU MUST UPDATE THIS WITH YOUR OWN INFORMATION

#define SSID       "#"
#define PASS       "#"

//globals for GET request
char json[] = "#;
char buffer[500];
String response;
const char* Server = "#;

//END OF KEYS SECTION

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

unsigned long previousMillis = 0;
const long postInt = 1000*30; 
const long GETint = 2000;

void setup(void) {

    u8g2.begin();

    Serial.begin(9600);

    delay(10);

    WiFi.begin(SSID,PASS);

    Serial.println();

    if(testJson == true){
        JsonTester();
    }


    delay(2000);
}

void loop(void) {

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= GETint){


            if (testJson == false){
                
                response = GETRequest(Server,json);   
                Serial.println(response);     
                JsonParser(response);
            }
            else{
                Serial.println("Test mode...");
            }

         previousMillis = currentMillis;
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
  

void olMessage(const char* theString){
        u8g2.clearBuffer();					// clear the internal memory
        u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);
        u8g2.setCursor(0,10);
        u8g2.print(theString);
        u8g2.sendBuffer();
        delay(2000);
        u8g2.clearBuffer();
    
}

void olMessage2(const char* theString){
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

String GETRequest(const char* theServer, const char* theObject){

    //Create object of class HTTPClient
    HTTPClient http;

    int n;

    n = sprintf(buffer,"%s%s",theServer,theObject);

    //Serial.println(buffer);

    //call the 'begin' method and pass through the URL
    http.begin(buffer);
    int httpCode = http.GET(); 

    String thePayload = http.getString();

    Serial.println(thePayload);

    http.end();

    return thePayload;
}

void JsonParser(String theJson){
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(theJson);

    if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
    }

    const char* sensor = root["i4"][0];
    Serial.println(sensor);
    olMessage(sensor);
}

void JsonTester(){
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(JsonTest);

    //check to make sure that json test succeded
    if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
    }

    //Fetch Values  
    const char* sensor = root["sensor"];
    long time = root["time"];
    double latitude = root["data"][0];
    double longitude = root["data"][1];

    // Print values.
    Serial.println(sensor);
    Serial.println(time);
    Serial.println(latitude, 6);
    Serial.println(longitude, 6);
  }






