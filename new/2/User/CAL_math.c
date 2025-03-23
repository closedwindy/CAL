    #include "ctype.h"
    #include "stdlib.h"
    #include "CAL_math.h"
    #include "stm32f10x.h"


    extern char expr[100];
    extern  int expr_pos;


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
