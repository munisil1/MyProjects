#include <DHT.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include "DHT.h"   //the library used for the DHT sensor
#define switch 4      //the switch is connected to pin 4 of the arduino
#define photocell A4    //photocell is connected to the analog pin A4 of arduino
#define sensor A0     //The sensor is connected to analog pin A0 of the sensor
#define DHTPIN 2     //the second pin of the humidity sensor connected to pin 2 of arduino
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);   //initialization for the DHT sensor

void setup() {
  //initialize communication with the serial monitor
  Serial.begin(9600);

  //set the switch, photocell and sensor as inputs in the arduino
  pinMode(switch, INPUT);
  pinMode(photocell, INPUT);
  pinMode(sensor, INPUT);

  //enable the dht function from the library
  dht.begin();
}

void loop() {
  // Wait a 20 seconds between measurements.
  delay(20);

  float light = analogRead(photocell);  //read the value of the ight from the photocell
  //read the value of the temperature from the sensor
  float temp = ((5.0 * analogRead(sensor) * 100.0) / 1024);
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

  //IF THE SWITCH IS OFF PRINT VALUES WITH UNITS
  if ( digitalRead(switch) == 0) {
    //read the temperature from the sensor
    Serial.print("Temperature from temperature sensor :");
    Serial.print(temp);
    Serial.println(" *C ");
    Serial.print("Temperature from DHT: ");
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print(f);
    Serial.println(" *F");
    Serial.print("Light level: ");
    Serial.print( light);
    Serial.println(" Lux");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println(" %");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C\t ");
    Serial.print(hif);
    Serial.println(" *F");
  }
  //WHEN THE SWITCH IS OFF PRINT ONLY NUMERICAL VALUES
  //only print temperature from the temperature sensor, ambient light and humidity from sensor
  //need to comment “,” if used in Matlab
  else {
    Serial.print(temp);
    Serial.print(",");
    Serial.print(light);
    Serial.print(",");
    Serial.println(h);

  }
}


