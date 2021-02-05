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
#include <smartring.h>                 // header file
#include "stm32f7xx_hal.h"             // MCU HAL
#include "stm32f723e_discovery_lcd.h"  // board LCD HAL
#include "stm32f723e_discovery_ts.h"   // touch screen HAL
#include "ts_calib.h"                  // calibration callbacks


/* define --------------------------------------------------------------------*/
/*
 * @brief maximum value of sweep counter
 */
#define MAX_SWCNT            99999
#define MAX_CNTDIGITS        9//5

/*
 * @brief touch screen backlight time
 * @note  must be lesser than 0xFFFF
 */
#define MAX_BACKLIGHT        10000     // in milliseconds
/*
 * @brief screen orientation
 */
#define SWEEP_RIGHT          1
#define SWEEP_LEFT           2

/*
 * @brief  footer configuration
 */
#define FOOTER_TXTCOLOR      LCD_COLOR_BLACK
#define FOOTER_FONT          Font16
#define FOOTER_TXTMSG        "MENU"

/*
 * @brief items to display in menu.
 */
#define MENU_FONT1           "FONT1"
#define MENU_FONT2           "FONT2"
#define MENU_FONT3           "FONT3"
#define MENU_DRIGHT          "RIGHT"
#define MENU_DLEFT           "LEFT"
#define MENU_EXIT            "EXIT"

/*
 * @brief main screen configuration
 */
#define MAIN_ORIEN           LCD_ORIENTATION_LANDSCAPE
#define MAIN_FONT1           Font12
#define MAIN_FONT2           Font16
#define MAIN_FONT3           Font24
#define MAIN_BGCOLOR         LCD_COLOR_WHITE     // background color
#define MAIN_TXTCOLOR        LCD_COLOR_BLACK     // text color


/* typedef -------------------------------------------------------------------*/

/* constant ------------------------------------------------------------------*/

/* variable ------------------------------------------------------------------*/
sr_appstatus gt_appstatus = as_init;   // process sequence to execute
uint32_t gui_swcnt = 0;                // sweep counter
sFONT *gps_font = &MAIN_FONT1;         // text font
uint32_t gui_sweepdir = SWEEP_RIGHT;   // sweep direction
int32_t gi_ismain = 1;                 // choose text form to print
TIM_HandleTypeDef gs_sleeptim;         // sleep timer
__IO uint32_t gui_timprescaler = 0;


/* class -------------------------------------------------------------------- */

/* function ----------------------------------------------------------------- */
/*
 * @name   sr_printcounter.
 * @brief  prints the sweep counter in screen center.
 * @param  none.
 * @retval none.
 */
