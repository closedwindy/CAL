    #include "stm32f10x.h"
    #include "Delay.h"
    #include "OLED.h"
    #include "Encoder.h"
    #include "Key.h"
    #include "math.h"
    #include "ctype.h"
    #include "stdlib.h"
    #include "stdio.h"
    #include "string.h"
 
    int16_t Num;                
    uint8_t KeyNum;             
    uint8_t Count = 1;       
    char expr[100] = {0};    
    int expr_pos = 0;           
    uint8_t line = 1; 

    const char allowed_chars[] = {
        '0','1','2','3','4','5','6','7','8','9',
        '+','-','*','/','(',')','=','.'
    };
    const double allowed_interachars[] = {
        1,2,3
    };
    double cal(const char** p_ptr);
    void Get_Expression(char ch);

    double result(const char* expr) {
        const char* p = expr;
        return cal(&p);
    }


    double cal(const char** p_ptr) {
        const char* p = *p_ptr;
        double nums[50] = {0};
        int nums_size = 0;
        char ops[50] = {0};
        int ops_size = 0;

        while (*p && *p != ')') {
            if (isdigit(*p) || *p == '.') {
                char* end;
                nums[nums_size++] = strtod(p, &end);
                p = end;
            } else if (*p == '(') {
                p++; // 跳过 '('
                nums[nums_size++] = cal(&p); // 递归处理括号
            } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
                ops[ops_size++] = *p;
                p++;
            } else {
                p++;
            }
        }

    
        int i = 0;
        while (i < ops_size) {
            if (ops[i] == '*' || ops[i] == '/') {
                double a = nums[i], b = nums[i+1];
                nums[i] = (ops[i] == '*') ? a * b : a / b;
                
            
                for (int j = i+1; j < nums_size-1; j++)
                    nums[j] = nums[j+1];
                nums_size--;
                
        
                for (int j = i; j < ops_size-1; j++)
                    ops[j] = ops[j+1];
                ops_size--;
            } else {
                i++;
            }
        }


        double res = nums[0];
        for (i = 0; i < ops_size; i++) {
            res = (ops[i] == '+') ? res + nums[i+1] : res - nums[i+1];
        }

    
        if (*p == ')') p++;
        *p_ptr = p;

        return res;
    }


    void Get_Expression(char ch) {
        if (expr_pos < 99) {
            expr[expr_pos++] = ch;
            expr[expr_pos] = '\0';
        }
    }

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
                    OLED_ShowFloatNum(line+1, (Count % 21) + 1, res, 2);
                                
                    
                } else {
                
                    Get_Expression(allowed_chars[Num]);
                    Count++;
                    if(Count==16)
                    {
                        Count =0;
                    
                        line++;
                    
                    }
                }
            }
        }
    }