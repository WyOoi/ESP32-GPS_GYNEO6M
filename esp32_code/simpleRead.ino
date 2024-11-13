#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss(16, 17); // RX, TX
unsigned long sysTick = 0;
const unsigned int TIME_INTERVAL = 2000; // 2 Sec

void setup() {
  Serial.begin(9600);      // Start Serial Monitor
  ss.begin(9600);          // Start GPS Serial
}

void read(){
  while (ss.available() > 0) {
    char c = ss.read();    // Read one character
    gps.encode(c);         // Encode the character into GPS data
    
    if (gps.location.isValid()) {// If have a valid location
      if (millis() > sysTick) {// Print latitude and longitude at specified intervals
        sysTick = millis() + TIME_INTERVAL;
        Serial.print("Latitude: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(" Longitude: ");
        Serial.println(gps.location.lng(), 6);
      }
    }
  }
  delay(100);// small delay prevent overwhelming the serial output
}
void loop() {
  // Check for available GPS data
  read();
}
