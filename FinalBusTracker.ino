#include <TinyGPS++.h>
#include <WiFi.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>  //ArduinoJson by Benoit Blanchon Version 6.13.0
#include <MQTT.h> 

TinyGPSPlus gps;
SoftwareSerial ss(16,17);//arduino rx,tx
WiFiClient client;
WiFiServer server(80);
MQTTClient mqtt(1024);

const char* ssid     = "iPhone 30 Pro Max";
const char* password = "123456789";
const char* broker ="broker.hivemq.com";  //Server PTSS
const char* unique_id = "cb36fe62-76c1-48a7-b9f7-0ca16ff1a0";

String topic_subscribe = "esp00/tracking"; 
String topic_publish = "esp00/tracking";
String yazi;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  mqtt.begin(broker,1883,client);
  setup_wifi();
  setup_mqtt();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: "); //address to view map
  Serial.println(WiFi.localIP());
  server.begin();
}
//------------------------------------------------

//User Function - setup_mqtt()
//------------------------------------------------
void setup_mqtt(){
  while(!mqtt.connect(unique_id)){
    Serial.print(".");
    delay(500);
  }
  Serial.println("MQTT connected.");
  mqtt.subscribe(topic_subscribe);
}

void loop(){
  smartdelay(1000);
  mqtt.loop();
  if(!mqtt.connected()){
    setup_mqtt();
  }
  WiFiClient client = server.available();    // listen for incoming clients
  float flat, flon;
  unsigned long age;
  flat = gps.location.lat();
  flon = gps.location.lng();
  age = gps.location.age();
  
  if (client) {                             
    Serial.println("new client");          
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            
      if (client.available()) {             // if there's client data
        char c = client.read();          // read a byte
          if (c == '\n') {                      // check for newline character,
          if (currentLine.length() == 0) {  // if line is blank it means its the end of the client HTTP request
      
9yazi="<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><meta http-equiv='X-UA-Compatible' content='ie=edge'><title>My Google Map</title><style>#map{height:400px;width:100%;}</style></head> <body><h1>My Google Map</h1><div id='map'></div><script>function initMap(){var options = {zoom:8,center:{lat:";
    yazi+=flat;
    yazi+=",lng:";
    yazi+=flon;
    yazi+="}};var map = new google.maps.Map(document.getElementById('map'), options);google.maps.event.addListener(map, 'click', function(event){addMarker({coords:event.latLng});});var markers = [{coords:{lat:";
yazi+=flat;
yazi+=",lng:";
yazi+=flon;
yazi+="}}];for(var i = 0;i < markers.length;i++){addMarker(markers[i]);}function addMarker(props){var marker = new google.maps.Marker({position:props.coords,map:map,});if(props.iconImage){marker.setIcon(props.iconImage);}if(props.content){var infoWindow = new google.maps.InfoWindow({content:props.content});marker.addListener('click', function(){infoWindow.open(map, marker);});}}}</script><script async defer src='https://maps.googleapis.com/maps/api/js?key=AIzaSyDHNUG9E870MPZ38LzijxoPyPgtiUFYjTM&callback=initMap'></script></body></html>";
 
 
            client.print(yazi);
            // The HTTP response ends with another blank line:
            client.println();

  //Publish Payload to MQTT every 15 Sec
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(" Longitude: ");
        Serial.println(gps.location.lng(), 6);

    StaticJsonDocument<200>data;
    data["Latitude"]=gps.location.lat(), 6;
    data["Longitude"]=gps.location.lng(), 6;
    
    //====Debuging Message=====
    serializeJson(data,Serial);
    Serial.println();
    serializeJsonPretty(data,Serial);
    Serial.println();
    //=========================
    
    String payload;
    serializeJson(data,payload);    

    mqtt.publish(topic_publish,payload);                        
      }
    }
  }
            // break out of the while loop:
            break;
          } else {   currentLine = ""; }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
         // here you can check for any keypresses if your web server page has any
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    }
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
