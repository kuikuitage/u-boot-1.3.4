/*
 * (C) Copyright 2008
 * Free Solutions, <lida-mail@163.com>
 *
 * Configuation settings for the SAMSUNG xyd2440 board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 *
 *
 * History:
 *		a)It was copyed form www.100ask.net, thanks thisway.diy
 *		b)Port SMDK2410 to xyd2440 demo board. Any thing confused,
 * 		  just let me know. So that I can fix the bug, Thanks.
 *											-- Daniel <lida-mail@163.com>
 */
 
#include <common.h>
#include <s3c2410.h>

#define GSTATUS1        (*(volatile unsigned int *)0x560000B0)
#define BUSY            1

/* 供外部调用的函数 */
void nand_init_ll(void);
void nand_read_ll(unsigned char *buf, unsigned long start_addr, int size);

/* NAND Flash操作的总入口, 它们将调用S3C2410或S3C2440的相应函数 */
static void nand_reset(void);
static void wait_idle(void);
static void nand_select_chip(void);
static void nand_deselect_chip(void);
static void write_cmd(int cmd);
static void write_addr(unsigned int addr);
static unsigned char read_data(void);

/* S3C2410的NAND Flash处理函数 */
static void s3c2410_nand_reset(void);
static void s3c2410_wait_idle(void);
static void s3c2410_nand_select_chip(void);
static void s3c2410_nand_deselect_chip(void);
static void s3c2410_write_cmd(int cmd);
static void s3c2410_write_addr(unsigned int addr);
static unsigned char s3c2410_read_data(void);

/* S3C2440的NAND Flash处理函数 */
static void s3c2440_nand_reset(void);
static void s3c2440_wait_idle(void);
static void s3c2440_nand_select_chip(void);
static void s3c2440_nand_deselect_chip(void);
static void s3c2440_write_cmd(int cmd);
static void s3c2440_write_addr(unsigned int addr);
static unsigned char s3c2440_read_data(void);

/* S3C2410的NAND Flash操作函数 */

/* 复位 */
static void s3c2410_nand_reset(void)
{
    s3c2410_nand_select_chip();
    s3c2410_write_cmd(0xff);  // 复位命令
    s3c2410_wait_idle();
    s3c2410_nand_deselect_chip();
}

/* 等待NAND Flash就绪 */
static void s3c2410_wait_idle(void)
{
    int i;
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFSTAT;
    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

/* 发出片选信号 */
static void s3c2410_nand_select_chip(void)
{
    int i;
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;

    s3c2410nand->NFCONF &= ~(1<<11);
    for(i=0; i<10; i++);    
}

/* 取消片选信号 */
static void s3c2410_nand_deselect_chip(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;

    s3c2410nand->NFCONF |= (1<<11);
}

/* 发出命令 */
static void s3c2410_write_cmd(int cmd)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;

    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFCMD;
    *p = cmd;
}

/* 发出地址 */
static void s3c2410_write_addr(unsigned int addr)
{
    int i;
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFADDR;
    
    *p = addr & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
}

/* 读取数据 */
static unsigned char s3c2410_read_data(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;

    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFDATA;
    return *p;
}

/* S3C2440的NAND Flash操作函数 */

/* 复位 */
static void s3c2440_nand_reset(void)
{
    s3c2440_nand_select_chip();
    s3c2440_write_cmd(0xff);  // 复位命令
    s3c2440_wait_idle();
    s3c2440_nand_deselect_chip();
}

/* 等待NAND Flash就绪 */
static void s3c2440_wait_idle(void)
{
    int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;

    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

/* 发出片选信号 */
static void s3c2440_nand_select_chip(void)
{
    int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

    s3c2440nand->NFCONT &= ~(1<<1);
    for(i=0; i<10; i++);    
}

/* 取消片选信号 */
static void s3c2440_nand_deselect_chip(void)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

    s3c2440nand->NFCONT |= (1<<1);
}

/* 发出命令 */
static void s3c2440_write_cmd(int cmd)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMD;
    *p = cmd;
}

/* 发出地址 */
static void s3c2440_write_addr(unsigned int addr)
{
    int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
    
    *p = addr & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
}

/* 读取数据 */
static unsigned char s3c2440_read_data(void)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;
    return *p;
}


/* 在第一次使用NAND Flash前，复位一下NAND Flash */
static void nand_reset(void)
{
    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
	    s3c2410_nand_reset();
	}
	else
	{
	    s3c2440_nand_reset();
	}
}

static void wait_idle(void)
{
    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
	    s3c2410_wait_idle();
	}
	else
	{
	    s3c2440_wait_idle();
	}
}

static void nand_select_chip(void)
{
    int i;
	
    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
	    s3c2410_nand_select_chip();
	}
	else
	{
	    s3c2440_nand_select_chip();
	}
	
    for(i=0; i<10; i++);
}

static void nand_deselect_chip(void)
{
    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
	    s3c2410_nand_deselect_chip();
	}
	else
	{
	    s3c2440_nand_deselect_chip();
	}	
}

