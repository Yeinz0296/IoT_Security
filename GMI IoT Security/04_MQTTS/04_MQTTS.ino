#include <WiFiClientSecure.h>
#include <MQTT.h>
#include "rootCA.h"

const char ssid[] = "TP-Link_A9C0";
const char password[] = "29699971";
const char mqttClientId[] = "NzcyNRQ5FwIfHSQHJxomEQ0";
const char mqttUsername[] = "NzcyNRQ5FwIfHSQHJxomEQ0";
const char mqttPassword[] = "WrFlbyyBYvTUXku2XcyxVcks";
const char publishChannel[] = "channels/1762262/publish";
const char subscribeChannel[] = "channels/1762262/subscribe";

WiFiClientSecure net;
MQTTClient mqtt(4096);

unsigned long lastMillis = 0;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi '" + String(ssid) + "' ...");

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println(" connected!");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("Incoming Status: " + payload);
  Serial.println();
}

void connectToThingspeakMQTT(){
  Serial.print("Requesting Secure Thingspeak MQTT ...");
  
  net.setCACert(rootCACertificate);
  
  mqtt.begin("mqtt3.thingspeak.com", 8883, net);
  mqtt.onMessage(messageReceived);
  
  while (!mqtt.connect(mqttClientId, mqttUsername, mqttPassword)) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(" connected!");

  Serial.println("Subscribe to: " + String(subscribeChannel));
  
  mqtt.subscribe(subscribeChannel);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
 
  connectToWiFi();
  connectToThingspeakMQTT();
}

void loop() {
  mqtt.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  if (!mqtt.connected()) {
    connectToThingspeakMQTT();
  }

  // publish a message roughly every 10 seconds.
  if (millis() - lastMillis > 16000) {
    lastMillis = millis();

    String payload = "field1=";
    payload += random(1, 10);
    payload += "&field2=";
    payload += random(11, 99);
    payload += "&status=MQTTPUBLISH";

    Serial.println("Data to Publish: " + payload);
    Serial.println("Publish to: " + String(publishChannel));
    
    mqtt.publish(String(publishChannel), payload);
  }
}
