/**************************************************************************************************
  Filename:       bsp_board.c
  Revised:        $Date: 2009-10-11 16:48:20 -0700 (Sun, 11 Oct 2009) $
  Revision:       $Revision: 20896 $

  Copyright 2007-2009 Texas Instruments Incorporated.  All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights granted under
  the terms of a software license agreement between the user who downloaded the software,
  his/her employer (which must be your employer) and Texas Instruments Incorporated (the
  "License"). You may not use this Software unless you agree to abide by the terms of the
  License. The License limits your use, and you acknowledge, that the Software may not be
  modified, copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio frequency
  transceiver, which is integrated into your product. Other than for the foregoing purpose,
  you may not use, reproduce, copy, prepare derivative works of, modify, distribute,
  perform, display or sell this Software and/or its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS”
  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
  IN NO EVENT SHALL TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
  INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST
  DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY
  THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *   BSP (Board Support Package)
 *   Target : Texas Instruments MSP-EXP430F5529
 *            "MSP430F5529 Experimenter Board"
 *   Top-level board code file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "bsp.h"
#include "bsp_config.h"
#include "PMM.c"

/* ------------------------------------------------------------------------------------------------
 *                                            Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                            Defines
 * ------------------------------------------------------------------------------------------------
 */
#define BSP_TIMER_CLK_MHZ   (BSP_CONFIG_CLOCK_MHZ_SELECT)

/* ------------------------------------------------------------------------------------------------
 *                                            Local Variables
 * ------------------------------------------------------------------------------------------------
 */
#if defined(SW_TIMER)
static uint8_t sIterationsPerUsec = 0;
#endif

/**************************************************************************************************
 * @fn          BSP_EARLY_INIT
 *
 * @brief       This function is called by start-up code before doing the normal initialization
 *              of data segments. If the return value is zero, initialization is not performed.
 *              The global macro label "BSP_EARLY_INIT" gets #defined in the bsp_msp430_defs.h
 *              file, according to the specific compiler environment (CCE or IAR). In the CCE
 *              environment this macro invokes "_system_pre_init()" and in the IAR environment
 *              this macro invokes "__low_level_init()".
 *
 * @param       None
 *
 * @return      0 - don't intialize data segments / 1 - do initialization
 **************************************************************************************************
*/
BSP_EARLY_INIT(void)
{
  /* Disable watchdog timer */
  WDTCTL = WDTPW | WDTHOLD;

  /* Return 1 - run seg_init */
  return (1);
}

/**************************************************************************************************
 * @fn          BSP_InitBoard
 *
 * @brief       Initialize the board.
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */

