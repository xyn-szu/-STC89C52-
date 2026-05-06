#include "delay.h"
#include <reg52.h>

// 引脚定义（位选低电平有效）
sbit SegmentG1 = P2^3;
sbit SegmentG2 = P2^2;
sbit SegmentG3 = P2^1;
sbit SegmentG4 = P2^0;

// 共阴数码管段码表（无小数点/有小数点）
const unsigned char arrNumber[] = {0x03 , 0x9f , 0x25 , 0x0d , 0x99 , 0x49 , 0x41 , 0x1f , 0x01 , 0x09}; //初始化数码管显示数字0-9
const unsigned char arrNumber1[] = {0x02 , 0x9e , 0x24 , 0x0c , 0x98 , 0x48 , 0x40 , 0x1e , 0x00 , 0x08}; //初始化数码管显示数字0-9带小数点
void shumaguan(unsigned char location, unsigned char number, bit dot)
{

    // 关闭所有位选（消隐）
    SegmentG1 = 1;
    SegmentG2 = 1;
    SegmentG3 = 1;
    SegmentG4 = 1;

    // 设置段码并打开目标位选
    switch(location)
    {
        case 1: SegmentG1 = 0; break;
        case 2: SegmentG2 = 0; break;  
        case 3: SegmentG3 = 0; break;
        case 4: SegmentG4 = 0; break;
    }
    if(dot)
    P0 = arrNumber1[number];
    else
    P0 = arrNumber[number];  
    delay(1);  // 保持显示
}