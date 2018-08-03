String temp,readString;
String humidity, soilMoisture;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
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
    temp = readString.substring(9,13);
    humidity=readString.substring(26,30);
    soilMoisture=readString.substring(45,46);
    
    Serial.println("Condition is");
    Serial.println(temp);
    Serial.println(humidity);
    Serial.println(soilMoisture);
    Serial.println(readString);
    readString="";
    
  }
}
