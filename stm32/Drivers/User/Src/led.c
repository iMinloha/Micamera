/***
	*************************************************************************************************
	*	@file  	main.c
	*	@version V1.0
	*  @date    2020-5-13
	*	@author  反客科技	
	*	@brief   LED闪烁实验
   *************************************************************************************************
   *  @description
	*
	*	实验平台：反客STM32H750VBT6核心板 （型号：FK750M1-VBT6）
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*	初始化LED的IO口，配置为推挽输出、上拉、速度等级2M。
	*
	************************************************************************************************
***/

#include "stm32h7xx_hal.h"
#include "led.h"  


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_LED1_CLK_ENABLE;		// 初始化LED1 GPIO时钟	


	HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);		// LED1引脚输出低，即点亮LED1


	GPIO_InitStruct.Pin 		= LED1_PIN;					// LED1引脚
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;	// 推挽输出模式
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;				// 不上下拉
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;	// 低速模式
	HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);


}

void key_init(void){
	// PA11
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);		// LED1引脚输出低，即点亮LED1


	GPIO_InitStruct.Pin 		= GPIO_PIN_15;					// LED1引脚
	GPIO_InitStruct.Mode 	= GPIO_MODE_INPUT;	// 推挽输出模式
	GPIO_InitStruct.Pull 	= GPIO_PULLUP;				// 不上下拉
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;	// 低速模式
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// PD8
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin 		= GPIO_PIN_8;					// LED1引脚
	GPIO_InitStruct.Mode 	= GPIO_MODE_INPUT;	// 推挽输出模式
	GPIO_InitStruct.Pull 	= GPIO_PULLUP;				// 不上下拉
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;	// 低速模式
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

int random(){
	return (214013*HAL_GetTick() +2531011)>>16&((1<<15)-1);
}