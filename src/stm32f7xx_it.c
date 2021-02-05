/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f7xx_it.h"
#include "smartring.h"                 // gt_appstatus
#include "stm32f723e_discovery_ts.h"   // BSP_TS_GetState


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern sr_appstatus gt_appstatus;
__IO int32_t gi_gesture = 1;
extern TIM_HandleTypeDef gs_sleeptim;         // sleep timer


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}

/*
 * ]name   EXTI9_5_IRQHandler.
 * @brief  touch screen press ISR.
 * @param  none.
 * @retval none.
 */
void
EXTI9_5_IRQHandler (void)
{
  /* clear IRQ */
  HAL_GPIO_EXTI_IRQHandler (TS_INT_PIN);

  /* prevent entering sleep mode */
  HAL_TIM_Base_Stop_IT (&gs_sleeptim);

  /* hanlde press/release gesture */
  if (gi_gesture == 1)
    {
      gt_appstatus = as_handletouch;
      gi_gesture = 2;
    }
  else
    {
	  gi_gesture = 1;
    }

  return;
}

/*
 * @name   TIM3_IRQHandler.
 * @brief  timer callback to enter sleep mode.
 * @param  none.
 * @retval none.
 */
void
TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler (&gs_sleeptim);

  return;
}

/*
 * @name   HAL_TIM_PeriodElapsedCallback.
 * @brief  Period elapsed callback in non blocking mode, used to measure inactivity time
 * @param  htim : TIM handle
 * @retval None
 */
void
HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3)
    {
      gt_appstatus = as_sleep;
      HAL_TIM_Base_Stop_IT(&gs_sleeptim);
    }

  return;
}
