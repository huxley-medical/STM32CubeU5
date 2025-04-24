/**
  ******************************************************************************
  * @file    stm32u5xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_it.h"
#include "stm32u5xx_hal.h"
#include "main.h"


/** @addtogroup Validation_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (USE_BSP_IO_CLASS == 1)
extern __IO uint32_t IddOnGoing;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M33 Processor Exceptions Handlers                        */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32U5xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32u5xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles GFXMMU interrupt request.
  * @param  None
  * @retval None
  */
void GFXMMU_IRQHandler(void)
{
  HAL_GFXMMU_IRQHandler(&hlcd_gfxmmu);
}

/**
  * @brief  This function handles LTDC interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hlcd_ltdc);
}

/**
  * @brief  This function handles LTDC error interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_ER_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hlcd_ltdc);
}

/**
  * @brief  This function handles DSI interrupt request.
  * @param  None
  * @retval None
  */
void DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&hlcd_dsi);
}

/**
  * @brief  This function handles EXTI13 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI13_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hpb_exti[0]);
}

/**
  * @brief  This function handles EXTI1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hpb_exti[1]);
}

/**
  * @brief  This function handles External line 8 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI8_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(TS_INT_PIN);
}

/**
  * @brief  This function handles External line 14 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI14_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(VL53L5A1_INT_PIN);
}

/**
  * @}
  */



