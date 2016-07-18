/*************************************************************************
    > File Name: init.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sun 17 Jul 2016 04:41:46 AM PDT
 ************************************************************************/
/*
 * init.c: 进行一些初始化
 */ 

#include "s3c24xx.h"

/*
 * LED1-4对应GPB5、GPB6、GPB7、GPB8
 */
#define GPB5_out        (1<<(5*2))      // LED1
#define GPB6_out        (1<<(6*2))      // LED2
#define GPB7_out        (1<<(7*2))      // LED3
#define GPB8_out        (1<<(8*2))      // LED4

/*
 * K1-K4对应GPG11、GPG3、GPF2、GPF0
 */
#define GPG11_eint      (2<<(11*2))     // K1,EINT19
#define GPG3_eint       (2<<(3*2))      // K2,EINT11
#define GPF2_eint       (2<<(2*2))      // K3,EINT2
#define GPF0_eint       (2<<(0*2))      // K4,EINT0

 
/*
 * 关闭WATCHDOG，否则CPU会不断重启
 */
void disable_watch_dog(void)
{
    WTCON = 0;  // 关闭WATCHDOG很简单，往这个寄存器写0即可
}

void init_led(void)
{
    GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out ;
}

/*
 * 初始化GPIO引脚为外部中断
 * GPIO引脚用作外部中断时，默认为低电平触发、IRQ方式(不用设置INTMOD)
 */ 
void init_irq( )
{
    GPFCON  = GPF0_eint | GPF2_eint;
    GPGCON  = GPG3_eint | GPG11_eint;
    
    // 对于EINT11、19，需要在EINTMASK寄存器中使能它们
    EINTMASK &= (~(1<<11)) & (~(1<<19));
        
    /*
     * 设定优先级：
     * ARB_SEL0 = 00b, ARB_MODE0 = 0: REQ1 > REQ3，即EINT0 > EINT2
     * 仲裁器1、6无需设置
     * 最终：
     * EINT0 > EINT2 > EINT11,EINT19，即K4 > K3 > K1,K2
     * EINT11和EINT19的优先级相同
     */
    PRIORITY = (PRIORITY & ((~0x01) | (0x3<<7))) | (0x0 << 7) ;

    // EINT0、EINT2、EINT8_23使能
    INTMSK   &= (~(1<<0)) & (~(1<<2)) & (~(1<<5));
}




