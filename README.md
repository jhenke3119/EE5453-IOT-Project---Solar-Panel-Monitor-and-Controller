***Work-In-Progress***

***Created for the UTSA EE5453 IOT Security class under Dr. Chen (Spring 2024)***

This repository contains Arduino code used for monitoring a solar panel and operating a smart appliance.

NOTE:  This is only an example use-case for how one can secure an Arduino/Mosquitto-based smart network and should not be used for any "production" type enviorment.  The codes and designs here are meant for educational purposes only.

# Folder Information
The folders are organized as such:

* Mosquitto Conf Files - Configuration files for a Mosquitto broker
* Original Demo Codes - Arduino codes used as part of the design demonstration portion of the project
* Post-Attack Codes - Arduino codes used as part of the attack demonstration portion of the project
* Presentation Files - PowerPoint slides for each project phase (TO BE ADDED)

# Code Description
The code used is mostly based off the example Arduino MQTT example available here:  
[Sending Data over MQTT](https://docs.arduino.cc/tutorials/uno-wifi-rev2/uno-wifi-r2-mqtt-device-to-device/)

The Arduino code modifications include additional variables for the voltage sensor measurements and relay control and data parsing for the voltage sensor data.  The included arduino_secrets.h file should include your own network SSID and password so the Arduinos can connect to the network and broker server.

The Mosquitto configuration files are sourced from the [Eclipse Mosquitto download page](https://mosquitto.org/download/). They have been modified for use in this presentation.

# Hardware Description
The hardware used is described in the design demo slides (TO BE ADDED).  The solar hardware set up can be modifed to fit your own hardware.  The code assumes you are using the Arduino R4 Plus Wifi, so bear this in mind if you decide to use differnt hardware.  Addition changes may be necesary if a different relay and voltage sensor are used.

A similar smart outlet design can be found in this [Circuit Basics article](https://www.circuitbasics.com/build-an-arduino-controlled-power-outlet/).  This example was modified for use in this project.

Hardware setup and Arduino connections can be viewed in the presentation files, or below for quick reference (DIAGRAMS TO BE ADDED).

# Video Presentation links
(TO BE ADDED)


