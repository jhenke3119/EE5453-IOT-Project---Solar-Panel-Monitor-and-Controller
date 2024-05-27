//Code for EE5453 IOT project

#include <ArduinoMqttClient.h>
#include <WiFiS3.h>
#include "arduino_secrets.h"

///////Please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

//IP and port of server host
const char broker[] = "192.168.1.153";
int        port     = 8885;

//Topic(s)
const char topic[]  = "Panel1_Voltage";

//Set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

//ADC measured voltage
float adc_voltage = 0.0;

//Actual measured voltage
float in_voltage = 0.0;

//Voltage sensor resistances
float R1 = 30000; //Ohms
float R2 = 7500;  //Ohms

//Arduino reference voltage
float ref_voltage = 5.0;

//Raw ADC values
int adc_value = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; //Wait for serial port to connect. Needed for native USB port only
  }

  //Attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  //Username and password to give to broker
  mqttClient.setUsernamePassword("panelvolt", "roadrunner");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  //Call poll() regularly to allow the library to send MQTT keep alive which
  //Avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    //Save the last time a message was sent
    previousMillis = currentMillis;

    //Read analog input
    int adc_value = analogRead(A0);

    //Section below is for debugging. Subscriber will perform calcuations with transmitted data
    //Determine voltage at ADC input
    adc_voltage  = (adc_value * ref_voltage) / 1024.0;
    
    //Calculate voltage at divider input

    in_voltage = adc_voltage*(R1+R2)/R2;

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(adc_value);
    Serial.println(adc_voltage);
    Serial.println(in_voltage);

    //Send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(adc_value);
    mqttClient.endMessage();


    Serial.println();
  }
}