#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "SUNDARS JIO"
#define WLAN_PASS       "abcd1234"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "varsha2704"
#define AIO_KEY         "aio_uNRI07wqsOkCEnDJULCQfIqqeEze"

/************ Global State (you don't need to change this!) ******************/
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


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

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
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
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(12,HIGH);
  }
 

 else{
    soilState = 0;
    Serial.print(F("\nSending soil state "));
    publishValue.publish(soilState);
     digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  digitalWrite(14, LOW);
  digitalWrite(12,LOW);
  }
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
