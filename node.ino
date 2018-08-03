
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "greenhouse-automation-75c24.firebaseio.com"
#define FIREBASE_AUTH "pZWPzfBGdh18RYqpCchSUhTqinzm4tyioVH3ixpT"
#define WIFI_SSID "WORDH FALCON"
#define WIFI_PASSWORD "gigabyte"

String temp,readString;
String humidity, soilMoisture;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}


void loop() { // run over and over
  if (Serial.available()) {
   while (Serial.available()) {
    delay(10);  
      if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
      }
   }
    temp = readString.substring(9,14);
    humidity=readString.substring(26,31);
    soilMoisture=readString.substring(45,47);
    
    Serial.println("Condition is");
    Serial.println(temp);
    Serial.println(humidity);
    Serial.println(soilMoisture);
    Serial.println(readString);
    readString="";

    // set value
     Firebase.setString("Temperature", humidity);
     Firebase.setString("Humidity", temp);
     Firebase.setString("Soil Moisture Level", soilMoisture);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
    }
    delay(1000);
    
  }
}