void
sr_inittimer (void)
{
  gui_timprescaler = (uint32_t)((SystemCoreClock / 2) / 1000) - 1;

  /* Set TIMx instance */
  gs_sleeptim.Instance = TIM3;

  /* Initialize TIMx peripheral as follows:
       + Period = 1000 - 1
       + Prescaler = ((SystemCoreClock / 2)/1000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  gs_sleeptim.Instance = TIM3;
  gs_sleeptim.Init.Period            = MAX_BACKLIGHT - 1;
  gs_sleeptim.Init.Prescaler         = gui_timprescaler;
  gs_sleeptim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  gs_sleeptim.Init.CounterMode       = TIM_COUNTERMODE_DOWN;
  gs_sleeptim.Init.RepetitionCounter = 0;
  gs_sleeptim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  __HAL_RCC_TIM3_CLK_ENABLE();
  if (HAL_TIM_Base_Init(&gs_sleeptim) != HAL_OK)
    {
      while (1);
    }
  NVIC_EnableIRQ(TIM3_IRQn);

  return;
}

/*
 * @name   sr_printcounter.
 * @brief  prints the sweep counter in screen center.
 * @param  none.
 * @retval none.
 */
void
sr_printmenu (void)
{
  uint16_t lu16_line;

  BSP_LCD_Clear (MAIN_BGCOLOR);

  lu16_line = 2 * ((BSP_LCD_GetYSize() / gps_font->Height) / 9);
  if (gps_font->Height == MAIN_FONT1.Height)
    {
	  BSP_LCD_DisplayStringAt (0, LINE(lu16_line), (uint8_t*)MENU_FONT1, CENTER_MODE);
    }
  else if (gps_font->Height == MAIN_FONT2.Height)
    {
	  BSP_LCD_DisplayStringAt (0, LINE(lu16_line), (uint8_t*)MENU_FONT2, CENTER_MODE);
    }
  else
    {
	  BSP_LCD_DisplayStringAt (0, LINE(lu16_line), (uint8_t*)MENU_FONT3, CENTER_MODE);
    }

  lu16_line = 5 * ((BSP_LCD_GetYSize() / gps_font->Height) / 9);
  if (gui_sweepdir == SWEEP_RIGHT)
    {
      BSP_LCD_DisplayStringAt (0, LINE(lu16_line), (uint8_t*)MENU_DRIGHT, CENTER_MODE);
    }
  else
    {
      BSP_LCD_DisplayStringAt (0, LINE(lu16_line), (uint8_t*)MENU_DLEFT, CENTER_MODE);
    }

  lu16_line = 8 * ((BSP_LCD_GetYSize() / gps_font->Height) / 9);
  BSP_LCD_DisplayStringAt (0, LINE(lu16_line), (uint8_t*)MENU_EXIT, CENTER_MODE);

  return;
}

/*
 * @name   sr_printcounter.
 * @brief  prints the sweep counter in screen center.
 * @param  none.
 * @retval none.
 */
void
sr_printcounter (void)
{
  char lpc_swcnt [MAX_CNTDIGITS];
  uint16_t lu16_line;

  /* stringify counter */
  snprintf ((char*)lpc_swcnt, MAX_CNTDIGITS+1, "%u", gui_swcnt);

  lu16_line = (BSP_LCD_GetYSize() / gps_font->Height) / 2;  // middle line

  /* clear and update */
  BSP_LCD_ClearStringLine (lu16_line);
  BSP_LCD_DisplayStringAt (0, LINE(lu16_line), (uint8_t*)lpc_swcnt, CENTER_MODE);

  return;
}

/*
 * @name   sr_clearcounter.
 * @brief  prints the sweep counter in screen center.
 * @param  none.
 * @retval none.
 */
void
sr_clearcounter (void)
{
  uint16_t lu16_line;

  lu16_line = (BSP_LCD_GetYSize() / gps_font->Height) / 2;  // middle line
  BSP_LCD_ClearStringLine (lu16_line);

  return;
}

/*
 * @name   sr_changefont.
 * @brief  changes the font of sweep counter.
 * @param  none.
 * @retval none.
 */
void
sr_changefont (void)
{
  sr_clearcounter ();

  /* change configuration */
  if (gps_font->Height == MAIN_FONT1.Height)
    {
      gps_font = &MAIN_FONT2;
    }
  else if (gps_font->Height == MAIN_FONT2.Height)
    {
      gps_font = &MAIN_FONT3;
    }
  else
    {
      gps_font = &MAIN_FONT1;
    }

  BSP_LCD_SetFont (gps_font);

  /* apply changes */
  if (gi_ismain == 1)
    {
      sr_printcounter ();
    }
  else
    {
      sr_printmenu ();
    }

  return;
}

/*
 * @name   sr_showfooter.
 * @brief  displays the application footer on the LCD screen
 * @param  none.
 * @retval none.
 */
void
sr_showfooter (void)
{
  uint16_t lu16_oldbackcolor, lu16_oldtxtcolor;

  /* backup configuration */
  lu16_oldbackcolor = BSP_LCD_GetBackColor ();
  lu16_oldtxtcolor = BSP_LCD_GetTextColor ();

  /* Set the LCD Font */
  BSP_LCD_SetFont (&FOOTER_FONT);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor (FOOTER_TXTCOLOR);
  BSP_LCD_DrawHLine (0, BSP_LCD_GetYSize() - FOOTER_FONT.Height - 2, BSP_LCD_GetXSize());
  BSP_LCD_DisplayStringAt (0, BSP_LCD_GetYSize() - FOOTER_FONT.Height, FOOTER_TXTMSG, CENTER_MODE);

  BSP_LCD_SetBackColor (lu16_oldbackcolor);
  BSP_LCD_SetTextColor (lu16_oldtxtcolor);
  BSP_LCD_SetFont (gps_font);

  return;
}

/*
 * @name   sr_printcounter.
 * @brief  prints the sweep counter in screen center.
 * @param  none.
 * @retval none.
 */
void
sr_handletouch (int32_t ini_ismain)
{
  TS_StateTypeDef ls_TSStatus;
  uint16_t lu16_xthreshold;
  uint16_t lu16_ythreshold;
  uint16_t lu16_yportion;

  /* localize position */
  BSP_TS_GetState (&ls_TSStatus);
  ls_TSStatus.touchX[0] = TouchScreen_Get_Calibrated_X (ls_TSStatus.touchX[0]);
  ls_TSStatus.touchY[0] = TouchScreen_Get_Calibrated_Y (ls_TSStatus.touchY[0]);

  if (gi_ismain == 1)
    {
      lu16_xthreshold = BSP_LCD_GetXSize() / 2;
      lu16_ythreshold = BSP_LCD_GetYSize() - (BSP_LCD_GetYSize() / 5);

      if (ls_TSStatus.touchY[0] < lu16_ythreshold)
        {
          /* simple sweep */
          gt_appstatus = as_update;
        }
      else //if (ls_TSStatus.touchX[0] > lu16_xthreshold)
        {
          /* switch to menu */
          sr_printmenu ();
          gi_ismain = 0;
          gt_appstatus = as_hold;
        }
//      else
//        {
//          /* do nothing */
//          gt_appstatus = as_hold;
//        }
    }
  else
    {
	  lu16_yportion = (BSP_LCD_GetYSize() / gps_font->Height) / 3;
      if (ls_TSStatus.touchY[0] < lu16_yportion)
        {
    	  /* switch to next font */
          sr_changefont ();
//          sr_printmenu ();
          gt_appstatus = as_hold;
        }
      else if (ls_TSStatus.touchY[0] < (2 * lu16_yportion))
        {
          /* switch sweep direction */
    	  if (gui_sweepdir == SWEEP_RIGHT)
            {
              gui_sweepdir = SWEEP_LEFT;
            }
    	  else
            {
              gui_sweepdir = SWEEP_RIGHT;
            }
          gt_appstatus = as_hold;
        }
      else
        {
          /* quit menu */
    	  gi_ismain = 1;
          gt_appstatus = as_init;
        }
    }

  return;
}

/*
 * @name   sr_init.
 * @brief  configures display and and touch screen.
 * @param  none.
 * @retval none.
 */
void
sr_init (void)
{
  /* configure display */
  BSP_LCD_InitEx (gui_sweepdir);
  BSP_LCD_Clear (MAIN_BGCOLOR);

  /* calibrate Phy values */

  /* configure touch screen */
  TS_IO_Init ();
  HAL_Delay (500);                     // for stability
  BSP_TS_InitEx ((uint16_t)BSP_LCD_GetXSize (), (uint16_t)BSP_LCD_GetYSize (),
                 MAIN_ORIEN);          // static orientation
  Touchscreen_Calibration ();          // calibrate Phy values
  BSP_TS_ITConfig ();
  BSP_LCD_InitEx (MAIN_ORIEN);
  BSP_LCD_SetTextColor (MAIN_TXTCOLOR);

  /* initilise counter */
  gui_swcnt = 0;

  /* configure sleep timer */
  sr_inittimer ();

  /* initialise screen */
  gt_appstatus = as_init;

  return;
}

/*
 * @name   sr_run.
 * @brief  main application process.
 * @param  none.
 * @retval none.
 */
void
sr_run (void)
{
  switch (gt_appstatus)
    {
    case as_init:
      BSP_LCD_Clear (MAIN_BGCOLOR);

      /* show footer */
      sr_showfooter ();

      /* show counter */
      sr_printcounter ();

      /* update status */
      gt_appstatus = as_hold;
      break;

    case as_handletouch:
      sr_handletouch (gi_ismain);
      break;

    case as_update:
      /* update sweep counter */
      if (gui_swcnt < MAX_SWCNT)
        {
          gui_swcnt += 1;
        }
      sr_printcounter ();

      /* update status */
      gt_appstatus = as_hold;
      break;

    case as_sleep:
      /* halt peripherals and MCU */
      BSP_LCD_DisplayOff ();
      while (gt_appstatus == as_sleep)
        {
          ;
        }
      BSP_LCD_DisplayOn ();
      /* update status */
//      gt_appstatus = as_hold;
      break;

    case as_hold:
      HAL_TIM_Base_Start_IT (&gs_sleeptim);
      while (gt_appstatus == as_hold);
      break;

    default:
      /* do nothing */
      break;
    }

  return;
}
