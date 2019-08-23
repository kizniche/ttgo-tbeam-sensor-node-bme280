## BME280 Sensor Node for The Things Network

### For the TTGO T-Beam development platform

### Proof-of-concept for using the T-BEAM to acquire measurements from the BME280 sensor and transmitting humidity, temperature, and pressure to The Things Network

#### Based on the code from [xoseperez/ttgo-beam-tracker](https://github.com/xoseperez/ttgo-beam-tracker), with excerpts from [dermatthias/Lora-TTNMapper-T-Beam](https://github.com/dermatthias/Lora-TTNMapper-T-Beam) to fix an issue with incorrect GPS data. I also added support for the US 915 MHz frequency.

This is a LoRaWAN node based on the [TTGO T-Beam](https://github.com/LilyGO/TTGO-T-Beam) development platform using the SSD1306 I2C OLED display.
It uses a RFM95 by HopeRF and the MCCI LoRaWAN LMIC stack. This sample code is configured to connect to The Things Network using the US 915 MHz frequency by default, but can be changed to EU 868 MHz.

To start, install the dependencies, below. Then edit ```src/credentials.h``` to use either ```USE_ABP``` or ```USE_OTAA``` and the Keys/EUIs for your Application's Device from [The Things Network](https://www.thethingsnetwork.org/). Add the TTN Mapper integration to your Application (and optionally the Data Storage integration if you want to access the GPS location information yourself), then add the Decoder code, below. Compile the code and upload it to your TTGO T-Beam. Turn on the device and once a GPS lock is acquired, the device will start sending data.

#### Arduino IDE Board

Follow the directions at [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32) and use board 'Heltec_WIFI_LoRa_32'.

#### Arduino IDE Library Dependencies

 - [mcci-catena/arduino-lmic](https://github.com/mcci-catena/arduino-lmic)
 - [mikalhart/TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)
 - [ThingPulse/esp8266-oled-ssd1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)
 - [adafruit/Adafruit_BME280_Library](https://github.com/adafruit/Adafruit_BME280_Library)
 - [adafruit/Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

#### TTN Decoder

```C
function Decoder(bytes, port) {
    // Decode an uplink message from a buffer
    // (array) of bytes to an object of fields.
    var decoded = {};

    // Humidity
    var rawHum = bytes[0] + bytes[1] * 256;
    decoded.humidity = sflt162f(rawHum) * 100;
    // Pressure
    var rawPress = bytes[2] + bytes[3] * 256;
    decoded.pressure = sflt162f(rawPress) * 100000;
    // Temperature
    var rawTemp = bytes[4] + bytes[5] * 256;
    decoded.temperature = sflt162f(rawTemp) * 100;
    // Dew point
    decoded.dewpoint = calculateDP(decoded.temperature, decoded.humidity);
    // Vapor Pressure Deficit
    decoded.vpd = calculateVPD(decoded.temperature, decoded.humidity);

    return decoded;
}

function calculateVPD(temperature, relative_humidity) {
    // Calculates Vapor Pressure Deficit
    var saturated_vapor_pressure = 610.7 * Math.pow(10, (7.5 * temperature / (237.3 + temperature)));
    return ((100 - relative_humidity) / 100) * saturated_vapor_pressure;
}

function calculateDP(temperature, humidity) {
    // Calculates Dew point
    if (temperature === null || humidity === null) {
        return;
    }
    var tem2 = temperature;
    var r = humidity;
    var tem = -1.0 * tem2;
    var es = 6.112 * Math.exp(-1.0 * 17.67 * tem / (243.5 - tem));
    var ed = r/100.0 * es;
    var eln = Math.log(ed / 6.112);

    return -243.5 * eln / (eln - 17.67);
}

function sflt162f(rawSflt16) {
    // rawSflt16 is the 2-byte number decoded from wherever;
    // it's in range 0..0xFFFF
    // bit 15 is the sign bit
    // bits 14..11 are the exponent
    // bits 10..0 are the the mantissa. Unlike IEEE format,
    // 	the msb is transmitted; this means that numbers
    //	might not be normalized, but makes coding for
    //	underflow easier.
    // As with IEEE format, negative zero is possible, so
    // we special-case that in hopes that JavaScript will
    // also cooperate.
    //
    // The result is a number in the open interval (-1.0, 1.0);
    //

    // throw away high bits for repeatability.
    rawSflt16 &= 0xFFFF;

    // special case minus zero:
    if (rawSflt16 === 0x8000)
        return -0.0;

    // extract the sign.
    var sSign = ((rawSflt16 & 0x8000) !== 0) ? -1 : 1;

    // extract the exponent
    var exp1 = (rawSflt16 >> 11) & 0xF;

    // extract the "mantissa" (the fractional part)
    var mant1 = (rawSflt16 & 0x7FF) / 2048.0;

    // convert back to a floating point number. We hope
    // that Math.pow(2, k) is handled efficiently by
    // the JS interpreter! If this is time critical code,
    // you can replace by a suitable shift and divide.
    return sSign * mant1 * Math.pow(2, exp1 - 15);
}

```

#### Wiring

The following description uses the T-BEAM pin diagram, below. The labeleing of the I2C pins of the BME280 may be confusing to some, therefore I've explained the connections.

### BME280

The BME280 can be powered by either the 5-volt pin or the 3.3-volt pin on the T-BEAM. If powering with 5 volts, connect the T-BEAM pin labeled "5V" to the "VIN" (or similar) of the BME280. If powering with 3.3 volts, connect the T-BEAM pin labeled "3.3" with the "3Vo" (or similar) pin of the BME280. Connect the T-BEAM pin labeled "GND" to the "GND" pin of the BME280. Pins 21 and 22 of the T-BEAM are the I2C SCL and SDA, respectively. The "SCL" pin of the T-BEAM must be connected to the "SCK" pin of the BME280, and the "SDA" pin of the T-BEAM connected to the "SDI" pin of the BME280.

### OLED Display

The OLED display has 4 pins, 3.3V, GND, SDA, and SCL. These must be connected to the corresponding T-BEAM pins of the same name. Make sure you only connect 3.3V to the T-BEAM's 3.3-volt pin, NOT the 5-volt pin.

### The TTGO T-Beam development platform

Note: There are now two versions, the first version (Rev0) and a newer version (Rev1). The GPS module on Rev1 is connected to different pins than Rev0. This code has been successfully tested on REV0, and is in the process of being tested on REV1.

### Rev0

![TTGO T-Beam 01](img/TTGO-TBeam-01.jpg)

![TTGO T-Beam 02](img/TTGO-TBeam-02.jpg)

![TTGO T-Beam 03](img/TTGO-TBeam-03.jpg)

### Rev1

![T-BEAM-Rev1-01](img/T-BEAM-Rev1-01.jpg)

![T-BEAM-Rev1-02](img/T-BEAM-Rev1-02.jpg)
