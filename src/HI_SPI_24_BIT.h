/**
 * @file HI_SPI_24_BIT.h
 * @brief Custom 24-Bit SPI Driver for Arduino Nano (ATmega328P)
 *
 * This library provides a custom, non-Arduino-SPI-library dependent
 * driver for 24-bit SPI communication on the Arduino Nano.
 * It uses direct register manipulation for speed and control.
 *
 * SPI Pin Mapping (Fixed for Arduino Nano):
 * MOSI: D11 (PB3)
 * MISO: D12 (PB4)
 * CLK:  D13 (PB5)
 * SS:   Selectable by user sketch (any digital pin)
 *
 * @version 1.0.0
 * @author David Mullis
 * @license GPL-3.0
 *
 * @disclaimer
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HI_SPI_24_BIT_H
#define HI_SPI_24_BIT_H

#include <Arduino.h>

// Define SPI Modes for clarity
#define SPI_MODE_0 0x00 // CPOL=0, CPHA=0
#define SPI_MODE_1 0x04 // CPOL=0, CPHA=1
#define SPI_MODE_2 0x08 // CPOL=1, CPHA=0
#define SPI_MODE_3 0x0C // CPOL=1, CPHA=1

class HI_SPI_24_BIT {
public:
    /**
     * @brief Constructor for the 24-bit SPI driver.
     * @param chipSelectPin The digital pin number to use for the Slave Select (SS) signal.
     */
    HI_SPI_24_BIT(uint8_t chipSelectPin);

    /**
     * @brief Initializes the SPI hardware (ATmega328P) and sets clock speed and mode.
     *
     * @param mode The SPI mode (SPI_MODE_0 to SPI_MODE_3). Default is SPI_MODE_0.
     * @param clockDiv The clock divider, affecting speed (e.g., SPI_CLOCK_DIV2, SPI_CLOCK_DIV16).
     * The accepted values are:
     * - 0x00: F_CPU/4
     * - 0x01: F_CPU/16
     * - 0x02: F_CPU/64
     * - 0x03: F_CPU/128
     * - 0x04: F_CPU/2 (requires SPI2X bit)
     * - 0x05: F_CPU/8 (requires SPI2X bit)
     * - 0x06: F_CPU/32 (requires SPI2X bit)
     * - 0x07: F_CPU/64 (requires SPI2X bit)
     */
    void begin(uint8_t mode = SPI_MODE_0, uint8_t clockDiv = 0x01);

    /**
     * @brief Transmits a 24-bit data value over SPI.
     *
     * @param data The 24-bit (3-byte) data value to send. Only the lower 3 bytes are used.
     */
    void transmit24Bit(uint32_t data);

    /**
     * @brief Transmits a 24-bit data value and receives a 24-bit response.
     *
     * @param data The 24-bit (3-byte) data value to send.
     * @return uint32_t The received 24-bit data (in the lower 3 bytes).
     */
    uint32_t transfer24Bit(uint32_t data);

    /**
     * @brief Manually asserts (pulls LOW) the Slave Select pin.
     */
    void beginTransaction();

    /**
     * @brief Manually deasserts (pulls HIGH) the Slave Select pin.
     */
    void endTransaction();

private:
    uint8_t _csPin;

    /**
     * @brief Internal function to transfer a single 8-bit byte.
     * @param data The byte to send.
     * @return uint8_t The received byte.
     */
    uint8_t transferByte(uint8_t data);
};

#endif // HI_SPI_24_BIT_H
