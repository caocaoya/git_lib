#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"


CAN_HandleTypeDef   CAN1_Handler;   //CAN1句柄
CAN_HandleTypeDef   CAN2_Handler;

CanTxMsgTypeDef     TxMessage1,TxMessage2;      //发送消息
CanRxMsgTypeDef     RxMessage1,RxMessage2;      //接收消息

////CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+tbs2+1)*brp); 其中tbs1和tbs2我们只用关注标识符上标志的序号，例如CAN_BS2_1TQ，我们就认为tbs2=1来计算即可。
//mode:CAN_MODE_NORMAL,普通模式;CAN_MODE_LOOPBACK,回环模式;
//则波特率为:36M/((8+9+1)*4)=500Kbps

u8 CAN1_Mode_Init(CAN_TypeDef *canx, u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance = canx; 
	
    CAN1_Handler.Init.Prescaler=brp;    //分频系数(Fdiv)为brp+1
    CAN1_Handler.Init.Mode=mode;        //模式设置 
    CAN1_Handler.Init.SJW=tsjw;         //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1=tbs1;         //tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2=tbs2;         //tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM=DISABLE;     //非时间触发通信模式 
    CAN1_Handler.Init.ABOM=DISABLE;     //软件自动离线管理
    CAN1_Handler.Init.AWUM=DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN1_Handler.Init.NART=DISABLE;      //禁止报文自动传送 
    CAN1_Handler.Init.RFLM=DISABLE;     //报文不锁定,新的覆盖旧的 
    CAN1_Handler.Init.TXFP=DISABLE;     //优先级由报文标识符决定 
	  
	  if(canx == CAN1)
		{
			CAN1_Handler.pTxMsg=&TxMessage1;     //发送消息
			CAN1_Handler.pRxMsg=&RxMessage1;     //接收消息
		}
		else if(canx == CAN2)
		{
			CAN1_Handler.pTxMsg=&TxMessage2;     //发送消息
			CAN1_Handler.pRxMsg=&RxMessage2;     //接收消息			
		}
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK) 
		{
		   return 1;   //初始化
		}
    
		CAN1_FilerConf.FilterNumber=0;          //过滤器0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;		
    CAN1_FilerConf.FilterIdHigh=0X0000;     //32位ID
    CAN1_FilerConf.FilterIdLow=0X0000;
    CAN1_FilerConf.FilterMaskIdHigh=0X0000; //32位MASK
    CAN1_FilerConf.FilterMaskIdLow=0X0000;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器0关联到FIFO0
		
    CAN1_FilerConf.FilterActivation=ENABLE; //激活滤波器0
    CAN1_FilerConf.BankNumber=14;
	
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK) 
		{
			return 2;//滤波器初始化
		}
    return 0;
}

//CAN底层驱动，引脚配置，时钟配置，中断配置
//此函数会被HAL_CAN_Init()调用
//hcan:CAN句柄
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
	GPIO_InitTypeDef GPIO_Initure;
	if(hcan->Instance == CAN1)
	{
		CAN1_GPIO_CLK();
		CAN1_CLK_ENABLE();
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_CAN1_1();

		GPIO_Initure.Pin=CAN1_TX_PIN;   			    //PA12
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;        //推挽复用
		GPIO_Initure.Pull=GPIO_PULLUP;            //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速
		HAL_GPIO_Init(CAN1_PART,&GPIO_Initure);       //初始化

		GPIO_Initure.Pin=CAN1_RX_PIN;   			    //PA11
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;     //推挽复用
		HAL_GPIO_Init(CAN1_PART,&GPIO_Initure);       //初始化
				
#if CAN1_RX0_INT_ENABLE
		__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);	//FIFO0消息挂起中断允许.	  
		HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 6, 0);
		HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
#endif			
	}
	if(hcan->Instance == CAN2)
	{
		CAN2_GPIO_CLK();
		CAN2_CLK_ENABLE();

		GPIO_Initure.Pin=CAN2_TX_PIN;   			    //PA12
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;        //推挽复用
		GPIO_Initure.Pull=GPIO_PULLUP;            //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速
		HAL_GPIO_Init(CAN2_PART,&GPIO_Initure);       //初始化

		GPIO_Initure.Pin=CAN2_RX_PIN;   			    //PA11
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;     //推挽复用
		HAL_GPIO_Init(CAN2_PART,&GPIO_Initure);       //初始化

#if CAN2_RX0_INT_ENABLE
		__HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);	//FIFO0消息挂起中断允许.	  
		HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
#endif						
	}
}
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{

  if(hcan->Instance==CAN1)
  {
    __HAL_RCC_CAN1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
  }
  else if(hcan->Instance==CAN2)
  {
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
  }

}
#if CAN1_RX0_INT_ENABLE                         	//使能RX0中断

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    int i=0;
		if(hcan->Instance == CAN1)
		{
			__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//重新开启FIF00消息挂号中断
			HAL_CAN_Receive_IT(hcan,CAN_FIFO0);
		}
		if(hcan->Instance == CAN2)
		{
			__HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);//重新开启FIF00消息挂号中断
			HAL_CAN_Receive_IT(hcan,CAN_FIFO0);	
		}
}
//CAN中断服务函数
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN1_Handler);//此函数会调用CAN_Receive_IT()接收数据
}
#endif	

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
	u16 i=0;
	
  CAN1_Handler.pTxMsg=&TxMessage1;     //发送消息
  CAN1_Handler.pRxMsg=&RxMessage1;     //接收消息	
	
	CAN1_Handler.pTxMsg->StdId=0X12;        //标准标识符
	CAN1_Handler.pTxMsg->ExtId=0x12;        //扩展标识符(29位)
	CAN1_Handler.pTxMsg->IDE=CAN_ID_STD;    //使用标准帧
	CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;  //数据帧
	CAN1_Handler.pTxMsg->DLC=len;                
	for(i=0;i<len;i++)
	{
		CAN1_Handler.pTxMsg->Data[i]=msg[i];
	}
	if(HAL_CAN_Transmit(&CAN1_Handler,100)!=HAL_OK) 
	{
		return 1;     //发送
	}
  return 0;		
	
}

u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	if(HAL_CAN_Receive_IT(&CAN1_Handler,CAN_FIFO0)!=HAL_OK) 
	{
		return 0;//接收数据，超时时间设置为0	
	}
	for(i=0;i<CAN1_Handler.pRxMsg->DLC;i++)
	{
		buf[i]=CAN1_Handler.pRxMsg->Data[i];
	}  
	return CAN1_Handler.pRxMsg->DLC;	
}
