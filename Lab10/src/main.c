//#include <inc/tm4c123gh6pm.h>
//#include <Math.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <stdint.h>
//#include "driverlib/interrupt.h"
//
//#include "Timer.h"
//#include "servo.h"
//#include "lcd.h"
//#include "button.h"
//
//void main(void)
//{
//    button_init();
//    init_button_interrupts();
//    timer_init();
//    lcd_init();
//    servo_init();
////    servo_calib();
//    set_calib_values(35900, 7500);
//    int degrees = 0;
//
//    while (1){
//        if(button_event)
//        {
//
//            if (button_num == 1)
//            {
//                if (degrees < 180){
//                    degrees++;
//                }
//            }
//            if (button_num == 2)
//            {
//                if (degrees <= 175){
//                    degrees += 5;
//                }
//            }
//
//            if (button_num == 3)
//            {
//                if (degrees > 0){
//                    degrees--;
//                }
//            }
//            if (button_num == 4)
//            {
//                if (degrees >= 5){
//                    degrees -= 5;
//                }
//            }
//
//            timer_waitMillis(100);
//            servo_move(degrees);
//            button_event = 0;
//            lcd_printf("%d", degrees);
//        }
//    }
//
//}


// Includes
#include <stdbool.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <Math.h>

#include "Timer.h"
#include "lcd.h"
#include "uart.h"
#include "driverlib/interrupt.h"
#include "open_interface.h"
#include "movement.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"

// Prototypes
void servo_scan_180();
void print_str_putty(char str[]);
void distance_calculation();
char get_average(char arr[], int start, int end);
void auto_drive(oi_t *sensor_data);

// ---------------------------

// Global Variables
volatile char uart_data;  // Your UART interrupt code can place read data here
volatile char flag;       // Your UART interrupt can update this flag

// Arrays
char distance_ping[91];
char distance_IR[91];
char degrees[91];
char objAngle[10];
char objDistance[10];
char objWidth[10];
char objLinearWidth[10];
char objCount = 0;

