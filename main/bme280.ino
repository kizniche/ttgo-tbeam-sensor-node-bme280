/*

GPS module

Copyright (C) 2018 by Xose Pérez <xose dot perez at gmail dot com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

uint32_t humidity_bme;
uint32_t pressure_bme;
uint32_t temperature_bme;

char bme_char[32]; // used to sprintf for Serial output

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

void bme_setup() {
    bool status;
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
}

float humidity() {
    return bme.readHumidity();
}

float pressure() {
    return bme.readPressure();
}

float temperature() {
    return bme.readTemperature();
}

void buildBMEPacket(uint8_t txBuffer[6])
{
  char buffer[40];

  //
  // Humidity
  //

  float humidity = bme.readHumidity();

  sprintf(bme_char, "Humidity: %f", humidity);
  Serial.println(bme_char);

  snprintf(buffer, sizeof(buffer), "Humidity: %10.1f\n", humidity);
  screen_print(buffer);

  // adjust for the f2sflt16 range (-1 to 1)
  humidity = humidity / 100;

  // float -> int
  uint16_t payloadHum = LMIC_f2sflt16(humidity);

  // int -> bytes
  byte humLow = lowByte(payloadHum);
  byte humHigh = highByte(payloadHum);

  txBuffer[0] = humLow;
  txBuffer[1] = humHigh;


  //
  // Pressure
  //

  float pressure = bme.readPressure();

  sprintf(bme_char, "Pressure: %f", pressure);
  Serial.println(bme_char);

  snprintf(buffer, sizeof(buffer), "Pressure: %10.1f\n", pressure);
  screen_print(buffer);

  // adjust for the f2sflt16 range (-1 to 1)
  pressure = pressure / 100000;

  // float -> int
  uint16_t payloadPress = LMIC_f2sflt16(pressure);

  // int -> bytes
  byte pressLow = lowByte(payloadPress);
  byte pressHigh = highByte(payloadPress);

  txBuffer[2] = pressLow;
  txBuffer[3] = pressHigh;


  //
  // Temperature
  //

  float temperature = bme.readTemperature();

  sprintf(bme_char, "Temperature: %f", temperature);
  Serial.println(bme_char);

  snprintf(buffer, sizeof(buffer), "Temperature: %10.1f\n", temperature);
  screen_print(buffer);

  // adjust for the f2sflt16 range (-1 to 1)
  temperature = temperature / 100;

  // float -> int
  uint16_t payloadTemp = LMIC_f2sflt16(temperature);

  // int -> bytes
  byte tempLow = lowByte(payloadTemp);
  byte tempHigh = highByte(payloadTemp);

  txBuffer[4] = tempLow;
  txBuffer[5] = tempHigh;
}
