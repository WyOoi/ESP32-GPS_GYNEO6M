#include <TinyGPS++.h> //https://github.com/mikalhart/TinyGPSPlus
#include <WiFi.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>

//enter WIFI credentials
const char* ssid     = "replace with your WiFi name";
const char* password = "replace with your password";

//enter domain name and path
//http://www.example.com/gpsdata.php
const char* SERVER_NAME = "http://abc.atwebpages.com/gpsdata.php";

//ESP32_API_KEY is the exact duplicate of, ESP32_API_KEY in config.php file
//Both values must be same
String ESP32_API_KEY = "Ad5F10jkBM0";

//Send an HTTP POST request every 30 seconds
unsigned long previousMillis = 0;
long interval = 30000;

SoftwareSerial neogps(16, 17);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  Serial.println("esp32 serial initialize");

  neogps.begin(9600);
  Serial.println("neogps serial initialize");
  delay(1000);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
       previousMillis = currentMillis;
       //Send an HTTP POST request every 30 seconds
       sendGpsToServer();
    }
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  delay(1000);  
}

int sendGpsToServer()
{
  //Can take up to 60 seconds
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 2000;){
    while(neogps.available()){
      if(gps.encode(neogps.read())){
        if(gps.location.isValid() == 1){
          newData = true;
          break;
        }
      }
    }
  }
  String latitude;
  latitude = String(gps.location.lat(), 6); // Latitude in degrees (double)

  //If newData is true
  if(true && latitude != "0.000000"){
    newData = false;
    
    String latitude, longitude;

    latitude = String(gps.location.lat(), 6); // Latitude in degrees (double)
    longitude = String(gps.location.lng(), 6); // Longitude in degrees (double)

    //Serial.print("Latitude= "); 
    //Serial.print(latitude);
    //Serial.print(" Longitude= "); 
    //Serial.println(longitude);

    HTTPClient http;
    
    //domain name with path
    http.begin(SERVER_NAME);
    
    //Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    //----------------------------------------------
    //HTTP POST request data
    String gps_data;
    gps_data = "api_key="+ESP32_API_KEY;
    gps_data += "&lat="+latitude;
    gps_data += "&lng="+longitude;

    Serial.print("gps_data: ");
    Serial.println(gps_data);
    //----------------------------------------------
  
    //Send HTTP POST Request and get response
    int httpResponseCode = http.POST(gps_data);
    String httpResponseString = http.getString();
  
    //--------------------------------------------------------------------------------
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println(httpResponseString);
    }
    else {
      Serial.print("Error on HTTP request - Error code: ");
      Serial.println(httpResponseCode);
      Serial.println(httpResponseString);
    }
    http.end();
  }//end if(true)  
}