int main(void)
{
    // Initialization
    timer_init();
    lcd_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    uart_init();
    uart_interrupt_init();
    adc_init();
    ping_init();
    servo_init();
    set_calib_values(35900, 7500);


    while(1){
        //FLAG CONDITION CHECK
        if (flag)
        {
            flag = 0;

            if(uart_data == 'm')
            {
                servo_scan_180(); //SCAN

                // Print the Header
                char arr[] = "Degrees   Distance_Sound(cm)    Distence_Ping(cm)\n\r";
                print_str_putty(arr);

                // Print the distances values at each degree from the both sensors
                char str[4];
                int j;

                for (j = 0; j < 91; j++)
                {
                    // Degree
                    sprintf(str, "%d", j * 2);
                    print_str_putty(str);
                    // Sound
                    sprintf(str,"\t\t%d", distance_ping[j]);
                    print_str_putty(str);

                    // IR
                    sprintf(str,"\t\t%d", distance_IR[j]);
                    print_str_putty(str);

                    // Format
                    uart_sendChar('\n');
                    uart_sendChar('\r');
                }

                // Do the calculation (after servo_scan_180)
                distance_calculation();

                //  Objects stats printing
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

            if (uart_data == 'p'){
                servo_scan_180(); //SCAN

                // Print the Header
                char arr[] = "Degrees   Distance_Sound(cm)    Distence_Ping(cm)\n\r";
                print_str_putty(arr);

                // Print the distances values at each degree from the both sensors
                char str[4];
                int j;

                for (j = 0; j < 91; j++)
                {
                    // Degree
                    sprintf(str, "%d", j * 2);
                    print_str_putty(str);
                    // Sound
                    sprintf(str,"\t\t%d", distance_ping[j]);
                    print_str_putty(str);

                    // IR
                    sprintf(str,"\t\t%d", distance_IR[j]);
                    print_str_putty(str);

                    // Format
                    uart_sendChar('\n');
                    uart_sendChar('\r');
                }

                // Do the calculation (after servo_scan_180)
                distance_calculation();

                //  Objects stats printing
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

                auto_drive(sensor_data);
            }

            if (uart_data == 'g'){

                servo_scan_180(); //SCAN
                char str[4];
                int j;

                for (j = 0; j < 91; j++)
                {
                    if (j < 90){
                        sprintf(str,"%d,", distance_IR[j]);
                        print_str_putty(str);
                    } else {
                        sprintf(str,"%d", distance_IR[j]);
                        print_str_putty(str);
                    }

                }

                uart_sendChar('\n');
            }

            if (uart_data == 'w')
            {
               char str[] = "forward";
               print_str_putty(str);
               uart_sendChar('\n');
               move_forward(sensor_data, 5, 100);

            }
            else if (uart_data == 'd')
            {
               char str[] = "clockwise";
               print_str_putty(str);
               uart_sendChar('\n');
               turn_clockwise(sensor_data, 5);

            }
            else if (uart_data == 'a')
            {
                char str[] = "counterclockwise";
                print_str_putty(str);
                uart_sendChar('\n');
                turn_counterclockwise(sensor_data, 5);
            }
            else if (uart_data == 's')
            {
                char str[] = "backward";
                print_str_putty(str);
                uart_sendChar('\n');
                move_backward(sensor_data, 5, 100);
            }
            else if (uart_data == 'f')
            {
                oi_free(sensor_data);
                char str[] = "free";
                print_str_putty(str);
                uart_sendChar('\n');
            }
        }
    }
}


// Takes a char pointer and prints till null
void print_str_putty(char str[]){

    int strindex = 0;

    while (str[strindex] != 0)
    {
        uart_sendChar(str[strindex]);
        strindex++;
    }

}

// Servo Scan function
void servo_scan_180(){

    int i;
    int j;
    int sound_distance_sum = 0;
    int IR_distance_sum = 0;
    servo_move(0);
    timer_waitMillis(500);
    for (i = 0; i < 181; i+=2)
    {
        // take tree different samples at the same angle
        for (j = 0; j < 3; j++){
            servo_move(i);
            timer_waitMillis(100);
            sound_distance_sum += (int) ping_read();
            IR_distance_sum += (int) 6000000 * pow(adc_read() , -1.681); // Bot #8
        }

        sound_distance_sum = sound_distance_sum / 3;
        IR_distance_sum = IR_distance_sum / 3;

        degrees[(i / 2)] = i;
        distance_ping[(i / 2)] = sound_distance_sum;
        distance_IR[(i / 2)] = IR_distance_sum; // cyBot 8
        lcd_printf("%d, %d, %d", i, distance_ping[(i / 2)], distance_IR[(i / 2)]);

        sound_distance_sum = 0;
        IR_distance_sum = 0;
    }
}

void distance_calculation(){

    int i;
    int start_angle = 0;
    int value = distance_IR[0];
    int count = 0;
    objCount = 0;

    for (i = 1; i < 91; i++) // compare distance_IR[i] to distance[i-1]
    {
        // compare to the first value
        if ((count <= 3) && abs(distance_IR[i] - value) <= 3)
        {
            count++;
        }
        else if ((count > 3 && count <= 5) && abs(distance_IR[i] - value) <= 5)
        {
            count++;
        }
        else if ((count > 5) && abs(distance_IR[i] - value) <= 7)
        {
            count++;
        }
        else if ((count >= 2) && (distance_IR[i - count - 1] <= 50) && (distance_IR[i - count - 1] >= 10))
        {
            int mid_i = i - ((i - start_angle) / 2);
            // In degrees
            objWidth[objCount] = count * 2;
            // Distance using the ping (Sound Sensor)
            objDistance[objCount] = distance_ping[mid_i];
            // The object's angular location
            objAngle[objCount] = degrees[mid_i];
            //  for next comparison
            value = distance_IR[i];
            objCount++;
            count = 0;
        }
        else
        {
            value = distance_IR[i];
            start_angle = i;
            count = 0;
        }
    }

    // calculate the linear width
    int j;
    double radians;

    for (j = 0; j < objCount; j++)
    {
        radians = ((double) (objWidth[j])) * (3.14159265 / 180.0);
        objLinearWidth[j] = 2 * (int) objDistance[j] * tan(radians);
    }
}

void auto_drive(oi_t *sensor_data){

    int smallest_obj = 0;
    int turn_degrees;
    int i;

    for (i = 1; i < objCount; i++)
    {
        if (objLinearWidth[i] < objLinearWidth[smallest_obj]){
            smallest_obj = i;
        }
    }

    turn_degrees = (objAngle[smallest_obj] - 90);

    if (turn_degrees > 0){
        turn_counterclockwise(sensor_data, turn_degrees);
        move_forward(sensor_data, objDistance[smallest_obj] , 100);
    } else if (turn_degrees < 0){
        turn_clockwise(sensor_data, abs(turn_degrees));
        move_forward(sensor_data, (objDistance[smallest_obj] - 20) , 100);

    }

}

char get_average(char arr[], int start, int end){
    int result = 0;
    int i;

    for (i = start; i < end; i++){
        result+= arr[i];
    }

    result = result / (end - start);

    return (char)result;
}
