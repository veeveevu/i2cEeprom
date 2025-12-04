#ifndef I2CEEPROM_BOARD_CONFIG_H
#define I2CEEPROM_BOARD_CONFIG_H


//----------i2c config
#define I2C_PORT i2c0
#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17
#define I2C_FREQUENCY 100000 //1 MHz (5V), 400 kHz (2.7V, 2.5V) and 100 kHz (1.8V) Compatibility

#define EEPROM_I2C_ADDRESS 0x50   //datasheet device addressing 1010 0 A1 A0 WP = 1010 000 WP --> 7-bit: 0x50

#define EEPROM_MAX_SIZE 32768 // organized as 16,384/32,768 words of 8 bits each
#define EEPROM_PAGE_SIZE 64
#define EEPROM_WRITE_DELAY_MS 5 // 20 or 5 ms
//(5ms applies to the AT24C128/256 devices bearing the process letter “B” on the package)

#define DEBOUNCE_MS 20

//-----------leds & buttons
#define LED_D1_PIN 22 //SW0
#define LED_D2_PIN 21 //SW1
#define LED_D3_PIN 20 //SW2

#define SW0_PIN 9
#define SW1_PIN 8
#define SW2_PIN 7

//-----------log
#define LOG_START_ADDRESS 0
#define LOG_ENTRY_SIZE 64
#define LOG_MAX_ENTRIES 32
#define LOG_TOTAL_SIZE (LOG_ENTRY_SIZE * LOG_MAX_ENTRIES)  //2048 bytes
#define LOG_MAX_STRING_LENGTH 61 //61 chars + null + 2 CRC



#endif //I2CEEPROM_BOARD_CONFIG_H