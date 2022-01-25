/*
 * Lora_Scanner.h
 *
 *  Created on: 16 gen 2022
 *      Author: Luca
 */

#ifndef INC_LORA_SCANNER_H_
#define INC_LORA_SCANNER_H_

#include "radio.h"
#include "platform.h"
#include "subghz.h"
#include "radio_board_if.h"  /* low layer api (bsp) */
#include "utilities_def.h"

#define FREQ_START			432000000	//[Hz]
#define FREQ_FINISH			434000000	//[Hz]
#define FREQ_STEP			    10000	//[Hz]
#define NUMBER_OF_POINTS		  200	//[#]
#define LORA_SENSITIVITY		    0	//[dB]
#define PLOT_SIZE				  150	//[#]
#define THREASHOLD				- 105   //[dB]
#define PLOT

int16_t Spectrum[NUMBER_OF_POINTS+1];

void LoraAnalyzer(void);
void RssiPlot(void);
uint8_t RssiScan(void);
int16_t arrayMax(int16_t *arr, int n);
int16_t arrayMin(int16_t *arr, int n);
int16_t arrayMean(int16_t *arr, int n);
#endif /* INC_LORA_SCANNER_H_ */
