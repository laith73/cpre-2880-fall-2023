
#include <inc/tm4c123gh6pm.h>
#include <Math.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"

#include "Timer.h"
#include "ping.h"
#include "lcd.h"

void main(void)
{
    timer_init();
    ping_init();
    lcd_init();
    char result[10] = "";

    while (1){
        sprintf(result, "%d CM", ping_read());
        lcd_puts(result);
        timer_waitMillis(500);
        lcd_clear();
    }
}
