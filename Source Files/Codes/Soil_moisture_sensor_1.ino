#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Arrnavv"
#define WLAN_PASS       "password"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "thearrnavvchawla"
#define AIO_KEY         "aio_aRkH16MxfJyCmgf6P5RBvmhPfqx0"

/************ Global State (you don't need to change this!) ******************/
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

int moisture;
int dry = 500;
int good = 300;
int sensorFinal;
int buzzer = 16;

Adafruit_MQTT_Publish  publishValue= Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/waterdetect");


/****************************** Feeds ***************************************/

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Starting.."));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

int soilState = 0;


void loop() {

  MQTT_connect();
  Serial.println(analogRead(0));
  delay(200);

 if(analogRead(0)> 600){
  soilState = 1;
  
  Serial.print(F("\nSending soil state "));
  publishValue.publish(soilState);
  }
  /*tone(buzzer, 800);
  delay(750);
  noTone(buzzer);
  delay(750);*/

  /*else if (300> analogRead(0))  
  tone(buzzer, 800);
  delay(750);
  noTone(buzzer);
  delay(100);*/

 else{
    soilState = 0;
    Serial.print(F("\nSending soil state "));
    publishValue.publish(soilState);
  }


  /*Serial.print(F("\nSending soil state "));
  Serial.print(x);
  Serial.print("...");
  if (! soilState.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }*/

delay(10000);
 
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
