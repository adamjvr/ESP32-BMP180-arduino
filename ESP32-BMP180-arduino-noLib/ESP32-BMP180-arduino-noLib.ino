#include <Wire.h>

#define BMP180_ADDRESS 0x77 // I2C address of BMP180 sensor

// BMP180 calibration coefficients
int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  readBMP180Coefficients();
}

void loop() {
  float temperature = readBMP180Temperature();
  float pressure = readBMP180Pressure() / 100.0F;
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C");
  Serial.print("\t");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  delay(1000);
}

// Read BMP180 calibration coefficients from sensor
void readBMP180Coefficients() {
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xAA);
  Wire.endTransmission();
  Wire.requestFrom(BMP180_ADDRESS, 22);
  AC1 = Wire.read() << 8 | Wire.read();
  AC2 = Wire.read() << 8 | Wire.read();
  AC3 = Wire.read() << 8 | Wire.read();
  AC4 = Wire.read() << 8 | Wire.read();
  AC5 = Wire.read() << 8 | Wire.read();
  AC6 = Wire.read() << 8 | Wire.read();
  B1 = Wire.read() << 8 | Wire.read();
  B2 = Wire.read() << 8 | Wire.read();
  MB = Wire.read() << 8 | Wire.read();
  MC = Wire.read() << 8 | Wire.read();
  MD = Wire.read() << 8 | Wire.read();
}

// Read BMP180 temperature in degrees Celsius
float readBMP180Temperature() {
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E); // Start temperature measurement
  Wire.endTransmission();
  delay(5);
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP180_ADDRESS, 2);
  int16_t UT = Wire.read() << 8 | Wire.read(); // Raw temperature reading
  int32_t X1 = (UT - AC6) * AC5 >> 15;
  int32_t X2 = MC << 11 / (X1 + MD);
  int32_t B5 = X1 + X2;
  float temperature = (B5 + 8) >> 4;
  temperature /= 10.0F;
  return temperature;
}

// Read BMP180 pressure in Pascals
int32_t readBMP180Pressure() {
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (3 << 6)); // Start pressure measurement with highest resolution (3)
  Wire.endTransmission();
  delay(26); // Wait for conversion to complete
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP180_ADDRESS, 3);
  int32_t UP = (Wire.read() << 16 | Wire.read() << 8 | Wire.read()) >> (8 - 3); // Raw pressure reading
  int32_t B3 = ((AC1 * 4 + X1)
