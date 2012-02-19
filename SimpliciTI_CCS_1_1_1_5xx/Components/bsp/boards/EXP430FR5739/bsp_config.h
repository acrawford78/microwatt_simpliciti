/**************************************************************************************************
  Revised:        $Date: 2007-07-06 11:19:00 -0700 (Fri, 06 Jul 2007) $
  Revision:       $Revision: 13579 $

  Copyright 2007 Texas Instruments Incorporated.  All rights reserved.

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
 *   Target : Texas Instruments MSP-EXP430FR5739
 *            "MSP430FR5739 Experimenter Board"
 *   Board configuration file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H


/**************************************************************************************************
 *                                       Configuration                                            *
 **************************************************************************************************
 */

/*
 *  Supported clock speeds : 1, 4, 8, 16, 20, and 24 MHz.
 *
 *  NOTE!  The clock speeds are approximate as they are derived from an internal
 *         digitally controlled oscillator.
 */
#define BSP_CONFIG_CLOCK_MHZ_SELECT     8  /* approximate MHz */

/*
 *  Custom clock configuration is available.  Provide values for CSCTL1 and CSCTL3
 *  shown in the example below.  The clock speed itself must be provided too.  Providing
 *  a custom configuration overrides the above selection.
 *
 *  #define BSP_CONFIG_CUSTOM_CLOCK_MHZ         5.33
 *  #define BSP_CONFIG_MSP430_CUSTOM_CSCTL1     DCOFSEL_0
 *  #define BSP_CONFIG_MSP430_CUSTOM_CSCTL3     (DIVA__1 + DIVS__1 + DIVM__1)
 */


/* ------------------------------------------------------------------------------------------------
 *                                Exported Clock Configuration
 * ------------------------------------------------------------------------------------------------
 */

/*
 *   DCO is adjusted via FSEL in CSCTL1 and clock dividers in CSCTL3
 *
 *   Check datasheet for CSCTL1 values.
 *
 */

#if (defined BSP_CONFIG_CUSTOM_CLOCK_MHZ)    || \
    (defined BSP_CONFIG_MSP430_CUSTOM_CSCTL1) || \
    (defined BSP_CONFIG_MSP430_CUSTOM_CSCTL3)
#define BSP_CONFIG_CLOCK_MHZ            BSP_CONFIG_CUSTOM_CLOCK_MHZ
#define BSP_CONFIG_MSP430_CSCTL1        BSP_CONFIG_MSP430_CUSTOM_CSCTL1
#define BSP_CONFIG_MSP430_CSCTL3        BSP_CONFIG_MSP430_CUSTOM_CSCTL3
#else

#if (BSP_CONFIG_CLOCK_MHZ_SELECT == 1)
#define BSP_CONFIG_CLOCK_MHZ        1.00
#define BSP_CONFIG_MSP430_CSCTL1    (DCOFSEL_3)
#define BSP_CONFIG_MSP430_CSCTL3    (DIVA__1 + DIVS__8 + DIVM__8)
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 4)
#define BSP_CONFIG_CLOCK_MHZ        4.00
#define BSP_CONFIG_MSP430_CSCTL1    (DCOFSEL_3)
#define BSP_CONFIG_MSP430_CSCTL3    (DIVA__1 + DIVS__2 + DIVM__2)
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 8)
#define BSP_CONFIG_CLOCK_MHZ        8.00
#define BSP_CONFIG_MSP430_CSCTL1    (DCOFSEL_3)
#define BSP_CONFIG_MSP430_CSCTL3    (DIVA__1 + DIVS__1 + DIVM__1)
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 16)
#define BSP_CONFIG_CLOCK_MHZ        16.00
#define BSP_CONFIG_MSP430_CSCTL1    (DCORSEL|DCOFSEL_0)
#define BSP_CONFIG_MSP430_CSCTL3    (DIVA__1 + DIVS__1 + DIVM__1)
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 20)
#define BSP_CONFIG_CLOCK_MHZ        20.00
#define BSP_CONFIG_MSP430_CSCTL1    (DCORSEL|DCOFSEL_1)
#define BSP_CONFIG_MSP430_CSCTL3    (DIVA__1 + DIVS__1 + DIVM__1)
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 24)
#define BSP_CONFIG_CLOCK_MHZ        24.00
#define BSP_CONFIG_MSP430_CSCTL1    (DCORSEL|DCOFSEL_3)
#define BSP_CONFIG_MSP430_CSCTL3    (DIVA__1 + DIVS__1 + DIVM__1)
#else
#error "ERROR: Unsupported clock speed.  Custom clock speeds are possible.  See comments in code."
#endif

#endif


/**************************************************************************************************
 */
#endif
