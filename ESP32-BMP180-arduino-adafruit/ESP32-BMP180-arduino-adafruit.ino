#include <Wire.h>
#include <Adafruit_BMP180.h>

#define BMP180_ADDRESS 0x77 // I2C address of BMP180 sensor

Adafruit_BMP180 bmp180;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  bmp180.begin(BMP180_ADDRESS);
}

void loop() {
  float temperature = bmp180.readTemperature();
  float pressure = bmp180.readPressure() / 100.0F;
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C");
  Serial.print("\t");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  delay(1000);
}
