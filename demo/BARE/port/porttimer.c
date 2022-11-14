/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "main.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
/* ----------------------- Variables ----------------------------------------*/
extern TIM_HandleTypeDef* modbusTimer;
uint16_t timerPeriod = 0;
uint16_t timerCounter = 0;
/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    timerPeriod = usTim1Timerout50us;
	return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	timerCounter = 0;
	HAL_TIM_Base_Start_IT(modbusTimer);
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
	
  HAL_TIM_Base_Stop_IT(modbusTimer);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}
/* --------------------------------------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback2(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == modbusTimer->Instance)
  {
    timerCounter++;
    if (timerCounter == timerPeriod)
    {
      prvvTIMERExpiredISR();
    }
  }
}

void vMBPortTimersDelay(USHORT  MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS )
{
	osDelay(MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS);
}
