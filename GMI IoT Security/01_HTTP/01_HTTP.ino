#include <WiFi.h>
#include <HTTPClient.h>

const char ssid[] = "TP-Link_A9C0";
const char password[] = "29699971";
const char deviceDeveloperId[] = "ESP32@ariffinastute";
const char deviceAccessToken[] = "fDgsqwYJR8OONzQhutHFTO2oATdRd3jn";

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
    
    // STEP 4 - Favoriot HTTP API Request - Send data to Favoriot
    HTTPClient http;
  
    http.begin("http://apiv2.favoriot.com/v2/streams");
  
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", deviceAccessToken);
    
    int httpCode = http.POST(jsonData);
  
    if(httpCode > 0){
      Serial.print("> HTTP Request: ");
      
      if (httpCode == 201) {
        Serial.print("Success, ");
      }
      else{
        Serial.print("Error, ");
      }
      
      Serial.println(http.getString());
    }
    else{
      Serial.println("> HTTP Request Connection Error!");
    }
  
    http.end();
    Serial.println();
  }
}
