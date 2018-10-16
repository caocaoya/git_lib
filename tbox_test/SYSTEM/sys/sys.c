
#include "sys.h"

//ʱ��ϵͳ���ú���
void Stm32_Clock_Init(void)
{
	
	RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Configure PLLs ------------------------------------------------------*/
  /* PLL2 configuration: PLL2CLK = (HSE / HSEPrediv2Value) * PLL2MUL = (25 / 5) * 8 = 40 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLL2CLK / HSEPredivValue = 40 / 5 = 8 MHz */
  /* PLL configuration: PLLCLK = PREDIV1CLK * PLLMUL = 8 * 9 = 72 MHz */ 

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState              = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue        = RCC_HSE_PREDIV_DIV5;
  oscinitstruct.Prediv1Source         = RCC_PREDIV1_SOURCE_PLL2;
  oscinitstruct.PLL.PLLState          = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL            = RCC_PLL_MUL9;
  oscinitstruct.PLL2.PLL2State        = RCC_PLL2_ON;
  oscinitstruct.PLL2.PLL2MUL          = RCC_PLL2_MUL8;
  oscinitstruct.PLL2.HSEPrediv2Value  = RCC_HSE_PREDIV2_DIV5;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
////////////////////////////////////////////////////////////////////////////////////////
////////////  RCC_OscInitTypeDef RCC_OscInitStruct;
////////////  RCC_ClkInitTypeDef RCC_ClkInitStruct;

////////////    /**Initializes the CPU, AHB and APB busses clocks 
////////////    */
////////////  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
////////////  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
////////////  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
////////////  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
////////////  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
////////////  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
////////////  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
////////////  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
////////////  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL2_ON;
////////////  RCC_OscInitStruct.PLL2.PLL2MUL = RCC_PLL2_MUL8;
////////////  RCC_OscInitStruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
////////////  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
////////////  {
//////////////    _Error_Handler(__FILE__, __LINE__);
////////////  }

////////////    /**Initializes the CPU, AHB and APB busses clocks 
////////////    */
////////////  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
////////////  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
////////////  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
////////////  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
////////////  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

////////////  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
////////////  {
//////////////    _Error_Handler(__FILE__, __LINE__);
////////////  }

//////////////  __HAL_RCC_PLLI2S_ENABLE();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
//    HAL_StatusTypeDef ret = HAL_OK;
//    RCC_OscInitTypeDef RCC_OscInitStructure; 
//    RCC_ClkInitTypeDef RCC_ClkInitStructure;
//    
//    __HAL_RCC_PWR_CLK_ENABLE(); //ʹ��PWRʱ��
//    
//    //������������������õ�ѹ�������ѹ�����Ա�������δ�����Ƶ�ʹ���
//    //ʱʹ�����빦��ʵ��ƽ�⣬�˹���ֻ��STM32F42xx��STM32F43xx�����У�
//    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//���õ�ѹ�������ѹ����1
//    
//    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //ʱ��ԴΪHSE
//    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //��HSE
//    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//��PLL
//    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLLʱ��Դѡ��HSE
//    RCC_OscInitStructure.PLL.PLLM=pllm; //��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
//    RCC_OscInitStructure.PLL.PLLN=plln; //��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.  
//    RCC_OscInitStructure.PLL.PLLP=pllp; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
//    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.
//    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
//	
//    if(ret!=HAL_OK) while(1);
//    
//    ret=HAL_PWREx_EnableOverDrive(); //����Over-Driver����
//    if(ret!=HAL_OK) while(1);
//    
//    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
//    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
//    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
//    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB��Ƶϵ��Ϊ1
//    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1��Ƶϵ��Ϊ4
//    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2��Ƶϵ��Ϊ2
//    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//ͬʱ����FLASH��ʱ����Ϊ5WS��Ҳ����6��CPU���ڡ�
//		
//    if(ret!=HAL_OK) while(1);
}

#ifdef  USE_FULL_ASSERT
//��������ʾ�����ʱ��˺����������������ļ���������
//file��ָ��Դ�ļ�
//line��ָ�����ļ��е�����
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
