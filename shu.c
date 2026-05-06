#include <reg52.h>
sbit SegmentG1 = P2^3;  //定义数码管1
sbit SegmentG2 = P2^2;  //定义数码管2
sbit SegmentG3 = P2^1;  //定义数码管3
sbit SegmentG4 = P2^0;  //定义数码管4
const unsigned char arrNumber[] = {0x03 , 0x9f , 0x25 , 0x0d , 0x99 , 0x49 , 0x41 , 0x1f , 0x01 , 0x09}; //初始化数码管显示数字0-9
unsigned char shumaguan(unsigned char guan ,unsigned char ans)
{
  ans=0;
  switch(guan)
  {
    case 1: SegmentG1 = 0;while(P0!=arrNumber[ans])  ans++;break;       //打开数码管1
    case 2: SegmentG2 = 0;while(P0!=arrNumber[ans])  ans++;break;       //打开数码管2
    case 3: SegmentG3 = 0;while(P0!=arrNumber[ans])  ans++;break;       //打开数码管3
    case 4: SegmentG4 = 0;while(P0!=arrNumber[ans])  ans++;break;       //打开数码管4
  }
  return ans;
}