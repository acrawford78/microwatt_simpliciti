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
 *   Generic LED driver code file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "bsp_leds.h"
#include "bsp_led_defs.h"


/* ------------------------------------------------------------------------------------------------
 *                                             Macros
 * ------------------------------------------------------------------------------------------------
 */
#define BSP_CONFIG_LED1()    __bsp_LED1_CONFIG__()
#define BSP_CONFIG_LED2()    __bsp_LED2_CONFIG__()
#define BSP_CONFIG_LED3()    __bsp_LED3_CONFIG__()
#define BSP_CONFIG_LED4()    __bsp_LED4_CONFIG__()
#define BSP_CONFIG_LED5()    __bsp_LED5_CONFIG__()
#define BSP_CONFIG_LED6()    __bsp_LED6_CONFIG__()
#define BSP_CONFIG_LED7()    __bsp_LED7_CONFIG__()
#define BSP_CONFIG_LED8()    __bsp_LED8_CONFIG__()

#ifdef __bsp_LED_EXTENDED_CONFIG__
#define BSP_LED_EXTENDED_CONFIG()  __bsp_LED_EXTENDED_CONFIG__()
#else
#define BSP_LED_EXTENDED_CONFIG()
#endif

#ifdef __bsp_LED_PORT1_CONFIG__
#define BSP_CONFIG_LED_PORT1(bit)   __bsp_LED_PORT1_CONFIG__(bit)
#else
#define BSP_CONFIG_LED_PORT1(bit)
#endif


/**************************************************************************************************
 * @fn          BSP_InitLeds
 *
 * @brief       Initialize LED hardware and driver.
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */
void BSP_InitLeds(void)
{
  /* configure LEDs */
  BSP_CONFIG_LED1();
  BSP_CONFIG_LED2();
  BSP_CONFIG_LED3();
  BSP_CONFIG_LED4();
  BSP_CONFIG_LED5();
  BSP_CONFIG_LED6();
  BSP_CONFIG_LED7();
  BSP_CONFIG_LED8();

  /* peform extended configuration if needed */
  BSP_LED_EXTENDED_CONFIG();

  /* turn all LEDs off as power-up default */
  BSP_TURN_OFF_LED1();
  BSP_TURN_OFF_LED2();
  BSP_TURN_OFF_LED3();
  BSP_TURN_OFF_LED4();
  BSP_TURN_OFF_LED5();
  BSP_TURN_OFF_LED6();
  BSP_TURN_OFF_LED7();
  BSP_TURN_OFF_LED8();
  
  /* allows led to be accessed as a port only used if enabled*/
  BSP_CONFIG_LED_PORT1(0);
  BSP_CONFIG_LED_PORT1(1);
  BSP_CONFIG_LED_PORT1(2);
  BSP_CONFIG_LED_PORT1(3);
  BSP_CONFIG_LED_PORT1(4);
  BSP_CONFIG_LED_PORT1(5);
  BSP_CONFIG_LED_PORT1(6);
  BSP_CONFIG_LED_PORT1(7);
  
  BSP_TURN_OFF_LED_PORT1(0);
  BSP_TURN_OFF_LED_PORT1(1);
  BSP_TURN_OFF_LED_PORT1(2);
  BSP_TURN_OFF_LED_PORT1(3);
  BSP_TURN_OFF_LED_PORT1(4);
  BSP_TURN_OFF_LED_PORT1(5);
  BSP_TURN_OFF_LED_PORT1(6);
  BSP_TURN_OFF_LED_PORT1(7);
}


/**************************************************************************************************
*/

   
   
