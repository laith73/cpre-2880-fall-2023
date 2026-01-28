/*
 * movement.h
 *
 *  Created on: Sep 7, 2023
 *      Author: laith73
 */


#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#endif /* MOVEMENT_H_ */


void move_backward(oi_t *sensor_date, int centimeters, int speed);

//  Move the bot x centimeters forward
void move_forward(oi_t *sensor, int centimeters, int speed);

//  Move the bot x centimeters forward with bump sensors on
void bump(oi_t *sensor_data);

// Turn the bot x degrees clockwise
void turn_clockwise(oi_t *sensor_data, int degrees);

// Turn the bot x degrees counterclockwise
void turn_counterclockwise(oi_t *sensor_data, int degrees);
