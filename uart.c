/***************************************************************************//**
  @file     uart.c
  @brief    UART driver for MSP430G2. Blocking implementation.
  @author   Nicol�s Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "uart.h"
#include "uart_cfg.h"
#include "hardware.h"

char txBuff[L] = "";
char *txp = txBuff;

char rxBuff[L] = "";
char *rxp = rxBuff;

void uartInit()
{
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = (unsigned char)(1250 & 0b11111111);                     
	UCA0BR1 = (unsigned char)(1250 >> 8);                    // escribo 1250 en el UCABR
	UCA0MCTL = UCBRF_0 | UCBRS_0 & ~UCOS16; // seteo los tres parámetros del UCA0MCTL en 0
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void uartSend()
{
    txp = txBuff;
	IE2 |= UCA0TXIE;
}

void uartRead()
{
    return rxBuff;
}

void uartFull()
{
    return rxp >= rxBuff + L;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
	if (txp >= txBuff + L || !*txp)
	{
		IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt
		return;
	}

  UCA0TXBUF = *(txp++);                 // TX next character
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
	if (uartFull())
		return;
	*(rxp++) = UCA0RXBUF;
	/*
	if(rxIdx >= 4)
	{
		memccpy(txBuff, rxBuff, 0, 16);
		txIdx = 0;
		rxIdx = 0;
		IE2 |= UCA0TXIE;
	}
	*/
}


