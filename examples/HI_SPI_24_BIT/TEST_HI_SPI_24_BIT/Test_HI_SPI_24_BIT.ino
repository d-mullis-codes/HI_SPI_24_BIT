/**
 * @file Test_HI_SPI_24_BIT.ino
 * @brief Example sketch to test the custom 24-Bit SPI Driver.
 *
 * This sketch demonstrates the basic initialization and use of
 * the HI_SPI_24_BIT library's 24-bit transfer functions.
 */

#include <HI_SPI_24_BIT.h>

// Define the pin to use for the Slave Select line (Example uses D7)
const int CHIP_SELECT_PIN = 7; 

// Instantiate the 24-bit SPI driver
HI_SPI_24_BIT spi24(CHIP_SELECT_PIN);

// Data to be sent (we care about the lower 24 bits)
uint32_t data_out_1 = 0xAA55AA; 
uint32_t data_out_2 = 0x123456;
uint32_t data_out_3 = 0xFF00FF;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial port to connect. Needed for native USB port only.
    }

    // Initialize SPI:
    // Mode 0 (CPOL=0, CPHA=0)
    // Clock Divider 0x01 (F_CPU/16, which is 1MHz on a 16MHz Nano)
    spi24.begin(SPI_MODE_0, 0x01); 
    
    Serial.println("--- HI_SPI_24_BIT Test Sketch ---");
    Serial.print("CS Pin set to D");
    Serial.println(CHIP_SELECT_PIN);
    Serial.println("SPI Initialized (Mode 0, F_CPU/16)");
    Serial.println("---------------------------------");
}

void loop() {
    uint32_t data_in = 0;

    // --- TEST 1: transfer24Bit ---
    Serial.println("TEST 1: 24-Bit Transfer (TX/RX)");
    Serial.print("   TX Data: 0x");
    Serial.println(data_out_1, HEX);

    spi24.beginTransaction();
    data_in = spi24.transfer24Bit(data_out_1);
    spi24.endTransaction();

    Serial.print("   RX Data: 0x");
    // Display the lower 24 bits of the received data
    Serial.println(data_in & 0xFFFFFF, HEX); 

    // --- TEST 2: transmit24Bit (Send Only) ---
    Serial.println("\nTEST 2: 24-Bit Transmit (TX Only)");
    Serial.print("   TX Data: 0x");
    Serial.println(data_out_2, HEX);

    spi24.beginTransaction();
    spi24.transmit24Bit(data_out_2);
    spi24.endTransaction();
    
    // --- TEST 3: Another transfer24Bit ---
    Serial.println("\nTEST 3: 24-Bit Transfer (TX/RX)");
    Serial.print("   TX Data: 0x");
    Serial.println(data_out_3, HEX);

    spi24.beginTransaction();
    data_in = spi24.transfer24Bit(data_out_3);
    spi24.endTransaction();

    Serial.print("   RX Data: 0x");
    Serial.println(data_in & 0xFFFFFF, HEX);

    Serial.println("\n--- Cycle Complete ---");

    delay(3000); 
}
