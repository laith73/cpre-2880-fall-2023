
#include <math.h>
#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"

/**
 * main.c
 */
int main(void)
{

    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
    cyBot_uart_init();

    //MAKE SURE TO ASSIGN THESE VALUES AFTER CALL TO cyBOT_init_Scan()
    //int right_calibration_value;
    //int left_calibration_value;

    // cyBOT_SERVO_cal();

    //Values for bot #15
    right_calibration_value = 253750;
    left_calibration_value = 1261750;

    //Values for Bot #8
    right_calibration_value = 243250;
    left_calibration_value = 1240750;

    int i;
    cyBOT_Scan_t scan;
    char degrees[91];
    char distance[91];

    for (i = 0; i <= 180; i += 2){
//        cyBOT_Scan(i, &scan);
        degrees[(i / 2)] = i;
        distance[(i / 2)] = (int) scan.sound_dist;
        lcd_printf("%d, %d", degrees[(i / 2)], distance[(i / 2)]);
//        timer_waitMillis(1000);
    }

    char temp;

    while (true){
        temp = cyBot_getByte();

        if (temp == 'm'){

            char arr[] = "Degrees        Distance(cm)\n\r";
            int i = 0;
            while (arr[i] != 0){
                cyBot_sendByte(arr[i]);
                i++;
            }

            short j;
            char str[4];
            short strindex = 0;

            for (j = 0; j < 91; j++){
                sprintf(str,"%d", degrees[j]);

                while (str[strindex] != 0){
                    cyBot_sendByte(str[strindex]);
                    strindex++;
                }
                strindex = 0;

                sprintf(str,"\t\t%d", distance[j]);

                while (str[strindex] != 0){
                    cyBot_sendByte(str[strindex]);
                    strindex++;
                }
                strindex = 0;
                cyBot_sendByte('\n');
                cyBot_sendByte('\r');
            }

            int x;
            int count = 0;
            int value = distance[0];
            char objAngle[10];
            char objDistance[10];
            char objWidth[10];
            char objLinearWidth[10];
            char objCount = 0;

            for (x = 1; x < 91; x++) {

                if (abs(distance[x] - value) <= 5){
                    count++;
                } else if (count >= 3 && distance[x - count - 1] <= 100){

                    objWidth[objCount] = count * 2;
                    objDistance[objCount] = distance[(x - count - 1)];
                    objAngle[objCount] = degrees[(x - count - 1)];
                    objCount++;
                    value = distance[x];
                    count = 0;

                } else {
                    value = distance[x];
                    count = 0;
                }

            }

            double radians;
            for (j = 0; j < objCount; j++){
                radians = ((int)objWidth[j] / 2.0) * (3.14159265 / 180.0);
                objLinearWidth[j] = 2 * (int)objDistance[j] * tan(radians);
            }


            char arr2[] = "Object#  Angle    Distance  Width (Linear)width\n\r";
            i = 0;
            while (arr2[i] != 0){
                cyBot_sendByte(arr2[i]);
                i++;
            }


            for (j = 0; j < objCount; j++){

                strindex = 0;

                sprintf(str,"%d", (j + 1));

                while (str[strindex] != 0){
                    cyBot_sendByte(str[strindex]);
                    strindex++;
                }

                strindex = 0;

                sprintf(str,"\t%d", objAngle[j]);

                while (str[strindex] != 0){
                    cyBot_sendByte(str[strindex]);
                    strindex++;
                }

                strindex = 0;

                sprintf(str,"\t%d", objDistance[j]);

                while (str[strindex] != 0){
                    cyBot_sendByte(str[strindex]);
                    strindex++;
                }

                strindex = 0;

                sprintf(str,"\t%d", objWidth[j]);

                while (str[strindex] != 0){
                    cyBot_sendByte(str[strindex]);
                    strindex++;
                }


                strindex = 0;

                sprintf(str,"\t%d", objLinearWidth[j]);

                while (str[strindex] != 0){
                    cyBot_sendByte(str[strindex]);
                    strindex++;
                }

                strindex = 0;
                cyBot_sendByte('\n');
                cyBot_sendByte('\r');
            }

            if (temp == '0'){
                break;

            }
        }
    }



	return 0;
}
