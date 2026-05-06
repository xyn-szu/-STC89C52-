#include <reg52.h>
 #include "shumaguan.h"
 #include "delay.h"
#define u8 unsigned char
#define u16 unsigned int	
 #define BEEP_ON BEEP = 0
#define BEEP_OFF BEEP = 1


//函数声明

void Display_Timer();
u8 Key_Scan();
void kongzhi();
void caideng();
void beep();
//const unsigned char arrNumber[] = {0x03 , 0x9f , 0x25 , 0x0d , 0x99 , 0x49 , 0x41 , 0x1f , 0x01 , 0x09}; //初始化数码管显示数字0-9  
//引脚定义
sbit SegmentG1 = P2^3;  //定义数码管1
sbit SegmentG2 = P2^2;  //定义数码管2
sbit SegmentG3 = P2^1;  //定义数码管3
sbit SegmentG4 = P2^0;  //定义数码管4

sbit LED1 = P2^4; //定义LED1
sbit LED2 = P2^5; //定义LED2
sbit LED3 = P2^6; //定义LED3
sbit LED4 = P2^7; //定义LED4

sbit KEY1 = P3^2; //定义按键KEY1
sbit KEY2 = P3^3; //定义按键KEY2
sbit KEY3 = P3^4; //定义按键KEY3

sbit BEEP = P1^0; //定义蜂鸣器
//函数功能：定时器1初始化
void Time1init()
{
  TMOD &= 0x0F;   // 保留低4位（定时器0配置）
  TMOD |= 0x10;   // 设置定时器1为模式1
	TF1=0;					//清除TF0标志
  TH1 = 0XFC;                      //重新设置 定时器0 计数初值高8位 
  TL1 = 0X18;                      //重新设置 定时器0 计数初值低8位，定时1ms后溢出
	TR1=1;					//定时器1允许计时
	ET1=1;					//中断允许
	EA=1;					//CPU中断允许位打开
}
 
//函数功能：定时器0初始化
void Time0init()
{
  TMOD &= 0xF0;   // 保留高4位（定时器1配置）
  TMOD |= 0x01;   // 设置定时器0为模式1
	TF0=0;					//清除TF0标志
  TH0 = 0XFC;                      //重新设置 定时器0 计数初值高8位 
  TL0 = 0X18;                      //重新设置 定时器0 计数初值低8位，定时1ms后溢出
  TR0=1;	 //定时器0允许计时
	ET0=1;					//中断允许
	EA=1;					//CPU中断允许位打开	
}


//全局变量
bit beepflag = 0;       // 蜂鸣器激活标志
unsigned int flag=0;    //
static u8 a1,a2,a3,a4;//全局变量，分秒
static u8 mode;//全局变量：状态切换，0：时钟显示，1：的一个数字；2：第二个数字；3：第三个数字，4第四个数字
bit flash_tip=1;//数码管闪烁标志，为0时数码管熄灭，为一时数码管显示
static u8 ledflag = 0;
static u16 ledcount = 0;
unsigned int ledIndex = 0;  // 当前点亮LED的索引
unsigned int sec;
unsigned int i=0;
static bit countdown_active = 0;  // 倒计时激活标志
static u8 kaishi=0;
static u8 beepcount;
static u8 k1=0,k2=0;
static u16 key3_time= 0; // KEY3按下时间计数器
static bit key3_pressing = 0;  // KEY3按下状态标志

