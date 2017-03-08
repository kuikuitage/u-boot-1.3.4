/*
 * (C) Copyright 2008
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <gj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the SAMSUNG SMDK2410 board.
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
 *
 *	     
 * 	      
 *											 
 *										       
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * I wanna track this xyd2440 board versions
 * (easy to manage)
 */
#define U_BOOT_xyd_PATCH_VER	"xyd2440 V-1.0"

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM920T		1	/* This is an ARM920T Core	*/
#define	CONFIG_S3C2410		1	/* in a SAMSUNG S3C2410 SoC     */
#define CONFIG_SMDK2410		1	/* on a SAMSUNG SMDK2410 Board  */

#define CONFIG_S3C2440				1	/* Add comment later					*/	
#define CONFIG_xyd2440_LED_DEBUG	1 	/* do led debug, if you want */

/* input clock of PLL */
#define CONFIG_SYS_CLK_FREQ	12000000/* the xyd2440 has 12MHz input clock */

#define USE_920T_MMU		1
#undef CONFIG_USE_IRQ			/* we don't need IRQ/FIQ stuff */

/*
 * Size of malloc() pool
 */
#define CFG_MALLOC_LEN		(CFG_ENV_SIZE + 128*1024)
#define CFG_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */

/*
 * Hardware drivers
 */


#if 0  
#define CONFIG_DRIVER_CS8900	    0     	/* we have a CS8900 on-board */
#define CS8900_BASE		    0x19000300
#define CS8900_BUS16	  	    1           /* the Linux driver does accesses as shorts */
#endif
#if !defined(CONFIG_DRIVER_CS8900)
#define CONFIG_DRIVER_DM9000        1	      	/* we have a DM9000A on-board 10/100M netchip :) */
#define CONFIG_DM9000_USE_16BIT     1
#define CONFIG_DM9000_BASE          0x20000000	/* on bank_3(nGCS3), see xyd2440 schematic */
#define DM9000_IO                   0x20000000  
#define DM9000_DATA                 0x20000004 	/* LADDR2, see xyd2440 schematic */
#endif





/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1          1	/* we use SERIAL 1 on xyd2440 */
#define CONFIG_BAUDRATE		115200

/************************************************************
 * RTC
 ************************************************************/
#define	CONFIG_RTC_S3C24X0	1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME


/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_DATE
#define CONFIG_CMD_ELF

#define CONFIG_CMD_PING
#define CONFIG_CMD_NAND


#define CONFIG_BOOTDELAY	3


/*#define CONFIG_BOOTARGS    	"noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0"*/
#define CONFIG_BOOTARGS    	"console=ttySAC0  root=/dev/nfs nfsroot=192.168.0.101:/opt/s3c2440/root_nfs  ip=192.168.0.99:192.168.0.101:192.168.0.1:255.255.255.0:www.arm.com:eth0:off"
/*#define CONFIG_BOOTARGS    	"noinitrd  console=ttySAC0 root=/dev/mtdblock2 rootfstype=yaffs"*/
#define CONFIG_ETHADDR	        08:00:3e:26:0a:5b
#define CONFIG_NETMASK          255.255.255.0
#define CONFIG_IPADDR		192.168.0.99
#define CONFIG_SERVERIP		192.168.0.30
/*#define CONFIG_BOOTFILE	"elinos-lart" */
/*#define CONFIG_BOOTCOMMAND	"nboot 0x32000000 0 100000; bootm 0x32000000"*/
/*#define CONFIG_BOOTCOMMAND	"nand read.jffs2 32000000 100000 0x250000; bootm 0x32000000"*/
#define CONFIG_BOOTCOMMAND	"tftp  0x32000000   uImage; bootm 0x32000000"

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200		/* speed to run kgdb serial port */
/* what's this ? it's not used anywhere */
#define CONFIG_KGDB_SER_INDEX	1		/* which serial port to use */
#endif

