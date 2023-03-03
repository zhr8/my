#include "motor_1.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
 
void Motor_12_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5);
}

void Motor_1_STOP(void)
{
	IN1(High);
	IN2(High);
}
 
void Motor_1_PRun(void)
{
	IN1(Low);
	IN2(High);
}
 
void Motor_1_NRun(void)
{
	IN1(High);
	IN2(Low);
}
 
void Motor_2_STOP(void)
{
	IN3(High);
	IN4(High);
}
 
void Motor_2_PRun(void)
{
	IN3(Low);
	IN4(High);
}
 
void Motor_2_NRun(void)
{
	IN3(High);
	IN4(Low);
}
