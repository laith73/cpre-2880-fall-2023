/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Describtion: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

int main (void) {

    timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called
    // and enables time functions (e.g. timer_waitMillis)

    lcd_init();   // Initialize the the LCD screen.  This also clears the screen.

    // lcd_printf("Hello, world"); // Clear the LCD screen and print "Hello, world" on the LCD

    // lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
    // step through in debug mode
    // and explain to TA how it
    // works


    // NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
    // NOTE: For time fuctions see Timer.h
    int index;
    int copy_index = 0;
    int display_len = 20;

    char sentence[54] = "                    Microcontrollers are lots of fun!";
    char print_arr[21];


    // Fill the print_arr with spaces
    for (index = 0; index < display_len; index++) {
        print_arr[index] = ' ';
    }

//      Add a Null character at the end of the array
    print_arr[20] = '\0';

    // Print the rotating banner
    while (1) {

        // Move all array's elements index back
        int i;
        for (i = 0; i < display_len - 1; i++) {
            print_arr[i] = print_arr[i + 1];
        }

        // Add a new letter
        print_arr[19] = sentence[copy_index];

        // Delay and LcD display print functions

        lcd_init();
        lcd_puts(print_arr);
        timer_waitMillis(300);
        // Increments the copy_index
        copy_index++;

        // Set copy_index to 0 if it equals to sentence length
        if (copy_index == strlen(sentence)) {
            copy_index = 0;
        }

        if (copy_index > strlen(sentence)){
            break;
        }

    }

    return 0;
}
