/***********************************************************************
 * 
 * Greenhouse Project : monitoring and controlling system
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2021-Present Téo Dos Santos, Gabriel Taffin, Alexandre Truong and Renaud Visioli.
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <stdbool.h>
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library
#include "twi.h"            // TWI library for AVR-GCC
#include "gpio.h"  
#include <util/delay.h> 

// servo motor part
#define SERVO_M PC1 //AVR pin where servo motor ( window ) is connected
//RELAYS part
#define RELAY1   PB5  // AVR pin where module relay 1 ( irrigation ) is connected
#define RELAY2   PB4  // AVR pin where module relay 2 ( ventilation ) is connected 

//LED
#define LED PB3 // AVR pin where LED is connected 

/* Variables ---------------------------------------------------------*/
typedef enum {              // FSM declaration
    STATE_IDLE = 1,
    STATE_SEND,
    STATE_ACK
} state_t;
 
 //processing variables
 
 int switchMeasure = 0; // variable used to switch between temperature, humidity and light measure
 /* 0 corresponds to temperature reading
	1 corresponds to humidity reading 
	2 corresponds to light reading 
	3 corresponds to the necessity of a carrier return in the uart 
	The values are cycled. */


_Bool change = false; // variable used for display management in uart

//output managing variable

_Bool windowState = false; // variable used to manage the window opening
/* if false, the window is closed
   if true, the window is opened */


//input variables
volatile uint16_t humidity = 0;
volatile uint16_t temperature = 0;
volatile uint16_t light = 0;


/*functions used in the output manager interrupt */

//function to close the window
void closeWindow (){
	for ( int i = 0; i < 50000; i++){
		_delay_ms(20);
		GPIO_toggle(&PORTC, SERVO_M);
		_delay_ms(2);
		GPIO_toggle(&PORTC, SERVO_M);
	}
}

//function to open the window
void openWindow(){
	for ( int i = 0; i < 50000; i++){
		_delay_ms(20);
		GPIO_toggle(&PORTC, SERVO_M);
		_delay_ms(1);
		GPIO_toggle(&PORTC, SERVO_M);
	}
}


int main(void)
{
	
	GPIO_config_output(&DDRC, SERVO_M); // SERVO_M at PORTC configuration
	GPIO_config_output(&DDRB, LED); // LED at PORTB configuration
	GPIO_config_output(&DDRB, RELAY1); // RELAY1 at PORTB configuration
	GPIO_config_output(&DDRB, RELAY2); // RELAY2 at PORTB configuration
	
	GPIO_write_low(&PORTC, SERVO_M);
	GPIO_write_low(&PORTB, LED);
	GPIO_write_low(&PORTB, RELAY1); 
	GPIO_write_low(&PORTB, RELAY2);
	
	// the window is closed at the beginning of the program
	closeWindow();
	
	/* setup for light sensor */
    
	ADMUX |= (1<<REFS0);   // =1
	ADMUX &= ~(1<<REFS1);  // =0
	// Set input channel to ADC0
	ADMUX &=~ (1<<MUX3 | 1<< MUX2 | 1<< MUX1 | 1<<MUX0);
	// Enable ADC module
	ADCSRA |= (1<<ADEN);

	// Enable conversion complete interrupt
	ADCSRA |= (1<<ADIE);
	// Set clock prescaler to 128
	ADCSRA |= (1<< ADPS2 | 1<<ADPS1 | 1<<ADPS0);
	
	/* end of the light sensor section */
	
	// Initialize I2C (TWI)
    twi_init();

    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    // Configure 16-bit Timer/Counter1 to update FSM
    // Set prescaler to 33 ms and enable interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();
	
	// Configure 8-bit Timer/Counter2 
	// Set prescaler to 16 ms and enable interrupt
	TIM2_overflow_16ms();
	TIM2_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ring buffer for transmitting via UART
    uart_puts("\r\nScan I2C-bus for devices:\r\n");

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */

    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose : FSM in order to update the input variables
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
	//Start ADC conversion ( for the light sensor )
	ADCSRA |= (1<<ADSC);
	
	
    static state_t state = STATE_IDLE;  // Current state of the FSM
    static uint8_t addr = 0x5c;            // I2C slave address
    uint8_t result = 1;                 // ACK result from the bus 
    char uart_string[2] = "00"; // String for converting numbers by itoa()
    // FSM
    switch (state)
    {
		
    case STATE_IDLE:
		state = STATE_SEND;
        break;
    
    // Transmit I2C slave address and get result
    case STATE_SEND:

        result = twi_start((addr<<1) + TWI_WRITE); 
		
	
		/* temperature and humidity data retrieve */
		
		//determining which parameter should be measured
		if (switchMeasure == 0)
			twi_write(0x02); //0x02 for temperature
		else if(switchMeasure == 1)
			twi_write(0x00); //0x00 for humidity
		
		if ( switchMeasure  <= 1 ){
			twi_stop();
			result = twi_start((addr<<1) + TWI_READ);
			
			//getting the integer part of the result
			result = twi_read_ack();
			
			//writing the integer part into the input variables 
			if (switchMeasure == 0){
				temperature = result;
			}
			if (switchMeasure == 1){
				humidity = result;
			}
			
			//printing on the uart the result
			itoa(result, uart_string, 10);
			uart_puts(uart_string);
			
			//getting the fractional part of the result
			result = twi_read_nack();
			twi_stop();
			
			//printing the fractional part on the uart
			itoa(result, uart_string, 10);
			uart_puts(".");
			uart_puts(uart_string);
			uart_puts(" ");
			
			//printing the corresponding unit
			if (switchMeasure == 0)
				uart_puts("Celsius degrees");
				
			if (switchMeasure == 1)
				uart_puts("% of humidity");
				
			uart_puts(" ");
		}
		
		switchMeasure ++; // adding 1 in order to switch to the next parameter to be measured ( cycling order )
		
		// if the cycle has ended, the counter is set to 0 and the process is repeated
		if ( switchMeasure == 3 ){
			switchMeasure = 0; 
			uart_puts("\r\n"); // carrier return in the uart
		}
		
		state = STATE_IDLE;
        break;

    // A module connected to the bus was found
    case STATE_ACK:
        // Send info about active I2C slave to UART and move to IDLE
		
		itoa(addr, uart_string, 16);
		uart_puts(uart_string);
		uart_puts(" ");
		state = STATE_IDLE;
        break;

    // If something unexpected happens then move to IDLE
    default:
        state = STATE_IDLE;
        break;
    }
}
	
