#include "delay.h"
#include <reg52.h>
#define BEEP_ON BEEP = 0
#define BEEP_OFF BEEP = 1
sbit BEEP = P1^0; //定义蜂鸣器
void beep() 
{
  BEEP_ON;        //打开蜂鸣器
  delay(500);
  BEEP_OFF;
}