/**
  ******************************************************************************
  * @file    inc/clock.h
  * @module  router
  * @author  Mohamed Hnezli
  * @version V0.1
  * @date    11-07-2018
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
#ifndef CLOCK_H_
#define CLOCK_H_

/* Include -------------------------------------------------------------------*/

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
systemclock_config (void);

#endif /* CLOCK_H_ */
