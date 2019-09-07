// Standard library:
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>

// Own library
#include "uart/uart.h"

// define
#define UART_BAUD_RATE	9600

// Variables
unsigned int get_data;
char buffor[26];
uint8_t counter = 0;

void get_uart_message();
void parse_data(char *data);
void operation(uint16_t a , uint16_t b, char znak);

//***************************** Main function: *******************************/
int main(void)
{
	// Initialize functions
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	_delay_ms(3000);
	// enable sei
	sei();
	// main loop
	while(1)
	{
		get_uart_message();
	}
}

void parse_data(char data[]){
	// parsowanie danych
	char znak;
	uint16_t a=0;
	uint16_t b=0;
	char *wsk = strtok(data," ");
	a = atoi(wsk);
	wsk = strtok(NULL," ");
	znak = wsk[0];
	wsk = strtok(NULL," ");
	b = atoi(wsk);
	operation(a,b,znak);
}

void operation(uint16_t a , uint16_t b, char znak){
	uint16_t wynik = 0;
	char asciiWynik[10];
	if( znak == '+'){
		wynik = a + b;
	}
	else if ( znak == '-'){
		wynik = a - b;
	}
	else if (znak == '*'){
		wynik = a * b;
	}
	else if (znak == '/'){
		wynik = a / b;
	}
	else{
		wynik = 0;
	}
	itoa(wynik, asciiWynik, 10);
	uart_puts(asciiWynik);
	uart_puts("\r\n");
}

void get_uart_message(){
        do{
                get_data = uart_getc();
                if ( get_data & UART_NO_DATA )  return;
                else
                {
                        if ( get_data & UART_FRAME_ERROR )      return;
                        if ( get_data & UART_OVERRUN_ERROR )    return;
                        if ( get_data & UART_BUFFER_OVERFLOW )  return;
                }
                buffor[counter] = get_data;
                counter++;
        }while(get_data != '\n');
        // parsowanie danych
        parse_data(buffor);
        counter=0;
}
