/*

Credentials file

*/

#pragma once

// Only one of these settings must be defined
//#define USE_ABP
#define USE_OTAA

//#ifdef USE_ABP

    // LoRaWAN NwkSKey, network session key
    static const u1_t PROGMEM NWKSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // LoRaWAN AppSKey, application session key
    static const u1_t PROGMEM APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // LoRaWAN end-device address (DevAddr)
    // This has to be unique for every node
    static const u4_t DEVADDR = 0x00000000;

#endif

#ifdef USE_OTAA

    // This EUI must be in little-endian format, so least-significant-byte
    // first. When copying an EUI from ttnctl output, this means to reverse
    // the bytes. For TTN issued EUIs the last bytes should be 0x00, 0x00,
    // 0x00.
    static const u1_t PROGMEM APPEUI[8]  = { 0x5E, 0xCC, 0x3C, 0x20, 0xED, 0x0A, 0xF1, 0x52 };

    // This should also be in little endian format, see above.
    static const u1_t PROGMEM DEVEUI[8]  = { 0xC5, 0x78, 0x05, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

    // This key should be in big endian format (or, since it is not really a
    // number but a block of memory, endianness does not really apply). In
    // practice, a key taken from ttnctl can be copied as-is.
    // The key shown here is the semtech default key.
    static const u1_t PROGMEM APPKEY[16] = { 0x98, 0xFD, 0x0C, 0xA0, 0x08, 0x06, 0xA1, 0xCC, 0xAB, 0xF3, 0xE2, 0x5F, 0x0C, 0x7B, 0xF7, 0x4C };

#endif
