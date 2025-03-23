    #include "stm32f10x.h"
    #include "OLED.h"
    #include "Encoder.h"
    #include "Key.h"
    #include "CAL_math.h"

uint8_t KeyNum_A;             

int mode=0;
  
    const double allowed_interachars[] = {
        1,2,3
    };
   



    int main(void) {
        OLED_Init();     
        Encoder_Init();   
        Key_Init();
         

        while(1)
        {
            KeyNum_A = Key_GetNum();     
        OLED_ShowString(1,2,"Select mode:");
        OLED_ShowString(2,1,"Key1:Basical");
        OLED_ShowString(3,1,"Key2:Interal");
        if(KeyNum_A == 3)
        {
            OLED_Clear();
            mode =1;
            break;
        }
        if(KeyNum_A == 2)
        {
            
            mode =2;
            break;
        }
        }
        
        
        
        
        while (1) {
        
switch (mode)
{
case 1: BASIC_CAL();  
    break;
case 2: Interal_CAL();

    break;
}
       
        }
    }