    #include "ctype.h"
    #include "stdlib.h"
    #include "CAL_math.h"
    #include "stm32f10x.h"
    #include "OLED.h"
    #include "Encoder.h"
    #include "Key.h"
    #include "math.h"
    #include "string.h"
  
    char expr[100];
    int i=0;
      int expr_pos;
   uint8_t line = 1; 
   uint8_t _Float_flag=0;
   int16_t Num;                
  uint8_t KeyNum;             
  uint8_t Count = 1;  
 int temp=1;

 char up[100] = {0};
char down[100] = {0};
int up_pos = 0;
int down_pos = 0;
uint8_t integral_state = 1;
 const char allowed_chars[] = {
    '0','1','2','3','4','5','6','7','8','9',
    '+','-','*','/','(',')','=','.'
};
const char allowed_Interal_num_chars[] = {
    '0','1','2','3','4','5','6','7','8','9','.'
   
};
    double cal(const char** p_ptr) {
    const char* p = *p_ptr;
    double nums[55] = {0};
    int nums_size = 0;
    char ops[55] = {0};
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

void Get_up(char ch) {
    if (expr_pos < 99) {
        up[expr_pos++] = ch;
        up[expr_pos] = '\0';
    }
}
void Get_down(char ch) {
    if (expr_pos < 99) {
        down[expr_pos++] = ch;
        down[expr_pos] = '\0';
    }
}
void Get_Expression(char ch) {
    if (expr_pos < 99) {
        expr[expr_pos++] = ch;
        expr[expr_pos] = '\0';
    }
}
double result(const char* expr) {
    const char* p = expr;
    return cal(&p);
}




void BASIC_CAL()


{
    OLED_Clear();
    
    
    while (1)
{
  
   
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
void Interal_CAL()
{ OLED_Clear();
    Count=7;
    while (1)
    {
       
        
    KeyNum = Key_GetNum();
    Num += Encoder_Get();

    
    if (Num < 0) Num = sizeof(allowed_Interal_num_chars)-1;
    if (Num >= sizeof(allowed_Interal_num_chars)) Num = 0;

   

    switch(integral_state){
        case 1: 
            OLED_ShowString(1, 1, "Upper:");
            
            
            OLED_ShowChar(1,Count % 21, allowed_Interal_num_chars[Num]);
            
            if(KeyNum == 3){
                if(up_pos < 99){
                    up[up_pos++] = allowed_Interal_num_chars[Num];
                    up[up_pos] = '\0';
                    Count++;
                }
            }
            else if(KeyNum == 2){ // 切換到下界
                integral_state = 2;
               
            }
            break;
            
        case 2: // 输入下限模式
            OLED_ShowString(2, 1, "Lower:");
           if(i=0)
{ Count=7;i=1;}
           
            // 显示当前选中字符
            OLED_ShowChar(2, Count % 21, allowed_Interal_num_chars[Num]);
            
            if(KeyNum == 3){ 
                if(down_pos < 99){
                    down[down_pos++] = allowed_Interal_num_chars[Num];
                    down[down_pos] = '\0';
                    Count++;
                }
            }
            else if(KeyNum == 2){ // 开始计算
                double a = strtod(up, NULL);
                double b = strtod(down, NULL);
                double res = trapezoidalIntegration(a, b, 1000);
                
                OLED_ShowString(4, 1, "Result:");
                OLED_ShowFloatNum(4, 8, res, 4);
             
                up_pos = 0;
                down_pos = 0;
                memset(up, 0, sizeof(up));
                memset(down, 0, sizeof(down));
                integral_state = 1;
            }
          
            break;
    }
}
}
   
   


double f(double x) {
    return x*x;
}


double trapezoidalIntegration(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    int i;

    for (i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += f(x);
    }

    return sum * h;
}

