# Contents

This folder contains a functional prototype including hardware setup and code base for demo'ing purposes. 

# About
This prototype is in the alpha stage of conceptualization, the idea is to have a light weight wireless module complete with OTA, Low Power and temp/ humidity feedback trigers. 

Two LED lights will be attached to pins for debugging.  The one of the lights will be for debugging purposes during OTA flashing.  One will be used to demonstrate triggers and wireless controls (i.e. phone apps or BLE)

# Triggers

The triggers are represented by the green and red LED's.  These will be used for toubleshooting, testing set points (feedback), and communication acknowlagement through third party IOS platforms.

# IOS platform(s)

Several IOS platforms are currently being explored, Ubidots, Thingspeak and adafruit.

Adafruit 

Pros 
- Ease of use
- interface
- library

cons
- restriction/ throttle on data
- limited analalytics

Thingspeak

pros
- Ease of use
- data analytics (matplot)
- IOS wiget

Cons 
- Visual appearence
- App 

Ubidots

(just begining to explore this option due to white label features) 


# MCu

The choice Mcu for this project is an adafruit feather ESP8266 Dev board, MRP is  15 - $20. Other cheeper versions were explored but this was the only one functioning properly for Over the air flashing- flash size requirements of the ESP8266 must be 4MB or larger (double check this).

This project aims to KSS (Keep it simple stupide), the setup proposed should have all the necessary components to test IOT integration and data integrity. (Not the most accurate temp humidity sensor, but good enough for testing critical functions in firmware developement.)