static void write_cmd(int cmd)
{
    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
	    s3c2410_write_cmd(cmd);
	}
	else
	{
	    s3c2440_write_cmd(cmd);
	}	
}
static void write_addr(unsigned int addr)
{
    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
	    s3c2410_write_addr(addr);
	}
	else
	{
	    s3c2440_write_addr(addr);
	}	
}

static unsigned char read_data(void)
{
    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
	    return s3c2410_read_data();
	}
	else
	{
	    return s3c2440_read_data();
	}	
}

/* 初始化NAND Flash */
void nand_init_ll(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

#define TACLS   0
#define TWRPH0  3
#define TWRPH1  0

    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
		/* 使能NAND Flash控制器, 初始化ECC, 禁止片选, 设置时序 */
        s3c2410nand->NFCONF = (1<<15)|(1<<12)|(1<<11)|(TACLS<<8)|(TWRPH0<<4)|(TWRPH1<<0);
    }
    else
    {
		/* 设置时序 */
        s3c2440nand->NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
        /* 使能NAND Flash控制器, 初始化ECC, 禁止片选 */
        s3c2440nand->NFCONT = (1<<4)|(1<<1)|(1<<0);
    }

	/* 复位NAND Flash */
	nand_reset();
}


#define NAND_SECTOR_SIZE    512
#define NAND_BLOCK_MASK     (NAND_SECTOR_SIZE - 1)

/* 读函数 */
void nand_read_ll(unsigned char *buf, unsigned long start_addr, int size)
{
    int i, j;
    
    if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
        return ;    /* 地址或长度不对齐 */
    }

    /* 选中芯片 */
    nand_select_chip();

    for(i=start_addr; i < (start_addr + size);) {
      /* 发出READ0命令 */
      write_cmd(0);

      /* Write Address */
      write_addr(i);
      wait_idle();

      for(j=0; j < NAND_SECTOR_SIZE; j++, i++) {
          *buf = read_data();
          buf++;
      }
    }

    /* 取消片选信号 */
    nand_deselect_chip();
    
    return ;
}

int bBootFrmNORFlash(void)
{
    volatile unsigned int *pdw = (volatile unsigned int *)0;
    unsigned int dwVal;
    
    /*
     * 无论是从NOR Flash还是从NAND Flash启动，
     * 地址0处为指令"b	Reset", 机器码为0xEA00000B，
     * 对于从NAND Flash启动的情况，其开始4KB的代码会复制到CPU内部4K内存中，
     * 对于从NOR Flash启动的情况，NOR Flash的开始地址即为0。
     * 对于NOR Flash，必须通过一定的命令序列才能写数据，
     * 所以可以根据这点差别来分辨是从NAND Flash还是NOR Flash启动:
     * 向地址0写入一个数据，然后读出来，如果没有改变的话就是NOR Flash
     */

    dwVal = *pdw;       
    *pdw = 0x12345678;
    if (*pdw != 0x12345678)
    {
        return 1;
    }
    else
    {
        *pdw = dwVal;
        return 0;
    }
}

int CopyCode2Ram(unsigned long start_addr, unsigned char *buf, int size)
{
    unsigned int *pdwDest;
    unsigned int *pdwSrc;
    int i;

    if (bBootFrmNORFlash())
    {
        pdwDest = (unsigned int *)buf;
        pdwSrc  = (unsigned int *)start_addr;
        /* 从 NOR Flash启动 */
        for (i = 0; i < size / 4; i++)
        {
            pdwDest[i] = pdwSrc[i];
        }
        return 0;
    }
    else
    {
        /* 初始化NAND Flash */
		nand_init_ll();
        /* 从 NAND Flash启动 */
        nand_read_ll(buf, start_addr, (size + NAND_BLOCK_MASK)&~(NAND_BLOCK_MASK));
		return 0;
    }
}

static inline void delay (unsigned long loops)
{
    __asm__ volatile ("1:\n"
      "subs %0, %1, #1\n"
      "bne 1b":"=r" (loops):"0" (loops));
}

/* S3C2440: Mpll = (2*m * Fin) / (p * 2^s), UPLL = (m * Fin) / (p * 2^s)
 * m = M (the value for divider M)+ 8, p = P (the value for divider P) + 2
 */
#define S3C2440_MPLL_400MHZ     ((0x5c<<12)|(0x01<<4)|(0x01))
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))
#define S3C2440_MPLL_100MHZ     ((0x5c<<12)|(0x01<<4)|(0x03))
#define S3C2440_UPLL_96MHZ      ((0x38<<12)|(0x02<<4)|(0x01))
#define S3C2440_UPLL_48MHZ      ((0x38<<12)|(0x02<<4)|(0x02))
#define S3C2440_CLKDIV          (0x05) // | (1<<3))    /* FCLK:HCLK:PCLK = 1:4:8, UCLK = UPLL/2 */
#define S3C2440_CLKDIV188       0x04    /* FCLK:HCLK:PCLK = 1:8:8 */
#define S3C2440_CAMDIVN188      ((0<<8)|(1<<9)) /* FCLK:HCLK:PCLK = 1:8:8 */

/* S3C2410: Mpll,Upll = (m * Fin) / (p * 2^s)
 * m = M (the value for divider M)+ 8, p = P (the value for divider P) + 2
 */
