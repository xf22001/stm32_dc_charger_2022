/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define out3_Pin GPIO_PIN_2
#define out3_GPIO_Port GPIOE
#define out4_Pin GPIO_PIN_3
#define out4_GPIO_Port GPIOE
#define out5_Pin GPIO_PIN_4
#define out5_GPIO_Port GPIOE
#define out6_Pin GPIO_PIN_5
#define out6_GPIO_Port GPIOE
#define out7_Pin GPIO_PIN_6
#define out7_GPIO_Port GPIOE
#define in1_Pin GPIO_PIN_0
#define in1_GPIO_Port GPIOF
#define in2_Pin GPIO_PIN_1
#define in2_GPIO_Port GPIOF
#define in3_Pin GPIO_PIN_2
#define in3_GPIO_Port GPIOF
#define in4_Pin GPIO_PIN_3
#define in4_GPIO_Port GPIOF
#define in5_Pin GPIO_PIN_4
#define in5_GPIO_Port GPIOF
#define in6_Pin GPIO_PIN_5
#define in6_GPIO_Port GPIOF
#define tc_pcb_Pin GPIO_PIN_6
#define tc_pcb_GPIO_Port GPIOF
#define tc_a__Pin GPIO_PIN_7
#define tc_a__GPIO_Port GPIOF
#define tc_a_F8_Pin GPIO_PIN_8
#define tc_a_F8_GPIO_Port GPIOF
#define tc_b__Pin GPIO_PIN_9
#define tc_b__GPIO_Port GPIOF
#define tc_b_F10_Pin GPIO_PIN_10
#define tc_b_F10_GPIO_Port GPIOF
#define adc1_Pin GPIO_PIN_0
#define adc1_GPIO_Port GPIOC
#define adc2_Pin GPIO_PIN_1
#define adc2_GPIO_Port GPIOC
#define acd3_Pin GPIO_PIN_2
#define acd3_GPIO_Port GPIOC
#define adc4_Pin GPIO_PIN_3
#define adc4_GPIO_Port GPIOC
#define ethcs_Pin GPIO_PIN_4
#define ethcs_GPIO_Port GPIOA
#define ethso_Pin GPIO_PIN_6
#define ethso_GPIO_Port GPIOA
#define ethsi_Pin GPIO_PIN_7
#define ethsi_GPIO_Port GPIOA
#define adc5_Pin GPIO_PIN_4
#define adc5_GPIO_Port GPIOC
#define adc6_Pin GPIO_PIN_5
#define adc6_GPIO_Port GPIOC
#define cc1_a_Pin GPIO_PIN_0
#define cc1_a_GPIO_Port GPIOB
#define cc1_b_Pin GPIO_PIN_1
#define cc1_b_GPIO_Port GPIOB
#define ledeth_Pin GPIO_PIN_11
#define ledeth_GPIO_Port GPIOF
#define ledcpu_Pin GPIO_PIN_12
#define ledcpu_GPIO_Port GPIOF
#define rey14_Pin GPIO_PIN_13
#define rey14_GPIO_Port GPIOF
#define rey13_Pin GPIO_PIN_14
#define rey13_GPIO_Port GPIOF
#define rey12_Pin GPIO_PIN_15
#define rey12_GPIO_Port GPIOF
#define rey11_Pin GPIO_PIN_0
#define rey11_GPIO_Port GPIOG
#define rey10_Pin GPIO_PIN_1
#define rey10_GPIO_Port GPIOG
#define rey9_Pin GPIO_PIN_7
#define rey9_GPIO_Port GPIOE
#define rey8_Pin GPIO_PIN_8
#define rey8_GPIO_Port GPIOE
#define rey7_Pin GPIO_PIN_9
#define rey7_GPIO_Port GPIOE
#define rey6_Pin GPIO_PIN_10
#define rey6_GPIO_Port GPIOE
#define rey5_Pin GPIO_PIN_11
#define rey5_GPIO_Port GPIOE
#define rey4_Pin GPIO_PIN_12
#define rey4_GPIO_Port GPIOE
#define rey3_Pin GPIO_PIN_13
#define rey3_GPIO_Port GPIOE
#define rey2_Pin GPIO_PIN_14
#define rey2_GPIO_Port GPIOE
#define rey1_Pin GPIO_PIN_15
#define rey1_GPIO_Port GPIOE
#define e2sck_Pin GPIO_PIN_10
#define e2sck_GPIO_Port GPIOB
#define e2cs_Pin GPIO_PIN_11
#define e2cs_GPIO_Port GPIOB
#define e2so_Pin GPIO_PIN_14
#define e2so_GPIO_Port GPIOB
#define e2si_Pin GPIO_PIN_15
#define e2si_GPIO_Port GPIOB
#define lock_b_Pin GPIO_PIN_8
#define lock_b_GPIO_Port GPIOD
#define lock_a_Pin GPIO_PIN_9
#define lock_a_GPIO_Port GPIOD
#define unlock_b_Pin GPIO_PIN_10
#define unlock_b_GPIO_Port GPIOD
#define unlock_a_Pin GPIO_PIN_11
#define unlock_a_GPIO_Port GPIOD
#define bk_b_Pin GPIO_PIN_12
#define bk_b_GPIO_Port GPIOD
#define bk_a_Pin GPIO_PIN_13
#define bk_a_GPIO_Port GPIOD
#define ledsta1_Pin GPIO_PIN_14
#define ledsta1_GPIO_Port GPIOD
#define ledsta2_Pin GPIO_PIN_15
#define ledsta2_GPIO_Port GPIOD
#define rey21_Pin GPIO_PIN_2
#define rey21_GPIO_Port GPIOG
#define rey20_Pin GPIO_PIN_3
#define rey20_GPIO_Port GPIOG
#define rey19_Pin GPIO_PIN_4
#define rey19_GPIO_Port GPIOG
#define rey18_Pin GPIO_PIN_5
#define rey18_GPIO_Port GPIOG
#define rey17_Pin GPIO_PIN_6
#define rey17_GPIO_Port GPIOG
#define rey16_Pin GPIO_PIN_7
#define rey16_GPIO_Port GPIOG
#define rey15_Pin GPIO_PIN_8
#define rey15_GPIO_Port GPIOG
#define vo_data_Pin GPIO_PIN_8
#define vo_data_GPIO_Port GPIOC
#define vo_cs_Pin GPIO_PIN_9
#define vo_cs_GPIO_Port GPIOC
#define vo_clk_Pin GPIO_PIN_8
#define vo_clk_GPIO_Port GPIOA
#define cancs_Pin GPIO_PIN_15
#define cancs_GPIO_Port GPIOA
#define adhe1_Pin GPIO_PIN_3
#define adhe1_GPIO_Port GPIOD
#define adhe2_Pin GPIO_PIN_4
#define adhe2_GPIO_Port GPIOD
#define adhe3_Pin GPIO_PIN_5
#define adhe3_GPIO_Port GPIOD
#define door_Pin GPIO_PIN_6
#define door_GPIO_Port GPIOD
#define stop_Pin GPIO_PIN_7
#define stop_GPIO_Port GPIOD
#define set1_Pin GPIO_PIN_9
#define set1_GPIO_Port GPIOG
#define set2_Pin GPIO_PIN_10
#define set2_GPIO_Port GPIOG
#define set3_Pin GPIO_PIN_11
#define set3_GPIO_Port GPIOG
#define set4_Pin GPIO_PIN_12
#define set4_GPIO_Port GPIOG
#define set5_Pin GPIO_PIN_13
#define set5_GPIO_Port GPIOG
#define set6_Pin GPIO_PIN_14
#define set6_GPIO_Port GPIOG
#define cansck_Pin GPIO_PIN_3
#define cansck_GPIO_Port GPIOB
#define canso_Pin GPIO_PIN_4
#define canso_GPIO_Port GPIOB
#define cansi_Pin GPIO_PIN_5
#define cansi_GPIO_Port GPIOB
#define canint0_Pin GPIO_PIN_6
#define canint0_GPIO_Port GPIOB
#define canint1_Pin GPIO_PIN_7
#define canint1_GPIO_Port GPIOB
#define led1_Pin GPIO_PIN_8
#define led1_GPIO_Port GPIOB
#define led2_Pin GPIO_PIN_9
#define led2_GPIO_Port GPIOB
#define out1_Pin GPIO_PIN_0
#define out1_GPIO_Port GPIOE
#define out2_Pin GPIO_PIN_1
#define out2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
