/***************************************************************************//**
  @file     uart.h
  @brief    ADC driver for MSP430G2. Blocking implementation.
  @author   Nicol�s Magliola
 ******************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ADC_CANT_BITS   10
#define ADC_VAL_MIN     0
#define ADC_VAL_MAX     (ADC_VAL_MIN+(1<<ADC_CANT_BITS)-1)

#define ADC_CH_CANT     8
#define ADC_CH_MIN      0
#define ADC_CH_MAX      (ADC_CH_MIN + ADC_CH_CANT-1)


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize ADC driver
 * @param ch_msk Channels to configure as ADC (bitmask)
 */
void adcInit(unsigned char ch_msk);


/**
 * @brief Read a channel from ADC. Blocking.
 * @param ch channel to read
 * @return ADC value
 */
unsigned int adcRead(unsigned char ch);



/*******************************************************************************
 ******************************************************************************/

#endif // _ADC_H_
