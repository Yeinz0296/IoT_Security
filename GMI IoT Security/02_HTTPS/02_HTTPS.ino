#include <WiFi.h>
#include <HTTPClient.h>
#include "rootCA.h"

const char ssid[] = "myinvententerprise";
const char password[] = "04222682";
const char deviceDeveloperId[] = "deviceDefault@Yein0296";
const char deviceAccessToken[] = "XY0ocqWVqjhYrENup9x8bpsiT71015In";

unsigned long lastMillis = 0;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi '" + String(ssid) + "' ...");

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println(" connected!");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);

  // STEP 1 - Connect to Wi-Fi Router
  connectToWiFi();

}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
  
  // STEP 2 - Data Acquisition
  int randomOne = random(1, 98);
  int randomTwo = random(99, 999);

  if(millis() - lastMillis > 10000){
    lastMillis = millis();

    Serial.println("Generated Data:");
    Serial.print("> RandomOne: " + String(randomOne));
    Serial.println(", RandomTwo: " + String(randomTwo));

    // STEP 3 - Generate JSON data
    String jsonData = "{";
    jsonData += "\"device_developer_id\":\"";
    jsonData += String(deviceDeveloperId) + "\",";
    jsonData += "\"data\": {";
    jsonData += "\"rOne\": " + (String)randomOne + ",";
    jsonData += "\"rTwo\": " + (String)randomTwo;
    jsonData += "}";
    jsonData += "}";

    // STEP 4 - Requesting Secure Connection
    Serial.println("Requesting HTTPS");
    WiFiClientSecure *client = new WiFiClientSecure;
  
    if(client) {
      client -> setCACert(rootCACertificate);
      {
        // STEP 5 - Favoriot HTTPS API Request
        
        HTTPClient https;
      
        https.begin(*client, "https://apiv2.favoriot.com/v2/streams");
      
        https.addHeader("Content-Type", "application/json");
        https.addHeader("apikey", deviceAccessToken);
        
        int httpCode = https.POST(jsonData);
      
        if(httpCode > 0){
          Serial.print("> HTTP Request: ");
            
          if (httpCode == 201) {
            Serial.print("Success, ");
          }
          else{
            Serial.print("Error, ");
          }
          
          Serial.println(https.getString());
        }
        else{
          Serial.println("> HTTPS Request Connection Error!");
        }
      
        https.end();
        Serial.println();
      }

      delete client;
    }

    else {
      Serial.println("Unable to create client");
    }
  }
}
