#ifndef PIN_WIRE_H
#define PIN_WIRE_H

// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS    4
#define BURNER_IN_PIN   5
#define ADC_PIN         A0

#define LED_01_PIN      2
#define LED_02_PIN      16


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

//              ┌────────┐
//        ADC0  |A0    30|  GPIO15 (D8) 
//    RESERVED  |RSV   29|  GPIO13 (D7) 
//    RESERVED  |RSV   28|  GPIO12 (D6) 
//      GPIO10  |SD3   27|  GPIO14 (D5) 
//       GPIO9  |SD2  26|  GPIO2 (D4) 
//        MOSI  |SD1    25|  GPIO0 (D3) 
//          CS  |CMD    24|  GPIO4 (D2) 
//   (RST) 3V3  |8    23|  GPIO5 (D1) 
//  GPIO9 (SD3) |9    22|  GPIO3 (TX) 
// GPIO11 (SD1) |10   21|  GPIO1 (RX) 
//  GPIO6 (SD0) |11   20|  GPIO16 (D0) 
//         GND  |12   19|  (A0) A0 
//         VIN  |13   18|  (A0) GND 
//        GPIO7 |14   17|  (A0) 3V3 
//        GPIO8 |15   16|  RST
//              └───────┘


#endif