//主函数
void main()
{
	a1=0;	a2=0;	a3=0;	a4=0;	
	mode=0;//初始化
  Time0init();//定时器0
	while(1)
	{ 
    kongzhi();
    Display_Timer();
    caideng();
 }

}

 
 //函数功能：数码管输入
 void kongzhi()
{
      u8 keynum;
 		keynum=Key_Scan();//按键返回值
		if(keynum)   //非0表示有按键按下
		{
			switch(keynum)  //判断是哪个按键按下，按键一调节位置，按键2自加
			{
				case 1://按下KEY1，选要调节数目管那个位置的数字，mode=0时KEY3才有可以开启计时的功能；
          if(kaishi==0&&ledflag==0){mode++;if(mode>4) mode=0;}
          else if(kaishi==1&&flag==1&&ledflag==0)
          {
          k1=1;
          if(k2==0){  
          TR1=0;
          LED1=0;
          beep();
          }else{break;}
          } break;  
				case 2://按下KEY2，所选位置数字加1
          if(kaishi==0&&ledflag==0){
          if(mode==0) break;
					else if(mode==1) {a1++;if(a1>9) a1=0;}//
          else if(mode==2) {a2++;if(a2>9) a2=0;} 
          else if(mode==3) {a3++;if(a3>=6) a3=0;}	
          else if(mode==4) {a4++;if(a4>9) a4=0;}}
          else if(kaishi==1&&flag==1&&ledflag==0)
          {
          k2=1;
          if(k1==0){  
          TR1=0;
          LED2=0;
          beep();
          }else{break;}
          }break;										
				case 3://短按KEY3：mode=0开始计时，mode≠0，则对所选位置数字减1
         if( mode== 0)
         {
          if(flag == 0) 
          {
           // 第一次按下：启动定时器
            sec=a1*60*10+a2*60+a3*10+a4;
            kaishi=1;mode=0;k1=0;k2=0;
              Time1init();
              flag = 1;
          } 
          else 
          {
            TR1 = 0;       // 停止定时器1
            flag = 0;
            kaishi=1;
            mode=0;
            // 关闭所有LED
            LED1 = 1;
            LED2 = 1;
            LED3 = 1;
            LED4 = 1;
          }
         }
         else {
        if(mode==1) {a1 = (a1 == 0) ? 9 : a1 - 1;}  // 0减1变为9 
        else if(mode==2) {a2 = (a2 == 0) ? 9 : a2 - 1;} // 0减1变为9
        else if(mode==3) {a3 = (a3 == 0) ? 5 : a3 - 1;} // 0减1变为5
        else if(mode==4) {a4 = (a4 == 0) ? 9 : a4 - 1;}  // 0减1变为9   
        }break;        
        case 4:       //长按KEY3 
          { 
            a1=0;a2=0;a3=0;a4=0;  
            kaishi=0;
            BEEP_OFF;      // 关闭蜂鸣器
            TR1 = 0;       // 停止定时器1
            ledflag = 0;   // 关闭彩灯
            flag=0;        //保证下一次开始计时时，KEY3不用按两次
            mode=0;
            // 关闭所有LED
            LED1 = 1;
            LED2 = 1;
            LED3 = 1;
            LED4 = 1;}
				default:break;
			}     
	   }      
}

//函数功能：数码管动态显示
//flash_tip为数码管闪烁标志，为0时数码管熄灭，为1时数码管显示
void Display_Timer()
{  
	// 根据当前模式显示
	switch(mode)
	{
    case 0:     shumaguan(1,a1,0);
                shumaguan(2,a2,1);
                shumaguan(3,a3,0);
                shumaguan(4,a4,0);break;
    case 1: if(flash_tip==1){ 
                shumaguan(1,a1,0);
                shumaguan(2,a2,1);
                shumaguan(3,a3,0);
                shumaguan(4,a4,0);}
             else{
                SegmentG1 =1;
                shumaguan(2,a2,1);
                shumaguan(3,a3,0);
                shumaguan(4,a4,0);
                }break;
    case 2: if(flash_tip==1){ 
                shumaguan(1,a1,0);
                shumaguan(2,a2,1);
                shumaguan(3,a3,0);
                shumaguan(4,a4,0);}
             else{
                SegmentG2 =1;
                shumaguan(1,a1,0);
                shumaguan(3,a3,0);
                shumaguan(4,a4,0);
                }break;
    case 3: if(flash_tip==1){ 
                shumaguan(1,a1,0);
                shumaguan(2,a2,1);
                shumaguan(3,a3,0);
                shumaguan(4,a4,0);}
             else{
                SegmentG3 =1;
                shumaguan(1,a1,0);
                shumaguan(2,a2,1);
                shumaguan(4,a4,0);
                }break;
    case 4: if(flash_tip==1){ 
                shumaguan(1,a1,0);
                shumaguan(2,a2,1);
                shumaguan(3,a3,0);
                shumaguan(4,a4,0);}
             else{
                SegmentG4 =1;
                shumaguan(1,a1,0);
                shumaguan(2,a2,1);
                shumaguan(3,a3,0);
                }break;            
             }
	}
 

