#include <msp430.h>
#include "adc.h"
#include "uart.h"
#include "pisr.h"

void int_cb();


int main(void)
{
  	WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
  	if (CALBC1_12MHZ==0xFF)					// If calibration constant erased
    	while(1);                               // do not load, trap CPU!!	
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_12MHZ;                    // Set DCO
	DCOCTL = CALDCO_12MHZ;

	// Activo los bits de P1SEL y P1SEL2 para asignar a P1.1 y P1.2 el RXD y TXD respectivamente
	P1SEL = BIT1 | BIT2;
	P1SEL2 = BIT1 | BIT2;

	// Activo el bit en P2DIR y P2Sel para configurarlo como salida de T1.2
	P2DIR = BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
	P2SEL = BIT5;

	// ADC
	adcInit(BIT0 | BIT3);

	pisrRegister(int_cb, PISR_MS2TICKS(1000));


	P2OUT = 0;

	ADC10CTL0 &= ~ ENC;	
	while (ADC10CTL1 & ADC10BUSY);               // Wait if ADC10 core is active
	ADC10SA = (unsigned int)adc_dest;
	ADC10CTL0 |= ENC + ADC10SC;             // convierto la primera vez
	__bis_SR_register(CPUOFF + GIE);        // apago la CPU y dejo activadas las interrupciones
}

void int_cb()
{
	unsigned int x = adcRead(0);
	char *p = utoa(&x, txBuff[L], '\0');
	uartSend();
}
