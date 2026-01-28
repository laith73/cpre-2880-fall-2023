/*
 * movement.c
 *
 *  Created on: Sep 7, 2023
 *      Author: Laith Al Sairafi & Daniel
 */

#include <stdio.h>
#include "open_interface.h"
#include "movement.h"

void turn_clockwise(oi_t *sensor_data, int degrees);
void turn_counterclockwise(oi_t *sensor_data, int degrees);

//  Move the bot x centimeters backward
void move_backward(oi_t *sensor, int centimeters, int speed){

  //  oi_init(sensor);

    double sum = 0;
    int distance = centimeters * 10;
    oi_setWheels(speed * -1, speed * -1); // move forward; full speed

    while (sum < distance) {

        oi_update(sensor);
        sum += sensor->distance * -1;
    }

    oi_setWheels(0, 0); // stop
    oi_free(sensor);
}

//  Move the bot x centimeters forward
int move_forward(oi_t *sensor, int centimeters, int speed){

//    oi_init(sensor);

    double sum = 0;
    int distance = centimeters * 10;
    oi_setWheels(speed, speed); // move forward; full speed

    while (sum < distance) {

        oi_update(sensor);
        sum += sensor->distance;

        if (sensor->bumpLeft == true || sensor->bumpRight == true){
            break;
        }

    }

    oi_setWheels(0, 0); // stop
    oi_free(sensor);

    return  (distance - sum) / 10;
}


void bump(oi_t *sensor_data){

 //   oi_init(sensor_data);

    if (sensor_data->bumpLeft == true){

        oi_setWheels(-250, -250);

        double back_up = 0;

        while (back_up < 150){
            oi_update(sensor_data);
            back_up += sensor_data->distance * -1;
        }

        turn_clockwise(sensor_data, 90);
        move_forward(sensor_data, 25, 100);
        turn_counterclockwise(sensor_data, 90);
    }

    else if (sensor_data->bumpRight == true){

        oi_setWheels(-250, -250);

        double back_up = 0;

        while (back_up < 150){
            oi_update(sensor_data);
            back_up += sensor_data->distance * -1;
        }

        turn_counterclockwise(sensor_data, 90);
        move_forward(sensor_data, 25, 100);
        turn_clockwise(sensor_data, 90);
    }

    oi_setWheels(0, 0); // stop
    oi_free(sensor_data);
}

// Turn the bot x degrees clockwise
void turn_clockwise(oi_t *sensor_data, int degrees){
    
//    oi_init(sensor_data);

    double sum = 0;
    degrees = degrees - (degrees * 0.10);

    oi_setWheels(-100, 100); // trun the bot clockwise

    while (sum < (degrees)){
        
        oi_update(sensor_data);
        sum += sensor_data->angle * -1;
    }

    oi_setWheels(0, 0); // stop
    oi_free(sensor_data);
}

// Turn the bot x degrees counterclockwise
void turn_counterclockwise(oi_t *sensor_data, int degrees){

//    oi_init(sensor_data);

    double sum = 0;
    degrees = degrees - (degrees * 0.10);

    oi_setWheels(100, -100); // trun the bot counterclockwise

    while (sum < (degrees)){

        oi_update(sensor_data);
        sum += sensor_data->angle;
    }

    oi_setWheels(0, 0); // stop
    oi_free(sensor_data);
}
