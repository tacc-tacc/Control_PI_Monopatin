#pragma once

#define L 32

extern char txBuff[L];
extern char rxBuff[L];

void uartInit();
void uartSend();
void uartRead();
void uartFull();