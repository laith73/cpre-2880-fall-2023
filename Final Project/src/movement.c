/*
 * movement.c
 *
 *  Created on: Sep 7, 2023
 *      Author: Laith Al Sairafi & Daniel
 */

#include <stdio.h>
#include "open_interface.h"
#include "movement.h"

//  Move the bot x centimeters backward
void move_backward(oi_t *sensor, int centimeters, int speed, int cliff){


    double sum = 0;
    int distance = centimeters * 10;
    oi_setWheels(speed * -1, speed * -1); // move forward; full speed
    oi_update(sensor);

    while (sum < distance) {
        oi_update(sensor);
        sum += sensor->distance * -1;

        if ((sensor->cliffFrontLeftSignal > 2500 || sensor->cliffFrontRightSignal > 2500) && cliff){
            move_forward(sensor, 10, 100, 0);
            break;
        }

        if ((sensor->cliffLeftSignal > 2700 || sensor->cliffRightSignal > 2700) && cliff){
            move_forward(sensor, 15, 100, 0);
            break;
        }


        if ((sensor->cliffFrontLeftSignal < 1000 || sensor->cliffFrontRightSignal < 1000) && cliff){
            move_forward(sensor, 10, 100, 0);
            break;
        }

        if ((sensor->cliffLeftSignal < 1000 || sensor->cliffRightSignal < 1000) && cliff){
            move_forward(sensor, 15, 100, 0);
            break;
        }
    }



    oi_setWheels(0, 0); // stop
}

//  Move the bot x centimeters forward
void move_forward(oi_t *sensor, int centimeters, int speed, int cliff){

    double sum = 0;
    int distance = centimeters * 10;
    oi_setWheels(speed, speed); // move forward; full speed
    oi_update(sensor);

    while (sum < distance) {
        oi_update(sensor);
        sum += sensor->distance;

        if (sensor->bumpLeft == true || sensor->bumpRight == true){
            bump(sensor);
            break;
        }

        if ((sensor->cliffFrontLeftSignal > 2500 || sensor->cliffFrontRightSignal > 2500) && cliff){
            move_backward(sensor, 10, 100, 0);
            break;
        }

        if ((sensor->cliffLeftSignal > 2700 || sensor->cliffRightSignal > 2700) && cliff){
            move_backward(sensor, 15, 100, 0);
            break;
        }

        if ((sensor->cliffFrontLeftSignal < 1000 || sensor->cliffFrontRightSignal < 1000) && cliff){
            move_backward(sensor, 10, 100, 0);
            break;
        }

        if ((sensor->cliffLeftSignal < 1000 || sensor->cliffRightSignal < 1000) && cliff){
            move_backward(sensor, 15, 100, 0);
            break;
        }
    }

    oi_setWheels(0, 0); // stop
}


void bump(oi_t *sensor_data){


    if (sensor_data->bumpLeft == true){

        oi_setWheels(-100, -100);

        double back_up = 0;

        while (back_up < 150){
            oi_update(sensor_data);
            back_up += sensor_data->distance * -1;
        }

        turn_clockwise(sensor_data, 90);
        move_forward(sensor_data, 15, 150, 1);
        turn_counterclockwise(sensor_data, 90);
    }

    else if (sensor_data->bumpRight == true){

        oi_setWheels(-150, -150);

        double back_up = 0;

        while (back_up < 150){
            oi_update(sensor_data);
            back_up += sensor_data->distance * -1;
        }

        turn_counterclockwise(sensor_data, 90);
        move_forward(sensor_data, 15, 150, 1);
        turn_clockwise(sensor_data, 90);
    }

    oi_setWheels(0, 0); // stop
}

// Turn the bot x degrees clockwise
void turn_clockwise(oi_t *sensor_data, int degrees){

    double sum = 0;
    degrees = degrees - (degrees * 0.10);

    oi_setWheels(-100, 100); // trun the bot clockwise
    oi_update(sensor_data);

    while (sum < (degrees)){
        oi_update(sensor_data);
        sum += sensor_data->angle * -1;
    }

    oi_setWheels(0, 0); // stop
}

// Turn the bot x degrees counterclockwise
void turn_counterclockwise(oi_t *sensor_data, int degrees){

    double sum = 0;
    degrees = degrees - (degrees * 0.10);

    oi_setWheels(100, -100); // trun the bot counterclockwise
    oi_update(sensor_data);

    while (sum < (degrees)){
        oi_update(sensor_data);
        sum += sensor_data->angle;
    }

    oi_setWheels(0, 0); // stop
}
