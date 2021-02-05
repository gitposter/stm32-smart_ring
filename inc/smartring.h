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
#ifndef SMARTRING_H_
#define SMARTRING_H_

/* Includes ------------------------------------------------------------------*/

/* define --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/
typedef enum
{
  as_init = 0,
  as_update,
  as_hold,
  as_handletouch,
  as_menu,
  as_sleep,
}sr_appstatus;


/* constant ------------------------------------------------------------------*/

/* variable ------------------------------------------------------------------*/

/* class -------------------------------------------------------------------- */

/* function ----------------------------------------------------------------- */
/*
 * @name   systemclock_config.
 * @brief  main application process.
 * @param  none.
 * @retval none.
 * @note   HSI is the source clock and is considered to have 25MHz of speed. SysClk is set to 25MHz.
 */
void
sr_init (void);

/*
 * @name   systemclock_config.
 * @brief  main application process.
 * @param  none.
 * @retval none.
 * @note   HSI is the source clock and is considered to have 25MHz of speed. SysClk is set to 25MHz.
 */
void
sr_run (void);


#endif /* SMARTRING_H_ */
