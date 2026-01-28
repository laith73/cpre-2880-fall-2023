/**
 * lab5_template.c
 * 
 * Template file for CprE 288 lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include <math.h>

#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#include "cyBot_Scan.h"  // For scan sensors 




// Defined in button.c : Used to communicate information between the
// the interupt handeler and main.
extern volatile int button_event;
extern volatile int button_num;


int main(void) {


    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
    cyBot_uart_init();
    button_init();
    init_button_interrupts();

//    cyBOT_SERVO_cal(); // for calibration

//    //Values for Bot #2
    right_calibration_value = 248500;
    left_calibration_value = 1256500;

	cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

	// Complete this code for configuring the  (GPIO) part of UART initialization

     SYSCTL_RCGCGPIO_R |= 0x02;
     timer_waitMillis(1);            // Small delay before accessing device after turning on clock
     GPIO_PORTB_AFSEL_R |= 0x03;
     GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the desired locations
     GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the desired locations
     GPIO_PORTB_DEN_R |= 0xFF;
     GPIO_PORTB_DIR_R &= 0b11111100;      // Force 0's in the desired locations
     GPIO_PORTB_DIR_R |= 0b00000010;      // Force 1's in the desired locations


    // Complete the UART device initialization part of configuration
     cyBot_uart_init_last_half();


// Part 2

//	while(1)
//	{
//
//	    temp = button_getButton();
//
//        if (final != temp){
//
//            if (temp == 0){
//                temp2 = '0';
//            } else if (temp == 1){
//                temp2 = '1';
//            } else if (temp == 2){
//                temp2 = '2';
//            } else if (temp == 3){
//                temp2 = '3';
//            } else if (temp == 4){
//                temp2 = '4';
//            }
//            cyBot_sendByte('\n');
//            cyBot_sendByte(temp2);
//        }
//
//
//	    lcd_printf("%d", temp);
//	    final = temp;
// }

//  Part 3

    cyBOT_Scan_t getScan; // Do not use a pointer, pass the the memory address
    char str[5];
    int i = 0;
    int distance_cm;

//      cyBot_15 | distance_cm = 50000000 * pow(getScan.IR_raw_val , -1.98);
//      cyBot_8 | distance_cm = 6000000 * pow(getScan.IR_raw_val , -1.681);

    while (1){

        if (button_event){

            button_event = 0;

            if (button_num == 1){
                cyBOT_Scan(90, &getScan);
            }

            sprintf(str,"%d", getScan.IR_raw_val);

            for (i = 0; i < strlen(str); i++){
                cyBot_sendByte(str[i]);
            }

            cyBot_sendByte('\n');
        }

        timer_waitMillis(500);

        distance_cm = 200000000 * pow(getScan.IR_raw_val , -2.266);
        lcd_printf("%d CM %d", distance_cm, getScan.IR_raw_val);
    }
}
