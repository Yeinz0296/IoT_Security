#include <WiFi.h>
#include <MQTT.h>

const char ssid[] = "myinvententerprise";
const char password[] = "04222682";
const char mqttClientId[] = "LAcWPTAQPSsINScALBArHQg";
const char mqttUsername[] = "LAcWPTAQPSsINScALBArHQg";
const char mqttPassword[] = "KZ3WGWUvNZSUeK6djIyKO0zk";
const char publishChannel[] = "channels/1778922/publish";
const char subscribeChannel[] = "channels/1778922/subscribe";

WiFiClient net;
MQTTClient mqtt(1024);

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
  Serial.print("Connecting to Thingspeak MQTT ...");
  
  mqtt.begin("mqtt3.thingspeak.com", net);
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

  // publish a message roughly every 16 seconds.
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
