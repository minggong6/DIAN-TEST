
//              GND   电源地
//              VCC   3.3v电源
//              SCL   PA0（SCLK）
//              SDA   PA1（MOSI）
//              RES   PA2
//              DC    PA3
//              CS    PA4
//              BLK   PA6

#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "anjian.h"
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_it.h"
#include "stm32f10x.h"          // 核心头文件
#include "stm32f10x_gpio.h"     // GPIO外设
#include "stm32f10x_tim.h"      // 定时器外设
#include "stm32f10x_rcc.h"      // 时钟配置

volatile uint32_t system_tick = 0;
GPIO_InitTypeDef GPIO_InitStructure;
uint8_t keynum=0;
int t=0;
int value=0;
#define DOUBLE_CLICK_TIME  50
#include "stm32f10x.h"

void PWM_Init() {
    // 1. 启用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 2. 配置GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. 配置定时器基础参数
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_TimeBaseStruct.TIM_Period = 999;
    TIM_TimeBaseStruct.TIM_Prescaler = 500;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);

    // 4. 配置PWM模式
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = value;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);

    // 5. 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}





// SysTick初始化（1ms中断）
void SysTick_Init(void) {
    SysTick_Config(SystemCoreClock / 1000); // 72MHz下1ms中断
}

// SysTick中断服务函数
/*void SysTick_Handler(void) {
    system_tick++;
}*/

// 获取当前时间
uint32_t GetTick(void) {
    return system_tick;
}

// 按键检测函数
uint8_t Key_GetNum(void) {
    return !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13); // 按下返回1
}

// 双击检测逻辑
void Check_DoubleClick(void) {
    static uint32_t first_press_time = 0;
    static uint8_t click_state = 0; // 0: 空闲, 1: 等待第二次点击
    static uint8_t last_key_state = 0;
    uint8_t current_key_state = Key_GetNum();

    // 消抖：检测稳定按下状态（持续20ms）
    if (current_key_state != last_key_state) {
        delay_ms(20);  // 等待抖动结束
        current_key_state = Key_GetNum();  // 重新读取状态
    }

    // 检测按下边沿（从0到1）
    if (current_key_state && !last_key_state) {
        uint32_t current_time = GetTick();
        if (click_state == 0) { // 首次按下
            first_press_time = current_time;
            click_state = 1;
        } else if (click_state == 1) { // 第二次按下
            if (current_time - first_press_time < DOUBLE_CLICK_TIME) {
               // t--; // 双击：字符串上移
                click_state = 0;
            } else {
                first_press_time = current_time; // 重置时间
            }
        }
    }

    // 处理超时单击（500ms内无第二次按下）
    if (click_state == 1 && (GetTick() - first_press_time >= DOUBLE_CLICK_TIME)) {
        t+=10; // 单击：字符串下移
        click_state = 0;
    }

    last_key_state = current_key_state;
}



int main(void) {
    delay_init();
    key_init();
    LED_Init();
    LCD_Init();
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    LED = 0;
    while (1) {
        Check_DoubleClick(); // 更新t值
	//	value++;
		PWM_Init();
		if(key_getnumB0()==1)
		{value+=10;
		}
		if(value==999)
		{value=0;
		}
		if(ket_getnum())
		{
		t++;}
       LCD_ShowString(0, t, "HELLO WORLD", BLACK, WHITE, 16, 0);
		LCD_ShowPicture(0,0,80,80,gImage_b);
LCD_ShowPicture(100,20,40,40,gImage_1);
		
    }
}
