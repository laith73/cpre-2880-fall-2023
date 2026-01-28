/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

//#define _RESET 1
//#define _PART1 0
//#define _PART2 0
//#define _PART3 0
#include "Timer.h"
#include "lcd.h"
#include "uart.h"
#include "cyBot_Scan.h"
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>
#include <Math.h>
#include "open_interface.h"
#include "movement.h"

volatile char uart_data;  // Your UART interupt code can place read data here
volatile char flag;       // Your UART interupt can update this flag

// Adding global volatile varibles for communcating between
// your Interupt Service Routine, and your non-interupt code.

//prototypes

void print_str_putty(char str[]);

void main()
{

//   part 3
//    timer_init();
//    lcd_init();
//    uart_init();
//    uart_interrupt_init();
//    char data[21];
//    int i = 0;
//
//    while (true){
//
//       if (flag){
//           data[i] = uart_data;
//           i++;
//           lcd_printf("Count: %d", i);
//
//           if (uart_data == '\r' || i == 20){
//               data[i - 1] = '\0';
//               uart_sendStr(data);
//               i = 0;
//           }
//
//           flag = 0;
//       }
//    }

    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
    uart_init();
    uart_interrupt_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    //Values for bot #15
    right_calibration_value = 253750;
    left_calibration_value = 1261750;

    cyBOT_Scan_t scan;
    char degrees[91];
    char distance_sound[91];
    char distance_ping[91];
    int i;
    char ch;


    while (1)
    {

        // New char from art interrupt
        if (flag)
        {
            ch = uart_data;
            flag = 0;

            if (ch == 'm')
            {
                // Servo 180 degrees scan
                for (i = 0; i <= 180; i += 2)
                {
                    cyBOT_Scan(i, &scan);
                    degrees[(i / 2)] = i;
                    distance_sound[(i / 2)] = (int) scan.sound_dist;
                    distance_ping[(i / 2)] = (int) 50000000 * pow(scan.IR_raw_val, -1.98);
                    lcd_printf("%d, %d, %d", degrees[(i / 2)], distance_sound[(i / 2)], distance_ping[(i / 2)]);
                    timer_waitMillis(100);
                }

                /////////////////////////////////////
                //////////////////////////////////////
                //////////////////////////////////////

                // Print the Header
                char arr[] = "Degrees   Distance_Sound(cm)    Distence_Ping(cm)\n\r";

                print_str_putty(arr);

                // ----------------------------------------

                // Print the distances values at each degree from the both sensors
                int j;
                char str[4];

                for (j = 0; j < 91; j++)
                {
                    // Degree
                    sprintf(str, "%d", degrees[j]);
                    print_str_putty(str);
                    // Sound
                    sprintf(str, "\t\t%d", distance_sound[j]);
                    print_str_putty(str);

                    // Ping
                    sprintf(str, "\t\t%d", distance_ping[j]);
                    print_str_putty(str);

                    // Format
                    uart_sendChar('\n');
                    uart_sendChar('\r');
                }
                // -----------------------------------------------

                int x;
                int count = 0;
                int value = distance_ping[0];
                char objAngle[10];
                char objDistance[10];
                char objWidth[10];
                char objLinearWidth[10];
                char objCount = 0;

                for (x = 1; x < 91; x++)
                {

                    if (abs(distance_ping[x] - value) <= 3)
                    {
                        count++;
                    }
                    else if (count >= 4 && distance_ping[x - count - 1] <= 70 && distance_ping[x - count - 1] >= 10)
                    {

                        objWidth[objCount] = count * 2;
                        objDistance[objCount] = distance_sound[(x - count - 1)];
                        objAngle[objCount] = degrees[(x - count - 1)];
                        objCount++;
                        value = distance_ping[x];
                        count = 0;

                    }
                    else
                    {
                        value = distance_ping[x];
                        count = 0;
                    }

                }

                double radians;

                for (j = 0; j < objCount; j++)
                {
                    radians = ((int) objWidth[j] / 2.0) * (3.14159265 / 180.0);
                    objLinearWidth[j] = 2 * (int) objDistance[j] * tan(radians);
                }

                char arr2[] = "Object#  Angle   Distance  Width (Linear)width\n\r";
                print_str_putty(arr2);

                for (j = 0; j < objCount; j++)
                {

                    sprintf(str, "%d", (j + 1));
                    print_str_putty(str);

                    sprintf(str, "\t%d", objAngle[j]);
                    print_str_putty(str);

                    sprintf(str, "\t%d", objDistance[j]);
                    print_str_putty(str);

                    sprintf(str, "\t%d", objWidth[j]);
                    print_str_putty(str);

                    sprintf(str, "\t%d", objLinearWidth[j]);
                    print_str_putty(str);

                    uart_sendChar('\n');
                    uart_sendChar('\r');
                }
            }
           // ------------------------------------------


            if (ch == 'w')
            {
               move_forward(sensor_data, 5, 100);
            }
            else if (ch == 'd')
            {
               turn_clockwise(sensor_data, 5);
            }
            else if (ch == 'a')
            {
                turn_counterclockwise(sensor_data, 5);
            }
            else if (ch == 's')
            {
                move_backward(sensor_data, 5, 100);
            }
        }
    }
}


// Takes a char pointer a prints till null
void print_str_putty(char str[]){

    int strindex = 0;

    while (str[strindex] != 0)
    {
        uart_sendChar(str[strindex]);
        strindex++;
    }
}
