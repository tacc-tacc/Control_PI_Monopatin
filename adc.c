/***************************************************************************//**
  @file     adc.c
  @brief    ADC driver for MSP430G2. Blocking implementation.
  @author   Nicol�s Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "adc.h"
#include "hardware.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ADC10CTL0_CFG   (SREF_0 | ADC10SHT_1 | ADC10ON) // VR+ = VCC ; S&H = 8 � ADC10CLKs ; ADC10 = ON
#define ADC10CTL1_CFG   (ADC10SSEL_3 | ADC10DIV_3 | CONSEQ_0 | SHS_0) // ADCCLK=SMCLK/4 ; Single-channel single-conversion ; SW trigger S&H
#define ADC_CH2INCH(ch) ((ch)<<12)

#define ADC_DEVELOPMENT_MODE 1
#if ADC_DEVELOPMENT_MODE
/* UART debug (time measure) */
// TestPoints: P1.4 and P1.5
#define TP_INIT()   do {P1OUT &= ~(BIT4|BIT5); P1DIR |= (BIT4|BIT5); } while(0)
#define TP1_SET()    (P1OUT |= BIT4)
#define TP1_CLEAR()  (P1OUT &= ~BIT4)
#define TP1_TOGGLE() (P1OUT ^= BIT4)
#define TP2_SET()    (P1OUT |= BIT5)
#define TP2_CLEAR()  (P1OUT &= ~BIT5)
#define TP2_TOGGLE() (P1OUT ^= BIT5)
#else
#define TP_INIT()
#define TP1_SET()
#define TP1_CLEAR()
#define TP1_TOGGLE()
#define TP2_SET()
#define TP2_CLEAR()
#define TP2_TOGGLE()
#endif // UART_DEVELOPMENT_MODE


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static bool yainit = false;

static unsigned char adc_ch_msk;
static unsigned char adc_ch_now = 0;


unsigned int adc_dest[4];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void adcInit(unsigned char ch_msk)
{
    if (yainit || !ch_msk)
        return;

    ADC10CTL1 = CONSEQ_1 + INCH_3;
	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
	ADC10DTC1 = 4;
    ADC10AE0 = ch_msk; // Configure each pin as analog pin
    adc_ch_msk = ch_msk;

	yainit = true;
    TP_INIT();
}


unsigned int adcRead(unsigned char ch)
{
    return adc_dest[ch];
}


// ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
	ADC10SA = (unsigned int)adc_dest; // este paso es necesario, lamentablemente no guarda la dirección inicial
	ADC10CTL0 |= ENC + ADC10SC;
  	//__bic_SR_register_on_exit(CPUOFF);
}