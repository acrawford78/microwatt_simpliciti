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

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS�
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
 *   Target : Texas Instruments MSP-EXP430FR5739
 *            "MSP430FR5739 Experimenter Board"
 *   Top-level board code file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "bsp.h"
#include "bsp_config.h"

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
    PJSEL0 |= BIT4 + BIT5;
    PJSEL1 &= ~(BIT4 + BIT5);

    /* Initialize the CS module*/
    CSCTL0_H = 0xA5;                            // Unlock registers
	CSCTL1 = BSP_CONFIG_MSP430_CSCTL1;          // Set DCO settings
	CSCTL2 = SELA_0 + SELS_3 + SELM_3;          // set ACLK = XT1; SMCLK/MCLK = DCO
    CSCTL3 = BSP_CONFIG_MSP430_CSCTL3;          // Se clock dividers
    CSCTL4 |= XT1DRIVE_0;
    CSCTL4 &= ~XT1OFF;

    do
    {
        CSCTL5 &= ~XT1OFFG;                     // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1&OFIFG);                     // Test oscillator fault flag
	CSCTL0_H = 0x01;                            // Lock Registers

  /* Configure TimerA for use by the delay function */
  /* Reset the timer */
  TA1CTL |= TACLR;                          // Reset the timer
  TA1CTL = 0x0;                             // Clear all settings
  TA1CTL |= TASSEL_2;                       // Select the clk source to be SMCLK

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

  TA1R = 0; /* initial count */
  TA1CCR0 = BSP_TIMER_CLK_MHZ*usec; /* compare count. (delay in ticks) */

  /* Start the timer in UP mode */
  TA1CTL |= MC_1;

  /* Loop till compare interrupt flag is set */
  while(!(TA1CCTL0 & CCIFG));

  /* Stop the timer */
  TA1CTL &= ~(MC_1);

  /* Clear the interrupt flag */
   TA1CCTL0 &= ~CCIFG;
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
// Error if DCOFSEL is missing
#if (!defined BSP_CONFIG_MSP430_CSCTL1)
#error "ERROR: Missing value for CSCTL1"
#endif

// Error if Clock dividers are missing
#if (!defined BSP_CONFIG_MSP430_CSCTL3)
#error "ERROR: Missing value for CSCTL3"
#endif


/**************************************************************************************************
*/




