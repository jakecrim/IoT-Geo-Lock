# IoT-Geo-Lock
> Automatic locking and unlocking for a door based on when I leave or arrive to my apartment.


# Usage
* The physical device is placed around the standard deadbolt hand twister on the inside of the door. 
* The ESP32 microcontroller holds a WiFi connection and is connected to a MQTT broker ready for an "arrived" or "departed" message to come in. Based on that message it will control a servo to either lock or unlock. 
* Since the microcontroller is holding a WiFi connection it can be commanded to unlock or lock from anywhere in the world in case I want to unlock the door for a family member to get into my place while I am away.

>Note: This project was mainly for making sure the door was locked to our apartment when my roomates would often leave it unlocked, or used on my bedroom door to my apartment to keep it locked throughout the day while I was on campus, if I were to ever distribute this as a product I understand it would need more consideration in regards to security.
