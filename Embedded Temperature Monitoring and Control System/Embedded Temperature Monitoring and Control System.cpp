#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Function to initialize ADC
void ADC_init() {
    ADMUX = (1<<REFS0); // AVCC as reference
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1); // Enable ADC, Prescaler = 64
}

// Function to read ADC value from a specified channel
uint16_t ADC_read(uint8_t channel) {
    channel &= 0x07; // Select ADC channel (0-7)
    ADMUX = (ADMUX & 0xF8) | channel; // Clear last 3 bits of ADMUX and set the channel
    ADCSRA |= (1<<ADSC); // Start conversion
    while(ADCSRA & (1<<ADSC)); // Wait for conversion to finish
    return ADC;
}

// Function to initialize LCD (assuming a 16x2 LCD connected)
void LCD_init() {
    // Code to initialize LCD for display (4-bit mode)
}

// Function to send command to LCD
void LCD_command(unsigned char cmnd) {
    // Code to send command to LCD
}

// Function to display data on LCD
void LCD_data(unsigned char data) {
    // Code to send data to LCD
}

// Function to display temperature on LCD
void display_temperature(uint16_t temp) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "Temp: %d C", temp);
    LCD_command(0x80); // Move to first line of LCD
    LCD_data(buffer);  // Display temperature
}

// Main function
int main() {
    ADC_init(); // Initialize ADC
    LCD_init(); // Initialize LCD
    uint16_t temperature;
    
    // Set threshold temperature for fan control (e.g., 30 degrees Celsius)
    uint16_t threshold = 300; // Value based on your sensor scaling

    // Set fan pin as output (assuming connected to PORTD)
    DDRD |= (1<<PD6);

    while(1) {
        // Read temperature from ADC channel 0 (connected to temperature sensor)
        temperature = ADC_read(0);

        // Display temperature on LCD
        display_temperature(temperature);

        // Control fan based on temperature threshold
        if (temperature > threshold) {
            PORTD |= (1<<PD6);  // Turn on fan
        } else {
            PORTD &= ~(1<<PD6); // Turn off fan
        }

        _delay_ms(500); // Delay to prevent frequent switching
    }
}
