#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "dht11.h"
#include "lcd.h"
#include "beep.h"
#include "dht11.h"
#include "fire.h"
#include "c2h5oh.h"
#include "motor_1.h"
#include "gizwits_product.h"

/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;

//WIFI����״̬
//wifi_sta 0: �Ͽ�
//         1: ������
u8 wifi_sta=1;

//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
  usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}

#define Temp_alarm  28   //�¶����ޣ���λ��C��
#define Humi_alarm  60   //ʪ�����ޣ���λ��%��
#define alarm_num 3  //��������

//���ݲɼ�
void userHandle(void)
{
  static u8 t=0;
	static u8 temp,hum;
  static u8 temp_num=0;
	static u8 humi_num=0;
	if(wifi_sta)
	{
		 if(t==10)//ÿ2S��ȡһ��
		 {
			 t=0;
			 
			 currentDataPoint.valuefire_sensor=PFin(0);
			 currentDataPoint.valueC2H5OH=PFin(2);
			 
			 DHT11_Read_Data(&temp,&hum);//��ȡDHT11������
			 LCD_ShowxNum(70,150+60+10,temp,3,16,0); 
			 LCD_ShowxNum(70,175+60+10,hum,3,16,0); 
			 currentDataPoint.valuetemp = temp ;//�¶�����
			 currentDataPoint.valuehumi = hum;//ʪ������ 
			 if(temp>=Temp_alarm)//�¶�>����ֵ 
			 {  
				 temp_num++;
				 if(temp_num>=alarm_num)//�����ﵽһ������
				 {
					 temp_num=0;
					 currentDataPoint.valuetemp_alert=1;//�¶ȴ����ޱ���
				   LCD_Fill(120,150+60+10,140,165+60+10,RED);
				 }else
         {
					 if(currentDataPoint.valuetemp_alert==0)
					 LCD_Fill(120,150+60+10,140,165+60+10,GREEN);
				 }  
				 
			 }else  
			 {
				 currentDataPoint.valuetemp_alert=0;
				 LCD_Fill(120,150+60+10,140,165+60+10,GREEN);
				 
			 }
			 if(hum>=Humi_alarm)//ʪ��>����ֵ
			 {
				 humi_num++;
				 if(humi_num>=alarm_num)//�����ﵽһ������
				 {
					 humi_num=0;
					 currentDataPoint.valuehumi_alert=1;//ʪ�ȴ����ޱ���
				   LCD_Fill(120,175+60+10,140,190+60+10,RED);
				 }
				 
			 }else
			 {
				 if(currentDataPoint.valuetemp_alert==0)
				 LCD_Fill(120,175+60+10,140,190+60+10,GREEN);
			 }
		if(!PFin(0))
		{
		PBout(8)=1;//����������
		delay_ms(500);
		PBout(8)=0;
		delay_ms(500);
		}
		if(!PFin(2))
		{
		PBout(8)=1;//����������
		delay_ms(500);
		PBout(8)=0;
		delay_ms(500);
		}
		}
		if(t%2)  LED0=!LED0;
			t++;
	}
	else
	{
		if(temp_num!=0||humi_num!=0) 
		{
			temp_num=0;humi_num=0;
		}
	}
}
//������
 int main(void)
 {
  int key;
	
	u8 wifi_con=0;//��¼wifi����״̬ 1:���� 0:�Ͽ�
	u8 buff[20]={0};
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	    //���ڳ�ʼ��Ϊ115200
	
	delay_init();	    	 //��ʱ������ʼ��
 	LED_Init();			    //LED�˿ڳ�ʼ��
	KEY_Init();             //������ʼ��  
	LCD_Init();			   	//��ʼ��LCD 
	fire_init();			
	c2h5oh_init();			
	Motor_12_Config();			
	BEEP_init();			
	Gizwits_Init();         //Э���ʼ��
	
	POINT_COLOR=RED;		//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"STM32F103ZE");	
	LCD_ShowString(30,90,200,16,16,"DOME test");	
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2023/3/3");
	POINT_COLOR=BLUE;		//��������Ϊ��ɫ 
	while(DHT11_Init())	//DHT11��ʼ��	
	{
		LCD_ShowString(30,130,200,16,16,"chip Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"chip OK");
	POINT_COLOR=BLUE;		//��������Ϊ��ɫ
	LCD_ShowString(30,150,200,16,16,"wifi OK");
  sprintf((char*)buff,"Temp alarm: %dC",Temp_alarm);
	LCD_ShowString(30,170,200,16,16,buff);
	memset(buff,0x00,20);
	sprintf((char*)buff,"Humi alarm: %d%%",Humi_alarm);
	LCD_ShowString(30,190,200,16,16,buff);
	POINT_COLOR=BLUE;		//��������Ϊ��ɫ
	LCD_ShowString(30,220,200,16,16,"Temp:  0C");
  LCD_ShowString(30,245,200,16,16,"Humi:  0%");	
	printf("--------������IOT-DHT11��ʪ�ȼ�ⱨ��ʵ��----------\r\n");
	printf("KEY1:AirLink����ģʽ\t KEY_UP:��λ\r\n\r\n");

	while(1)
	{
	  
	  userHandle();//�û��ɼ�
    gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
	  key = KEY_Scan(0);
		
		if(key==KEY1_PRES)//KEY1����
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
			LCD_Fill(120,150+60+10,140,165+60+10,WHITE);
			LCD_Fill(120,175+60+10,140,190+60+10,WHITE);
			POINT_COLOR=BLUE;		//��������Ϊ��ɫ 
			LCD_ShowxNum(70,150+60+10,0,3,16,0); 
			LCD_ShowxNum(70,175+60+10,0,3,16,0); 
			wifi_sta=0;//��־wifi�ѶϿ�
			LED0=1;
		}

		if(PFin(5)==0|PFin(6)==0)
		{
			//Motor_1_PRun();
			Motor_2_NRun();
		}
		else
		{
			//Motor_1_STOP();
			Motor_2_STOP();
		}
		delay_ms(200);
		LED1=!LED1;
	}
}

