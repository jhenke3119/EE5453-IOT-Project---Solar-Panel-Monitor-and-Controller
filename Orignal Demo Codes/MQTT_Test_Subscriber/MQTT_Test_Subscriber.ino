//Code for EE5453 IOT Project

#include <ArduinoMqttClient.h>
#include <WiFiS3.h>
#include "arduino_secrets.h"

///////Please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID
char pass[] = SECRET_PASS;        // your network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

//IP and port of server host
const char broker[] = "192.168.1.153";        
int        port     = 1883;

//Topic(s)
const char topic[]    = "Panel1_Voltage";

//Relay pin constant
int relayOut = 8;

//Panel voltage variables
int intVolt;                  //Convert from int ASCII
uint8_t charVolt[128];        //  to unsigned int array

//Temporary variable for conversion to integer
int receivedInt = 0;

//Received raw ADC values
int adc_value = 0;

//Voltage sensor resistances
int R1 = 30000;  //Ohms
int R2 = 7500;   //Ohms

//Arduino reference voltage
float ref_voltage = 5.0;

//ADC measured voltage
float adc_voltage = 0.0;

//Actual measured voltage
float in_voltage = 0.0;

void setup() {
  //Initialize relay for smart outlet
  pinMode(relayOut, OUTPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; //Wait for serial port to connect. Needed for native USB port only
  }
  //Attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  //Set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  //Subscribe to a topic
  mqttClient.subscribe(topic);

  Serial.print("Topic: ");
  Serial.println(topic);


  Serial.println();
}

void loop() {
  //Call poll() regularly to allow the library to receive MQTT messages and
  //Send MQTT keep alive which avoids being disconnected by the broker
  mqttClient.poll();
}

void onMqttMessage(int messageSize) {
  //We received a message, print out the topic and contents
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  //Use the Stream interface to print the contents
  while (mqttClient.available()) {

    //Read MQTT datastream
    intVolt = mqttClient.read(charVolt, sizeof(charVolt));

    //The MQTT datastream is of ASCII decimals.  Function below performs conversion to int
    if(intVolt > 0) {
      receivedInt = 0;
      for(int i = 0; i < intVolt; i++) {
        receivedInt = receivedInt * 10 + (charVolt[i] - '0');
      }
    }

    //Renaming to something more useful
    adc_value = receivedInt;

    //Perform conversion of raw analog voltages to actual voltages
    adc_voltage  = (adc_value * ref_voltage) / 1024.0;
    in_voltage = adc_voltage*(R1+R2)/R2;

    //Print raw, measured, and actual voltages
    Serial.println(adc_value);
    Serial.println(adc_voltage);
    Serial.println(in_voltage);
  }
  Serial.println();
  Serial.println();

  if(in_voltage >= 11){
    //If the panel is operating normally, keep the load on
    digitalWrite(relayOut, HIGH);
    Serial.print("Outlet Power:  ON");
    Serial.println();
    Serial.println();
  }
  else{
    //If the panel falls below a useful voltage, turn the load off
    digitalWrite(relayOut, LOW);
    Serial.print("Outlet Power:  OFF");
    Serial.println();
    Serial.println();
  }
}