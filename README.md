# HI_SPI_24_BIT - Custom 24-Bit SPI Master Driver

## Overview
This library provides a low-level, register-based 24-bit SPI Master driver specifically tailored for the **Arduino Nano (ATmega328P)**. It is designed to offer maximum performance and control by *not* relying on the standard Arduino SPI library, as requested.

## Features
* **24-Bit Transfer:** Optimized functions for sending and receiving 3-byte (24-bit) data chunks.
* **Direct Register Control:** Utilizes `SPCR`, `SPSR`, and `SPDR` registers for high-speed operation.
* **Custom CS Pin:** Allows the user to select any digital pin for the Slave Select (CS) signal.
* **SPI Modes:** Supports all four SPI modes (0, 1, 2, 3).
* **Flexible Clock Speed:** Allows configuration of the clock divider.

## SPI Pin Mapping (Fixed on Arduino Nano)
| Function | Arduino Pin | ATmega328P Pin |
| :--- | :--- | :--- |
| MOSI | D11 | PB3 |
| MISO | D12 | PB4 |
| CLK | D13 | PB5 |
| SS | User Selectable | N/A |

## Usage

### Installation
1.  Create a folder named `HI_SPI_24_BIT` in your Arduino `libraries` folder.
2.  Place the `src/HI_SPI_24_BIT.h` and `src/HI_SPI_24_BIT.cpp` files inside a subfolder named `src`.
3.  Place `keywords.txt`, `library.properties`, and this `README.md` file in the main `HI_SPI_24_BIT` folder.

### Basic Example

```cpp
#include <HI_SPI_24_BIT.h>

// Define the pin to use for the Slave Select line
const int CS_PIN = 7; 

// Instantiate the driver
HI_SPI_24_BIT spi24(CS_PIN);

void setup() {
    Serial.begin(115200);
    // Initialize SPI: Mode 0, Clock Divider 0x01 (F_CPU/16 = 1MHz)
    spi24.begin(SPI_MODE_0, 0x01); 
    Serial.println("HI_SPI_24_BIT Driver Initialized.");
}

void loop() {
    // Example: Transmit and receive a 24-bit value
    uint32_t data_out = 0xDEADC0FFEE; // Only the lower 24 bits (0xADC0FFEE) are used
    uint32_t data_in = 0;

    Serial.print("Sending 0x");
    Serial.println(data_out & 0xFFFFFF, HEX); // Print only 24 bits

    // Begin the transaction (CS LOW)
    spi24.beginTransaction();
    
    // Transfer data
    data_in = spi24.transfer24Bit(data_out);
    
    // End the transaction (CS HIGH)
    spi24.endTransaction();

    Serial.print("Received 0x");
    // Only the lower 24 bits of the received 32-bit variable are relevant
    Serial.println(data_in & 0xFFFFFF, HEX); 

    delay(2000); 
}
