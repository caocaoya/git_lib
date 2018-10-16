#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"


CAN_HandleTypeDef   CAN1_Handler;   //CAN1���
CAN_HandleTypeDef   CAN2_Handler;

CanTxMsgTypeDef     TxMessage1,TxMessage2;      //������Ϣ
CanRxMsgTypeDef     RxMessage1,RxMessage2;      //������Ϣ

////CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+tbs2+1)*brp); ����tbs1��tbs2����ֻ�ù�ע��ʶ���ϱ�־����ţ�����CAN_BS2_1TQ�����Ǿ���Ϊtbs2=1�����㼴�ɡ�
//mode:CAN_MODE_NORMAL,��ͨģʽ;CAN_MODE_LOOPBACK,�ػ�ģʽ;
//������Ϊ:36M/((8+9+1)*4)=500Kbps

u8 CAN1_Mode_Init(CAN_TypeDef *canx, u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance = canx; 
	
    CAN1_Handler.Init.Prescaler=brp;    //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN1_Handler.Init.Mode=mode;        //ģʽ���� 
    CAN1_Handler.Init.SJW=tsjw;         //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1=tbs1;         //tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2=tbs2;         //tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM=DISABLE;     //��ʱ�䴥��ͨ��ģʽ 
    CAN1_Handler.Init.ABOM=DISABLE;     //����Զ����߹���
    CAN1_Handler.Init.AWUM=DISABLE;     //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN1_Handler.Init.NART=DISABLE;      //��ֹ�����Զ����� 
    CAN1_Handler.Init.RFLM=DISABLE;     //���Ĳ�����,�µĸ��Ǿɵ� 
    CAN1_Handler.Init.TXFP=DISABLE;     //���ȼ��ɱ��ı�ʶ������ 
	  
	  if(canx == CAN1)
		{
			CAN1_Handler.pTxMsg=&TxMessage1;     //������Ϣ
			CAN1_Handler.pRxMsg=&RxMessage1;     //������Ϣ
		}
		else if(canx == CAN2)
		{
			CAN1_Handler.pTxMsg=&TxMessage2;     //������Ϣ
			CAN1_Handler.pRxMsg=&RxMessage2;     //������Ϣ			
		}
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK) 
		{
		   return 1;   //��ʼ��
		}
    
		CAN1_FilerConf.FilterNumber=0;          //������0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;		
    CAN1_FilerConf.FilterIdHigh=0X0000;     //32λID
    CAN1_FilerConf.FilterIdLow=0X0000;
    CAN1_FilerConf.FilterMaskIdHigh=0X0000; //32λMASK
    CAN1_FilerConf.FilterMaskIdLow=0X0000;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//������0������FIFO0
		
    CAN1_FilerConf.FilterActivation=ENABLE; //�����˲���0
    CAN1_FilerConf.BankNumber=14;
	
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK) 
		{
			return 2;//�˲�����ʼ��
		}
    return 0;
}

//CAN�ײ��������������ã�ʱ�����ã��ж�����
//�˺����ᱻHAL_CAN_Init()����
//hcan:CAN���
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
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;        //���츴��
		GPIO_Initure.Pull=GPIO_PULLUP;            //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //����
		HAL_GPIO_Init(CAN1_PART,&GPIO_Initure);       //��ʼ��

		GPIO_Initure.Pin=CAN1_RX_PIN;   			    //PA11
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;     //���츴��
		HAL_GPIO_Init(CAN1_PART,&GPIO_Initure);       //��ʼ��
				
#if CAN1_RX0_INT_ENABLE
		__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);	//FIFO0��Ϣ�����ж�����.	  
		HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 6, 0);
		HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
#endif			
	}
	if(hcan->Instance == CAN2)
	{
		CAN2_GPIO_CLK();
		CAN2_CLK_ENABLE();

		GPIO_Initure.Pin=CAN2_TX_PIN;   			    //PA12
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;        //���츴��
		GPIO_Initure.Pull=GPIO_PULLUP;            //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //����
		HAL_GPIO_Init(CAN2_PART,&GPIO_Initure);       //��ʼ��

		GPIO_Initure.Pin=CAN2_RX_PIN;   			    //PA11
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;     //���츴��
		HAL_GPIO_Init(CAN2_PART,&GPIO_Initure);       //��ʼ��

#if CAN2_RX0_INT_ENABLE
		__HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);	//FIFO0��Ϣ�����ж�����.	  
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
#if CAN1_RX0_INT_ENABLE                         	//ʹ��RX0�ж�

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    int i=0;
		if(hcan->Instance == CAN1)
		{
			__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//���¿���FIF00��Ϣ�Һ��ж�
			HAL_CAN_Receive_IT(hcan,CAN_FIFO0);
		}
		if(hcan->Instance == CAN2)
		{
			__HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);//���¿���FIF00��Ϣ�Һ��ж�
			HAL_CAN_Receive_IT(hcan,CAN_FIFO0);	
		}
}
//CAN�жϷ�����
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN1_Handler);//�˺��������CAN_Receive_IT()��������
}
#endif	

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
	u16 i=0;
	
  CAN1_Handler.pTxMsg=&TxMessage1;     //������Ϣ
  CAN1_Handler.pRxMsg=&RxMessage1;     //������Ϣ	
	
	CAN1_Handler.pTxMsg->StdId=0X12;        //��׼��ʶ��
	CAN1_Handler.pTxMsg->ExtId=0x12;        //��չ��ʶ��(29λ)
	CAN1_Handler.pTxMsg->IDE=CAN_ID_STD;    //ʹ�ñ�׼֡
	CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;  //����֡
	CAN1_Handler.pTxMsg->DLC=len;                
	for(i=0;i<len;i++)
	{
		CAN1_Handler.pTxMsg->Data[i]=msg[i];
	}
	if(HAL_CAN_Transmit(&CAN1_Handler,100)!=HAL_OK) 
	{
		return 1;     //����
	}
  return 0;		
	
}

u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	if(HAL_CAN_Receive_IT(&CAN1_Handler,CAN_FIFO0)!=HAL_OK) 
	{
		return 0;//�������ݣ���ʱʱ������Ϊ0	
	}
	for(i=0;i<CAN1_Handler.pRxMsg->DLC;i++)
	{
		buf[i]=CAN1_Handler.pRxMsg->Data[i];
	}  
	return CAN1_Handler.pRxMsg->DLC;	
}
