# Contents

This folder contains a functional prototype including hardware setup and code base for demo'ing purposes. 

# About
This prototype is in the alpha stage of conceptualization, the idea is to have a light weight wireless module complete with OTA, Low Power and temp/ humidity feedback trigers. 

# Triggers

The triggers are represented by the green and red LED's.  These will be used for toubleshooting, testing set points (feedback), and communication acknowlagement through third party IOS platforms.

# IOS platform

The selected IOT platform will be thingspeak.  Thingspeak has been chosen due to MQTT protocal, data analytic features (matlab), and already functioning IOS app and widget. (Note: this has not been explored with android device just yet)

In addition to features above, ThingTweat will also be explored.  This will allow for the user to have notifications set for specified devices on the data feed. 

# MCu

The choice Mcu for this project is an adafruit feather ESP8266 Dev board, MRP is  15 - $20. Other cheeper versions were explored but this was the only one functioning properly for Over the air flashing- flash size requirements of the ESP8266 must be 4MB or larger (double check this).

This project aims to KSS (Keep it simple stupide), the setup proposed should have all the necessary components to test IOT integration and data integrity. (Not the most accurate temp humidity sensor, but good enough for testing critical functions in firmware developement.)


