/**
 * lab4_template.c
 * 
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#define REPLACEME 0



int main(void) {

    button_init();
	lcd_init();
	// Don't forget to initialze the cyBot UART before trying to use it
	cyBot_uart_init();
	
	int temp = 0;
	char temp2 = 0;
	int final = 0;
	while(1)
	{

	    temp = button_getButton();

	    if (final != temp){

	        if (temp == 0){
	            temp2 = '0';
	        } else if (temp == 1){
	            temp2 = '1';
	        } else if (temp == 2){
	            temp2 = '2';
	        } else if (temp == 3){
	            temp2 = '3';
	        } else if (temp == 4){
	            temp2 = '4';
	        }

	        cyBot_sendByte(temp2);
	    }


	    // YOUR CODE HERE
	   lcd_printf("%d", temp);
	   final = temp;
	
	}
	
}
