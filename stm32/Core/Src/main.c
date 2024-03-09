/***
	*************************************************************************************************
	*	@file  	main.c
	*	@version V1.0
	*  @date    2022-3-21
	*	@author  ���ͿƼ�	
	*	@brief   OV5640ͼ��ɼ�
   *************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32H750VBT6���İ� ���ͺţ�FK750M1-VBT6��
	*				+ 2.00��Һ��ģ�飨�ͺţ�SPI200M1-240*320��
	*				+ OV5640ģ�飨�ͺţ�OV5640M1-500W�� 
	*
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*
>>>>> ����˵����
	*
	*	OV5640�ɼ�ͼ����ʾ����Ļ
	*
	************************************************************************************************
***/

#include "main.h"
#include "led.h"
#include "usart.h"
#include "lcd_spi_200.h"
#include "dcmi_ov5640.h" 

#define Camera_Buffer	0x24000000    // ����ͷͼ�񻺳���


/********************************************** �������� *******************************************/

void SystemClock_Config(void);		// ʱ�ӳ�ʼ��
void MPU_Config(void);					// MPU����

void TakePhoto();
void TakeVideo();

/***************************************************************************************************
*	�� �� ��: main
*
*	˵    ��: ������
*
****************************************************************************************************/

int main(void)
{
	MPU_Config();				// MPU����
	SCB_EnableICache();		// ʹ��ICache
	SCB_EnableDCache();		// ʹ��DCache
	HAL_Init();					// ��ʼ��HAL��
	SystemClock_Config();	// ����ϵͳʱ�ӣ���Ƶ480MHz
	LED_Init();					// ��ʼ��LED����
	USART1_Init();				// USART1��ʼ��	
	
 	SPI_LCD_Init();      	// Һ�����Լ�SPI��ʼ�� 
  key_init(); 
	DCMI_OV5640_Init();   			 	// DCMI�Լ�OV5640��ʼ��
  int mode = Camera;

	OV5640_AF_Download_Firmware();	// д���Զ��Խ��̼�
	OV5640_AF_Trigger_Constant();		// �Զ��Խ� ������ ��������OV5640��⵽��ǰ���治�ڽ���ʱ����һֱ�����Խ�
//	OV5640_AF_Trigger_Single();		//	�Զ��Խ� ������ ���� 
	
//	120�Ⱥ�160�ȵĹ�Ǿ�ͷĬ�ϵķ���ʹ��Զ��Խ��ľ�ͷ��һ�����û����Ը���ʵ��ȥ����
//	OV5640_Set_Vertical_Flip( OV5640_Disable );		// ȡ����ֱ��ת
//	OV5640_Set_Horizontal_Mirror( OV5640_Enable );	// ˮƽ����

	OV5640_DMA_Transmit_Continuous(Camera_Buffer, Display_BufferSize);  // ����DMA��������
  OV5640_Set_Pixformat(Pixformat_RGB565);						// ����ͼ�������ʽ
  char *dis = CameraFlag;
  int time = 0;
	while (1)
	{
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET) {
      // ��ȡ����
      HAL_Delay(20);
      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET){
        if (mode == Camera){
          printf("%d", Camera);
          HAL_Delay(1000);
        }else if (mode == Video){
          if(++time % 2 == 1){
            printf("%d", Video);
            dis = TakeFlag;
          }else{
            printf("%d", Video_Stop);
            dis = StopFlag;
          }
          HAL_Delay(1000);
        }
      }
    }else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET){
      // �л�ģʽ
      HAL_Delay(20);
      if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET){
        mode = (mode == Camera ? Video : Camera);
        // �л�Ϊ����ģʽ
        if(mode == Camera){
          dis = CameraFlag;
          printf("%d", Video_Stop);
        }
        // �л�Ϊ¼��ģʽ
        else dis = VideoFlag;
        HAL_Delay(1000);
      }
    }else{
      if ( OV5640_FrameState == 1 )	// �ɼ�����һ֡ͼ��
        {		
            OV5640_FrameState = 0;		// �����־λ
            LCD_CopyBuffer(0,0,Display_Width,Display_Height, (uint16_t *)Camera_Buffer);	// ��ͼ�����ݸ��Ƶ���Ļ
            LCD_DisplayString( 80 ,290, (char*) dis);
            // LCD_DisplayNumber( 132,264, OV5640_FPS,2) ;	// ��ʾ֡��	
            LED1_Toggle;	
        }	
    }
	}
}

/****************************************************************************************************/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 480000000 (CPU Clock)
  *            HCLK(Hz)                       = 240000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  120MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  120MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  120MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  120MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 192
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  
  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_SPI4;
  
	PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
	PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;

	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}      
  
}

void TakePhoto(){
	// ���ղ����浽SD��
	// 1. ����
	printf("%d", Camera);
}

//	����MPU
//
void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	HAL_MPU_Disable();		// �Ƚ�ֹMPU

	MPU_InitStruct.Enable 				= MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress 		= 0x24000000;
	MPU_InitStruct.Size 					= MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission 	= MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable 		= MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable 		= MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable 		= MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number 				= MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField 		= MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable 	= 0x00;
	MPU_InitStruct.DisableExec 		= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);	

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);	// ʹ��MPU
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
