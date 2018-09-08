
#define SelectBit_A  4  // Arduino Digital I/O pin number
#define SelectBit_B  3
#define SelectBit_C  2


// DHT Sensor Part Starts
#include "DHT.h"
#define DHTPIN 6     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
// DHT Sensor Part Ended

// Soil Moisture Sensor Part Starts

int sensorPin = A0;  
int sensorValue = 0;  
int percent = 0;
// SM Ends

String str;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial1.begin(115200);

  Serial.println("DHTxx test!");

  dht.begin();

  digitalWrite(SelectBit_A, 1);
  digitalWrite(SelectBit_B, 1);
  digitalWrite(SelectBit_C, 1);

  //---( THEN set pins as outputs )----  
  pinMode(SelectBit_A, OUTPUT);   
  pinMode(SelectBit_B, OUTPUT);  
  pinMode(SelectBit_C, OUTPUT);

  delay(4000); //Check that all relays are inactive at Reset


}

void loop() {
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


  //Soil moisture
  sensorValue = analogRead(sensorPin);
  percent = convertToPercent(sensorValue);
  percent = percent -40;
  printValuesToSerial();
  delay(1000);
  // Ends

  str =String("Humidity:")+String(h)+String("Tempurature:")+String(t)+String("Soil Moisture:")+String(percent);
  Serial1.println(str);


  if(percent<100)
  {
    digitalWrite(SelectBit_A, 0);
    digitalWrite(SelectBit_B, 0);
    digitalWrite(SelectBit_C, 1);
    Serial.println("Water Pump On");
  }

  else if(t>25)
  {
  digitalWrite(SelectBit_A, 0);
  digitalWrite(SelectBit_B, 0);
  digitalWrite(SelectBit_C, 0);
  Serial.println("Cooling Fan On");  
  }

  else if (h>50)
  {
  digitalWrite(SelectBit_A, 0);
  digitalWrite(SelectBit_B, 1);
  digitalWrite(SelectBit_C, 1);
  Serial.println("Exhaust Fan On");
  }

  else{
  digitalWrite(SelectBit_A, 1);
  digitalWrite(SelectBit_B, 1);
  digitalWrite(SelectBit_C, 1); 
  Serial.println("Normal Condition");   
  }

//  digitalWrite(SelectBit_A, 0);
//  digitalWrite(SelectBit_B, 0);
//  digitalWrite(SelectBit_C, 1);
//
//  delay(1000);
//
//  digitalWrite(SelectBit_A, 0);
//  digitalWrite(SelectBit_B, 1);
//  digitalWrite(SelectBit_C, 0);
//
//  delay(1000);
//
//  digitalWrite(SelectBit_A, 0);
//  digitalWrite(SelectBit_B, 1);
//  digitalWrite(SelectBit_C, 1);
//
//  delay(1000);
//
//  digitalWrite(SelectBit_A, 0);
//  digitalWrite(SelectBit_B, 0);
//  digitalWrite(SelectBit_C, 0);

  
}

// Soil Moisture codes 
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
// Soil Moisture Code ends. 
