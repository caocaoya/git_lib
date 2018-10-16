
#include "sys.h"

//时钟系统配置函数
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
//    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟
//    
//    //下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
//    //时使性能与功耗实现平衡，此功能只有STM32F42xx和STM32F43xx器件有，
//    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别1
//    
//    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
//    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
//    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//打开PLL
//    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLL时钟源选择HSE
//    RCC_OscInitStructure.PLL.PLLM=pllm; //主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//    RCC_OscInitStructure.PLL.PLLN=plln; //主PLL倍频系数(PLL倍频),取值范围:64~432.  
//    RCC_OscInitStructure.PLL.PLLP=pllp; //系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
//    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
//	
//    if(ret!=HAL_OK) while(1);
//    
//    ret=HAL_PWREx_EnableOverDrive(); //开启Over-Driver功能
//    if(ret!=HAL_OK) while(1);
//    
//    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
//    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
//    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
//    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB分频系数为1
//    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1分频系数为4
//    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2分频系数为2
//    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//同时设置FLASH延时周期为5WS，也就是6个CPU周期。
//		
//    if(ret!=HAL_OK) while(1);
}

#ifdef  USE_FULL_ASSERT
//当编译提示出错的时候此函数用来报告错误的文件和所在行
//file：指向源文件
//line：指向在文件中的行数
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
