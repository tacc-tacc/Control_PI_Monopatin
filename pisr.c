/***************************************************************************//**
  @file     pisr.c
  @brief    Periodic Interrupt (RISR) driver
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "pisr.h"
#include "hardware.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define PISR_MAX_CANT  8

 
// WDT peripheral configuration
#if SMCLK_FREQ_HZ == 1000000UL
#if PISR_TICKS_MS == 32
#define WDTISX              0
#elif PISR_TICKS_MS == 8
#define WDTISX              1
#else
#error Valor de PISR_TICKS_MS no valido!
#endif // RTI_TICKS_MS == xx
#elif SMCLK_FREQ_HZ == 8000000UL
#if PISR_TICKS_MS == 4
#define WDTISX              0
#elif PISR_TICKS_MS == 1
#define WDTISX              1
#else
#error Valor de PISR_TICKS_MS no valido!
#endif // RTI_TICKS_MS == xx
#elif SMCLK_FREQ_HZ == 16000000UL
#if PISR_TICKS_MS == 2
#define WDTISX              0
#else
#error Valor de PISR_TICKS_MS no valido!
#endif // RTI_TICKS_MS == xx
#else
#error Valor de PISR_TICKS_MS no valido!
#endif // SMCLK_FREQ_HZ == xx

#define WDTCTL_INIT         (WDTPW|WDTTMSEL|WDTCNTCL|WDTISX)


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {
    uint16_t            period;
    uint16_t            cnt;
    pisr_callback_t      callback;
} pisr_t;


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void pisrInit (void);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static pisr_t pisrData[PISR_MAX_CANT];
static char pisrCant = 0;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

char pisrRegister (pisr_callback_t fun, int per)
{
	char statusOK = 0;
    
	if ((pisrCant < PISR_MAX_CANT) && (per > 0) && (fun != NULL)) // validate
    {
        if (pisrCant == 0) // initialize peripheral (just first time)
            pisrInit();
    
        pisrData[pisrCant].period = per; // configure new rti data
        pisrData[pisrCant].cnt = per;
        pisrData[pisrCant].callback = fun;
        
        ++pisrCant; // update rti quantity
        statusOK = 1;
    }
    
    return statusOK;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void pisrInit (void)
{
    WDTCTL = WDTCTL_INIT; // Interval timer mode ; according interval selection
    IE1 |=  WDTIE; // Enable the WDTIE bit
}


#pragma vector = WDT_VECTOR             //Interval timer vector location
__interrupt void pisr_isr(void)
{
    char i;
    
    for (i=0 ; i<pisrCant ; ++i)
    { // decremento los timers
        if (--(pisrData[i].cnt) == 0)
        { // período cumplido!
            pisrData[i].callback(); // 1) execute action
            pisrData[i].cnt = pisrData[i].period; // 2) update counter
        }
    }
}


/******************************************************************************/

