## BME280 Sensor Node for The Things Network

### For the TTGO T-Beam development platform

### Proof-of-concept for acquiring measurements from the BME280 sensor and transmitting humidity, temperature, and pressure to The Things Network

#### Based on the code from [xoseperez/ttgo-beam-tracker](https://github.com/xoseperez/ttgo-beam-tracker), with excerpts from [dermatthias/Lora-TTNMapper-T-Beam](https://github.com/dermatthias/Lora-TTNMapper-T-Beam) to fix an issue with incorrect GPS data. I also added support for the US 915 MHz frequency.

This is a LoRaWAN node based on the [TTGO T-Beam](https://github.com/LilyGO/TTGO-T-Beam) development platform using the SSD1306 I2C OLED display.
It uses a RFM95 by HopeRF and the MCCI LoRaWAN LMIC stack. This sample code is configured to connect to The Things Network using the US 915 MHz frequency by default, but can be changed to EU 868 MHz.

To start, install the dependencies, below. Then edit ```src/credentials.h``` to use either ```USE_ABP``` or ```USE_OTAA``` and the Keys/EUIs for your Application's Device from [The Things Network](https://www.thethingsnetwork.org/). Add the TTN Mapper integration to your Application (and optionally the Data Storage integration if you want to access the GPS location information yourself). Compile the code and upload it to your TTGO T-Beam. Turn on the device and once a GPS lock is acquired, the device will start sending data.

#### Arduino IDE Board

Follow the directions at [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32) and use board 'Heltec_WIFI_LoRa_32'.

#### Arduino IDE Library Dependencies

 - [mcci-catena/arduino-lmic](https://github.com/mcci-catena/arduino-lmic)
 - [mikalhart/TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)
 - [ThingPulse/esp8266-oled-ssd1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)
 - [adafruit/Adafruit_BME280_Library](https://github.com/adafruit/Adafruit_BME280_Library)
 - [adafruit/Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
 
#### Wiring

The folowing description uses the T-BEAM pin diagram, below. The labeleing of the I2C pins of the BME280 may be confusing to some, therefore I've explained the connections.

### BME280

The BME280 can be powered by either the 5-volt pin or the 3.3-volt pin on the T-BEAM. If powering with 5 volts, connect the T-BEAM pin labeled "5V" to the "VIN" (or similar) of the BME280. If powering with 3.3 volts, connect the T-BEAM pin labeled "3.3" with the "3Vo" (or similar) pin of the BME280. Connect the T-BEAM pin labeled "GND" to the "GND" pin of the BME280. Pins 21 and 22 of the T-BEAM are the I2C SCL and SDA, respectively. The "SCL" pin of the T-BEAM must be connected to the "SCK" pin of the BME280, and the "SDA" pin of the T-BEAM connected to the "SDI" pin of the BME280.

### OLED Display

The OLED display has 4 pins, 3.3V, GND, SDA, and SCL. These must be connected to the corresponding T-BEAM pins of the same name. Make sure you only connect 3.3V to the T-BEAM's 3.3-volt pin, NOT the 5-volt pin.

#### Images

![TTGO T-Beam 01](img/TTGO-TBeam-01.jpg)

![TTGO T-Beam 02](img/TTGO-TBeam-02.jpg)

![TTGO T-Beam 03](img/TTGO-TBeam-03.jpg)
