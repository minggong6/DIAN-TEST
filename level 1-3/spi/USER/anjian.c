#include "stm32f10x.h"                  // Device header
#include "delay.h"
 void key_init(void)
 {
	 GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_0;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStruct);
 }	
 
 
 uint8_t ket_getnum(void)
 {
	 uint8_t keynum=0;
	   if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0) {
                // Device header
      delay_ms(20);  // 延时消抖
       while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0) {
		   keynum=1;
           // 确认按键按下
       }
   }
	   
 
	   return keynum;

 }
 
  uint8_t key_getnumB0(void)
 {
	 uint8_t keynum=0;
	   if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
                // Device header
      delay_ms(20);  // 延时消抖
       while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
		   keynum=1;
           // 确认按键按下
       }
   }
	   
 
	   return keynum;

 }
 