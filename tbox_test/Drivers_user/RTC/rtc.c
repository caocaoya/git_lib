#include "rtc.h"
#include "delay.h"
#include "led.h"

RTC_HandleTypeDef RTC_Handler;  //RTC���

//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:@RTC_AM_PM_Definitions:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
HAL_StatusTypeDef RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_TimeStructure.Hours=hour;
	RTC_TimeStructure.Minutes=min;
	RTC_TimeStructure.Seconds=sec;
//	RTC_TimeStructure.TimeFormat=ampm;
//	RTC_TimeStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
//  RTC_TimeStructure.StoreOperation=RTC_STOREOPERATION_RESET;
	return HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);	
}

//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef RTC_DateStructure;
    
	RTC_DateStructure.Date=date;
	RTC_DateStructure.Month=month;
	RTC_DateStructure.WeekDay=week;
	RTC_DateStructure.Year=year;
	return HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
}

//RTC��ʼ��
//����ֵ:0,��ʼ���ɹ�;
//       2,�����ʼ��ģʽʧ��;
u8 RTC_Init(void)
{      

//	
//	  RTC_Handler.Instance=RTC;
//    RTC_Handler.Init.HourFormat=RTC_HOURFORMAT_24;//RTC����Ϊ24Сʱ��ʽ 
//    RTC_Handler.Init.AsynchPrediv=0X7F;           //RTC�첽��Ƶϵ��(1~0X7F)
//    RTC_Handler.Init.SynchPrediv=0XFF;            //RTCͬ����Ƶϵ��(0~7FFF)   
//    RTC_Handler.Init.OutPut=RTC_OUTPUT_DISABLE;     
//    RTC_Handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
//    RTC_Handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
//    if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK) return 2;
//      
//    if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5050)//�Ƿ��һ������
//    { 
//        RTC_Set_Time(23,59,56,RTC_HOURFORMAT12_PM);	        //����ʱ�� ,����ʵ��ʱ���޸�
//		RTC_Set_Date(15,12,27,7);		                    //��������
//        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050);//����Ѿ���ʼ������
//    }
//    return 0;
}

/*

void BSP_RTC_Init(void)
{
  uint8_t data_set[6] = {0x18,0x01,0x01,0,0,0};
  
  Time_RtcHandle.Instance = RTC; 
  Time_RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  Time_RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  Time_RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  Time_RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  Time_RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  Time_RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if(HAL_RTC_Init(&Time_RtcHandle) != HAL_OK)
  {
    test_falg = 9;
    Error_Handler(); 
  }
  RTC_CalendarConfig(data_set[0], data_set[1], data_set[2],data_set[3],data_set[4], data_set[5]);
  
}
*/

//RTC�ײ�������ʱ������
//�˺����ᱻHAL_RTC_Init()����
//hrtc:RTC���
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();//ʹ�ܵ�Դʱ��PWR
	  HAL_PWR_EnableBkUpAccess();//ȡ����������д����
    
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE����
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTCʹ��LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//����ΪRTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTCʱ��ԴΪLSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        
    __HAL_RCC_RTC_ENABLE();//RTCʱ��ʹ��
}

//��������ʱ��(����������,24Сʱ��)
//week:���ڼ�(1~7) @ref  RTC_WeekDay_Definitions
//hour,min,sec:Сʱ,����,����
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //Сʱ
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //����
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //��
//    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
//    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
//    
//    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//��ȷƥ�����ڣ�ʱ����
//    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
//    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//������
//    RTC_AlarmSturuct.AlarmDateWeekDay=week; //����
//    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //����A
//    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

//�����Ի��Ѷ�ʱ������  
/*wksel:  @ref RTCEx_Wakeup_Timer_Definitions
#define RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
#define RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
#define RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)
*/
//cnt:�Զ���װ��ֵ.����0,�����ж�.
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
//    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);//���RTC WAKE UP�ı�־
//	
//	HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel);            //������װ��ֵ��ʱ�� 
//	
//    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //��ռ���ȼ�1,�����ȼ�2
//    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
}
    
//RTC����A�жϴ���ص�����
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    printf("ALARM A!\r\n");
}

//RTC WAKE UP�жϷ�����
void RTC_WKUP_IRQHandler(void)
{
//    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 
}

//RTC WAKE UP�жϴ���
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    LED1=!LED1;
}
/*

/*       
static void RTC_CalendarConfig(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  //##-1- Configure the Date 
  // Set Date: Monday May 30th 2014 
  sdatestructure.Year = year;
  sdatestructure.Month = month;
  sdatestructure.Date = day;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if(HAL_RTC_SetDate(&Time_RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    test_falg =15;
    Error_Handler(); 
  } 
  
  //##-2- Configure the Time 
  //Set Time: 02:00:00 
  stimestructure.Hours = hour;
  stimestructure.Minutes = min;
  stimestructure.Seconds = sec;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&Time_RtcHandle,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    test_falg =16;
    Error_Handler(); 
  }
  
  //##-3- Writes a data in a RTC Backup data Register0 
  HAL_RTCEx_BKUPWrite(&Time_RtcHandle,RTC_BKP_DR0,0x32F2);  
}



static void RTC_CalendarShow(uint8_t* showtime)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  
  //Get the RTC current Time 
  HAL_RTC_GetTime(&Time_RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  //Get the RTC current Date 
  HAL_RTC_GetDate(&Time_RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  // Get the RTC current Time 
  HAL_RTC_GetTime(&Time_RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  // Get the RTC current Date 
  HAL_RTC_GetDate(&Time_RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);  

  // Display time Format : hh:mm:ss 
//  if(Flag_Sys_State & SERVER_AGREEMENT_FLAG)
  {
    sprintf((char*)showtime,"%04d-%02d-%02d%c%02d:%02d:%02d",(2000 + sdatestructureget.Year), sdatestructureget.Month, sdatestructureget.Date,0x20,stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  
    if(stimestructureget.Hours == 0 ||  stimestructureget.Hours == 1)
    {
      Flag_Sys_State |= MCU_UPDATE_TIME_FLAG;
    }
    else
    {
      Flag_Sys_State &= (~MCU_UPDATE_TIME_FLAG);
    }
  }
//  else
//  {
//    sprintf((char*)showtime,"%04d-%02d-%02dT%02d:%02d:%02d+8:00",(2000 + sdatestructureget.Year), sdatestructureget.Month, sdatestructureget.Date, stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
//
//  }
   
}  

*/
