#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Encoder.h"
#include "Key.h"
#include "CAL_math.h"
#include "ctype.h"
#include "stdlib.h"
#include "stdio.h"


int16_t Num;
uint8_t KeyNum;
uint8_t Count = 1;
char expr[100] = {0};
static int expr_pos = 0; 

double result(const char* expr);
double cal(const char* p);

char allowed_chars[] = {'0','1','2','3','4','5','6','7','8','9','+','-','*','/','(',')','=','.',};

void Get_Expression(char a) {
    if (expr_pos < 99) {
        expr[expr_pos++] = a;
        expr[expr_pos] = '\0';
    }
}

double result(const char* expr) {
    if (expr == NULL || *expr == '\0') return 0.0;  
    const char* p = expr;
    return cal(p);
}

double cal(const char* p) {
    double nums[100];
    int nums_size = 0;
    char ops[100];
    int ops_size = 0;

    while (*p && *p != ')') {
        if (isdigit(*p) || *p == '.') {
            if (nums_size >= 100) break;
            char* end;
            double temp = strtod(p, &end);
            if (end == p) { p++; continue; }
            nums[nums_size++] = temp;
            p = end;
        } else if (*p == '(') {
            p++;
            if (nums_size >= 100) break;
            double temp = cal(p);
            nums[nums_size++] = temp;
            p++;  // 跳过 ')'
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            if (ops_size >= 100) break;
            ops[ops_size++] = *p;
            p++;
        } else {
            p++;
        }
    }

    // 处理乘除
    int i = 0;
    while (i < ops_size) {
        if (ops[i] == '*' || ops[i] == '/') {
            double a = nums[i], b = nums[i+1];
            nums[i] = (ops[i] == '*') ? a * b : a / b;
            for (int j = i+1; j < nums_size-1; j++) nums[j] = nums[j+1];
            nums_size--;
            for (int j = i; j < ops_size-1; j++) ops[j] = ops[j+1];
            ops_size--;
        } else {
            i++;
        }
    }

    // 处理加减
    double res = nums[0];
    for (i = 0; i < ops_size; i++) {
        res = (ops[i] == '+') ? res + nums[i+1] : res - nums[i+1];
    }
    return res;
}

int main(void) {
    OLED_Init();
    OLED_Menu();
    Encoder_Init();
    Key_Init();
    while (1) {
        KeyNum = Key_GetNum();
        Num += Encoder_Get();
        if (Num > 17 || Num < 0) 
        Num = 0;
        OLED_ShowChar(1, Count % 21, allowed_chars[Num]);  
        if (KeyNum == 3)
        {
       
        Get_Expression(allowed_chars[Num]);
         Count++;
        }
        
        
       
        while(allowed_chars[Num]=='='&&KeyNum==3)
        {
            
            OLED_ShowFloatNum(1, (Count % 21) + 1, result(expr),2);
        }
    }
}