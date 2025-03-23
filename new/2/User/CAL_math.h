#pragma once
#ifndef __CAL_MATH_H
#define __CAL_MATH_H

double cal(const char** p_ptr);
void Get_Expression(char ch);
double result(const char* expr);
void BASIC_CAL();
void Interal_CAL();
double trapezoidalIntegration(double a, double b, int n);
void substitute_x(const char* src, double x, char* dst);
#endif 