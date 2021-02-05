/**
  ******************************************************************************
  * @file    inc/hal_cpp/gpio.h
  * @module  router
  * @author  Mohamed Hnezli
  * @version V0.1
  * @date    04-02-2018
  * @brief   This file defines a GPIO HAL in C++. It is based on stm32 HAL
  *          library.
  ******************************************************************************
  * @attention
  *
  * - The present software had been tested only on STM32L476RG and may be non
  * functional on other targets.
  *
  * - This software is a part of the WiDe project. It may be non functional in
  * other environment.
  *
  * <h2><center>&copy COPYRIGHT 2018 CodinTek </center></h2>
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "stm32f723e_discovery.h"      // board LEDs HAL
#include "clock.h"                     // clock configuration
#include "smartring.h"                 // application routines
#include "stm32f723e_discovery_lcd.h"


/* define --------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* constant ------------------------------------------------------------------*/
/* variable ------------------------------------------------------------------*/
/* class -------------------------------------------------------------------- */
/* function ----------------------------------------------------------------- */
/*
 * @name   main.
 * @brief  main infinit loop executed after system startup functions.
 * @param  none.
 * @retval none.
 */
int
main(void)
{
  /* configure system clocks */
  systemclock_config ();

  /* configure LEDs */
  BSP_LED_Init (LED5);
  BSP_LED_Init (LED6);
  BSP_LED_Toggle (LED6);

  /* initalise application */
  sr_init ();

  while (1)
    {
	  /* execute main smart ring process */
      sr_run ();

      /* fancy blink */
      BSP_LED_Toggle (LED5);
      BSP_LED_Toggle (LED6);
      HAL_Delay (50);
    }

  return 0;        // should never get here
}
