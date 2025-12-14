/**
 * @file HI_SPI_24_BIT.cpp
 * @brief Implementation for the Custom 24-Bit SPI Driver.
 */

#include "HI_SPI_24_BIT.h"
#include <avr/io.h> // Required for direct register access

// SPI Register Definitions (ATmega328P)
// SPCR: SPI Control Register
// SPSR: SPI Status Register
// SPDR: SPI Data Register
// PORTB: Port B Data Register (Contains MOSI, MISO, CLK, and the standard SS D10)

HI_SPI_24_BIT::HI_SPI_24_BIT(uint8_t chipSelectPin)
    : _csPin(chipSelectPin) {
    // Constructor only stores the CS pin number.
}

void HI_SPI_24_BIT::begin(uint8_t mode, uint8_t clockDiv) {
    // 1. Configure the CS pin
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH); // Deassert CS by default

    // 2. Configure the SPI pins (D11=MOSI, D12=MISO, D13=CLK) as per Nano's mapping
    // D11 (MOSI) is PB3
    // D12 (MISO) is PB4 (Input)
    // D13 (CLK) is PB5
    
    // Set MOSI (PB3) and SCK (PB5) as outputs. MISO (PB4) is left as input (default).
    // Note: The standard SS pin (D10/PB2) is also set as output to enable Master mode.
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);

    // 3. Configure the SPCR register
    // - Enable SPI (SPE)
    // - Set Master mode (MSTR)
    // - Set Clock Polarity/Phase (CPOL/CPHA) via 'mode' bits (3 and 2)
    // - Set Clock Rate via 'clockDiv' bits (0 and 1)
    
    // Base configuration: Enable SPI, Master mode
    uint8_t control_reg = (1 << SPE) | (1 << MSTR);
    
    // Add mode bits (CPOL/CPHA)
    control_reg |= (mode & (1 << CPOL)) | (mode & (1 << CPHA));
    
    // Add clock bits (SPR0/SPR1)
    control_reg |= (clockDiv & 0x03);

    SPCR = control_reg;

    // 4. Configure the SPSR register for clock speed (SPI2X)
    // Check if the fast-mode clock divider bits (0x04-0x07) were requested.
    if (clockDiv & 0x04) {
        SPSR |= (1 << SPI2X); // Set double speed bit
    } else {
        SPSR &= ~(1 << SPI2X); // Clear double speed bit
    }
}

uint8_t HI_SPI_24_BIT::transferByte(uint8_t data) {
    SPDR = data;                  // Start transmission
    while (!(SPSR & (1 << SPIF))); // Wait for transfer to complete (SPIF bit set)
    return SPDR;                  // Read the received data
}

void HI_SPI_24_BIT::transmit24Bit(uint32_t data) {
    // Transmit only - assumes CS is managed externally or transaction is started.
    
    // Send MSB (Byte 2)
    transferByte((uint8_t)(data >> 16));
    
    // Send Mid Byte (Byte 1)
    transferByte((uint8_t)(data >> 8));
    
    // Send LSB (Byte 0)
    transferByte((uint8_t)(data));
}

uint32_t HI_SPI_24_BIT::transfer24Bit(uint32_t data) {
    uint32_t receivedData = 0;
    
    // 1. Send MSB (Byte 2) and receive High Byte
    receivedData |= ((uint32_t)transferByte((uint8_t)(data >> 16)) << 16);
    
    // 2. Send Mid Byte (Byte 1) and receive Mid Byte
    receivedData |= ((uint32_t)transferByte((uint8_t)(data >> 8)) << 8);
    
    // 3. Send LSB (Byte 0) and receive Low Byte
    receivedData |= ((uint32_t)transferByte((uint8_t)(data)));
    
    return receivedData;
}

void HI_SPI_24_BIT::beginTransaction() {
    digitalWrite(_csPin, LOW); // Assert CS (active LOW)
}

void HI_SPI_24_BIT::endTransaction() {
    digitalWrite(_csPin, HIGH); // Deassert CS
}