void BSP_InitBoard(void)
{
  /* Setup XT1 crystal */
  P5SEL |= BIT4|BIT5;   // Enable xtal pins
  while ( (SFRIFG1 & OFIFG) )
  {
    UCSCTL7 &= ~(XT1LFOFFG + DCOFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive
                                            // strength

  /* Set appropriate core voltage */
  if(BSP_CONFIG_CLOCK_MHZ_SELECT <= 8)       SetVCore(0);
  else if(BSP_CONFIG_CLOCK_MHZ_SELECT <= 12) SetVCore(1);
  else if(BSP_CONFIG_CLOCK_MHZ_SELECT <= 20) SetVCore(2);
  else                                       SetVCore(3);

  /* Initialize the UCS module*/
  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = BSP_CONFIG_MSP430_DCORSEL;       // Select suitable range
  UCSCTL2 = BSP_CONFIG_MSP430_FLLDx + BSP_CONFIG_MSP430_N; // Select divider, multiplier bits
  UCSCTL3 = 0;                              // FLL Reference Clock = XT1
  UCSCTL4 = SELS__DCOCLK  |  SELM__DCOCLK;  // MCLK & SMCLK = DCOCLK
  UCSCTL4 |= SELA__XT1CLK;                  // ACLK = XT1 (32 kHz REFO)

  UCSCTL6 &= ~XT1DRIVE_3;                   // Set DCO drive to consume least power
  UCSCTL6 |= XCAP_3 ;                       // Setup internal caps for crystal
  __bic_SR_register(SCG0);

  /* Give FLL enough time to settle */
  if(BSP_CONFIG_CLOCK_MHZ_SELECT <= 8)       __delay_cycles(250000);
  else if(BSP_CONFIG_CLOCK_MHZ_SELECT <= 12) __delay_cycles(375000);
  else if(BSP_CONFIG_CLOCK_MHZ_SELECT <= 20) __delay_cycles(625000);
  else                                       __delay_cycles(812500);

 // Configure button ports
    PADIR &= ~0x0480;               // Buttons on P1.7/P2.2 are inputs

    // Configure CapTouch ports
    P1OUT &= ~0x7E;
    P1DIR |= 0x7E;
    P6OUT = 0x00;
    P6DIR = 0x00;

    // Configure GDO ports
    P2DIR &= ~(BIT1 | BIT3);        //GDO pins

    // Configure Cma3000 ports
    P2DIR &= ~BIT5;                 // ACCEL_INT pin is input
    P2OUT &= ~BIT7;                 // SCK
    P2DIR |= BIT7;
    P3OUT &= ~(BIT3 + BIT5 + BIT6); // ACCEL_SIMO, ACCEL_CS, ACCEL_PWR to low
    P3DIR &= ~BIT4;                 // ACCEL_SOMI pin is input
    P3DIR |= BIT3 + BIT5 + BIT6;    // ACCEL_SIMO, ACCEL_CS, ACCEL_PWR as outp

    // Configure Dogs102x6 ports
    P5OUT &= ~(BIT6 + BIT7);        // LCD_C/D, LCD_RST
    P5DIR |= BIT6 + BIT7;
    P7OUT &= ~(BIT4 + BIT6);        // LCD_CS, LCD_BL_EN
    P7DIR |= BIT4 + BIT6;
    P4OUT &= ~(BIT1 + BIT3);        // SIMO, SCK
    P4DIR &= ~BIT2;                 // SOMI pin is input
    P4DIR |= BIT1 + BIT3;

    // Configure SDCard ports
    P3OUT |= BIT7;                  // SD_CS to high
    P3DIR |= BIT7;

    // Configure Wheel ports
    P6DIR &= ~BIT5;                 // A5 ADC input
    P8OUT &= ~BIT0;                 // POT_PWR
    P8DIR |= BIT0;

    // Configure unused ports for low power
    P2OUT &= ~(BIT0 + BIT4 + BIT6);
    P2DIR |= BIT0 + BIT4 + BIT6;

    P3OUT &= ~(BIT0 + BIT1 + BIT2);
    P3DIR |= BIT0 + BIT1 + BIT2;

    P4OUT &= ~(BIT0 + BIT6 + BIT7);
    P4DIR |= BIT0 + BIT6 + BIT7;

    P5OUT &= ~BIT1;
    P5DIR |= BIT1;

    P6OUT &= ~(BIT6 + BIT7);
    P6DIR |= BIT6 + BIT7;

    P7OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT5 + BIT7);
    P7DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT5 + BIT7;

  /* Configure TimerA for use by the delay function */

  /* Reset the timer */
  TA0CTL |= TACLR;                          // Reset the timer
  TA0CTL = 0x0;                             // Clear all settings
  TA0CTL |= TASSEL_2;                       // Select the clk source to be SMCLK

#if defined(SW_TIMER)
#define MHZ_CLOCKS_PER_USEC      BSP_CLOCK_MHZ
#define MHZ_CLOCKS_PER_ITERATION 10

  sIterationsPerUsec = (uint8_t)(((MHZ_CLOCKS_PER_USEC)/(MHZ_CLOCKS_PER_ITERATION))+.5);

  if (!sIterationsPerUsec)
  {
    sIterationsPerUsec = 1;
  }
#endif   /* SW_TIMER */
}

/**************************************************************************************************
 * @fn          BSP_Delay
 *
 * @brief       Sleep for the requested amount of time.
 *
 * @param       # of microseconds to sleep.
 *
 * @return      none
 **************************************************************************************************
 */
void BSP_Delay(uint16_t usec)
#if !defined(SW_TIMER)
{

  TA0R = 0; /* initial count */
  TA0CCR0 = BSP_TIMER_CLK_MHZ*usec; /* compare count. (delay in ticks) */

  /* Start the timer in UP mode */
  TA0CTL |= MC_1;

  /* Loop till compare interrupt flag is set */
  while(!(TA0CCTL0 & CCIFG));

  /* Stop the timer */
  TA0CTL &= ~(MC_1);

  /* Clear the interrupt flag */
   TA0CCTL0 &= ~CCIFG;
}


#else  /* !SW_TIMER */


{
  /* Declared 'volatile' in case User optimizes for speed. This will
   * prevent the optimizer from eliminating the loop completely. But
   * it also generates more code...
   */
  volatile uint16_t repeatCount = sIterationsPerUsec*usec;

  while (repeatCount--) ;

  return;
}

#endif  /* !SW_TIMER */

/**************************************************************************************************
 *                                  Compile Time Integrity Checks
 **************************************************************************************************
 */
// Error if FLL multiplier > 127
#if (!defined BSP_CONFIG_MSP430_N) || \
    (BSP_CONFIG_MSP430_N == 0) || (BSP_CONFIG_MSP430_N > 127)
#error "ERROR: Missing or illegal value for N (see register UCSCTL2)."
#endif

// Error if Frequency Loop Divisor > 4
#if (!defined BSP_CONFIG_MSP430_FLLDx) || (BSP_CONFIG_MSP430_FLLDx > FLLD_2)
#error "ERROR: Missing or illegal value for FLLDx (see register UCSCTL2)."
#endif

// Error if DCORSEL > 7
#if (!defined BSP_CONFIG_MSP430_DCORSEL) || (BSP_CONFIG_MSP430_DCORSEL > DCORSEL_7)
#error "ERROR: Missing or illegal value for FLLDx (see register SCFI0)."
#endif

/**************************************************************************************************
*/