//函数功能：独立按键检测
u8 Key_Scan() 
{
    static u8 key_up = 1;
    u8 key_val = 0;
    static bit longpress=0;
    if(key_up && (KEY1==0 || KEY2==0 || KEY3==0))
    {
        delay(10);
        key_up = 0;
        longpress=0;
        if(KEY1==0) key_val = 1;
        else if(KEY2==0) key_val = 2;
        else if(KEY3==0) {
            key3_pressing = 1;
            key3_time = 0;
        }
    }
    else if(KEY1 == 1 && KEY2 == 1 && KEY3 == 1) 
    {
        key_up = 1;     
        // 仅处理未触发长按的情况
        if(key3_pressing && key3_time < 1500&&!longpress) {
            key_val = 3;
        }
        key3_pressing = 0;
    }
    // 长按处理：不重置key_up，保证不会重复扫描
    if(key3_pressing && key3_time >= 1500&&!longpress) {
        key_val = 4;
     longpress=1;
    } 
    return key_val;
}

//彩灯流水
void caideng()
{ 
  if(ledflag == 1)
  {
    // 关闭所有LED（根据实际电路调整电平）
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    // 流水灯控制（每300ms切换一个LED）
    switch(ledIndex) {
      case 0: LED1 = 0; break;  // 点亮LED1
      case 1: LED2 = 0; break;  // 点亮LED2
      case 2: LED3 = 0; break;  // 点亮LED3
      case 3: LED4 = 0; break;  // 点亮LED4
    }
  }
}
void beep()
{
    beepcount=400;
    BEEP_ON;            // 打开蜂鸣器
}
 
//定时器0中断服务函数
void Time0() interrupt 1
{
	static unsigned int count=0;
  TH0 = 0XFC;                      //重新设置 定时器0 计数初值高8位 
  TL0 = 0X18;                      //重新设置 定时器0 计数初值低8位，定时1ms后溢出
	count++;
  ledcount++;
   if(count>=500)//实现数码管闪烁的计数
   {
    count=0;
     flash_tip=!flash_tip;
   }
   if(ledcount>200)//实现彩灯流水的计数
   {
    ledcount=0;
    ledIndex++;
     if(ledIndex>3)//实现彩灯流水
     {
     ledIndex=0;
     }
    }
   if(beepcount > 0)//实现蜂鸣器短鸣
    {
      beepcount--;
      if(beepcount == 0)
        {
          BEEP_OFF;
        }
    }
    if(key3_pressing) 
    {
      key3_time++;
    }
    
}

//定时器1中断服务函数
void Time1() interrupt 3
{ 
   u16 sec_remain=sec;
    static unsigned int count1 = 0; 
    TH1 = 0XFC;
    TL1 = 0X18;
    count1++;
    if(count1 >= 1000) { // 每1秒触发
        count1 = 0;
        if(sec > 0) {
            sec--; // 秒数减1
            // 更新全局显示变量
            sec_remain= sec;
            a1 = sec_remain / 600;         // 分钟十位
            a2 = (sec_remain % 600) / 60;  // 分钟个位
            a3 = (sec_remain % 60) / 10;   // 秒十位
            a4 = sec_remain % 10;          // 秒个位
        } 
         if(a1==0&&a2==0&&a3==0&&a4==0)
        {
            // 倒计时结束
            BEEP_ON;//开启蜂鸣器
            ledflag = 1;
            TR1 = 0; // 停止定时器1
        }
    }
 }
