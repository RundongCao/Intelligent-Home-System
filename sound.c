/*
 * sound.c
 *
 *  Created on: 2015-5-10
 *      Author: CRD
 */

#include "msp430g2553.h"

unsigned char n=0;  //n为节拍常数变量
unsigned char  music_tab[] ={
0x18, 0x30, 0x1C , 0x10, //格式为: 频率常数, 节拍常数, 频率常数, 节拍常数,
0x20, 0x40, 0x1C , 0x10,
0x18, 0x10, 0x20 , 0x10,
0x1C, 0x10, 0x18 , 0x40,
0x1C, 0x20, 0x20 , 0x20,
0x1C, 0x20, 0x18 , 0x20,
0x20, 0x80, 0xFF , 0x20,
0x30, 0x1C, 0x10 , 0x18,
0x20, 0x15, 0x20 , 0x1C,
0x20, 0x20, 0x20 , 0x26,
0x40, 0x20, 0x20 , 0x2B,
0x20, 0x26, 0x20 , 0x20,
0x20, 0x30, 0x80 , 0xFF,
0x20, 0x20, 0x1C , 0x10,
0x18, 0x10, 0x20 , 0x20,
0x26, 0x20, 0x2B , 0x20,
0x30, 0x20, 0x2B , 0x40,
0x20, 0x20, 0x1C , 0x10,
0x18, 0x10, 0x20 , 0x20,
0x26, 0x20, 0x2B , 0x20,
0x30, 0x20, 0x2B , 0x40,
0x20, 0x30, 0x1C , 0x10,
0x18, 0x20, 0x15 , 0x20,
0x1C, 0x20, 0x20 , 0x20,
0x26, 0x40, 0x20 , 0x20,
0x2B, 0x20, 0x26 , 0x20,
0x20, 0x20, 0x30 , 0x80,
0x20, 0x30, 0x1C , 0x10,
0x20, 0x10, 0x1C , 0x10,
0x20, 0x20, 0x26 , 0x20,
0x2B, 0x20, 0x30 , 0x20,
0x2B, 0x40, 0x20 , 0x15,
0x1F, 0x05, 0x20 , 0x10,
0x1C, 0x10, 0x20 , 0x20,
0x26, 0x20, 0x2B , 0x20,
0x30, 0x20, 0x2B , 0x40,
0x20, 0x30, 0x1C , 0x10,
0x18, 0x20, 0x15 , 0x20,
0x1C, 0x20, 0x20 , 0x20,
0x26, 0x40, 0x20 , 0x20,
0x2B, 0x20, 0x26 , 0x20,
0x20, 0x20, 0x30 , 0x30,
0x20, 0x30, 0x1C , 0x10,
0x18, 0x40, 0x1C , 0x20,
0x20, 0x20, 0x26 , 0x40,
0x13, 0x60, 0x18 , 0x20,
0x15, 0x40, 0x13 , 0x40,
0x18, 0x80, 0x00
};


void delay (unsigned char m)   //控制频率延时
{
 unsigned i=3*m;
 while(--i);
}

void delaymms(unsigned int a)  //毫秒延时子程序
{
  unsigned int t;
     while(--a)
        for(t=0;t<200;t++);
}

void main(void)
{
   unsigned char p,m;   //m为频率常数变量
   unsigned char i=0;

   WDTCTL = WDTPW + WDTHOLD;     // 停止看门狗

   P1DIR = 0Xff;
   P2DIR = 0Xff;

   CCTL0 |= CCIE;                             // CCR0 interrupt enabled
   CCR0 = 10000;                         //10ms
   TACTL = TASSEL_2 ;                  // SMCLK, contmode
   _EINT();


   while(1)
      {
         p=music_tab[i];
          if(p==0x00)
          {
            i=0;
            delaymms(1000);
            continue;;
          }     //如果碰到结束符,延时1秒,回到开始再来一遍
          else if(p==0xff)
          {
            i=i+1;
            delaymms(100);
            TACTL &=~MC_1;
            continue;
          }  //若碰到休止符,延时100ms,继续取下一音符
          else
          {
             m=music_tab[i++];
             n=music_tab[i++];
          }  //取频率常数 和 节拍常数

           TACTL |= MC_1+TACLR;                                           //开定时器
           while(n!=0) P1OUT ^= BIT0,delay(m);                         //等待节拍完成
           TACTL &=~MC_1;                                           //关定时器
    }

}


// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    n--;
}


