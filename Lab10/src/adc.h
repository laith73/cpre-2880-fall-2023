/*
 *
 *  ADC header file
 *
 *  Created on: 10/19/23, CprE288 Lab-8
 *  @author Laith, Daniel
 */

#ifndef ADC_H_
#define ADC_H_

// initialize the port and the ADC module
void adc_init(void);

// returns the raw conversion value
int adc_read(void);

#endif
