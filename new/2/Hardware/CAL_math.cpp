#include "ctype.h"
#include "stdlib.h"
#include "stdio.h"


double cal(const char* p) {
    double nums[100];
    int nums_size = 0;
    char ops[100];
    int ops_size = 0;

    while (*p && *p != ')') {
        if (isdigit(*p) || *p == '.') {
            if (nums_size >= 100) break;  // 防溢出
            char* end;
            double temp = strtod(p, &end);
            if (end == p) {  // 解析失败
                ++p;
                continue;
            }
            nums[nums_size++] = temp;
            p = end;
        } else if (*p == '(') {
            ++p;  // 跳过 '('
            if (nums_size >= 100) break;
            double temp = cal(p);
            nums[nums_size++] = temp;
            ++p;  // 跳过 ')'
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            if (ops_size >= 100) break;  // 防溢出
            ops[ops_size++] = *p;
            ++p;
        } else {
            ++p;  // 跳过无效字符
        }
    }

    // 处理乘除
    int i = 0;
    while (i < ops_size) {
        if (ops[i] == '*' || ops[i] == '/') {
            double a = nums[i];
            double b = nums[i + 1];
            double res = (ops[i] == '*') ? a * b : a / b;
            nums[i] = res;
            // 前移数组元素
            for (int j = i + 1; j < nums_size - 1; ++j)
                nums[j] = nums[j + 1];
            nums_size--;
            for (int j = i; j < ops_size - 1; ++j)
                ops[j] = ops[j + 1];
            ops_size--;
        } else {
            ++i;
        }
    }

    // 处理加减
    double result = nums[0];
    for (int i = 0; i < ops_size; ++i) {
        if (ops[i] == '+') {
            result += nums[i + 1];
        } else if (ops[i] == '-') {
            result -= nums[i + 1];
        }
    }
    return result;
}
// double cal(const char* p) {
//     double nums[100];
//     int nums_size = 0;
//     char ops[100];
//     int ops_size = 0;

//     while (*p && *p != ')') {
//         if (isdigit(*p) || *p == '.') {
//             char* end;
//             double temp = strtod(p, &end);
//             nums[nums_size++] = temp;
//             p = end;
//         } else if (*p == '(') {
//             ++p;
//             double temp = cal(p);
//             nums[nums_size++] = temp;
//             ++p; // Skip ')'
//         } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
//             ops[ops_size++] = *p;
//             ++p;
//         } else {
//             ++p;
//         }
//     }


//     int i = 0;
//     while (i < ops_size) {
//         if (ops[i] == '*' || ops[i] == '/') {
//             double a = nums[i];
//             double b = nums[i + 1];
//             double res = (ops[i] == '*') ? a * b : a / b;
//             nums[i] = res;


//             for (int j = i + 1; j < nums_size - 1; ++j)
//                 nums[j] = nums[j + 1];
//             nums_size--;

//             for (int j = i; j < ops_size - 1; ++j)
//                 ops[j] = ops[j + 1];
//             ops_size--;
//         } else {
//             ++i;
//         }
//     }

  

double result(const char* expr) {
    const char* p = expr;
    return cal(p);
}
char expr[100];




void Get_Expression(char a) {

    
    int expr_pos = 0;
    if (expr_pos < 99) {
        expr[expr_pos++] = a;
        expr[expr_pos] = '\0';
    }
}