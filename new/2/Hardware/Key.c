#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define LONG_PRESS_TIME 1000            
#define CONTINUOUS_PRESS_INTERVAL 100   
#define DOUBLE_CLICK_TIME 300           

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
    
    GPIO_InitTypeDef GPIO_InitStructure;                    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);                  
}

uint8_t Key_Scan(void)
{
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
    {
        return 1;
    }
    else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
    {
        return 2;
    }
    else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
    {
        return 3;
    }
    
    return 0;
}

uint8_t Key_GetNum(void)
{
    static uint32_t PressTime = 0;       
    static uint32_t LastReleaseTime = 0; 
    static uint8_t LastKeyNum = 0;       
    uint8_t KeyNum = 0;                  
    static uint32_t CurrentTime = 0;     
    
    KeyNum = Key_Scan();
    if (KeyNum != 0)
    {
        while(Key_Scan() == KeyNum)
        {
            Delay_ms(10);
            PressTime += 10;
            CurrentTime += 10;
            
            if (PressTime >= LONG_PRESS_TIME)
            {
                while (Key_Scan() == KeyNum)
                {
                    Delay_ms(CONTINUOUS_PRESS_INTERVAL);
                    return KeyNum;
                }
            }
        }
        
        if (PressTime >= 20 && PressTime < LONG_PRESS_TIME)
        {
            if ((LastKeyNum == KeyNum) && ((CurrentTime - LastReleaseTime) < DOUBLE_CLICK_TIME))
            {
                LastKeyNum = 0;         
                LastReleaseTime = 0;     
                PressTime = 0;           
                return KeyNum + 10;      
            }
            else
            {
                LastKeyNum = KeyNum;     
                LastReleaseTime = CurrentTime; 
                PressTime = 0;           
                return KeyNum;           
            }
        }
    }
    else
    {
        PressTime = 0;                   
        CurrentTime += 10;              
    }
    
    return 0;
}
