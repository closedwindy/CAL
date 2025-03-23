    #include "stm32f10x.h"
    #include "OLED.h"
    #include "Encoder.h"
    #include "Key.h"
    #include "CAL_math.h"
 
    int16_t Num;                
    uint8_t KeyNum;             
    uint8_t Count = 1;       
    char expr[100] = {0};    
    int expr_pos = 0;           
    uint8_t line = 1; 
    uint8_t _Float_flag=0;


    
    const char allowed_chars[] = {
        '0','1','2','3','4','5','6','7','8','9',
        '+','-','*','/','(',')','=','.'
    };
    const double allowed_interachars[] = {
        1,2,3
    };
   



    int main(void) {
        OLED_Init();     
        Encoder_Init();   
        Key_Init();         
        OLED_Menu();      

        while (1) {
            KeyNum = Key_GetNum();      
            Num += Encoder_Get();          
            
        
            if (Num < 0) Num = sizeof(allowed_chars)-1;
            if (Num >= sizeof(allowed_chars)) Num = 0;

        
            OLED_ShowChar(line, Count % 21, allowed_chars[Num]);

            if (KeyNum == 3) { 
                if (allowed_chars[Num] == '=') {
                    double res = result(expr);
                    if(_Float_flag)
                    {
                    OLED_ShowFloatNum(line+1,1 , res, 2);
                    }  
                    else
                    {
                    OLED_ShowSignedNum(line+1,1 , res);
                    }         
                    
                } else {
                    OLED_Clear_Line(line+2);
                    Get_Expression(allowed_chars[Num]);
                    if(allowed_chars[Num]=='.'||'/')
                    {
                        _Float_flag=1;
                    }
                    Count++;
                    if(Count==16)
                    {
                        Count =1;
                    
                        line++;
                    if(line==3){
                        
                        
                       
                    }
                    }
                }
            }
        }
    }