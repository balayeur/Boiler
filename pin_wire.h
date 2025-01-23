#ifndef PIN_WIRE_H
#define PIN_WIRE_H

// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS    4   // Пин для подключения датчиков температуры
#define BURNER_IN_PIN   5   // Пин для чтения сигнала от горелки
#define BURNER_OUT_PIN  14  // Пин для управления реле горелки 
#define ADC_PIN         A0  


#define LED_01_PIN      2   // Пин для светодиода
#define LED_02_PIN      16  // Пин для светодиода


// static const int pwmInputPin = BURNER_IN_PIN;

// #define PIN_WIRE_SDA (4)
// #define PIN_WIRE_SCL (5)

// static const uint8_t SDA = PIN_WIRE_SDA;
// static const uint8_t SCL = PIN_WIRE_SCL;

// static const uint8_t LED_BUILTIN = 16;
// static const uint8_t BUILTIN_LED = 16;

// static const uint8_t D0   = 16;
// static const uint8_t D1   = 5;
// static const uint8_t D2   = 4;
// static const uint8_t D3   = 0;
// static const uint8_t D4   = 2;
// static const uint8_t D5   = 14;
// static const uint8_t D6   = 12;
// static const uint8_t D7   = 13;
// static const uint8_t D8   = 15;
// static const uint8_t D9   = 3;
// static const uint8_t D10  = 1;

// ESP8266 12-E NodeMCU Kit
// The ESP8266 12-E NodeMCU kit pinout diagram is shown below

//              ┌────────┐
//        ADC0  |A0    D0|  GPIO16  WAKE 
//    RESERVED  |RSV   D1|  GPIO5   SCL 
//    RESERVED  |RSV   D2|  GPIO4   SDA 
// SDD3 GPIO10  |SD3   D3|  GPIO0   FLASH 
// SDD2  GPIO9  |SD2   D4|  GPIO2   TXD1
// SDD1   MISO  |SD1  3V3|  3V3
// SDCMD    CS  |CMD  GND|  GND 
// SDD0   MISO  |SDD   D5|  GPIO14  SCLK 
// SDCLK  SCLK  |CLK   D6|  GPIO12  MISO 
//         GND  |GND   D7|  GPIO13  MOSI RXD2
//         3V3  |3V3   D8|  GPIO15  CS   TXD2 
//          EN  |EN    RX|  GPIO3   RXD0 
//         RST  |RST   TX|  GPIO1   TXD0 
//         GND  |GND  GND|  GND 
//         VIN  |VIN  3V3|  3V3
//              └────────┘


#endif