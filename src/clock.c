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

/* Includes ------------------------------------------------------------------*/
#include "clock.h"                     // header file
#include "stm32f7xx.h"                 // MCU HAL


/* define --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

/* constant ------------------------------------------------------------------*/

/* variable ------------------------------------------------------------------*/

/* class -------------------------------------------------------------------- */

/* function ----------------------------------------------------------------- */
/*
 * @name   systemclock_config.
 * @brief  configures and activates system clocks.
 * @param  none.
 * @retval none.
 * @note   HSI is the source clock and is considered to have 25MHz of speed. SysClk is set to 25MHz.
 */
void
systemclock_config (void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /* SysClk = (PLL_N * (HSI_VALUE / PLL_M)) / PLL_P   is subject to:
   * Freq(AHB)  < 168MHz
   * Freq(APB)  < 42MHz
   * Freq(APB2) < 84MHz */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 25;    // 25MHz
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 25;   // [1..8]
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;    // SysClk = 25MHz
  RCC_OscInitStruct.PLL.PLLQ = 10;
  /* NO PLL_R in this MCU circuitry */
//  RCC_OscInitStruct.PLL.PLLR =
  if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK)
    {
      while (1)
        {
          ;
        }
    }

  /* initialize the SysClk and AHB and APB busses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
      while (1)
        {
          ;
        }
    }

  /* enable timer prescaler */
  PeriphClkInit.TIMPresSelection = RCC_TIMPRES_ACTIVATED;
  if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInit) != HAL_OK)
    {
	  while (1) ;
    }

  /* Configure the main internal regulator output voltage */
//  if (HAL_PWREx_ControlVoltageScaling (PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
//    {
//      while (1)
//        {
//          ;
//        }
//    }

  /* Configure the Systick interrupt time */
  HAL_SYSTICK_Config (HAL_RCC_GetHCLKFreq () / 1000);

  /* Configure the Systick */
  HAL_SYSTICK_CLKSourceConfig (SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority (SysTick_IRQn, 0, 0);     // highest IRQs

  return;
}