ISR(ADC_vect)
{
	if ( switchMeasure == 2){
		
		
		uint16_t value = 0;
		char lcd_string[4] = "";
		
		value = ADC;                  // Copy ADC result to 16-bit variable
		light = ADC;
		itoa(value, lcd_string, 10);  // Convert decimal value to string

		//Send ADC value to UART Tx
		if ( change == false ){
			uart_puts(lcd_string);
			uart_puts(" light");
		}
		change = !change;
	}
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose : output manager 
 **********************************************************************/
ISR(TIMER2_OVF_vect){
	
	// turn on the LED if the light is low 
	if ( light < 320 ){
		GPIO_write_high(&PORTB, LED);
	}
	else{
		GPIO_write_low(&PORTB, LED); 
	}
	
	/* Determining the actions of the outputs in function 
	of the parameters humidity and temperature */
	
	//high temperature and high humidity
	if (temperature > 27 && humidity > 30){
		
		// stop irrigation and start ventilation
		GPIO_write_low(&PORTB, RELAY1);
		GPIO_write_high(&PORTB, RELAY2);
		
		// open the window
		if ( windowState == false){
				openWindow();
				windowState = true;
		}
	}
	
	
	//high temperature and low humidity
	else if ( temperature > 27 && humidity < 5){
		
		// start irrigation and start ventilation
		GPIO_write_high(&PORTB, RELAY1);
		GPIO_write_high(&PORTB, RELAY2);
		
		// open the window
		if ( windowState == false){
			openWindow();
			windowState = true;
		}
	}
	
	//low temperature and high humidity
	else if ( temperature < 10 && humidity > 30){
		// stop irrigation and start ventilation
		GPIO_write_low(&PORTB, RELAY1);
		GPIO_write_high(&PORTB, RELAY2);
		
		// close the window
		if ( windowState == true){
			closeWindow();
			windowState = false;
		}
	}
	
	//low temperature and low humidity
	else if ( temperature < 10 && humidity < 5){
		// start irrigation and stop ventilation
		GPIO_write_high(&PORTB, RELAY1);
		GPIO_write_low(&PORTB, RELAY2);
		
		// close the window
		if ( windowState == true){
			closeWindow();
			windowState = false;
		}
	}
	
	//high temperature 
	else if ( temperature > 27 ){
		// start irrigation and start ventilation
		GPIO_write_high(&PORTB, RELAY1); 
		GPIO_write_high(&PORTB, RELAY2);
		
		// open the window
		if ( windowState == false){
			openWindow();
			windowState = true;
		}
	}
	
	//high humidity
	else if ( humidity > 30){
		
		// start ventilation
		GPIO_write_low(&PORTB, RELAY1);
		GPIO_write_high(&PORTB, RELAY2);
		
		// open the window
		if ( windowState == false){
			openWindow();
			windowState = true;
		}
	}
	
	//low temperature 
	else if ( temperature < 10){
		
		// stop irrigation and stop ventilation
		GPIO_write_low(&PORTB, RELAY1);
		GPIO_write_low(&PORTB, RELAY2);
		
		// close the window
		for ( int i = 0; i < 50000; i++){
			_delay_ms(20);
			GPIO_toggle(&PORTC, SERVO_M);
			_delay_ms(2);
			GPIO_toggle(&PORTC, SERVO_M);
		}
	}
	
	//low humidity
	else if ( humidity < 5){
		
		// start irrigation and stop ventilation
		GPIO_write_high(&PORTB, RELAY1);
		GPIO_write_low(&PORTB, RELAY2);
		
		// close the window
		if ( windowState == true){
			closeWindow();
			windowState = false;
		}
	}
	
	//if the parameters are at the optimal level
	else{
		// stop irrigation and stop ventilation
		GPIO_write_low(&PORTB, RELAY1);
		GPIO_write_low(&PORTB, RELAY2);
		
		// close the window
		if ( windowState == true){
			closeWindow();
			windowState = false;
		}
	}
}
