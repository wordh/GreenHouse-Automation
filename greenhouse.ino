
// Code to use SoftwareSerial
#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(15,14);      // arduino RX pin=15  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin



/*-----( Import needed libraries )-----*/
#include "DHT.h"

/*-----( Declare Constants )-----*/
#define RELAY_ON 0
#define RELAY_OFF 1

/*-----( Declare objects )-----*/
/*-----( Declare Variables )-----*/
#define Relay_1  2  // Arduino Digital I/O pin number
#define Relay_2  3
#define Relay_3  4
#define Relay_4  5
#define Relay_5  7  
#define Relay_6  8
#define Relay_7  9
#define Relay_8  10

#define DHTPIN 6     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);


String apiKey = "01VDQTGVXGKWDYST";     // replace with your channel's thingspeak WRITE API key
String ssid="No_Internet";    // Wifi network SSID
String password ="angel3070";  // Wifi network password

int sensorPin = A0;  
int sensorValue = 0;  
int percent = 0;


boolean DEBUG=true;

//======================================================================== showResponce
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial.available()){
        c=espSerial.read();
        if (DEBUG) Serial.print(c);
      }
    }
                   
}


//========================================================================
boolean thingSpeakWrite(float value1, float value2){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  espSerial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(espSerial.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  // getStr +="&field3=";
  // getStr += String(value3);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  espSerial.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(espSerial.find(">")){
    espSerial.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else{
    espSerial.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}


void setup()   /****** SETUP: RUNS ONCE ******/
{

  DEBUG=true;           // enable debug serial
  
//-------( Initialize Pins so relays are inactive at reset)----
  digitalWrite(Relay_1, RELAY_OFF);
  digitalWrite(Relay_2, RELAY_OFF);
  digitalWrite(Relay_3, RELAY_OFF);
  digitalWrite(Relay_4, RELAY_OFF);
  digitalWrite(Relay_5, RELAY_OFF);
  digitalWrite(Relay_6, RELAY_OFF);
  digitalWrite(Relay_7, RELAY_OFF);
  digitalWrite(Relay_8, RELAY_OFF);   
  
//---( THEN set pins as outputs )----  
  pinMode(Relay_1, OUTPUT);   
  pinMode(Relay_2, OUTPUT);  
  pinMode(Relay_3, OUTPUT);  
  pinMode(Relay_4, OUTPUT);    
  pinMode(Relay_5, OUTPUT);   
  pinMode(Relay_6, OUTPUT);  
  pinMode(Relay_7, OUTPUT);  
  pinMode(Relay_8, OUTPUT);
  delay(4000); //Check that all relays are inactive at Reset

  dht.begin();

   espSerial.begin(115200);  // enable software serial
  
  espSerial.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
  showResponse(5000);

   if (DEBUG)  Serial.println("Setup completed");

}//--(end setup )---

int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1023, 465, 0, 100);
  return percentValue;
}

void printValuesToSerial()
{
  Serial.print("\n\nAnalog Value: ");
  Serial.print(sensorValue);
  Serial.print("\nPercent: ");
  Serial.print(percent);
  Serial.print("%");
  Serial.print("\n");
}


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  sensorValue = analogRead(sensorPin);
  percent = convertToPercent(sensorValue);
  printValuesToSerial();
  delay(1000);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");


  if(f>80) // Temp High
  {
    digitalWrite(Relay_3, RELAY_OFF);// set the Relay OFF
    delay(1000);              // wait for a second
    digitalWrite(Relay_4, RELAY_ON);// set the Relay ON
    delay(1000);              // wait for a second
    Serial.println("Fan On");
  }
  else {
    digitalWrite(Relay_3, RELAY_OFF);// set the Relay OFF
    delay(1000);              // wait for a second
    digitalWrite(Relay_4, RELAY_OFF);// set the Relay ON
    delay(1000);              // wait for a second
    Serial.println("Fan OFF");
  }
  if(f<60) // Temp Low
  {
    digitalWrite(Relay_7, RELAY_OFF);// set the Relay OFF
    delay(1000);              // wait for a second
    digitalWrite(Relay_8, RELAY_ON);// set the Relay ON
    delay(1000);              // wait for a second
  }
  else{
    digitalWrite(Relay_7, RELAY_OFF);// set the Relay OFF
    delay(1000);              // wait for a second
    digitalWrite(Relay_8, RELAY_OFF);// set the Relay ON
    delay(1000);              // wait for a second
  }

  if(h>70) // Humidity High
  {
    digitalWrite(Relay_1, RELAY_OFF);// set the Relay OFF
    delay(1000);              // wait for a second
    digitalWrite(Relay_2, RELAY_ON);// set the Relay ON
    delay(1000);              // wait for a second
    Serial.println("Exhaust Fan On");    
    
  }
  else {
    digitalWrite(Relay_1, RELAY_OFF);// set the Relay OFF
    delay(1000);              // wait for a second
    digitalWrite(Relay_2, RELAY_OFF);// set the Relay ON
    delay(1000);              // wait for a second
    Serial.println("Exhaust Fan OFF");
  }

  if(percent <30)
  {
    digitalWrite(Relay_5, RELAY_OFF);// set the Relay OFF
    delay(1000);              // wait for a second
    digitalWrite(Relay_6, RELAY_ON);// set the Relay ON
    delay(1000);              // wait for a second
    
  }
  else {
    digitalWrite(Relay_5, RELAY_OFF);// set the Relay OFF
    delay(1000);              // wait for a second
    digitalWrite(Relay_6, RELAY_OFF);// set the Relay ON
    delay(1000);              // wait for a second
    
    }
  thingSpeakWrite(t,h);
  delay(20000);

}//--(end main loop )---



//*********( THE END )***********