#define S3C2410_MPLL_200MHZ     ((0x5c<<12)|(0x04<<4)|(0x00))
#define S3C2410_UPLL_48MHZ      ((0x28<<12)|(0x01<<4)|(0x02))
#define S3C2410_CLKDIV          0x03    /* FCLK:HCLK:PCLK = 1:2:4 */
void clock_init(void)
{
	S3C24X0_CLOCK_POWER *clk_power = (S3C24X0_CLOCK_POWER *)0x4C000000;

    /* support both of S3C2410 and S3C2440, by www.100ask.net */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
        /* FCLK:HCLK:PCLK = 1:2:4 */
        clk_power->CLKDIVN = S3C2410_CLKDIV;

        /* change to asynchronous bus mod */
        __asm__(    "mrc    p15, 0, r1, c1, c0, 0\n"    /* read ctrl register   */  
                    "orr    r1, r1, #0xc0000000\n"      /* Asynchronous         */  
                    "mcr    p15, 0, r1, c1, c0, 0\n"    /* write ctrl register  */  
                    :::"r1"
                    );
        
        /* to reduce PLL lock time, adjust the LOCKTIME register */
        clk_power->LOCKTIME = 0xFFFFFFFF;

        /* configure UPLL */
        clk_power->UPLLCON = S3C2410_UPLL_48MHZ;

        /* some delay between MPLL and UPLL */
        delay (4000);

        /* configure MPLL */
        clk_power->MPLLCON = S3C2410_MPLL_200MHZ;

        /* some delay between MPLL and UPLL */
        delay (8000);
    }
    else
    {
        /* FCLK:HCLK:PCLK = 1:4:8 */
        clk_power->CLKDIVN = S3C2440_CLKDIV;

        /* change to asynchronous bus mod */
        __asm__(    "mrc    p15, 0, r1, c1, c0, 0\n"    /* read ctrl register   */  
                    "orr    r1, r1, #0xc0000000\n"      /* Asynchronous         */  
                    "mcr    p15, 0, r1, c1, c0, 0\n"    /* write ctrl register  */  
                    :::"r1"
                    );

        /* to reduce PLL lock time, adjust the LOCKTIME register */
        clk_power->LOCKTIME = 0xFFFFFFFF;

        /* configure UPLL */
        clk_power->UPLLCON = S3C2440_UPLL_48MHZ;

        /* some delay between MPLL and UPLL */
        delay (4000);

        /* configure MPLL */
        clk_power->MPLLCON = S3C2440_MPLL_400MHZ;

        /* some delay between MPLL and UPLL */
        delay (8000);
    }
}

/*
 * LED control C functions
 * These code is used for debug if serial is NOT properly initialized.
 * It caused me a lot trouble in the first. And I will keep for others.
 * Well, Nand is good for code storage and bettern than Nor-Flash.
 *
 * Suggestion: If you don't like these code in first 4K page, you may probably 
 * re-arrange the code section.
 *											-- Daniel Li
 */

#define rGPFCON      (*(volatile unsigned long *)0x56000050)
#define rGPFDAT      (*(volatile unsigned long *)0x56000054)

static void led_dl1(int nStatus);
static void led_dl2(int nStatus);
static void led_dl3(int nStatus);
static void led_dl4(int nStatus);

/*
 *	API for leds initialization and control.
 */
void	coloured_LED_init (void)
{
	led_dl1(0);
	led_dl2(0);
	led_dl3(0);
	led_dl4(0);
}
void	red_LED_on(void)
{
	led_dl1(1);
}
void	red_LED_off(void)
{
	led_dl1(0);
}
void	green_LED_on(void)
{
	led_dl2(1);
}
void	green_LED_off(void)
{
	led_dl2(0);
}
void	yellow_LED_on(void)
{
	led_dl3(1);
}
void	yellow_LED_off(void)
{
	led_dl3(0);
}
void	blue_LED_on(void)
{
	led_dl4(1);
}
void	blue_LED_off(void)
{
	led_dl4(0);
}

static void led_dl1(int nStatus)
{
	if(nStatus)
	{
		rGPFCON |= 1<<8;
		rGPFDAT &= ~(1<<4);
	}else
	{
		rGPFCON |= 1<<8;
		rGPFDAT |= 1<<4;
	}
}

static void led_dl2(int nStatus)
{
	if(nStatus)
	{
		rGPFCON |= 1<<10;
		rGPFDAT &= ~(1<<5);
	}else
	{
		rGPFCON |= 1<<10;
		rGPFDAT |= 1<<5;
	}
}

static void led_dl3(int nStatus)
{
	if(nStatus)
	{
		rGPFCON |= 1<<12;
		rGPFDAT &= ~(1<<6);
	}else
	{
		rGPFCON |= 1<<12;
		rGPFDAT |= 1<<6;
	}
}

static void led_dl4(int nStatus)
{
	if(nStatus)
	{
		rGPFCON |= 1<<14;
		rGPFDAT &= ~(1<<7);
	}else
	{
		rGPFCON |= 1<<14;
		rGPFDAT |= 1<<7;
	}
}