/*
 * Miscellaneous configurable options
 */
#define	CFG_LONGHELP					/* undef to save memory		*/
#define	CFG_PROMPT		"xyd2440 # "	/* Monitor Command Prompt	*/
#define	CFG_CBSIZE		256				/* Console I/O Buffer Size	*/
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16) 	/* Print Buffer Size */
#define	CFG_MAXARGS		16				/* max number of command args	*/
#define CFG_BARGSIZE		CFG_CBSIZE	/* Boot Argument Buffer Size	*/

#define CFG_MEMTEST_START	0x30000000	/* memtest works on	*/
#define CFG_MEMTEST_END		0x33F00000	/* 63 MB in DRAM	*/

#undef  CFG_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define	CFG_LOAD_ADDR		0x33000000	/* default load address	*/

/* the PWM TImer 4 uses a counter of 15625 for 10 ms, so we need */
/* it to wrap 100 times (total 1562500) to get 1 sec. */
#define	CFG_HZ			1562500

/* valid baudrates */
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	   /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		0x30000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x04000000 /* 64 MB */

#define PHYS_FLASH_1		0x00000000 /* Flash Bank #1 */

#define CFG_FLASH_BASE		PHYS_FLASH_1

/*-----------------------------------------------------------------------
 * Nor-FLASH and environment organization
 */

#define CFG_NO_FLASH			/* xyd2440 demo board don't mount Nor-Flash*/

#if 0
#define CONFIG_AMD_LV400	1       /* uncomment this if you have a LV400 flash */
#endif
#define CONFIG_AMD_LV800	1       /* uncomment this if you have a LV800 flash */


#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#ifdef CONFIG_AMD_LV800
#define PHYS_FLASH_SIZE		0x00200000 /* 1MB */
#define CFG_MAX_FLASH_SECT	(19)	/* max number of sectors on one chip */
#define CFG_ENV_ADDR		(CFG_FLASH_BASE + 0x0F0000) /* addr of environment */
#endif
#ifdef CONFIG_AMD_LV400
#define PHYS_FLASH_SIZE		0x00080000 /* 512KB */
#define CFG_MAX_FLASH_SECT	(11)	/* max number of sectors on one chip */
#define CFG_ENV_ADDR		(CFG_FLASH_BASE + 0x070000) /* addr of environment */
#endif

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(5*CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(5*CFG_HZ) /* Timeout for Flash Write */

//#define	CFG_ENV_IS_IN_FLASH	1
//#define CFG_ENV_SIZE		0x10000	/* Total Size of Environment Sector */

/*-----------------------------------------------------------------------
 * Nand-FLASH and environment organization
 */

#define	CFG_ENV_IS_IN_NAND	1
#define CFG_NAND_BASE           0
#define CFG_MAX_NAND_DEVICE     1
#define NAND_MAX_CHIPS          1
#define CFG_ENV_OFFSET          0x40000
#define CFG_ENV_SIZE		0x10000	/* Total Size of Environment Sector */


/*-----------------------------------------------------------------------
 * Extra environment settings
 */

//#define CONFIG_EXTRA_ENV_SETTINGS		\
	"erase-env=nand erase 40000 20000\0"	\
	"bootargs=root=/dev/mtdblock2 rw console=ttySAC0,115200\0"	\
	"bootcmd=nand read.i 0x32000000 0x60000 0x200000; bootm 0x32000000\0"	\
	"install-bootloader=tftp 30000000 u-boot.bin;nand erase 0 40000;nand write 30000000 0 40000\0"		\
	"install-kernel=tftp 30000000 uImage;nand erase 60000 200000;nand write.i 30000000 60000 $(filesize)\0"	\
	"install-filesystem=tftp 30000000 filesystem.yaffs;nand erase 260000 3d9c000;nand write.yaffs 30000000 260000 $(filesize)\0"	\


#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS

#endif	/* __CONFIG_H */
