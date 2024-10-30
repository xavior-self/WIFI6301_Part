#define	_MAIN_GLOBAL_
/*********************************************************************************************************
  头文件                           
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"                                                    /* LPC17xx外设寄存器            */
#define extern
#include "main.h"
#include "uart.h"
#include "othercfg.h"
#include "LEDblink.h"
#include "SPI.h"
#include "I2CINT.h"
#include "key.h"
#include "camera.h"
#include "TAS3108.h"
#include "cmd.h"
#include "window.h"
#include "EtheApp.h"

//uint8_t	iar_temp1;
//uint8_t	iar_temp2;
//uint8_t	iar_temp3;
//uint8_t	iar_temp4;
//uint8_t	iar_temp5;
//uint8_t	iar_temp6;
//uint8_t	iar_temp7;
//uint8_t	iar_temp8;

//uint8_t	iar_temp[12];
uint8_t	iar_temp[8];

typedef struct{
	uint8_t	download;

	uint8_t	CRC1;
	uint8_t	CRC2;
	uint8_t	CRC;
}App_cSysInfo;
App_cSysInfo App_boot_SysInfo;
App_cSysInfo	*App_boot_pSysInfo;

#define FLASH_APP_ADDR	0x60000    //26

#define BAUD_RATE					115200
#define FIO2DIR						(*(volatile unsigned long *)0x2009c040)
#define FIO2PIN						(*(volatile unsigned long *)0x2009c054)

#define LED_ON    {FIO2DIR=FIO2DIR|(1<<5);FIO2PIN=FIO2PIN &(~(1<<5));}
#define LED_OFF   {FIO2DIR=FIO2DIR|(1<<5);FIO2PIN=FIO2PIN | (1<<5)  ;}

#define	IAP_CMD_COPY_RAM_TO_FLASH		51
/* IAP boot ROM location and access function */
#define IAP_ROM_LOCATION				0x1FFF1FF1UL
#define IAP_EXECUTE_CMD(a, b)			((void (*)())(IAP_ROM_LOCATION))(a, b)


/*******************************************************************************************************
** Function name:	u32IAP_CopyRAMToFlash
**
** Description:		Program the flash memory with data stored in RAM.
**
** Parameters:	   	u32DstAddr - Destination Flash address, should be a 256
**                               byte boundary.
**			 		u32SrcAddr - Source RAM address, should be a word boundary
**			 		u32Len     - Number of 8-bit bytes to write, must be a
**			 					 multiple of 256.
*
** Returned value:	Status code returned by IAP ROM function.
**
********************************************************************************************************/
uint32_t u32IAP_CopyRAMToFlash(uint32_t u32DstAddr, uint32_t u32SrcAddr, uint32_t u32Len)
{
	uint32_t au32Result[3];
	uint32_t au32Command[5];

	au32Command[0] = IAP_CMD_COPY_RAM_TO_FLASH;
	au32Command[1] = u32DstAddr;
	au32Command[2] = u32SrcAddr;
	au32Command[3] = u32Len;
	au32Command[4] = SystemFrequency / 1000UL;	                                /* Core clock frequency in kHz  */

	IAP_EXECUTE_CMD(au32Command, au32Result);

	return au32Result[0];
}

//uint8_t DEBUG_message[] = "Save Parameter!\r\n";
uint8_t DEBUG_Success[] = "Write flag Success!\r\n";
//uint8_t DEBUG_Failed[] = "Write flag Failed!\r\n";

void SaveParameter(void)
{
   uint8_t result;

   result = getCRC8((uint8_t *)App_boot_pSysInfo, sizeof(App_cSysInfo)-1);
   if(App_boot_pSysInfo -> CRC!=result)
    {   
    	#ifdef DEBUG 
		     //vUARTSend(DEBUG_message, sizeof(DEBUG_message));
			 #endif
      App_boot_pSysInfo -> CRC=result; 
      result = WriteFlash(FLASH_APP_ADDR, (uint32_t)App_boot_pSysInfo ,512);	//17408   16384
			//u32IAP_CopyRAMToFlash(FLASH_APP_ADDR, (uint32_t)App_boot_pSysInfo, sizeof(App_cSysInfo));
			if(!(result)){
					//vUARTSendChar(result);
					vUARTSend(DEBUG_Success, sizeof(DEBUG_Success));
			}
			else{
				//vUARTSend(DEBUG_Failed, sizeof(DEBUG_Failed));
			}
    }
}

/*********************************************************************************************************
函数名称: WindowInit
输入说明: 主变量初始化
*********************************************************************************************************/
void WindowInit(void)
{
	ChooseNum 	= 1; CurrentPage	= 0; BeInFlag  = 0;
	RefreshFlag	= 1; EnterFlag		= 0; ParaIndex = 1;
	/*MODE*/
	MenuWindow[mode].currentWindow  = &DisplayMicModeWnd;					/* Mic Mode menu	*/
	MenuWindow[mode].nextFlag		= 0;									/* 没有下级菜单 	*/
	MenuWindow[mode].currentPara	= &SysInfo.MicMode;						/* 参数			 	*/
	MenuWindow[mode].paraRange		= 3;									/* 参数范围			*/
	MenuWindow[mode].modifyType		= ModeType;								/* 修改MODE类型		*/
	MenuWindow[mode].enterFlag		= 0;									/*进入下级菜单标志	*/
	/*ACTIVE*/
	MenuWindow[active].currentWindow = &DisplayActiveMicNumWnd;				/* Active Mode menu */
	MenuWindow[active].nextFlag		 = 0;									/* 没有下级菜单  	*/
	MenuWindow[active].currentPara	 = &MicNum;								/* 参数要转换SysInfo.SpkNum值*/
	MenuWindow[active].paraRange	 = 3;									/* 参数范围		 	*/
	MenuWindow[active].modifyType	 = ActiveType;							/* 修改Active类型	*/
	MenuWindow[active].enterFlag	 = 0;									/*进入当前菜单标志	*/
	/*VOLUME*/
	MenuWindow[volume].currentWindow = &DisplayVolumeWnd;					/* volume menu 		*/
	MenuWindow[volume].nextFlag		 = 1;									/*  有下级菜单  	*/
	MenuWindow[volume].currentPara	 = &ChooseNum;							/* 参数				*/
	MenuWindow[volume].paraRange	 = 6;									/* 参数范围		 	*/
	MenuWindow[volume].modifyType	 = NoneType;							/* 左右键修改类型	*/
	MenuWindow[volume].enterFlag	 = 0;									/* 进入当前菜单标志	*/
	MenuWindow[volume].nextPage		 = VolumeMenu;							/* 指向子菜单结构体 */
														  					/*此结构体指针在选择菜单入口时需要转换*/
	VolumeMenu[linein].currentWindow = &DisplayLineIn;						/* 线入显示函数		*/
	VolumeMenu[linein].nextFlag		 = 0;									/* 没有下级菜单		*/
	VolumeMenu[linein].currentPara	 = &SysInfo.LineInVol;					/* 参数				*/
	VolumeMenu[linein].paraRange	 = 41;									/* 线入音量大小		*/
	VolumeMenu[linein].modifyType	 = LineinType;							/* 修改线入类型		*/
	VolumeMenu[linein].enterFlag	 = 0;

	VolumeMenu[lineout].currentWindow = &DisplayLineOut;					/* 线出显示函数		*/
	VolumeMenu[lineout].nextFlag	  = 0;									/* 没有下级菜单		*/
	VolumeMenu[lineout].currentPara	  = &SysInfo.LinOutVol;					/* 参数				*/
	VolumeMenu[lineout].paraRange	  = 41;									/* 线出音量大小		*/
	VolumeMenu[lineout].modifyType	  = LineoutType;						/* 修改线出类型		*/
	VolumeMenu[lineout].enterFlag	  = 0;

	VolumeMenu[unit].currentWindow = &DisplayUnit;							/* 单元线出显示函数	*/
	VolumeMenu[unit].nextFlag	   = 0;										/* 没有下级菜单		*/
	VolumeMenu[unit].currentPara   = &SysInfo.UintOutVol;					/* 参数				*/
	VolumeMenu[unit].paraRange	   = 41;									/* 	音量大小		*/
	VolumeMenu[unit].modifyType	   = UnitoutType;							/* 修改单元线出		*/
	VolumeMenu[unit].enterFlag	   = 0;

	VolumeMenu[treble].currentWindow = &DisplayTreble;						/* 高音显示函数		*/
	VolumeMenu[treble].nextFlag	     = 0;									/* 没有下级菜单		*/
	VolumeMenu[treble].currentPara   = &SysInfo.TrebleVol;					/* 参数				*/
	VolumeMenu[treble].paraRange	 = 24;									/* 	音量大小		*/
	VolumeMenu[treble].modifyType	 = TrebleType;							/* 修改高音			*/
	VolumeMenu[treble].enterFlag	 = 0;

	VolumeMenu[bass].currentWindow	= &DisplayBass;							/* 低音显示函数		*/
	VolumeMenu[bass].nextFlag		= 0;									/* 没有下级菜单		*/
	VolumeMenu[bass].currentPara   	= &SysInfo.BassVol;						/* 参数				*/
	VolumeMenu[bass].paraRange		= 24;									/* 	音量大小		*/
	VolumeMenu[bass].modifyType		= BassType;								/* 修改低音			*/
	VolumeMenu[bass].enterFlag		= 0;

	VolumeMenu[remotein].currentWindow = &DisplayRemote;						/* 远程会议显示函数	*/
	VolumeMenu[remotein].nextFlag	   = 0;									/* 没有下级菜单		*/
	VolumeMenu[remotein].currentPara   = &SysInfo.LinkOutVol;				/* 参数				*/
	VolumeMenu[remotein].paraRange	   = 41;								/* 	音量大小		*/
	VolumeMenu[remotein].modifyType	   = remoteType;						/* 修改远程音量		*/
	VolumeMenu[remotein].enterFlag	   = 0;

	VolumeMenu[speaker].currentWindow = &DisplaySpeaker;					/* 喇叭显示函数		*/
	VolumeMenu[speaker].nextFlag	  = 0;									/* 没有下级菜单		*/
	VolumeMenu[speaker].currentPara   = NULL;								/*U_REG_Default1.U_REG_Default1.Speaker*/
	VolumeMenu[speaker].paraRange	 = 1;									/* 	音量大小		*/
	VolumeMenu[speaker].modifyType	 = SpeakerType;							/* 修改喇叭			*/
	VolumeMenu[speaker].enterFlag	 = 0;
	/*EQ*/
	MenuWindow[eq].currentWindow 	= &DisplayEQWnd;						/* EQ		 menu	*/
	MenuWindow[eq].nextFlag			= 1;									/* 没有下级菜单 	*/
	MenuWindow[eq].currentPara		= &ChooseNum;							/* 参数			 	*/
	MenuWindow[eq].paraRange		= 2;									/* 参数范围			*/
	MenuWindow[eq].modifyType		= NoneType;								/* 没有修改类型		*/
	MenuWindow[eq].enterFlag		= 0;
	MenuWindow[eq].nextPage		 	= EQMenu; 

	EQMenu[effet].currentWindow 	= &DisplayEffect;						/* 音效显示函数		*/
	EQMenu[effet].nextFlag	  		= 0;									/* 没有下级菜单		*/
	EQMenu[effet].currentPara   	= &ParaIndex;							/* ENTER->SysInfo.EQSelect*/
	EQMenu[effet].paraRange	 		= 8;									/* 	8个选项			*/
	EQMenu[effet].modifyType	 	= NoneType;								/* 修改音效			*/
	EQMenu[effet].enterFlag	 		= 0;

	EQMenu[custom].currentWindow 	= &DisplayEqCustom;						/* 音效自定义显示	*/
	EQMenu[custom].nextFlag	  		= 1;									/* 没有下级菜单		*/
	EQMenu[custom].currentPara   	= &ParaIndex;				
	EQMenu[custom].paraRange	 	= 7;									/* 	8个选项			*/
	EQMenu[custom].modifyType	 	= NoneType;								/* 修改				*/
	EQMenu[custom].enterFlag	 	= 0;
	EQMenu[custom].nextPage		 	= SoundCustomMenu;

	SoundCustomMenu[band1].currentWindow = &DisplayBand;
	SoundCustomMenu[band1].enterFlag	 = 0;								/* 没有下级菜单		*/
	SoundCustomMenu[band1].currentPara	 = &SysInfo.Band1;					/* 均衡器 1段		*/
	SoundCustomMenu[band1].paraRange	 = 41;
	SoundCustomMenu[band1].modifyType	 = Band1Type;

	SoundCustomMenu[band2].currentWindow = &DisplayBand;
	SoundCustomMenu[band2].enterFlag	 = 0;								/* 没有下级菜单		*/
	SoundCustomMenu[band2].currentPara	 = &SysInfo.Band2;					/* 均衡器 2段		*/
	SoundCustomMenu[band2].paraRange	 = 41;
	SoundCustomMenu[band2].modifyType	 = Band2Type;

	SoundCustomMenu[band3].currentWindow = &DisplayBand;
	SoundCustomMenu[band3].enterFlag	 = 0;								/* 没有下级菜单		*/
	SoundCustomMenu[band3].currentPara	 = &SysInfo.Band3;					/* 均衡器 3段		*/
	SoundCustomMenu[band3].paraRange	 = 41;
	SoundCustomMenu[band3].modifyType	 = Band3Type;

	SoundCustomMenu[band4].currentWindow = &DisplayBand;
	SoundCustomMenu[band4].enterFlag	 = 0;								/* 没有下级菜单		*/
	SoundCustomMenu[band4].currentPara	 = &SysInfo.Band4;					/* 均衡器 4段		*/
	SoundCustomMenu[band4].paraRange	 = 41;
	SoundCustomMenu[band4].modifyType	 = Band4Type;

	SoundCustomMenu[band5].currentWindow = &DisplayBand;
	SoundCustomMenu[band5].enterFlag	 = 0;								/* 没有下级菜单		*/
	SoundCustomMenu[band5].currentPara	 = &SysInfo.Band5;					/* 均衡器 5段		*/
	SoundCustomMenu[band5].paraRange	 = 41;
	SoundCustomMenu[band5].modifyType	 = Band5Type;

	SoundCustomMenu[band6].currentWindow = &DisplayBand;
	SoundCustomMenu[band6].enterFlag	 = 0;								/* 没有下级菜单		*/
	SoundCustomMenu[band6].currentPara	 = &SysInfo.Band6;					/* 均衡器 6段		*/
	SoundCustomMenu[band6].paraRange	 = 41;
	SoundCustomMenu[band6].modifyType	 = Band6Type;

	SoundCustomMenu[band7].currentWindow = &DisplayBand;
	SoundCustomMenu[band7].enterFlag	 = 0;								/* 没有下级菜单		*/
	SoundCustomMenu[band7].currentPara	 = &SysInfo.Band7;					/* 均衡器 7段		*/
	SoundCustomMenu[band7].paraRange	 = 41;
	SoundCustomMenu[band7].modifyType	 = Band7Type;
	/*ZONE*/
	MenuWindow[zone].currentWindow = &DisplayZoneWnd;						/* zone menu 		*/
	MenuWindow[zone].nextFlag	   = 1;										/*  有下级菜单  	*/
	MenuWindow[zone].currentPara   = &ChooseNum;							/* 参数				*/
	MenuWindow[zone].paraRange	   = 5;										/* 参数范围		 	*/
	MenuWindow[zone].modifyType	   = NoneType;								/* 左右键修改类型	*/
	MenuWindow[zone].enterFlag	   = 0;										/* 进入当前菜单标志	*/
	MenuWindow[zone].nextPage	   = ZoneMenu;

	ZoneMenu[speaker1].currentWindow	= &DisplayZonePage;					/* ZONE1显示		*/
	ZoneMenu[speaker1].nextFlag	  		= 1;								/* 没有下级菜单		*/
	ZoneMenu[speaker1].currentPara   	= &ParaIndex;				
	ZoneMenu[speaker1].paraRange	 	= 5;								/* 	8个选项			*/
	ZoneMenu[speaker1].modifyType	 	= NoneType;							/* 修改				*/
	ZoneMenu[speaker1].enterFlag	 	= 0;
	ZoneMenu[speaker1].nextPage			= zone1Page;

	zone1Page[endid].currentWindow  = &DisplayZoneID;
	zone1Page[endid].enterFlag	    = 0;									/* 没有下级菜单		    */
	zone1Page[endid].currentPara	= NULL;									/*参数范围为4000特殊处理*/
	zone1Page[endid].paraRange	 	= 0;									
	zone1Page[endid].modifyType	 	= Zone1IDType;

	zone1Page[gain01].currentWindow	= &DisplayZoneGain1;
	zone1Page[gain01].enterFlag		= 0;									/* 没有下级菜单		  */
	zone1Page[gain01].currentPara	= &HornZone.SoundZone[0].gain1;			/* 4个增益参数特殊处理*/
	zone1Page[gain01].paraRange		= 41;								
	zone1Page[gain01].modifyType	= Zone1GainType;
	zone1Page[gain02].currentWindow	= &DisplayZoneGain1;
	zone1Page[gain02].enterFlag		= 0;									/* 没有下级菜单		  */
	zone1Page[gain02].currentPara	= &HornZone.SoundZone[1].gain1;			/* 4个增益参数特殊处理*/
	zone1Page[gain02].paraRange		= 41;								
	zone1Page[gain02].modifyType	= Zone1GainType;
	zone1Page[gain03].currentWindow	= &DisplayZoneGain1;
	zone1Page[gain03].enterFlag		= 0;									/* 没有下级菜单		  */
	zone1Page[gain03].currentPara	= &HornZone.SoundZone[2].gain1;			/* 4个增益参数特殊处理*/
	zone1Page[gain03].paraRange		= 41;								
	zone1Page[gain03].modifyType	= Zone1GainType;
	zone1Page[gain04].currentWindow	= &DisplayZoneGain1;
	zone1Page[gain04].enterFlag		= 0;									/* 没有下级菜单		  */
	zone1Page[gain04].currentPara	= &HornZone.SoundZone[3].gain1;			/* 4个增益参数特殊处理*/
	zone1Page[gain04].paraRange		= 41;								
	zone1Page[gain04].modifyType	= Zone1GainType;

	ZoneMenu[speaker2].currentWindow	= &DisplayZonePage;					/* ZONE2显示		*/
	ZoneMenu[speaker2].nextFlag	  		= 1;								/* 没有下级菜单		*/
	ZoneMenu[speaker2].currentPara   	= &ParaIndex;				
	ZoneMenu[speaker2].paraRange	 	= 5;								/* 	8个选项			*/
	ZoneMenu[speaker2].modifyType	 	= NoneType;							/* 修改				*/
	ZoneMenu[speaker2].enterFlag	 	= 0;
	ZoneMenu[speaker2].nextPage			= zone2Page;

	zone2Page[endid].currentWindow  = &DisplayZoneID;
	zone2Page[endid].enterFlag	    = 0;									/* 没有下级菜单		    */
	zone2Page[endid].currentPara	= NULL;									/*参数范围为4000特殊处理*/
	zone2Page[endid].paraRange	 	= 0;									
	zone2Page[endid].modifyType	 	= Zone2IDType;

	zone2Page[gain01].currentWindow	= &DisplayZoneGain2;
	zone2Page[gain01].enterFlag		= 0;									/* 没有下级菜单		  */
	zone2Page[gain01].currentPara	= &HornZone.SoundZone[0].gain2;			/* 4个增益参数特殊处理*/
	zone2Page[gain01].paraRange		= 41;								
	zone2Page[gain01].modifyType	= Zone2GainType;
	zone2Page[gain02].currentWindow	= &DisplayZoneGain2;
	zone2Page[gain02].enterFlag		= 0;									/* 没有下级菜单		  */
	zone2Page[gain02].currentPara	= &HornZone.SoundZone[1].gain2;			/* 4个增益参数特殊处理*/
	zone2Page[gain02].paraRange		= 41;								
	zone2Page[gain02].modifyType	= Zone2GainType;
	zone2Page[gain03].currentWindow	= &DisplayZoneGain2;
	zone2Page[gain03].enterFlag		= 0;									/* 没有下级菜单		  */
	zone2Page[gain03].currentPara	= &HornZone.SoundZone[2].gain2;			/* 4个增益参数特殊处理*/
	zone2Page[gain03].paraRange		= 41;								
	zone2Page[gain03].modifyType	= Zone2GainType;
	zone2Page[gain04].currentWindow	= &DisplayZoneGain2;
	zone2Page[gain04].enterFlag		= 0;									/* 没有下级菜单		  */
	zone2Page[gain04].currentPara	= &HornZone.SoundZone[3].gain2;			/* 4个增益参数特殊处理*/
	zone2Page[gain04].paraRange		= 41;								
	zone2Page[gain04].modifyType	= Zone2GainType;

	ZoneMenu[speaker3].currentWindow	= &DisplayZonePage;					/* ZONE3显示		*/
	ZoneMenu[speaker3].nextFlag	  		= 1;								/* 没有下级菜单		*/
	ZoneMenu[speaker3].currentPara   	= &ParaIndex;				
	ZoneMenu[speaker3].paraRange	 	= 5;								/* 	8个选项			*/
	ZoneMenu[speaker3].modifyType	 	= NoneType;							/* 修改				*/
	ZoneMenu[speaker3].enterFlag	 	= 0;
	ZoneMenu[speaker3].nextPage			= zone3Page;

	zone3Page[endid].currentWindow  = &DisplayZoneID;
	zone3Page[endid].enterFlag	    = 0;									/* 没有下级菜单		    */
	zone3Page[endid].currentPara	= NULL;									/*参数范围为4000特殊处理*/
	zone3Page[endid].paraRange	 	= 0;									
	zone3Page[endid].modifyType	 	= Zone3IDType;

	zone3Page[gain01].currentWindow	= &DisplayZoneGain3;
	zone3Page[gain01].enterFlag		= 0;									/* 没有下级菜单		  */
	zone3Page[gain01].currentPara	= &HornZone.SoundZone[0].gain3;			/* 4个增益参数特殊处理*/
	zone3Page[gain01].paraRange		= 41;								
	zone3Page[gain01].modifyType	= Zone3GainType;
	zone3Page[gain02].currentWindow	= &DisplayZoneGain3;
	zone3Page[gain02].enterFlag		= 0;									/* 没有下级菜单		  */
	zone3Page[gain02].currentPara	= &HornZone.SoundZone[1].gain3;			/* 4个增益参数特殊处理*/
	zone3Page[gain02].paraRange		= 41;								
	zone3Page[gain02].modifyType	= Zone3GainType;
	zone3Page[gain03].currentWindow	= &DisplayZoneGain3;
	zone3Page[gain03].enterFlag		= 0;									/* 没有下级菜单		  */
	zone3Page[gain03].currentPara	= &HornZone.SoundZone[2].gain3;			/* 4个增益参数特殊处理*/
	zone3Page[gain03].paraRange		= 41;								
	zone3Page[gain03].modifyType	= Zone3GainType;
	zone3Page[gain04].currentWindow	= &DisplayZoneGain3;
	zone3Page[gain04].enterFlag		= 0;									/* 没有下级菜单		  */
	zone3Page[gain04].currentPara	= &HornZone.SoundZone[3].gain3;			/* 4个增益参数特殊处理*/
	zone3Page[gain04].paraRange		= 41;								
	zone3Page[gain04].modifyType	= Zone3GainType;

	ZoneMenu[speaker4].currentWindow	= &DisplayZonePage;					/* ZONE4显示		*/
	ZoneMenu[speaker4].nextFlag	  		= 1;								/* 没有下级菜单		*/
	ZoneMenu[speaker4].currentPara   	= &ParaIndex;				
	ZoneMenu[speaker4].paraRange	 	= 2;								/* 	8个选项			*/
	ZoneMenu[speaker4].modifyType	 	= NoneType;							/* 修改				*/
	ZoneMenu[speaker4].enterFlag	 	= 0;								
	ZoneMenu[speaker4].nextPage			= zone4Page;

	zone4Page[endid].currentWindow  = &DisplayZoneID;
	zone4Page[endid].enterFlag	    = 0;									/* 没有下级菜单		    */
	zone4Page[endid].currentPara	= NULL;									/*参数范围为4000特殊处理*/
	zone4Page[endid].paraRange	 	= 0;									
	zone4Page[endid].modifyType	 	= Zone4IDType;

	zone4Page[gain01].currentWindow	= &DisplayZoneGain4;
	zone4Page[gain01].enterFlag		= 0;										/* 没有下级菜单		  */
	zone4Page[gain01].currentPara	= &HornZone.SoundZone[0].gain4;			/* 4个增益参数特殊处理*/
	zone4Page[gain01].paraRange		= 41;								
	zone4Page[gain01].modifyType	= Zone4GainType;
	zone4Page[gain02].currentWindow	= &DisplayZoneGain4;
	zone4Page[gain02].enterFlag		= 0;										/* 没有下级菜单		  */
	zone4Page[gain02].currentPara	= &HornZone.SoundZone[1].gain4;			/* 4个增益参数特殊处理*/
	zone4Page[gain02].paraRange		= 41;								
	zone4Page[gain02].modifyType	= Zone4GainType;
	zone4Page[gain03].currentWindow	= &DisplayZoneGain4;
	zone4Page[gain03].enterFlag		= 0;										/* 没有下级菜单		  */
	zone4Page[gain03].currentPara	= &HornZone.SoundZone[2].gain4;			/* 4个增益参数特殊处理*/
	zone4Page[gain03].paraRange		= 41;								
	zone4Page[gain03].modifyType	= Zone4GainType;
	zone4Page[gain04].currentWindow	= &DisplayZoneGain4;
	zone4Page[gain04].enterFlag		= 0;										/* 没有下级菜单		  */
	zone4Page[gain04].currentPara	= &HornZone.SoundZone[3].gain4;			/* 4个增益参数特殊处理*/
	zone4Page[gain04].paraRange		= 41;								
	zone4Page[gain04].modifyType	= Zone4GainType;

	ZoneMenu[zonesw].currentWindow	= &DisplaySwPage;						/* ZONE SW显示		*/
	ZoneMenu[zonesw].nextFlag	  	= 1;									/* 没有下级菜单		*/
	ZoneMenu[zonesw].currentPara   	= &ParaIndex;				
	ZoneMenu[zonesw].paraRange	 	= 2;								
	ZoneMenu[zonesw].modifyType	 	= NoneType;								/* 修改类型			*/
	ZoneMenu[zonesw].enterFlag	 	= 0;
	ZoneMenu[zonesw].nextPage		= zoneSw;

	zoneSw[zoonswth].currentWindow	= &DisplayZoneSW;
	zoneSw[zoonswth].enterFlag		= 0;										
	zoneSw[zoonswth].currentPara	= NULL;			
	zoneSw[zoonswth].paraRange		= 0;								
	zoneSw[zoonswth].modifyType		= ZoneSWType;

	zoneSw[unitanc].currentWindow	= &DisplayUnitANC;
	zoneSw[unitanc].enterFlag		= 0;										
	zoneSw[unitanc].currentPara		= NULL;			
	zoneSw[unitanc].paraRange		= 0;								
	zoneSw[unitanc].modifyType		= ZoneSWType;

	zoneSw[zoondsp].currentWindow	= &DisplayZoneDsp;
	zoneSw[zoondsp].enterFlag		= 0;										
	zoneSw[zoondsp].currentPara		= NULL;			
	zoneSw[zoondsp].paraRange		= 0;								
	zoneSw[zoondsp].modifyType		= ZoneSWType;

	zoneSw[zoondsp2].currentWindow	= &DisplayZoneDsp2;
	zoneSw[zoondsp2].enterFlag		= 0;										
	zoneSw[zoondsp2].currentPara	= NULL;			
	zoneSw[zoondsp2].paraRange		= 0;								
	zoneSw[zoondsp2].modifyType		= ZoneSWType;

	ZoneMenu[remoteout].currentWindow	= &DisplayRemoteOut;				/* ZONE SW显示		*/
	ZoneMenu[remoteout].nextFlag	  	= 0;								/* 没有下级菜单		*/
	ZoneMenu[remoteout].currentPara   	= &ParaIndex;				
	ZoneMenu[remoteout].paraRange	 	= 4;								
	ZoneMenu[remoteout].modifyType	 	= NoneType;							/* 修改类型			*/
	ZoneMenu[remoteout].enterFlag	 	= 0;

	/*UNIT SCAN*/
	MenuWindow[manage].currentWindow = &DisplayUnitScanWnd;					/* unit scan menu	*/
	MenuWindow[manage].nextFlag		 = 1;									/* 没有下级菜单 	*/
	MenuWindow[manage].currentPara	 = NULL;								/* 参数			 	*/
	MenuWindow[manage].paraRange	 = 0;									/* 参数范围			*/
	MenuWindow[manage].modifyType	 = ScanType;							/* 修改				*/
	MenuWindow[manage].enterFlag	 = 0;
	/*NET WORK*/
	MenuWindow[net].currentWindow = &DisplayNetWnd;							/* net work menu	*/
	MenuWindow[net].nextFlag	  = 1;								
	MenuWindow[net].currentPara	  = &ChooseNum;								/* 参数			 	*/
	MenuWindow[net].paraRange	  = 2;										/* 参数范围			*/
	MenuWindow[net].modifyType	  = NoneType;								/* 修改				*/
	MenuWindow[net].enterFlag	  = 0;
	MenuWindow[net].nextPage	  = NetMenu;

	NetMenu[aip].currentWindow	= &DisplayIPPage;							/* IP设置			 */							
	NetMenu[aip].nextFlag	  	= 1;										
	NetMenu[aip].currentPara   	= &ParaIndex;				
	NetMenu[aip].paraRange	 	= 5;										
	NetMenu[aip].modifyType	 	= NoneType;									
	NetMenu[aip].enterFlag	 	= 0;
	NetMenu[aip].nextPage		= ipPage;

	ipPage[ip1].currentWindow  = &DisplayIPOption;
	ipPage[ip1].enterFlag	   = 0;										
	ipPage[ip1].currentPara	   = &NetSetBuff[0];									
	ipPage[ip1].paraRange	   = 255;								
	ipPage[ip1].modifyType	   = IpType;

	ipPage[ip2].currentWindow  = &DisplayIPOption;
	ipPage[ip2].enterFlag	   = 0;										
	ipPage[ip2].currentPara	   = &NetSetBuff[1];									
	ipPage[ip2].paraRange	   = 255;								
	ipPage[ip2].modifyType	   = IpType;

	ipPage[ip3].currentWindow  = &DisplayIPOption;
	ipPage[ip3].enterFlag	   = 0;										
	ipPage[ip3].currentPara	   = &NetSetBuff[2];									
	ipPage[ip3].paraRange	   = 255;								
	ipPage[ip3].modifyType	   = IpType;

	ipPage[ip4].currentWindow  = &DisplayIPOption;
	ipPage[ip4].enterFlag	   = 0;										
	ipPage[ip4].currentPara	   = &NetSetBuff[3];									
	ipPage[ip4].paraRange	   = 255;								
	ipPage[ip4].modifyType	   = IpType;

	ipPage[ipok].currentWindow = &DisplayNETOk;
	ipPage[ipok].enterFlag	   = 0;										
	ipPage[ipok].currentPara   = NULL;									
	ipPage[ipok].paraRange	   = 0;								
	ipPage[ipok].modifyType	   = NoneType;

	NetMenu[amac].currentWindow	= &DisplayMACPage;							/* MAC设置			 */							
	NetMenu[amac].nextFlag	  	= 1;										
	NetMenu[amac].currentPara   = &ParaIndex;				
	NetMenu[amac].paraRange	 	= 5;									
	NetMenu[amac].modifyType	= NoneType;								
	NetMenu[amac].enterFlag	 	= 0;
	NetMenu[amac].nextPage		= macPage;

	macPage[mac1].currentWindow = &DisplayMACOption;
	macPage[mac1].enterFlag	    = 0;										
	macPage[mac1].currentPara	= &MACAddrBuff[0];									
	macPage[mac1].paraRange	    = 255;								
	macPage[mac1].modifyType	= MacType;

	macPage[mac2].currentWindow = &DisplayMACOption;
	macPage[mac2].enterFlag	    = 0;										
	macPage[mac2].currentPara	= &MACAddrBuff[1];									
	macPage[mac2].paraRange	    = 255;								
	macPage[mac2].modifyType	= MacType;

	macPage[mac3].currentWindow = &DisplayMACOption;
	macPage[mac3].enterFlag	    = 0;										
	macPage[mac3].currentPara	= &MACAddrBuff[2];									
	macPage[mac3].paraRange	    = 255;								
	macPage[mac3].modifyType	= MacType;

	macPage[mac4].currentWindow = &DisplayMACOption;
	macPage[mac4].enterFlag	    = 0;										
	macPage[mac4].currentPara	= &MACAddrBuff[3];									
	macPage[mac4].paraRange	    = 255;								
	macPage[mac4].modifyType	= MacType;

	macPage[mac5].currentWindow = &DisplayMACOption;
	macPage[mac5].enterFlag	    = 0;										
	macPage[mac5].currentPara	= &MACAddrBuff[4];									
	macPage[mac5].paraRange	    = 255;								
	macPage[mac5].modifyType	= MacType;

	macPage[mac6].currentWindow = &DisplayMACOption;
	macPage[mac6].enterFlag	    = 0;										
	macPage[mac6].currentPara	= &MACAddrBuff[5];									
	macPage[mac6].paraRange	    = 255;								
	macPage[mac6].modifyType	= MacType;

	macPage[macok].currentWindow = &DisplayNETOk;
	macPage[macok].enterFlag	 = 0;										
	macPage[macok].currentPara	 = NULL;									
	macPage[macok].paraRange	 = 0;								
	macPage[macok].modifyType	 = NoneType;
	/*翻译间设置*/
	MenuWindow[trans].currentWindow = &DisplayTransWnd;						
	MenuWindow[trans].nextFlag		= 1;								
	MenuWindow[trans].currentPara	= &ChooseNum;								/* 参数			 	*/
	MenuWindow[trans].paraRange		= 5;										/* 参数范围			*/
	MenuWindow[trans].modifyType	= NoneType;									/* 修改				*/
	MenuWindow[trans].enterFlag		= 0;
	MenuWindow[trans].nextPage		= TransMenu;

	TransMenu[chnlsum].currentWindow	= &DisplayChannelNum;					/* CHANNEL SUM		 */							
	TransMenu[chnlsum].nextFlag			= 0;										
	TransMenu[chnlsum].currentPara   	= &SysInfo.InterChanTotalNum;				
	TransMenu[chnlsum].paraRange	 	= 32;										
	TransMenu[chnlsum].modifyType	 	= ChannelsumType;									
	TransMenu[chnlsum].enterFlag	 	= 0;

	TransMenu[chnllang].currentWindow	= &DisplayChannelLnag;					/* CHANNEL LANG		 */							
	TransMenu[chnllang].nextFlag		= 0;										
	TransMenu[chnllang].currentPara   	= NULL;				
	TransMenu[chnllang].paraRange	 	= 0;										
	TransMenu[chnllang].modifyType	 	= ChannellangType;									
	TransMenu[chnllang].enterFlag	 	= 0;

	TransMenu[chnlauto].currentWindow	= &DisplayChannelAuto;					/* CHANNEL AUTO		 */							
	TransMenu[chnlauto].nextFlag		= 0;										
	TransMenu[chnlauto].currentPara   	= NULL;				
	TransMenu[chnlauto].paraRange	 	= 0;										
	TransMenu[chnlauto].modifyType	 	= ChannelautoType;									
	TransMenu[chnlauto].enterFlag	 	= 0;

	TransMenu[chnllock].currentWindow	= &DisplayChannelLock;					/* CHANNEL LOCK		 */							
	TransMenu[chnllock].nextFlag		= 0;										
	TransMenu[chnllock].currentPara   	= &InterRoomFlag;				
	TransMenu[chnllock].paraRange	 	= 1;										
	TransMenu[chnllock].modifyType	 	= ChannelLockType;									
	TransMenu[chnllock].enterFlag	 	= 0;

	TransMenu[chnlroom].currentWindow	= &DisplayRoomEntry;					/* CHANNEL ROOM		 */							
	TransMenu[chnlroom].nextFlag		= 1;										
	TransMenu[chnlroom].currentPara   	= &ParaIndex;				
	TransMenu[chnlroom].paraRange	 	= 2;										
	TransMenu[chnlroom].modifyType	 	= NoneType;									
	TransMenu[chnlroom].enterFlag	 	= 0;
	TransMenu[chnlroom].nextPage		= roomPage;

	roomPage[roomsum].currentWindow	= &DisplayRoomNUM;
	roomPage[roomsum].enterFlag		= 0;										
	roomPage[roomsum].currentPara	= &SysInfo.InterRoomNum;									
	roomPage[roomsum].paraRange		= 32;								
	roomPage[roomsum].modifyType	= RoomSumType;

	roomPage[roomset].currentWindow	= &DisplayStartRoom;
	roomPage[roomset].enterFlag		= 0;										
	roomPage[roomset].currentPara	= &InterRoomSet;									
	roomPage[roomset].paraRange		= 1;								
	roomPage[roomset].modifyType	= RoomSetType;				
	/*摄像头控制*/
	MenuWindow[camera].currentWindow = &DisplayCameraWnd;						
	MenuWindow[camera].nextFlag		 = 1;								
	MenuWindow[camera].currentPara	 = &ChooseNum;								
	MenuWindow[camera].paraRange	 = 3;										
	MenuWindow[camera].modifyType	 = NoneType;									
	MenuWindow[camera].enterFlag	 = 0;
	MenuWindow[camera].nextPage		 = CameraMenu;

	CameraMenu[camProtocol].currentWindow	= &DisplayCamProtocol;				/* Protocol		 */							
	CameraMenu[camProtocol].nextFlag		= 0;										
	CameraMenu[camProtocol].currentPara   	= NULL;				
	CameraMenu[camProtocol].paraRange	 	= 0;										
	CameraMenu[camProtocol].modifyType	 	= camProtlType;									
	CameraMenu[camProtocol].enterFlag	 	= 0;

	CameraMenu[cammap].currentWindow	= &DisplayCamMap;						/* map	 */							
	CameraMenu[cammap].nextFlag			= 1;										
	CameraMenu[cammap].currentPara   	= &ParaIndex;				
	CameraMenu[cammap].paraRange	 	= 0;										
	CameraMenu[cammap].modifyType	 	= NoneType;									
	CameraMenu[cammap].enterFlag	 	= 0;
	CameraMenu[cammap].nextPage			= mapPage;

	mapPage[camnum].currentWindow	= &DisplayMapCamNum;
	mapPage[camnum].enterFlag		= 0;										
	mapPage[camnum].currentPara		= &Camera_Serial;									
	mapPage[camnum].paraRange		= 16;								
	mapPage[camnum].modifyType		= cammapnumType;

	mapPage[camaddr].currentWindow	= &DisplayMapCamAddr;
	mapPage[camaddr].enterFlag		= 0;										
	mapPage[camaddr].currentPara	= NULL;									
	mapPage[camaddr].paraRange		= 0;								
	mapPage[camaddr].modifyType		= cammapaddrType;

	mapPage[cammatrix].currentWindow	= &DisplayMapMatrix;
	mapPage[cammatrix].enterFlag		= 0;										
	mapPage[cammatrix].currentPara		= NULL;									
	mapPage[cammatrix].paraRange		= 0;								
	mapPage[cammatrix].modifyType		= cammaparreyType;

	CameraMenu[camset].currentWindow	= &DisplayCameraSet;					/* set		 */							
	CameraMenu[camset].nextFlag			= 0;										
	CameraMenu[camset].currentPara   	= &Camera_Serial;				
	CameraMenu[camset].paraRange	 	= 255;										
	CameraMenu[camset].modifyType	 	= camsetType;									
	CameraMenu[camset].enterFlag	 	= 0;
	/*LANG*/
	MenuWindow[lang].currentWindow	= &DisplayLangWnd;						
	MenuWindow[lang].nextFlag		= 0;								
	MenuWindow[lang].currentPara	= &SysInfo.Languge;								
	MenuWindow[lang].paraRange		= 1;										
	MenuWindow[lang].modifyType		= langType;									
	MenuWindow[lang].enterFlag		= 0;
	/*MISC*/
	MenuWindow[misc].currentWindow   = &DisplayMiscWnd;					/* MISC menu 		*/
	MenuWindow[misc].nextFlag		 = 1;								/*  有下级菜单  	*/
	MenuWindow[misc].currentPara	 = &ChooseNum;						/* 参数				*/
	MenuWindow[misc].paraRange	 	 = 10;								/* 参数范围		 	*/
	MenuWindow[misc].modifyType	 	 = NoneType;						/* 左右键修改类型	*/
	MenuWindow[misc].enterFlag	 	 = 0;								/* 进入当前菜单标志	*/
	MenuWindow[misc].nextPage		 = MiscMenu;						/* 指向子菜单结构体 */
	/*DSP*/
	MiscMenu[dsp].currentWindow	= &DisplayDSPWnd;										
	MiscMenu[dsp].nextFlag		= 0;										
	MiscMenu[dsp].currentPara   = NULL;				
	MiscMenu[dsp].paraRange	 	= 2;										
	MiscMenu[dsp].modifyType	= NoneType;									
	MiscMenu[dsp].enterFlag	 	= 0;
	/*WIFI IR*/
	MiscMenu[ir].currentWindow		= &DisplayIRTrackWnd;						
	MiscMenu[ir].nextFlag			= 0;								
	MiscMenu[ir].currentPara		= NULL;								
	MiscMenu[ir].paraRange			= 1;										
	MiscMenu[ir].modifyType			= wifiirType;									
	MiscMenu[ir].enterFlag			= 0;
	/*WIFI DELAY*/
	MiscMenu[delaytime].currentWindow		= &DisplayDelayWnd;						
	MiscMenu[delaytime].nextFlag			= 0;								
	MiscMenu[delaytime].currentPara			= NULL;								
	MiscMenu[delaytime].paraRange			= 16;										
	MiscMenu[delaytime].modifyType			= NoneType;									
	MiscMenu[delaytime].enterFlag			= 0;
	/*WIFI POWER*/
	MiscMenu[savepower].currentWindow		= &DisplayPOWERWnd;						
	MiscMenu[savepower].nextFlag			= 0;								
	MiscMenu[savepower].currentPara			= NULL;								
	MiscMenu[savepower].paraRange			= 1;										
	MiscMenu[savepower].modifyType			= NoneType;									
	MiscMenu[savepower].enterFlag			= 0;
	/*COMPAT*/
	MiscMenu[compat].currentWindow		= &DisplayCompatWnd;						
	MiscMenu[compat].nextFlag			= 0;								
	MiscMenu[compat].currentPara		= NULL;								
	MiscMenu[compat].paraRange			= 2;										
	MiscMenu[compat].modifyType			= NoneType;									
	MiscMenu[compat].enterFlag			= 0;
	/*RF ASKT*/
	MiscMenu[autho].currentWindow		= &DisplayAuthoWnd;						
	MiscMenu[autho].nextFlag			= 0;								
	MiscMenu[autho].currentPara			= NULL;								
	MiscMenu[autho].paraRange			= 2;										
	MiscMenu[autho].modifyType			= NoneType;									
	MiscMenu[autho].enterFlag			= 0;
	/*ALC*/
	MiscMenu[alc].currentWindow			= &DisplayALCWnd;						
	MiscMenu[alc].nextFlag				= 0;								
	MiscMenu[alc].currentPara			= NULL;								
	MiscMenu[alc].paraRange				= 2;										
	MiscMenu[alc].modifyType			= NoneType;									
	MiscMenu[alc].enterFlag				= 0;
	/*IR LIGHT*/
	MiscMenu[light].currentWindow		= &DisplayIRLightWnd;						
	MiscMenu[light].nextFlag			= 0;								
	MiscMenu[light].currentPara			= NULL;								
	MiscMenu[light].paraRange			= 15;										
	MiscMenu[light].modifyType			= NoneType;									
	MiscMenu[light].enterFlag			= 0;
	/*scan LIGHT*/
	MiscMenu[scan].currentWindow		= &DisplayScanWnd;						
	MiscMenu[scan].nextFlag				= 0;								
	MiscMenu[scan].currentPara			= NULL;								
	MiscMenu[scan].paraRange			= 2;										
	MiscMenu[scan].modifyType			= NoneType;									
	MiscMenu[scan].enterFlag			= 0;
	/*DEFAULT*/
	MenuWindow[defaultpara].currentWindow	= &DisplayDefaultWnd;						
	MenuWindow[defaultpara].nextFlag		= 0;								
	MenuWindow[defaultpara].currentPara		= NULL;								
	MenuWindow[defaultpara].paraRange		= 0;										
	MenuWindow[defaultpara].modifyType		= defaultType;									
	MenuWindow[defaultpara].enterFlag		= 0;
	/*VERSION*/
	MenuWindow[version].currentWindow	= &DisplayVersionWnd;						
	MenuWindow[version].nextFlag		= 0;								
	MenuWindow[version].currentPara		= NULL;								
	MenuWindow[version].paraRange		= 0;										
	MenuWindow[version].modifyType		= NoneType;									
	MenuWindow[version].enterFlag		= 0;
}
/*********************************************************************************************************
函数名称: MainValInit
输入说明: 主变量初始化
*********************************************************************************************************/
void MainValInit(void)
{
	uint16 i;
	DonwLoader = 0;			//下载更新代码标识

	for(i=0; i<UartTempSize; i++)
	{
		UartTempRx[i] = 0;	
	}
	
	UartRxPoint = UartTempRx;
	UartRxProcess = UartTempRx;
	UartRxTail = UartTempRx;	


	for(i=0; i<MAX_DELEG_NUM; i++)
	{
		OnliSlaverId[i] = 0;	
		DelegInfoTable[i]=0;
	}

	OnlineNum	= 0;
	OnlineChairNum = 0;
	OnlineSlaveNum = 0;
	OnlineInterNum = 0;
	OnlineWifiNum  = 0;

	// MIC信息操作缓冲区
	for (i = 0; i < MIC_INFOR_BUF_SIZE; i++) 
	{				
	   	MicOprInforBuff[i].OprMod	= 0;	
	   	MicOprInforBuff[i].Oprid	= 0;	
	}
	MicWrPnt = 0x00;
	MicRdPnt = 0x00;

	PowerUpState = 2;
	Fpga2Port2Value = 0xFF;
	Fpga2Port1Value = 0xFF;
	SysTickCount = 0;
	SysTickValue = 0;
	ChangToHostCount = 0;


	ScanData.DelayCount = 0;
	ScanData.State	= 0;

	for (i = 0; i < CARD_RX_BUFF_SIZE; i++) 
	{
		CardRx.Buff[i] = 0x00;
	}
	CardRx.Point = CardRx.Buff;
	CardRx.Process = CardRx.Buff;
	CardRx.Tail = CardRx.Buff;
	CardRx.Delay = 0;
	
	SysInfo.MeetSta = MEET_STATE;
	SysInfo.PollId = 1;						// 轮询ID初始化为0	

	for (i= 0; i<InterMicBuffSize; i++)
	{
		InterMicBuff[i].OpenId = 0;
		InterMicBuff[i].Room = 0;
		CurrentCount = 0;
	}
	for (i= 0; i<INTER_MAX_SPK_NUM; i++)
	{
		InterMic[i].OpenId = 0;
		InterMic[i].Room = 0;			
	}
	
	

	VoteTimer = 0;
	VoteTimeState = 0;
	VoteLookres = 0;
	Vote_10ms = 0;
	EpiTxFrameMax_Scan = 0;
	SignExitTimeStart = 0; 
	
	SysInfo.DefaultValue = 0;
	RegOther2.RegOther2Val = 0;
	RegOther2.RegOther2.AutoZero = 1;

	SysInfo.SetStartIdNum = 1;
}
void WifiStateInit(void)
{
	uint8_t i=0;
	for (i=0; i<15; i++){
		WifiState[i].WifiStaReg1.WifiStaReg1Value = 0;
		WriteFpgaREG(REG_StateWifi+3*i, 0);
		WifiState[i].WifiStaReg2.WifiStaReg2Value = 0;
		WriteFpgaREG(REG_StateWifi+3*i+1, 0);
		WifiState[i].WifiStaReg3.WifiStaReg3Value = 0;
		WriteFpgaREG(REG_StateWifi+3*i+2, 0);
	}
}
void ICStateInit(void)
{
	uint8_t i=0;
	for (i=0; i<15; i++){
		WriteFpgaREG(REG_StateWifi+3*i, ICState[i].ICState1);
		WriteFpgaREG(REG_StateWifi+3*i+1, ICState[i].ICState2);
		WriteFpgaREG(REG_StateWifi+3*i+2, ICState[i].ICState3);
	}
}
void ICStateUpdate(void)
{
	uint8_t i=0;
	for (i=0; i<15; i++){
		ICState[i].ICState1 = 0;
		ICState[i].ICState2 = 0;
		ICState[i].ICState3 = 0;
	}
}
void ICStateVolume(uint8_t id)
{
	uint8_t index 	 = id / 16;
	uint8_t fixation = id % 16;
	if ((id > 240)||(id == 0))	return;

	switch (fixation){
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			ICState[index].ICState1 = WifiState[index].WifiStaReg1.WifiStaReg1Value;
			break;
		case 6:
			ICState[index].ICState1 = WifiState[index].WifiStaReg1.WifiStaReg1Value;
			ICState[index].ICState2 = WifiState[index].WifiStaReg2.WifiStaReg2Value;
			break;
		case 7:
		case 8:
		case 9:
		case 10:
			ICState[index].ICState2 = WifiState[index].WifiStaReg2.WifiStaReg2Value;
			break;
		case 11:
			ICState[index].ICState2 = WifiState[index].WifiStaReg2.WifiStaReg2Value;
			ICState[index].ICState3 = WifiState[index].WifiStaReg3.WifiStaReg3Value;
			break;
		case 12:
		case 13:
		case 14:
		case 15:
		case 0:
			ICState[index].ICState3 = WifiState[index].WifiStaReg3.WifiStaReg3Value;
			break;
		default:
			break;
	}
}
void WifiStateUpdate(uint8_t id, uint8_t state)
{	
	uint8_t index 	 = id / 16;
	uint8_t fixation = id % 16;
	if ((id > 240)||(id == 0))	return;

	switch (fixation){
		case 1:
			WifiState[index].WifiStaReg1.WifiStaReg1.id1state = state;
			WriteFpgaREG(REG_StateWifi+3*index, WifiState[index].WifiStaReg1.WifiStaReg1Value);
			break;
		case 2:
			WifiState[index].WifiStaReg1.WifiStaReg1.id2state = state;
			WriteFpgaREG(REG_StateWifi+3*index, WifiState[index].WifiStaReg1.WifiStaReg1Value);
			break;
		case 3:
			WifiState[index].WifiStaReg1.WifiStaReg1.id3state = state;
			WriteFpgaREG(REG_StateWifi+3*index, WifiState[index].WifiStaReg1.WifiStaReg1Value);
			break;
		case 4:
			WifiState[index].WifiStaReg1.WifiStaReg1.id4state = state;
			WriteFpgaREG(REG_StateWifi+3*index, WifiState[index].WifiStaReg1.WifiStaReg1Value);
			break;
		case 5:
			WifiState[index].WifiStaReg1.WifiStaReg1.id5state = state;
			WriteFpgaREG(REG_StateWifi+3*index, WifiState[index].WifiStaReg1.WifiStaReg1Value);
			break;
		case 6:
			WifiState[index].WifiStaReg1.WifiStaReg1.id6state = state&0x01;
			WriteFpgaREG(REG_StateWifi+3*index, WifiState[index].WifiStaReg1.WifiStaReg1Value);
			WifiState[index].WifiStaReg2.WifiStaReg2.id6state = state>>1;
			WriteFpgaREG(REG_StateWifi+3*index+1, WifiState[index].WifiStaReg2.WifiStaReg2Value);
			break;
		case 7:
			WifiState[index].WifiStaReg2.WifiStaReg2.id7state = state;
			WriteFpgaREG(REG_StateWifi+3*index+1, WifiState[index].WifiStaReg2.WifiStaReg2Value);
			break;
		case 8:
			WifiState[index].WifiStaReg2.WifiStaReg2.id8state = state;
			WriteFpgaREG(REG_StateWifi+3*index+1, WifiState[index].WifiStaReg2.WifiStaReg2Value);
			break;
		case 9:
			WifiState[index].WifiStaReg2.WifiStaReg2.id9state = state;
			WriteFpgaREG(REG_StateWifi+3*index+1, WifiState[index].WifiStaReg2.WifiStaReg2Value);
			break;
		case 10:
			WifiState[index].WifiStaReg2.WifiStaReg2.id10state = state;
			WriteFpgaREG(REG_StateWifi+3*index+1, WifiState[index].WifiStaReg2.WifiStaReg2Value);
			break;
		case 11:
			WifiState[index].WifiStaReg2.WifiStaReg2.id11state = state&0x03;
			WriteFpgaREG(REG_StateWifi+3*index+1, WifiState[index].WifiStaReg2.WifiStaReg2Value);
			WifiState[index].WifiStaReg3.WifiStaReg3.id11state = state>>2;
			WriteFpgaREG(REG_StateWifi+3*index+2, WifiState[index].WifiStaReg3.WifiStaReg3Value);
			break;
		case 12:
			WifiState[index].WifiStaReg3.WifiStaReg3.id12state = state;
			WriteFpgaREG(REG_StateWifi+3*index+2, WifiState[index].WifiStaReg3.WifiStaReg3Value);
			break;
		case 13:
			WifiState[index].WifiStaReg3.WifiStaReg3.id13state = state;
			WriteFpgaREG(REG_StateWifi+3*index+2, WifiState[index].WifiStaReg3.WifiStaReg3Value);
			break;
		case 14:
			WifiState[index].WifiStaReg3.WifiStaReg3.id14state = state;
			WriteFpgaREG(REG_StateWifi+3*index+2, WifiState[index].WifiStaReg3.WifiStaReg3Value);
			break;
		case 15:
			WifiState[index].WifiStaReg3.WifiStaReg3.id15state = state;
			WriteFpgaREG(REG_StateWifi+3*index+2, WifiState[index].WifiStaReg3.WifiStaReg3Value);
			break;
		case 0:
			WifiState[index-1].WifiStaReg3.WifiStaReg3.id16state = state;
			WriteFpgaREG(REG_StateWifi+3*(index-1)+2, WifiState[index-1].WifiStaReg3.WifiStaReg3Value);
			break;
		default:
			break;
	}
}
/*********************************************************************************************************
** Function name:       VoteModeValInit
** Descriptions:        表决模式下相关变量初始化
*********************************************************************************************************/
void VoteModeValInit(void)
{
	uint16_t i;
	
	// 清空投票结果
	for(i = 0; i < MAX_DELEG_NUM; i++)
	{
		DelegInfoTable[i] &= (VNC+0xff00);
	}
	
	for(i = 0; i < 5; i++)
	{
		VoteResult[i] = 0;
	}
	VoteResultFlag	= 0x00;

	for(i = 0; i < VOTE_SENT_BUF_SIZE; i++) 
	{
	    SignVoteIdBuf[i] = 0;		// 清除签到/表决ID发送缓冲区
	    VoteResultBuf[i] = 0;		// 清除表决结果发送缓冲区
	}
	for(i = 0; i < sizeof(SignVoteSendList); i++) 
	{
		SignVoteSendList[i] = 0;	// 清除签到/表决ID标志列表
	}
	VoteWrPnt	  	= 0x00;
	VoteRdPnt	  	= 0x00;
	VoteSendPcFlag	= FALSE;
	StopScanFlag	= FALSE;

	VoteResRtnOk	= TRUE;
	QuitVoteFlag	= FALSE;
	
	VoteResRtnCnt	= 0x00;
    VoteResRtnEnable= FALSE;	
	VoteResRtnStart = FALSE;   		// 发送结果标志位
}
/*********************************************************************************************************
** Function name:       SignModeValInit
** Descriptions:        签到模式下相关变量初始化
*********************************************************************************************************/
void SignModeValInit(void)
{
	uint16_t i;

	for(i = 0; i < VOTE_SENT_BUF_SIZE; i++) 
	{
	    SignVoteIdBuf[i] = 0;		// 清除签到/表决ID发送缓冲区
	}
	for(i = 0; i < sizeof(SignVoteSendList); i++) 
	{
		SignVoteSendList[i] = 0;	// 清除签到/表决ID标志列表
	}
	VoteWrPnt	  = 0x00;
	VoteRdPnt	  = 0x00;
	VoteSendPcFlag= FALSE;
	StopScanFlag  = FALSE;	
	
	SignedNum 	  = 0;
	SignStartFlag = FALSE;
}
/*********************************************************************************************************
** Function name:       NetValInit
** Descriptions:        TCP/IP相关变量初始化
*********************************************************************************************************/
void NetValInit(void)
{
	NetRxPoint = NetRxBuff;
	NetRxProcess = NetRxBuff;
	NetRxTail = NetRxBuff;
	NetRxRelay = 0;
    tempNUM = 6;
	NetRxPoint1	= (uint8 *)NetRxBuff;
	NetRxProcess1 = (unsigned char *)NetRxBuff;
	Ptobuff = (unsigned char *)SMS_Tx_Buff;
	FrameCount = 0; 
}
/*********************************************************************************************************
** Function name:       VariableInit
** Descriptions:        变量初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void VariableInit(void)
{
	WindowInit();
	MainValInit();
	KeyValInit();
	SoftUartValInit();			// 模拟串口模块相关变量初始化
	HardUart0ValInit();			// 硬件串口0模块相关变量初始化
	HardUart1ValInit();			// 硬件串口0模块相关变量初始化
	NetValInit();
	VoteModeValInit();			// 表决模式下相关变量初始化
	SignModeValInit();			// 签到模式下相关变量初始化
	ResetMicInfoPrg();
	Lm75Init();
	CurrID=0x01;
	IDRepeat=FALSE;          //ID冲突标志位 
	Repeat_ID=0; 
	KeyOpr=FALSE; 
	SetIDDelay=0; 
	Setdestid=0;
	ScanEnd=0;
	Mic_WaitTx_Updata=FALSE;
	StartScan=FALSE;
	VersionDis=0;
	VerDelay=0;
	RxTimeDelay = 0;
	PCLoadDelay = 0;
	ModeSwitch=0;
	ModeSwDelay=0;
	AlarmCount=0;
	QuitFlag=FALSE;
	QuitDelay=0;
	PauseID=0;
	TypeID=0;
	Camera_Info.CameraRD = Camera_Info.CameraWR =0;
	Camera_Info.Camera_Set = 0;
	BlinkPortValue = 0;
	WifiTxWrite = 0;
	WifiTxRead  = 0;
	WifiRxWrite = 0;
	WifiRxRead  = 0; RangeBit = 0;
	CmdModeFlag = 0; WifiPollID = 0; RfTime = 0;NumDsp = 1;
	WifiInfor.KeyContent[0] = WifiInfor.SsidContent[0] = 0xFF;
	WifiInfor.KeyContent[1] = WifiInfor.SsidContent[1] = 0xFF;
	WifiInfor.KeyContent[2] = WifiInfor.SsidContent[2] = 0x00;
	WifiInfor.KeyContent[5] = WifiInfor.SsidContent[5] = 0x00;
	WifiInfor.KeyContent[3] = 0x44; WifiInfor.SsidContent[3] = 0x24;
	WifiInfor.KeyContent[4] = 0x0A; WifiInfor.SsidContent[4] = 0x0B;
	WifiInfor.KeyContent[70] = 0xFC; WifiInfor.SsidContent[38] = 0xFC;
	WifiInfor.KeyContent[71] = 0xFC; WifiInfor.SsidContent[39] = 0xFC;
	WifiInfor.wififlag.controlwifi = 0x0000;
	WifiInfor.DelayTime[0] = 160;  WifiInfor.DelayTime[1] = 208;
	WifiInfor.DelayTime[2] = 256;  WifiInfor.DelayTime[3] = 304;
	WifiInfor.DelayTime[4] = 352;  WifiInfor.DelayTime[5] = 400;
	WifiInfor.DelayTime[6] = 448;  WifiInfor.DelayTime[7] = 496;
	WifiInfor.DelayTime[8] = 544;  WifiInfor.DelayTime[9] = 592;
	WifiInfor.DelayTime[10] = 640;  WifiInfor.DelayTime[11] = 688;
	WifiInfor.DelayTime[12] = 784;  WifiInfor.DelayTime[13] = 880;
	WifiInfor.DelayTime[14] = 976;  WifiInfor.DelayTime[15] = 1120;
	WifiInfor.controlwifi[0] = 0x00;WifiInfor.controlwifi[1] = 0x0D;
	WifiInfor.controlwifi[5] = 0xFC;WifiInfor.controlwifi[6] = 0xFC;
	DbToGain[0] =0xFF;DbToGain[1]=0xE4;DbToGain[2]=0xCB;DbToGain[3]=0xB5;
	DbToGain[4] =0xA1;DbToGain[5]=0x90;DbToGain[6]=0x80;DbToGain[7]=0x72;
	DbToGain[8] =0x66;DbToGain[9]=0x5B;DbToGain[10]=0x51;DbToGain[11]=0x48;
	DbToGain[12]=0x40;DbToGain[13]=0x39;DbToGain[14]=0x33;DbToGain[15]=0x2E;
	DbToGain[16]=0x29;DbToGain[17]=0x24;DbToGain[18]=0x20;DbToGain[19]=0x1D;
	DbToGain[20]=0x1A;DbToGain[21]=0x17;DbToGain[22]=0x14;DbToGain[23]=0x12;
	DbToGain[24]=0x10;DbToGain[25]=0x0E;DbToGain[26]=0x0D;DbToGain[27]=0x0B;
	DbToGain[28]=0x0A;DbToGain[29]=0x09;DbToGain[30]=0x08;DbToGain[31]=0x07;
	DbToGain[32]=0x06;DbToGain[33]=0x05;DbToGain[34]=0x05;DbToGain[35]=0x05;
	DbToGain[36]=0x04;DbToGain[37]=0x04;DbToGain[38]=0x04;DbToGain[39]=0x04;
	DbToGain[40]=0x03;DbToGain[41]=0x00;
	debugstart_Count=0x0;
	
	NetRxBuff_Size = 0x0;
	NetRxBuff_Type = 0x0;
	NetRxBuff_Type_DWIN = 0x0;
	NetRxBuff_Size_i = 0x0;
}
/*********************************************************************************************************
** Function name:       PcHasPara
** Descriptions:       PC软件获取软件界面内部显示参数
*********************************************************************************************************/
void PcHasPara(void)
{
	uint8_t	data[124];
	data[0] = 0x00; data[1] = 0x70; data[2] = SysInfo.MicMode; data[3] = SysInfo.SpkNum;
	data[4] =  SysInfo.Languge; data[5] = SysInfo.LineInVol; data[6] = SysInfo.LinOutVol;
	data[7] = SysInfo.UintOutVol; data[8] = SysInfo.LinkOutVol; data[9] = SysInfo.TrebleVol;
	data[10] = SysInfo.BassVol; data[11] = SysInfo.EQSelect; data[12] = SysInfo.Band1; 
	data[13] = SysInfo.Band2; data[14] = SysInfo.Band3; data[15] = SysInfo.Band4; 
	data[16] = SysInfo.Band5; data[17] = SysInfo.Band6; data[18] = SysInfo.Band7; 
	data[19] = Camera_Info.Camera_Type; data[20] = Camera_Map[0].Map_Addr; data[21] = Camera_Map[0].Map_Chan; 
	data[22] = Camera_Map[1].Map_Addr; data[23] = Camera_Map[1].Map_Chan; data[24] = Camera_Map[2].Map_Addr; 
	data[25] = Camera_Map[2].Map_Chan; data[26] = Camera_Map[3].Map_Addr; data[27] = Camera_Map[3].Map_Chan; 
	data[28] = Camera_Map[4].Map_Addr; data[29] = Camera_Map[4].Map_Chan; data[30] = Camera_Map[5].Map_Addr; 
	data[31] = Camera_Map[5].Map_Chan; data[32] = Camera_Map[6].Map_Addr; data[33] = Camera_Map[6].Map_Chan; 
	data[34] = Camera_Map[7].Map_Addr; data[35] = Camera_Map[7].Map_Chan; data[36] = Camera_Map[8].Map_Addr; 
	data[37] = Camera_Map[8].Map_Chan; data[38] = Camera_Map[9].Map_Addr; data[39] = Camera_Map[9].Map_Chan;
	data[40] = Camera_Map[10].Map_Addr; data[41] = Camera_Map[10].Map_Chan; data[42] = Camera_Map[11].Map_Addr; 
	data[43] = Camera_Map[11].Map_Chan; data[44] = Camera_Map[12].Map_Addr; data[45] = Camera_Map[12].Map_Chan; 
	data[46] = Camera_Map[13].Map_Addr; data[47] = Camera_Map[13].Map_Chan; data[48] = Camera_Map[14].Map_Addr; 
	data[49] = Camera_Map[14].Map_Chan; data[50] = Camera_Map[15].Map_Addr; data[51] = Camera_Map[15].Map_Chan;
	data[52] = SysInfo.InterChanTotalNum; data[53] = SysInfo.InterRoomNum; data[54] = SysInfo.ChannelLan[1]; 
	data[55] = SysInfo.ChannelLan[2]; data[56] = SysInfo.ChannelLan[3]; data[57] = SysInfo.ChannelLan[4]; 
	data[58] = SysInfo.ChannelLan[5]; data[59] = SysInfo.ChannelLan[6]; data[60] = SysInfo.ChannelLan[7];
	data[61] = SysInfo.ChannelLan[8]; data[62] = SysInfo.ChannelLan[9]; data[63] = SysInfo.ChannelLan[10]; 
	data[64] = SysInfo.ChannelLan[11]; data[65] = SysInfo.ChannelLan[12]; data[66] = SysInfo.ChannelLan[13]; 
	data[67] = SysInfo.ChannelLan[14]; data[68] = SysInfo.ChannelLan[15]; data[69] = SysInfo.ChannelLan[16]; 
	data[70] = SysInfo.ChannelLan[17]; data[71] = SysInfo.ChannelLan[18]; data[72] = SysInfo.ChannelLan[19];
	data[73] = SysInfo.ChannelLan[20]; data[74] = SysInfo.ChannelLan[21]; data[75] = SysInfo.ChannelLan[22]; 
	data[76] = SysInfo.ChannelLan[23]; data[77] = SysInfo.ChannelLan[24]; data[78] = SysInfo.ChannelLan[25]; 
	data[79] = SysInfo.ChannelLan[26]; data[80] = SysInfo.ChannelLan[27]; data[81] = SysInfo.ChannelLan[28]; 
	data[82] = SysInfo.ChannelLan[29]; data[83] = SysInfo.ChannelLan[30]; data[84] = SysInfo.ChannelLan[31];
	data[85] = InterRoomFlag; data[86] = (HornZone.reg_zoon.SwitchZone&0xFF); data[87] = (HornZone.reg_zoon.SwitchZone>>8); 
	data[88] = (HornZone.SoundZone[0].endID&0xFF); data[89] = (HornZone.SoundZone[0].endID>>8); 
	data[90] = (HornZone.SoundZone[1].endID&0xFF); data[91] = (HornZone.SoundZone[1].endID>>8);
	data[92] = (HornZone.SoundZone[2].endID&0xFF); data[93] = (HornZone.SoundZone[2].endID>>8); 
	data[94] = (HornZone.SoundZone[3].endID&0xFF); data[95] = (HornZone.SoundZone[3].endID>>8); 
	data[96] = HornZone.SoundZone[0].gain1; data[97] = HornZone.SoundZone[1].gain1;
	data[98] = HornZone.SoundZone[2].gain1; data[99] = HornZone.SoundZone[3].gain1; 
	data[100] = HornZone.SoundZone[0].gain2; data[101] = HornZone.SoundZone[1].gain2; 
	data[102] = HornZone.SoundZone[2].gain2; data[103] = HornZone.SoundZone[3].gain2;
	data[104] = HornZone.SoundZone[0].gain3; data[105] = HornZone.SoundZone[1].gain3; 
	data[106] = HornZone.SoundZone[2].gain3; data[107] = HornZone.SoundZone[3].gain3;
	data[108] = HornZone.SoundZone[0].gain4; data[109] = HornZone.SoundZone[1].gain4; 
	data[110] = HornZone.SoundZone[2].gain4; data[111] = HornZone.SoundZone[3].gain4;
	data[112] = RegTrackBack.RegTrackBack.TrackSw; data[113] = SysInfo.Sound_AFNGC1; data[114] = SysInfo.Sound_AFNGC2;
	data[115] = MACAddrBuff[5]; data[116] = MACAddrBuff[4]; data[117] = MACAddrBuff[3]; 
	data[118] = MACAddrBuff[2];	data[119] = MACAddrBuff[1]; data[120] = MACAddrBuff[0]; 
	data[121] = 0xFC; data[122] = 0xFC;
	send(0,data,123);
}
/*********************************************************************************************************
** Function name:       BackupHostInit
** Descriptions:       备份主机相关变量初始化
*********************************************************************************************************/
void BackupHostInit(void)
{
	uint8 i;
   
	ScanID = 1;
	ScanOnlineId=4;
	HostStart=FALSE;
	HostCheckDelay=0x00;
	MicUpdata=FALSE;
	MicWaitUpdata=FALSE;
	TxFlag=FALSE;
	ParaInit=TRUE;
	SignNumber=0;
	SetIDNumber=1;
	SendResult=0;
	RegValue=0;
	StaDelay=0;
	BlinkFlag=TRUE;
	HostSta=0x00;
	HostTemp=0x00;
	OnlineDelay=0;
	CheckHostDelay=0;
	ModeStaDelay=0;
	ModeSta=0;
	MicStaUpdata=0;
	BackupStart=TRUE;
	HostTemp=HostSta;
	HostFlag=TRUE;
	PowerFlag=FALSE;
	PowerOnFlag=TRUE;
	ModeSwitch=0;
	BackupHostOnline=FALSE;
	HostOnline=0x00;
	KeyLockStatus=FALSE;
	PowerOnDelay=0;
	VoteRtnFlag = FALSE;
	BidFlag = FALSE;
	for (i=0;i<12;i++)
		Mic_Wait_Buff[i]=0;

	if ((((ReadFpgaREG(REG_ByPass)>>12)&0x0F)==0x0F)||
		(((ReadFpgaREG(REG_ByPass)>>12)&0x0F)==0x0A))
	{
		HostSta=0x01;
		HostOnline=0x02;
		ParaInit=FALSE;
		ScanOnlineId=0x00;
		HostTemp=HostSta;
		HostFlag=FALSE;
		HostStart=TRUE;
		WriteFpgaREG(REG_Default3,0x00);
		ScanOnliIdInit(1);   //扫描单元
		SysInfo.MeetSta = MEET_STATE;
		// 通知所有机器进入会议模式
		EPI_SntBuffInit(0xD2, MeetCmdHead | 0x0F, MEET_STATE);		
	    // 话筒工作模式
		EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x20 + SysInfo.MicMode);	
	    // 最大发言人
		EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x40 + SysInfo.SpkNum);	
 	    // 发送译员最大翻译间
		EPI_SntBuffInit(0xD2, InterLoadCmd | 0x0F, 0x80|SysInfo.InterRoomNum);	
		NormalChannel();
    }
}
/*********************************************************************************************************
** Function name:       ParameterInit
** Descriptions:        状态切换 相关变量初始化
*********************************************************************************************************/
void ParameterInit(void)
{
	uint16_t temp;
	uint8_t i;
	switch (HostSta)
	{
		case 0x00:	   //备份主机状态
//		case 0x01:
			temp=ReadFpgaREG(REG_ByPass);	  //Power off切换
			temp=(temp&0xFFCF);
			WriteFpgaREG(REG_ByPass,temp);
			HostOnline=0x01;
			ScanOnlineId=4;
			HostStart=FALSE;
			ParaInit=TRUE;
			ScanID=1;
			SendResult=0;
			SetIDNumber=1;
			ChkOnlineIdFlag = FALSE;
			VoteRtnFlag = FALSE;
			VoteModeValInit();
			SignModeValInit();
			for (i=0;i<12;i++)
				Mic_Wait_Buff[i]=0;
			for(temp = 0; temp < MAX_DELEG_NUM; temp++)
			{
				DelegInfoTable[temp] = 0x00;
				OnliSlaverId[i] = 0X00;
			}

			SysInfo.PollId 	= 1;
			OnlineNum = 0;
			OnlineSlaveNum = 0;
			OnlineChairNum = 0;
			OnlineInterNum = 0;
			OnlineWifiNum  = 0;

			ResetMicInfoPrg();	   //相关MIC列表初始化
			break;
		default:
			break;
	}
}
/*********************************************************************************************************
函数功能: 主机清除代表机的MIC信息和当前发言的代表机ID
输入说明: 
*********************************************************************************************************/
void ResetMicInfoPrg(void)
{
	uint16_t i;

	for(i = 0; i < MAX_MIC_ON_NUM; i++)
	{
		SlaverMicOnList[i] = 0x00;			// 当前发言代表机MIC 的ID列表
	}
	for (i = 0; i < FREE_MIC_ON_NUM; i++)
	{
		FreeMicOnList[i] = 0x00;
	}
	
	for (i=0; i<MAX_MIC_ON_NUM; i++)
	{
		UnitMicNumList[i] = 0x00;
		WriteFpgaREG((REG_Channel0ID + i), 0x0000);
	}

	for(i = 0; i < MAX_MIC_ON_NUM; i++)
	{
		MasterMicOnList[i] = 0x00;			// 当前发言代表机MIC 的ID列表
	}

	for(i = 0; i < INTER_MAX_SPK_NUM; i++)
	{
		InterMicOnList[i] = 0x00;			// 
	}
	
	for(i = 0; i < MAX_MIC_WAIT_NUM; i++)
	{
		WaitMicList[i] = 0x00;				// 当前的等待列表
		WriteFpgaREG((REG_Wait0ID + i), 0x0000);
	}
	SysInfo.MicWaitNum	 = 0;
	SysInfo.MicOnMasterNum = 0;
	SysInfo.MicOnSlaveNum = 0;
	SysInfo.MicOnInterNum = 0;
				
	for (i = 0; i < CEMARE_SERIES; i++)
	{
		CameraList[i] = 0xFCFC;
	}
	ChairPosition = 0x00;
	CameraPnt = 0x00;
	


	for (i = 0; i < INTER_MAX_SPK_NUM; i++)
	{
		InterMicOutList[i] = 0;	
	}

	for(i = 0; i < CALL_CHAIR_ON_NUM; i++)
	{
		CallChairOnList[i] = 0;
	}
	PauseID=0;

	CheckRepeatIdFlag = 0;
	PauseOnList=FALSE;
}
/*********************************************************************************************************
** Function name:       InitDevices
** Descriptions:        备份主机启动 ，扫描在线单元，上传机器类型码
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ScanUint(void)
{
	uint16_t dest_id;

	if (EpiTxFrameCount<EPI_SCAN_TX_MAX_SIZE) // 主机没有下发的控制信息，这时需要下发相应模式下的轮询码
	{ 			
		if (ScanOnlineId > 0) // 还在扫描       
		{	
			dest_id = ScanID;									
			EpiTxFrameBuff[TxWriteCount].FpgaDest = dest_id;
			EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(dest_id);
			EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0xA0 | (dest_id >> 8))<<8)|0x00);
			__disable_irq();
			TxWriteCount++;
			if (TxWriteCount>=TX_Frame_MAX_SIZE)
			{
				TxWriteCount = 0; 
			}
			EpiTxFrameCount++;
			__enable_irq();	
			ScanID++;

			if(ScanID > (SYS_MAX_DELEG_NUM + MAX_EXMAC_NUM))// 1240台代表机/主席机+10台扩展主机 
			{ 	
				ScanID = 1;		
				ScanOnlineId--;
			}
			if(ScanOnlineId==0x00)
				HostStart=TRUE;
		 }
	}
}
/*********************************************************************************************************
** Function name:       InitDevices
** Descriptions:        硬件初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void InitDevices(void)
{
	FanInit();											 // 风扇控制，关
	LED1Init();											 // LED1GPIO初始化
	RelayOutInit();										 // 延时输出初始化
	BlinkInit();										 // 595GPIO初始化
	KeyInit();											 // 按键GPIO初始化
	
	LEDAGC_ON();
	LEDANC_ON();
	LEDAFC_ON();
	Blinkled(0xFFFF);									 // 上电点亮前板全部LED

	SPIInit();											 // 初始化总线
	WriteFpgaREG(REG_ByPass,0x00);
	/******************************************************************************************/
///	timer0Init();										 // Timer Init
///	GPIOInit();											 //FPGA有数据中断
	I2C1Init(100000);									 // IIC初始化

//-----------------------------------------------------------------------------    
	#if (DEBUG_DOG == 0) 
///	wdogInit();											//Initialize Watchdog
	#endif
	LCD1602_Init();
	delay(20);
	LCD1602_Write_Str(2, 1, "STARTING...");	
}
/*********************************************************************************************************
** Function name:       DefaultInit
** Descriptions:     
*********************************************************************************************************/
void DefaultInit(void)
{
	uint8 i = 0;
	
	// 状态信息的默认值	 
	MicNum = 0;
	SysInfo.SpkNum = 1;
	SysInfo.MicMode    = FIFO;
	SysInfo.VoteType   = VOTE_MODE;
	
	// 把这些值存放到EEPROM中   
	WriteEeprom(SysInfo.SpkNum, SPEAKER_NUM_ADDR);
	WriteEeprom(SysInfo.MicMode, MIC_MODE_ADDR);
	WriteEeprom(0x00, SPK_ADDR);		// 默认为开启
	U_REG_Default1.U_REG_Default1.Speaker = 1;
 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);
	EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x20 + SysInfo.MicMode);
	EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x40 + SysInfo.SpkNum);

	// 设置标记，表示已经设置过状态信息	    
	WriteEeprom('j', SIGN_BYTES + 0);
	WriteEeprom('o', SIGN_BYTES + 1);
	WriteEeprom('n', SIGN_BYTES + 2);
	WriteEeprom('y', SIGN_BYTES + 3);
	
	Scanflag = 1;
	WriteEeprom(1, SCAN_ADDR);
	RegData5.other.alcfun = 0;
	WriteEeprom(0, ALCSwitch);
	FanControl.tos	 = 50;
	FanControl.thyst = 30;
	WriteEeprom(FanControl.tos, TOS_ADDR);
	WriteEeprom(FanControl.thyst, THYST_ADDR);

	SysInfo.InterChanTotalNum = 16;
	RegData5.other.TotalChannel = SysInfo.InterChanTotalNum-1;
	WriteFpgaREG(REG_Default5,RegData5.value);	
	SysInfo.InterRoomNum = 2;
	EPI_SntBuffInit(0xD2, InterLoadCmd | 0x0F, 0x80|SysInfo.InterRoomNum);
	WriteEeprom(16, ChanneleTotalAdd);
	WriteEeprom(2, TranslateRoomAdd);
	WriteEeprom(0, Move_ADDR);	
	//-------------------------------------------------------
	RegOther2.RegOther2.AutoZero = 1;
	WriteFpgaREG(REG_Other2,RegOther2.RegOther2Val);		//自动0通道打开。
	WriteEeprom(0x01, AUTO_ZERO_ADDR);	
	SignFuncFlag = TRUE;					// 默认为关闭签到功能
	VoteFuncFlag = TRUE;					// 默认为开启表决
	SysInfo.Languge = LAN_CN;					// 默认为中文
	WriteEeprom(SysInfo.Languge, LANG_ADDR);		
	RegIR.RegIR.IRLight = 15;
	WriteFpgaREG(REG_IRLight,RegIR.REG_IRLightVal);
	WriteEeprom(15, IRLIGHT_ADDR);
	//-------------------------------------------------------
	//译员机通道语言
	for (i=0; i<32; i++)
	{
		SysInfo.ChannelLan[i] = i;
		WriteEeprom(SysInfo.ChannelLan[i],INTER_LANG_ADDR+i);
	}
	
	// 发言单元的内置喇叭

	//------------------------------------------------------------		
	SysInfo.LineInVol = 0;		
	WriteEeprom(SysInfo.LineInVol, LINEIN_ADDR);
	
	SysInfo.LinkOutVol = 0;
	WriteEeprom(SysInfo.LinkOutVol, LinkVol_ADDR);
	WriteFpgaREG(REG_LineIn,(((uint16_t)(DbToGain[SysInfo.LinkOutVol]))<<8)|DbToGain[SysInfo.LineInVol]);		
	
	SysInfo.BassVol = 12;		// 取中间值
	WriteEeprom(SysInfo.BassVol, BASS_ADDR);
	
	SysInfo.TrebleVol = 12;		// 取中间值
	WriteEeprom(SysInfo.TrebleVol, TREBLE_ADDR);		

	SysInfo.LinOutVol = 0;		
	WriteEeprom(SysInfo.LinOutVol, VOLUME_ADDR);

	SysInfo.UintOutVol = 0;		
	WriteEeprom(SysInfo.UintOutVol, UnitVol_ADDR);
	WriteFpgaREG(REG_Volume, ((uint16_t)DbToGain[SysInfo.LinOutVol]<<8)|DbToGain[SysInfo.UintOutVol]);

	SysInfo.IR_ChanVol = 6;		// 取中间值
	WriteEeprom(SysInfo.IR_ChanVol, IR_CHAN_ADDR);		

	SysInfo.Band1 = 20;		// 取中间值
	WriteEeprom(SysInfo.Band1, Band1_Addr);
	SysInfo.Band2 = 20;		
	WriteEeprom(SysInfo.Band2, Band2_Addr);
	SysInfo.Band3 = 20;		
	WriteEeprom(SysInfo.Band3, Band3_Addr);
	SysInfo.Band4 = 20;		
	WriteEeprom(SysInfo.Band4, Band4_Addr);
	SysInfo.Band5 = 20;		
	WriteEeprom(SysInfo.Band5, Band5_Addr);
	SysInfo.Band6 = 20;		
	WriteEeprom(SysInfo.Band6, Band6_Addr);
	SysInfo.Band7 = 20;		
	WriteEeprom(SysInfo.Band7, Band7_Addr);

	WriteEeprom(0x08, Affect_addr);
	SysInfo.EQSelect = 	0x07;

	/*SysInfo.Sound_AFNGC1 = 0x03;                 //mask_xavior
	SysInfo.Sound_AFNGC2 = 0x00;
	WriteEeprom(SysInfo.Sound_AFNGC1, AFNGC1_Add);
	WriteEeprom(SysInfo.Sound_AFNGC2, AFNGC2_Add);
	LEDAFC_ON();LEDANC_ON();

	// ByPass
	ByPassValue &= (~ByPass_3108_Bit);
	ByPassValue |= (ByPass_AFC_Bit | ByPass_ANC_Bit);
	WriteFpgaREG(REG_ByPass,ByPassValue);
	
	
	Tas3108SetBass((35-SysInfo.BassVol)*2);	
	Tas3108SetTreble((35-SysInfo.TrebleVol)*2);
	*/
/*************************xavior************************************/
	//关闭音效板1
	ByPassValue &= ~(ByPass_AGC_Bit);LEDAGC_OFF();
	SysInfo.Sound_AFNGC1 &= ~0x04;
	//WriteEeprom(SysInfo.Sound_AFNGC1, AFNGC1_Add);
	ByPassValue &= ~(ByPass_ANC_Bit);LEDANC_OFF();
	SysInfo.Sound_AFNGC1 &= ~0x02;
	//WriteEeprom(SysInfo.Sound_AFNGC1, AFNGC1_Add);
	ByPassValue &= ~(ByPass_AFC_Bit);LEDAFC_OFF();
	SysInfo.Sound_AFNGC1 &= ~0x01;
	WriteEeprom(SysInfo.Sound_AFNGC1, AFNGC1_Add);
	//关闭音效板2
	ByPassValue &= ~(ByPass2_AGC_Bit);
	SysInfo.Sound_AFNGC2 &= ~0x04;
	//WriteEeprom(SysInfo.Sound_AFNGC2, AFNGC2_Add);
	ByPassValue &= ~(ByPass2_ANC_Bit);
	SysInfo.Sound_AFNGC2 &= ~0x02;
	//WriteEeprom(SysInfo.Sound_AFNGC2, AFNGC2_Add);
	ByPassValue &= ~(ByPass2_AFC_Bit);
	SysInfo.Sound_AFNGC2 &= ~0x01;
	WriteEeprom(SysInfo.Sound_AFNGC2, AFNGC2_Add);
	WriteFpgaREG(REG_ByPass,ByPassValue);
	//关闭3108
	//SysInfo.LinOutVol=6;
	//WriteEeprom(SysInfo.LinOutVol, VOLUME_ADDR);
	SysInfo.TrebleVol=12;
	Tas3108SetTreble((35-SysInfo.TrebleVol)*2);    //Treble
	WriteEeprom(SysInfo.TrebleVol, TREBLE_ADDR);
	SysInfo.BassVol=12;
	Tas3108SetBass((35-SysInfo.BassVol)*2);        //Bass
	WriteEeprom(SysInfo.BassVol, BASS_ADDR);
	SysInfo.EQSelect=0x07;
	SysInfo.Band1=20;
	Tas3108EQBand(SysInfo.Band1,0);                //EQ
	WriteEeprom(SysInfo.Band1, Band1_Addr);
	SysInfo.Band2=20;
	Tas3108EQBand(SysInfo.Band2,1);
	WriteEeprom(SysInfo.Band2, Band2_Addr);
	SysInfo.Band3=20;
	Tas3108EQBand(SysInfo.Band3,2);
	WriteEeprom(SysInfo.Band3, Band3_Addr);
	SysInfo.Band4=20;
	Tas3108EQBand(SysInfo.Band4,3);
	WriteEeprom(SysInfo.Band4, Band4_Addr);
	SysInfo.Band5=20;
	Tas3108EQBand(SysInfo.Band5,4);
	WriteEeprom(SysInfo.Band5, Band5_Addr);
	SysInfo.Band6=20;
	Tas3108EQBand(SysInfo.Band6,5);
	WriteEeprom(SysInfo.Band6, Band6_Addr);
	SysInfo.Band7=20;
	Tas3108EQBand(SysInfo.Band7,6);
	WriteEeprom(SysInfo.Band7, Band7_Addr);
	ByPassValue &= (~ByPass_3108_Bit);
	WriteFpgaREG(REG_ByPass,ByPassValue);
/*************************xavior************************************/

	WifiInfor.wififlag.wififlag.timedelay = 8;
	WriteEeprom(8, Delay_ADDR);
	WriteFpgaREG(REG_WifiDelay,WifiInfor.DelayTime[8]);
	WifiInfor.controlwifi[2] = 8;
	/*******************扬声器分区************************/
	HornZone.SoundZone[0].gain1		= 0; HornZone.SoundZone[0].gain2		= 0;
	HornZone.SoundZone[0].gain3		= 0; HornZone.SoundZone[0].gain4		= 0;
	HornZone.SoundZone[0].endID		= 4000;
	HornZone.SoundZone[1].gain1		= 0; HornZone.SoundZone[1].gain2		= 0;
	HornZone.SoundZone[1].gain3		= 0; HornZone.SoundZone[1].gain4		= 0;
	HornZone.SoundZone[1].endID		= 4000;
	HornZone.SoundZone[2].gain1		= 0; HornZone.SoundZone[2].gain2		= 0;
	HornZone.SoundZone[2].gain3		= 0; HornZone.SoundZone[2].gain4		= 0;
	HornZone.SoundZone[2].endID		= 4000;
	HornZone.SoundZone[3].gain1		= 0; HornZone.SoundZone[3].gain2		= 0;
	HornZone.SoundZone[3].gain3		= 0; HornZone.SoundZone[3].gain4		= 0;
	HornZone.SoundZone[3].endID		= 4000;

	WriteEeprom(HornZone.SoundZone[0].gain1, FirstGain1_ADDR); WriteEeprom(HornZone.SoundZone[0].gain2, FirstGain2_ADDR);
	WriteEeprom(HornZone.SoundZone[0].gain3, FirstGain3_ADDR); WriteEeprom(HornZone.SoundZone[0].gain4, FirstGain4_ADDR);
	WriteEeprom((HornZone.SoundZone[0].endID & 0xFF), End1_ADDR);
	WriteEeprom((HornZone.SoundZone[0].endID >> 8), End1_ADDR+1);
	WriteEeprom(HornZone.SoundZone[1].gain1, SecGain1_ADDR); WriteEeprom(HornZone.SoundZone[1].gain2, SecGain2_ADDR);
	WriteEeprom(HornZone.SoundZone[1].gain3, SecGain3_ADDR); WriteEeprom(HornZone.SoundZone[1].gain4, SecGain4_ADDR);
	WriteEeprom((HornZone.SoundZone[1].endID & 0xFF), End2_ADDR);
	WriteEeprom((HornZone.SoundZone[1].endID >> 8), End2_ADDR+1);
	WriteEeprom(HornZone.SoundZone[2].gain1, ThirdGain1_ADDR); WriteEeprom(HornZone.SoundZone[2].gain2, ThirdGain2_ADDR);
	WriteEeprom(HornZone.SoundZone[2].gain3, ThirdGain3_ADDR); WriteEeprom(HornZone.SoundZone[2].gain4, ThirdGain4_ADDR);
	WriteEeprom((HornZone.SoundZone[2].endID & 0xFF), End3_ADDR);
	WriteEeprom((HornZone.SoundZone[2].endID >> 8), End3_ADDR+1);
	WriteEeprom(HornZone.SoundZone[3].gain1, FourthGain1_ADDR); WriteEeprom(HornZone.SoundZone[3].gain2, FourthGain2_ADDR);
	WriteEeprom(HornZone.SoundZone[3].gain3, FourthGain3_ADDR); WriteEeprom(HornZone.SoundZone[3].gain4, FourthGain4_ADDR);
	WriteEeprom((HornZone.SoundZone[3].endID & 0xFF), End4_ADDR);
	WriteEeprom((HornZone.SoundZone[3].endID >> 8), End4_ADDR+1);

	WriteFpgaREG(REG_Part1Gain12,(((uint16_t)(DbToGain[HornZone.SoundZone[0].gain2]))<<8)|DbToGain[HornZone.SoundZone[0].gain1]);
	WriteFpgaREG(REG_Part1Gain34,(((uint16_t)(DbToGain[HornZone.SoundZone[0].gain4]))<<8)|DbToGain[HornZone.SoundZone[0].gain3]);
	WriteFpgaREG(REG_Part1End,HornZone.SoundZone[0].endID);

	WriteFpgaREG(REG_Part2Gain12,(((uint16_t)(DbToGain[HornZone.SoundZone[1].gain2]))<<8)|DbToGain[HornZone.SoundZone[1].gain1]);
	WriteFpgaREG(REG_Part2Gain34,(((uint16_t)(DbToGain[HornZone.SoundZone[1].gain4]))<<8)|DbToGain[HornZone.SoundZone[1].gain3]);
	WriteFpgaREG(REG_Part2End,HornZone.SoundZone[1].endID);

	WriteFpgaREG(REG_Part3Gain12,(((uint16_t)(DbToGain[HornZone.SoundZone[2].gain2]))<<8)|DbToGain[HornZone.SoundZone[2].gain1]);
	WriteFpgaREG(REG_Part3Gain34,(((uint16_t)(DbToGain[HornZone.SoundZone[2].gain4]))<<8)|DbToGain[HornZone.SoundZone[2].gain3]);
	WriteFpgaREG(REG_Part3End,HornZone.SoundZone[2].endID);

	WriteFpgaREG(REG_Part4Gain12,(((uint16_t)(DbToGain[HornZone.SoundZone[3].gain2]))<<8)|DbToGain[HornZone.SoundZone[3].gain1]);
	WriteFpgaREG(REG_Part4Gain34,(((uint16_t)(DbToGain[HornZone.SoundZone[3].gain4]))<<8)|DbToGain[HornZone.SoundZone[3].gain3]);
	WriteFpgaREG(REG_Part4End,HornZone.SoundZone[3].endID);

	HornZone.reg_zoon.SwitchZone = 0x0000;	//远程会议关闭，分区功能开，DSP声道为1
	WriteEeprom(HornZone.reg_zoon.SwitchZone, FanZone_ADDR);
	WriteEeprom(HornZone.reg_zoon.SwitchZone>>8, FanZone_ADDR+1);
	WriteFpgaREG(REG_SwitchZone,HornZone.reg_zoon.SwitchZone);
	
	RegTrackBack.RegTrackBackVal = 0x1000;
	WriteFpgaREG(REG_IR_Track,RegTrackBack.RegTrackBackVal);
	WriteEeprom(0x01, Track_Add);
	/***************************KEY和SSID恢复默认************************************/
	WifiInfor.SsidContent[6] = 'C';WifiInfor.SsidContent[7] = 'R';
	WifiInfor.SsidContent[8] = 'E';WifiInfor.SsidContent[9] = 'A';
	WifiInfor.SsidContent[10] = 'T';WifiInfor.SsidContent[11] = 'O';
	WifiInfor.SsidContent[12] = 'R';
	for (i=0; i<25; i++){
		WifiInfor.SsidContent[13+i] = 0x00;
	}
	WifiInfor.KeyContent[6] = '1';WifiInfor.KeyContent[7] = '2';
	WifiInfor.KeyContent[8] = '3';WifiInfor.KeyContent[9] = '4';
	WifiInfor.KeyContent[10] = '5';WifiInfor.KeyContent[11] = '6';
	WifiInfor.KeyContent[12] = '7';WifiInfor.KeyContent[13] = '8';
	for (i=0; i<56; i++){
		WifiInfor.SsidContent[14+i] = 0x00;
	}
	for (i=0; i<64; i++){
		WriteEeprom(WifiInfor.KeyContent[i+6], Key_ADDR+i);
	}
	for (i=0; i<32; i++){
		WriteEeprom(WifiInfor.SsidContent[i+6], Ssid_ADDR+i);
	}
	//-------------------------------------------------------
	// 网络信息
	NetDefault();

	ICStateInit();
	WifiStateInit();
	Blinkled(0X0000);LEDAGC_OFF();LEDANC_OFF();LEDAFC_OFF();
}
/*********************************************************************************************************
函数功能: PwrOnSetup
输入说明: 系统上电设置，读取EEPROM设置参数
返回 值 : 
说    明: 
*********************************************************************************************************/
void PwrOnSetup(void)
{
	uint8_t 	temp = 0,delaytime1 = 0;
	uint16_t 	i = 0;
	uint8_t 	buff[64];

	SignFuncFlag = TRUE;
	VoteFuncFlag = TRUE;
	
	temp = ReadEeprom(LANG_ADDR);
	if (temp == 0x00)			//读LCD显示语言标志,0x00中文
	{
		SysInfo.Languge = LAN_CN;				// 默认为中文
		U_REG_Default1.U_REG_Default1.Language = LAN_CN;
	}
	else if (temp == 0x01)		//读LCD显示语言标志,0x00中文
	{
		SysInfo.Languge = LAN_EN;
		U_REG_Default1.U_REG_Default1.Language = LAN_EN;
	}
	else 
	{
		SysInfo.Languge = LAN_CN;				// 默认为中文
		U_REG_Default1.U_REG_Default1.Language = LAN_CN;
		WriteEeprom(SysInfo.Languge, LANG_ADDR);
	}
	temp = ReadEeprom(Display_ADDR);
	if (temp < 0x03){
		U_REG_Default1.U_REG_Default1.PC_Online = ((U_REG_Default1.U_REG_Default1.PC_Online)&0xFC)+temp;
	}
	else{
		WriteEeprom(0x00, Display_ADDR);
	}
	RegData5.other.ChannelFlag = 1;
	RegData5.other.vfdmove = ReadEeprom(Move_ADDR);		//VFD MOVE
	WriteFpgaREG(REG_Default5,RegData5.value);	
	// 自动零通道
	temp = ReadEeprom(AUTO_ZERO_ADDR);
	if (temp == 1) // 开
	{
		RegOther2.RegOther2.AutoZero = 1;	
		WriteFpgaREG(REG_Other2,RegOther2.RegOther2Val);			
	}
	else
	{
		RegOther2.RegOther2.AutoZero = 0;	
		WriteFpgaREG(REG_Other2,RegOther2.RegOther2Val);			
	}

	SysInfo.Sound_AFNGC1 = ReadEeprom(AFNGC1_Add) & 0x07;
	if (SysInfo.Sound_AFNGC1 & 0x04)	{ByPassValue |= (ByPass_AGC_Bit);LEDAGC_ON();}
	else								{ByPassValue &= ~(ByPass_AGC_Bit);LEDAGC_OFF();}
	if (SysInfo.Sound_AFNGC1 & 0x02)	{ByPassValue |= (ByPass_ANC_Bit);LEDANC_ON();}
	else								{ByPassValue &= ~(ByPass_ANC_Bit);LEDANC_OFF();}
	if (SysInfo.Sound_AFNGC1 & 0x01)	{ByPassValue |= (ByPass_AFC_Bit);LEDAFC_ON();}
	else								{ByPassValue &= ~(ByPass_AFC_Bit);LEDAFC_OFF();}
	SysInfo.Sound_AFNGC2 = ReadEeprom(AFNGC2_Add) & 0x07;
	if (SysInfo.Sound_AFNGC2 & 0x04)	ByPassValue |= (ByPass2_AGC_Bit);
	else								ByPassValue &= ~(ByPass2_AGC_Bit);
	if (SysInfo.Sound_AFNGC2 & 0x02)	ByPassValue |= (ByPass2_ANC_Bit);
	else								ByPassValue &= ~(ByPass2_ANC_Bit);
	if (SysInfo.Sound_AFNGC2 & 0x01)	ByPassValue |= (ByPass2_AFC_Bit);
	else								ByPassValue &= ~(ByPass2_AFC_Bit);
	WriteFpgaREG(REG_ByPass,ByPassValue);
	Startup_3108 = 1;
	TypeID = ReadEeprom(TYPE_ADDR);
	if (TypeID !=0x20 )
	 WriteEeprom(0x20,TYPE_ADDR);

	SysInfo.MicChannelNum = ReadEeprom(MicChannel_ADDR);
	if ((SysInfo.MicChannelNum == 8)||(SysInfo.MicChannelNum == 6)){
		if (SysInfo.MicChannelNum == 8){
			WifiInfor.controlwifi[3] = 8;
			SysInfo.MicFreeOnNum = 6;
		}
		else if (SysInfo.MicChannelNum == 6){
			SysInfo.MicFreeOnNum = 4;
			WifiInfor.controlwifi[3] = 6;
		}
	}
	else{
		SysInfo.MicChannelNum = 6;
		WriteEeprom(SysInfo.MicChannelNum, MicChannel_ADDR);
		WifiInfor.controlwifi[3] = 6;
		SysInfo.MicFreeOnNum = 4;
	}
	temp = ReadEeprom(SpareWIFI_ADDR);
	if (temp == 0){
		WifiInfor.wififlag.wififlag.modeflag = 0;
		WifiInfor.controlwifi[4] = 0;
	}
	else if (temp == 1){
		WifiInfor.wififlag.wififlag.modeflag = 1;
		WifiInfor.controlwifi[4] = 1;
	}
	else{
		WifiInfor.wififlag.wififlag.modeflag = 0;
		WifiInfor.controlwifi[4] = 0;
		WriteEeprom(0, SpareWIFI_ADDR);
	}
	/******下面为Sony摄像球的初始化过程，读取相关的参数*******************************/
	Camera_Info.Camera_Type = ReadEeprom(CameraID);

	if (Camera_Info.Camera_Type > CUSTOM)
	{
		Camera_Info.Camera_Type=SAMSUNG;		//use sansung	protocol
	}
	UARTInit();
	UART1Init();	
	SoftUartValInit();	 
	SonyCameraInit();
	//获取全景
	I2C_ReadNByte(0xAE, 2, PrePos_ADDR, buff, 8);
	I2C_ReadNByte(0xAE, 2, PrePos_ADDR+Camera_Zoom, &buff[8], 4);
	for (i=0; i<12; i++){
		FullView_Con[i] = buff[i];
	}
	
	I2C_ReadNByte(0xAE, 2, CHAN_ADDR, buff, 16);
	for (i=0; i<16; i++)
	{
		Camera_Map[i].Map_Chan = buff[i];
	}
	I2C_ReadNByte(0xAE, 2, CAME_ADDR, buff, 16);
	for (i=0; i<16; i++)
	{
		Camera_Map[i].Map_Addr = buff[i];
	}
	
	delay(3400);  
	Init8306();												// 复位后等1S再配置
	//--------------------------------------------------------------------------
	// 读取EEpron信息成功
	if((ReadEeprom(SIGN_BYTES) == 'j') || (ReadEeprom(SIGN_BYTES + 1) == 'o') 
				|| (ReadEeprom(SIGN_BYTES + 2) == 'n') || (ReadEeprom(SIGN_BYTES + 3)== 'y')) 
	{
		i = ReadEeprom(IRLIGHT_ADDR);
		if (i <= 15){
			RegIR.RegIR.IRLight = i;
			WriteFpgaREG(REG_IRLight,RegIR.REG_IRLightVal);
		}
		else{
			RegIR.RegIR.IRLight = 15;
			WriteFpgaREG(REG_IRLight,RegIR.REG_IRLightVal);
			WriteEeprom(15, IRLIGHT_ADDR);
		}

		i = ReadEeprom(SCAN_ADDR);
		if (i <= 1){
			Scanflag = i;
		}
		else{
			Scanflag = 1;
			WriteEeprom(1, SCAN_ADDR);
		}

		i = ReadEeprom(ALCSwitch);
		if (i == 0){
			RegData5.other.alcfun = 0;
		}
		else if (i == 1){
			RegData5.other.alcfun = 1;
		}
		else{
			RegData5.other.alcfun = 0;
			WriteEeprom(0, ALCSwitch);
		}
		WriteFpgaREG(REG_Default5,RegData5.value);
		//-------------------------------------------------------
		FanControl.tos	 = ReadEeprom(TOS_ADDR);
		FanControl.thyst = ReadEeprom(THYST_ADDR);
		// 发言单元的内置喇叭
		if (ReadEeprom(SPK_ADDR) > 0x00) // 1, 开; 0, 关
		{
			WriteEeprom(0x01, SPK_ADDR);		// 默认为开启
			U_REG_Default1.U_REG_Default1.Speaker = 0;
		}
		else
		{
			U_REG_Default1.U_REG_Default1.Speaker = 1;			
		}

		WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);
		/**************************话筒模式和最大发言人*****************************/
		SysInfo.SpkNum 	= ReadEeprom(SPEAKER_NUM_ADDR);
		SysInfo.MicMode    = ReadEeprom(MIC_MODE_ADDR);	

		// 初始化MIC 数量
		if ((SysInfo.SpkNum > 6)|(SysInfo.SpkNum==0)){
			SysInfo.SpkNum = 1;
			WriteEeprom(SysInfo.SpkNum, SPEAKER_NUM_ADDR);			
		}
		//if (SysInfo.MicMode == 2)		SysInfo.SpkNum = 6;
		if (SysInfo.MicMode == 2){
		  if (SysInfo.MicChannelNum == 6) SysInfo.SpkNum = 4;
		   else SysInfo.SpkNum = 6;
		}
		MicNum = SysInfo.SpkNum / 2;

		// 初始化表决模式模式
		if (SysInfo.VoteType > EVALUA_MODE)
		{
			SysInfo.VoteType = VOTE_MODE;
		}
		// 初始化MIC 模式
		if (SysInfo.MicMode > APPLY) 
		{
			SysInfo.MicMode = FIFO;
			WriteEeprom(SysInfo.MicMode, MIC_MODE_ADDR);
		}
	
		//------------------------------------------------------------			
		SysInfo.LineInVol  = ReadEeprom(LINEIN_ADDR);		// 读取输入音量	
		SysInfo.UintOutVol  = ReadEeprom(UnitVol_ADDR);		// 读取输入音量
		SysInfo.LinkOutVol  = ReadEeprom(LinkVol_ADDR);	
		SysInfo.BassVol = ReadEeprom(BASS_ADDR);			// 读取低音值			
		SysInfo.TrebleVol = ReadEeprom(TREBLE_ADDR);		// 读取高音值		
		SysInfo.LinOutVol = ReadEeprom(VOLUME_ADDR);		// 读取总音量值	
		SysInfo.IR_ChanVol = ReadEeprom(IR_CHAN_ADDR);		// 读取红外音量值	
		SysInfo.Band1 = ReadEeprom(Band1_Addr);
		SysInfo.Band2 = ReadEeprom(Band2_Addr);
		SysInfo.Band3 = ReadEeprom(Band3_Addr);
		SysInfo.Band4 = ReadEeprom(Band4_Addr);
		SysInfo.Band5 = ReadEeprom(Band5_Addr);
		SysInfo.Band6 = ReadEeprom(Band6_Addr);
		SysInfo.Band7 = ReadEeprom(Band7_Addr);
		
		if (SysInfo.LinkOutVol  > 41) 				// 值超出范围
		{		
			SysInfo.LinkOutVol = 41;						// 取中间值
			WriteEeprom(SysInfo.LinkOutVol, LinkVol_ADDR);
		}
		if (SysInfo.LineInVol  > 41) 				// 值超出范围
		{		
			SysInfo.LineInVol = 41;						// 取中间值
			WriteEeprom(SysInfo.LineInVol, LINEIN_ADDR);
		}
		WriteFpgaREG(REG_LineIn,(((uint16_t)(DbToGain[SysInfo.LinkOutVol]))<<8)|DbToGain[SysInfo.LineInVol]);
				
		if (SysInfo.BassVol > MaxBass) 					// 值超出范围
		{		
			SysInfo.BassVol = 12;							// 取中间值
			WriteEeprom(SysInfo.BassVol, BASS_ADDR);
		}		
		if (SysInfo.TrebleVol > MaxTreble) 				// 值超出范围
		{		
			SysInfo.TrebleVol = 12;						// 取中间值
			WriteEeprom(SysInfo.TrebleVol, TREBLE_ADDR);
		}		
//		if (SysInfo.LinOutVol > MaxVolume) 				// 值超出范围
//		{		
//			SysInfo.LinOutVol = 6;						// 取中间值
//			WriteEeprom(SysInfo.LinOutVol, VOLUME_ADDR);
//		}		
		if (SysInfo.IR_ChanVol > MaxVolume) 			// 值超出范围
		{		
			SysInfo.IR_ChanVol = 6;					// 取中间值
			WriteEeprom(SysInfo.IR_ChanVol, IR_CHAN_ADDR);
		}

		if (SysInfo.Band1  > MaxBand) {
			SysInfo.Band1 = 20;
			WriteEeprom(SysInfo.Band1, Band1_Addr);
		}
		if (SysInfo.Band2  > MaxBand) {
			SysInfo.Band2 = 20;
			WriteEeprom(SysInfo.Band2, Band2_Addr);
		}
		if (SysInfo.Band3  > MaxBand) {
			SysInfo.Band3 = 20;
			WriteEeprom(SysInfo.Band3, Band3_Addr);
		}
		if (SysInfo.Band4  > MaxBand) {
			SysInfo.Band4 = 20;
			WriteEeprom(SysInfo.Band4, Band4_Addr);
		}
		if (SysInfo.Band5  > MaxBand) {
			SysInfo.Band5 = 20;
			WriteEeprom(SysInfo.Band5, Band5_Addr);
		}
		if (SysInfo.Band6  > MaxBand) {
			SysInfo.Band6 = 20;
			WriteEeprom(SysInfo.Band6, Band6_Addr);
		}	
		if (SysInfo.Band7  > MaxBand) {
			SysInfo.Band7 = 20;
			WriteEeprom(SysInfo.Band7, Band7_Addr);
		}
				
		// 3108配置	
		Tas3108SetBass((35-SysInfo.BassVol)*2);
		Tas3108SetTreble((35-SysInfo.TrebleVol)*2);
		WriteFpgaREG(REG_Volume, ((uint16_t)DbToGain[SysInfo.LinOutVol]<<8)|DbToGain[SysInfo.UintOutVol]);
	
		// ByPass
		Set3108Bit();	
		SysInfo.EQSelect = 	ReadEeprom(Affect_addr)-1;		
		EffectInitSet();   
		
		if (((SysInfo.BassVol == 12)&&(SysInfo.TrebleVol == 12)&&(SysInfo.EQSelect == 7)&&(SysInfo.Band1 == 20)&&\
			(SysInfo.Band2 == 20)&&(SysInfo.Band3 == 20)&&(SysInfo.Band4 == 20)&&(SysInfo.Band5 == 20)&&\
			(SysInfo.Band6 == 20)&&(SysInfo.Band7 == 20))||(SysInfo.EQSelect == 6)){
			ByPassValue &= (~ByPass_3108_Bit);
			WriteFpgaREG(REG_ByPass,ByPassValue);		
		}
		else{
			ByPassValue |= ByPass_3108_Bit;
			WriteFpgaREG(REG_ByPass,ByPassValue);
		}			
		//-------------------------------------------------------
		SysInfo.InterChanTotalNum = ReadEeprom(ChanneleTotalAdd);
		RegData5.other.TotalChannel = SysInfo.InterChanTotalNum-1;
		WriteFpgaREG(REG_Default5,RegData5.value);	

		SysInfo.InterRoomNum = ReadEeprom(TranslateRoomAdd);
		
		if (SysInfo.InterChanTotalNum>ChanneleTotalMax)
		{
			SysInfo.InterChanTotalNum = ChanneleTotalMax;
		}

		if (SysInfo.InterRoomNum > TranslateRoomMax)// 翻译间数
		{
			SysInfo.InterRoomNum = TranslateRoomMax;
		}
		else if (SysInfo.InterRoomNum < 2){
			SysInfo.InterRoomNum = 2;
		}		
		EPI_SntBuffInit(0xD2, InterLoadCmd | 0x0F, 0x80|SysInfo.InterRoomNum);
		for (i=0; i<32; i++)							// 译员机通道语言
		{
			SysInfo.ChannelLan[i] = ReadEeprom(INTER_LANG_ADDR+i);
			if (SysInfo.ChannelLan[i]>178)
			{
				SysInfo.ChannelLan[i] = 1;
			}			
		}
		/****************************控制WIFI延时***************************/
		delaytime1 = ReadEeprom(Delay_ADDR);
		if (delaytime1 > 0x0F){
			delaytime1 = 0x08;
			WriteEeprom(8, Delay_ADDR);
			WifiInfor.controlwifi[2] = 8;
		}
		WifiInfor.wififlag.wififlag.timedelay = delaytime1;
		WifiInfor.controlwifi[2] = delaytime1;
		WriteFpgaREG(REG_WifiDelay,WifiInfor.DelayTime[delaytime1]);	
		/****************************扬声器分区*****************************/
		HornZone.SoundZone[0].endID = ReadEeprom(End1_ADDR);
		HornZone.SoundZone[0].endID |= ((uint16_t)ReadEeprom(End1_ADDR+1)<<8);
		WriteFpgaREG(REG_Part1End,HornZone.SoundZone[0].endID);
		HornZone.SoundZone[0].gain1	= ReadEeprom(FirstGain1_ADDR); 
		HornZone.SoundZone[0].gain2	= ReadEeprom(FirstGain2_ADDR);
		HornZone.SoundZone[0].gain3	= ReadEeprom(FirstGain3_ADDR); 
		HornZone.SoundZone[0].gain4	= ReadEeprom(FirstGain4_ADDR);
		WriteFpgaREG(REG_Part1Gain12,(((uint16_t)(DbToGain[HornZone.SoundZone[0].gain2]))<<8)|DbToGain[HornZone.SoundZone[0].gain1]);
		WriteFpgaREG(REG_Part1Gain34,(((uint16_t)(DbToGain[HornZone.SoundZone[0].gain4]))<<8)|DbToGain[HornZone.SoundZone[0].gain3]);
		
		HornZone.SoundZone[1].endID = ReadEeprom(End2_ADDR);
		HornZone.SoundZone[1].endID |= ((uint16_t)ReadEeprom(End2_ADDR+1)<<8);
		WriteFpgaREG(REG_Part2End,HornZone.SoundZone[1].endID);
		HornZone.SoundZone[1].gain1	= ReadEeprom(SecGain1_ADDR); 
		HornZone.SoundZone[1].gain2	= ReadEeprom(SecGain2_ADDR);
		HornZone.SoundZone[1].gain3	= ReadEeprom(SecGain3_ADDR); 
		HornZone.SoundZone[1].gain4	= ReadEeprom(SecGain4_ADDR);
		WriteFpgaREG(REG_Part2Gain12,(((uint16_t)(DbToGain[HornZone.SoundZone[1].gain2]))<<8)|DbToGain[HornZone.SoundZone[1].gain1]);
		WriteFpgaREG(REG_Part2Gain34,(((uint16_t)(DbToGain[HornZone.SoundZone[1].gain4]))<<8)|DbToGain[HornZone.SoundZone[1].gain3]);
		
		HornZone.SoundZone[2].endID = ReadEeprom(End3_ADDR);
		HornZone.SoundZone[2].endID |= ((uint16_t)ReadEeprom(End3_ADDR+1)<<8);
		WriteFpgaREG(REG_Part3End,HornZone.SoundZone[2].endID);
		HornZone.SoundZone[2].gain1	= ReadEeprom(ThirdGain1_ADDR); 
		HornZone.SoundZone[2].gain2	= ReadEeprom(ThirdGain2_ADDR);
		HornZone.SoundZone[2].gain3	= ReadEeprom(ThirdGain3_ADDR); 
		HornZone.SoundZone[2].gain4	= ReadEeprom(ThirdGain4_ADDR);
		WriteFpgaREG(REG_Part3Gain12,(((uint16_t)(DbToGain[HornZone.SoundZone[2].gain2]))<<8)|DbToGain[HornZone.SoundZone[2].gain1]);
		WriteFpgaREG(REG_Part3Gain34,(((uint16_t)(DbToGain[HornZone.SoundZone[2].gain4]))<<8)|DbToGain[HornZone.SoundZone[2].gain3]);

		HornZone.SoundZone[3].endID = ReadEeprom(End4_ADDR);
		HornZone.SoundZone[3].endID |= ((uint16_t)ReadEeprom(End4_ADDR+1)<<8);
		WriteFpgaREG(REG_Part4End,HornZone.SoundZone[3].endID);
		HornZone.SoundZone[3].gain1	= ReadEeprom(FourthGain1_ADDR); 
		HornZone.SoundZone[3].gain2	= ReadEeprom(FourthGain2_ADDR);
		HornZone.SoundZone[3].gain3	= ReadEeprom(FourthGain3_ADDR); 
		HornZone.SoundZone[3].gain4	= ReadEeprom(FourthGain4_ADDR);
		WriteFpgaREG(REG_Part4Gain12,(((uint16_t)(DbToGain[HornZone.SoundZone[3].gain2]))<<8)|DbToGain[HornZone.SoundZone[3].gain1]);
		WriteFpgaREG(REG_Part4Gain34,(((uint16_t)(DbToGain[HornZone.SoundZone[3].gain4]))<<8)|DbToGain[HornZone.SoundZone[3].gain3]);
	
		HornZone.reg_zoon.SwitchZone = ReadEeprom(FanZone_ADDR);
		HornZone.reg_zoon.SwitchZone |= ((uint16_t)ReadEeprom(FanZone_ADDR+1)<<8);
		WriteFpgaREG(REG_SwitchZone,HornZone.reg_zoon.SwitchZone);
		
		temp = ReadEeprom(Track_Add);
		if (temp == 0x01)	RegTrackBack.RegTrackBackVal = 0x1000;
		else				RegTrackBack.RegTrackBackVal = 0x0000;
		WriteFpgaREG(REG_IR_Track,RegTrackBack.RegTrackBackVal);
		/**********************************************读出KEY和SSID************************************************/
		for (i=0; i<64; i++){
			WifiInfor.KeyContent[i+6] = ReadEeprom(Key_ADDR+i);
		}
		for (i=0; i<32; i++){
			WifiInfor.SsidContent[i+6] = ReadEeprom(Ssid_ADDR+i);
		}
		//-------------------------------------------------------
		// 网络信息
		
		for(i=0; i<4; i++){
			SysInfo.IPAddValue[i] = ReadEeprom(NetIpAdd+i);
			SysInfo.SubMaskValue[i] = ReadEeprom(NetSubMaskAdd+i);
			SysInfo.GatewayValue[i] = ReadEeprom(NetGateWayAdd+i);
		}
		
		for (i=0; i<5; i++){
			MACAddrBuff[i] = ReadEeprom(MACAddr+i);
		}
		MACAddrBuff[5] = ReadEeprom(MACAddr6);
		NetPower();

		ICStateInit();
		WifiStateInit();
	}
	else 
	{
		DefaultInit();
		
	}
	WriteFpgaREG(REG_WifiControl,(WifiInfor.wififlag.controlwifi & WIFIDEBUG));  
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	//网络初始化
	EtheApp_Init();

	//---------------------------------------------------------------
	U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
	U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);

	timer0Init();										 // Timer Init
	GPIOInit();											 //FPGA有数据中断
	RelayOut_ON;
	Blinkled(0X0000);LEDAGC_OFF();LEDANC_OFF();LEDAFC_OFF();
	adcInit();
}
/*********************************************************************************************************
** Function name:       MicOnChannleNumber
** Descriptions:        给开话筒的单元分配声音通道0-7              
*********************************************************************************************************/
uint8_t MicOnChannleNumber(uint16_t DestId)
{
	uint8_t MicNum_i = 7;
 	uint8_t AddrL, Ver;
	uint8_t	temp = CmdModeFlag;
	uint8_t flag = 0;
	
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);

	for (MicNum_i=0; MicNum_i<8; MicNum_i++)
	{
		if (UnitMicNumList[MicNum_i] == DestId)
		{
			break;
		}
	}

	if (MicNum_i<8){
		CmdModeFlag = 0;
		EPI_SntBuffInit(AddrL, LoadCmdHead|Ver, MIC_NumberOpen + MicNum_i);
		CmdModeFlag = 1;
		EPI_SntBuffInit(AddrL, LoadCmdHead|Ver, MIC_NumberOpen + MicNum_i);
		WriteFpgaREG((REG_Channel0ID + MicNum_i), DestId);
		flag = 1;
	}
	else
	{
		if (SysInfo.MicChannelNum == 8){
			if (((UnitMicNumList[6] == 0)||(UnitMicNumList[7] == 0))&&
				((DelegInfoTable[DestId] & 0xF000) != 0xF000)){			//(CmdModeFlag == 1)
				if (UnitMicNumList[6] == 0){
					UnitMicNumList[6] = DestId;
					MicNum_i = 6;
				}
				else if (UnitMicNumList[7] == 0){
					UnitMicNumList[7] = DestId;
					MicNum_i = 7;
				}
			}
			else{
				for (MicNum_i=0; MicNum_i<6; MicNum_i++){
					if (UnitMicNumList[MicNum_i] == 0){
						UnitMicNumList[MicNum_i] = DestId;
						break;
					}
					if (MicNum_i == 5)	MicNum_i = 8;
			 	}
			}
		}
		else if (SysInfo.MicChannelNum == 6){
			for (MicNum_i=0; MicNum_i<6; MicNum_i++){
				if (UnitMicNumList[MicNum_i] == 0){
					UnitMicNumList[MicNum_i] = DestId;
					break;
				}
				if (MicNum_i == 5)	MicNum_i = 8;
		 	}
		}
		if (MicNum_i<8){
			CmdModeFlag = 0;
			EPI_SntBuffInit(AddrL, LoadCmdHead|Ver, MIC_NumberOpen + MicNum_i);
			CmdModeFlag = 1;
			EPI_SntBuffInit(AddrL, LoadCmdHead|Ver, MIC_NumberOpen + MicNum_i);
			WriteFpgaREG((REG_Channel0ID + MicNum_i), DestId);
			flag = 1;
		}
	}
	CmdModeFlag = temp;
	return (flag);
}
/*********************************************************************************************************
** Function name:       InsertOnliSlrIdHandle
** Descriptions:        在OnliSlaverId列表中插入一个代表机/主席机ID          
*********************************************************************************************************/
void InsertOnliSlrIdHandle(uint16 DestId)
{
	uint16 i;
	
	i = OnlineNum;
	while ((i > 0) && (DestId < OnliSlaverId[i - 1])) 
	{
		OnliSlaverId[i] = OnliSlaverId[i - 1];
		DelegInfoTable[i] = DelegInfoTable[i - 1];
		i--;
	}
	OnliSlaverId[i] = DestId;
	DelegInfoTable[i] |= ONLINE;
	OnlineNum++;
	RefreshFlag = 1;

	//更新EpiTxFrameMax_Scan的值，对于备份主机如果不更新在投票签到模式下不会扫描单元
	if (OnlineNum<EPI_SCAN_TX_MAX_SIZE)	   
	{
		EpiTxFrameMax_Scan = OnlineNum;
	}
	else
	{
		EpiTxFrameMax_Scan = EPI_SCAN_TX_MAX_SIZE;
	}
}
/*********************************************************************************************************
** Function name:       MasterClsPriWtHandle
** Descriptions: 当主席机关闭所有代表机的话筒时，如果此时等待列表中有等待发言的话筒ID，则开启等待发言的ID(有等待话筒)           
*********************************************************************************************************/
void MasterClsPriWtHandle(uint16_t dest_id)
{
	uint8_t i, j, k =0,temp=0,MicFlag=0,channeltemp=0,waittemp=0;
	uint16_t buffer[7];
	uint8_t flag = CmdModeFlag;
	// 把打开话筒指定id的信息传给PC机
	SendToPcBuff(0x11,(uint8_t)(((dest_id >> 4) & 0xF0) | 0x0B),(uint8)dest_id);
	if (SysInfo.MicMode == FREE){
		for (i=0; i<CameraPnt; i++){
			for (j=0; j<SysInfo.MicOnSlaveNum; j++){
				if (FreeMicOnList[j] == CameraList[i]) {
					temp+=1;
					CameraList[i] = 0xFCFC;
					break;
				}				
			}	
		}
		
		j=0;
		for (i=0; i<CameraPnt; i++){
			if (CameraList[i]!=0xFCFC){
				buffer[j++] = CameraList[i];
				CameraList[i] = 0xFCFC;
			}
		}
		for (i=0 ; i<j; i++){
			CameraList[k++] = buffer[i];
		}
		CameraPnt-=temp;

	}
	else{
		for (i=0; i<CameraPnt; i++){
			for (j=0; j<SysInfo.MicOnSlaveNum; j++){
				if (SlaverMicOnList[j] == CameraList[i]) {
					temp+=1;
					CameraList[i] = 0xFCFC;
					break;
				}
			}	
		}
		
		j=0;
		for (i=0; i<CameraPnt; i++){
			if (CameraList[i]!=0xFCFC){
				buffer[j++] = CameraList[i];
				CameraList[i] = 0xFCFC;
			}
		}
		for (i=0 ; i<j; i++){
			CameraList[k++] = buffer[i];
		}		
		CameraPnt-=temp;
	}
	
	for (i=0; i<SysInfo.MicChannelNum; i++){
		if (MasterMicOnList[i]== dest_id) break;
	}
	if (i<SysInfo.MicChannelNum)
		MicFlag = 1;
	
	if (MicFlag == 1){		//主席在开话筒列表中
		for (i=0; i<CameraPnt; i++){
			if (CameraList[i] == dest_id) {
				for (j=i; j<(CameraPnt-1); j++)
					CameraList[j] = CameraList[j+1];
				CameraList[j] = dest_id;
				break;
			}
		}
	}

	// 有等待发言的ID
	for(i = 0; i < SysInfo.MicChannelNum; i++)// 当前发言MIC 的ID列表
	{
		for(j = 0; j < SysInfo.MicChannelNum; j++) 	// 在MIC通道号中找开话筒ID号
		{
			if (SlaverMicOnList[i] == UnitMicNumList[j])
			{
				UnitMicNumList[j] = 0;
				WriteFpgaREG((REG_Channel0ID + j), 0x0000);
			}
		}		
		SlaverMicOnList[i] = 0;	
	}
	SysInfo.MicOnSlaveNum = 0;
	EpiTxFrameBuff[TxWriteCount].FpgaDest=0xffff;
	EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(dest_id);
	EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0x00 | (dest_id >> 8))<<8)|MaClsPri);
	__disable_irq();
	TxWriteCount++;
	if (TxWriteCount>=TX_Frame_MAX_SIZE)
	{
	 TxWriteCount = 0; 
	}
	EpiTxFrameCount++;
	__enable_irq();
	WifiTxBuff[WifiTxWrite].FpgaDest= 0xffff;
	WifiTxBuff[WifiTxWrite].Type_AddrL= (uint8)(dest_id);
	WifiTxBuff[WifiTxWrite].Para_CmdL= (((0x00 | (dest_id >> 8))<<8)|MaClsPri);
	__disable_irq();
	WifiTxWrite++;
	if (WifiTxWrite >= RX_Frame_MAX_SIZE){
		WifiTxWrite = 0;	
	}
	__enable_irq();

	// 只有在NORMAL模式下才会有等待事件发生
	if (SysInfo.MicMode == NORMAL) 
	{
		for (i = 0; i < SysInfo.MicWaitNum; i++) 
		{
			channeltemp = MicOnChannleNumber(WaitMicList[i]);
			if (channeltemp == 1){
				SlaveMicInvitePrg(WaitMicList[i]);
				SlaverMicOnList[i - waittemp] = WaitMicList[i];
				WaitMicList[i]= 0;
				WriteFpgaREG((REG_Wait0ID + i - waittemp), SlaverMicOnList[i - waittemp]);
				WriteFpgaREG((REG_Wait0ID + i), 0x0000);
//				CameraList[CameraPnt + SysInfo.MicWaitNum - 1 - i] = SlaverMicOnList[i - waittemp];
			}
			else{
				waittemp++;
				CmdModeFlag = 0;
				EPI_SntBuffInit((WaitMicList[i]) % 256, (WaitMicList[i]) / 256, DeMicClose);
				CmdModeFlag = 1;
				EPI_SntBuffInit((WaitMicList[i]) % 256, (WaitMicList[i]) / 256, DeMicClose);
				WaitMicList[i]= 0;
				WriteFpgaREG((REG_Wait0ID + i), 0x00);
			}
		}
		for (i = 0; i <(SysInfo.MicWaitNum-waittemp); i++){
			CameraList[CameraPnt + SysInfo.MicWaitNum - waittemp - 1 - i] = SlaverMicOnList[i - waittemp];
		}
		CameraPnt += (SysInfo.MicWaitNum - waittemp);
		SysInfo.MicOnSlaveNum = SysInfo.MicWaitNum - waittemp;
		SysInfo.MicWaitNum = 0x00;
	}
	
	// 系统当前开启的话筒数
	CmdModeFlag = 0;
	EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
	CmdModeFlag = flag;
	U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
	U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);	// 摄像跟踪处理
	if (SysInfo.MicMode == NORMAL) {
		if (MicFlag==0)
			CameraSendOpenCode(dest_id, 1, 0);	
		else
			CameraSendOpenCode(CameraList[CameraPnt + SysInfo.MicWaitNum - waittemp - 1], 0, 0);		
	}
	else 
		CameraSendOpenCode(dest_id, 1, 0);	
}
/*********************************************************************************************************
** Function name:       MasterClsPriHandle			  dest_id : 执行关闭功能的主席机ID 
** Descriptions: 当主席机关闭所有代表机的话筒时，如果此时等待列表中有等待发言的话筒ID，则开启等待发言的ID(无等待话筒)             
*********************************************************************************************************/
void MasterClsPriHandle(uint16_t destid)
{
	uint8_t i, j, k=0, temp=0,MicFlag=0;
	uint16_t dest_id,buffer[7];
	uint8_t flag = CmdModeFlag;
	dest_id = destid;
	// 把执行优先权的id信息传给PC机
	SendToPcBuff(0x11,(uint8)(((dest_id >> 4) & 0xF0) | 0x0B),(uint8)dest_id);

	for (i=0; i<MAX_MIC_ON_NUM; i++){
		if (MasterMicOnList[i]== dest_id) break;
	}
	if (i<MAX_MIC_ON_NUM)
		MicFlag = 1;
	
	if (SysInfo.MicMode == FREE) {
		for (i=0; i<CameraPnt; i++){
			for (j=0; j<SysInfo.MicOnSlaveNum; j++){
				if (FreeMicOnList[j] == CameraList[i]) {
					temp +=1;
					CameraList[i] = 0xFCFC;
					break;
				}				
			}	
		}
		
		j=0;
		for (i=0; i<CameraPnt; i++){
			if (CameraList[i]!=0xFCFC){
				buffer[j++] = CameraList[i];
				CameraList[i] = 0xFCFC;
			}
		}
		for (i=0 ; i<j; i++){
			CameraList[k++] = buffer[i];
		}
		CameraPnt-=temp;		
	}
	else {
		for (i=0; i<CameraPnt; i++){
			for (j=0; j<SysInfo.MicOnSlaveNum; j++){
				if (SlaverMicOnList[j] == CameraList[i]) {
					temp +=1;
					CameraList[i] = 0xFCFC;
					break;
				}				
			}	
		}
	
		j=0;
		for (i=0; i<CameraPnt; i++){
			if (CameraList[i]!=0xFCFC){
				buffer[j++] = CameraList[i];
				CameraList[i] = 0xFCFC;
			}
		}
		for (i=0 ; i<j; i++){
			CameraList[k++] = buffer[i];
		}
		CameraPnt-=temp;
	}
	
	if (MicFlag == 1){		//主席在开话筒列表中
		for (i=0; i<CameraPnt; i++){
			if (CameraList[i] == destid) {
				for (j=i; j<(CameraPnt-1); j++)
					CameraList[j] = CameraList[j+1];
				CameraList[j] = destid;
				break;
			}
		}
	}

	
	if (SysInfo.MicMode == FREE) 
	{
		for(i = 0; i < SysInfo.MicOnSlaveNum; i++) 
		{
			for(j = 0; j < SysInfo.MicChannelNum; j++) 	// 在MIC通道号中找开话筒ID号
			{
				if (FreeMicOnList[i] == UnitMicNumList[j])
				{
					UnitMicNumList[j] = 0;
					WriteFpgaREG((REG_Channel0ID + j), 0x0000);
				}
			}			
			FreeMicOnList[i] = 0;			// 当前发言MIC 的ID列表
		}
	}
	else 
	{
		for(i = 0; i < SysInfo.MicChannelNum; i++) 
		{	
			for(j = 0; j < SysInfo.MicChannelNum; j++) 	// 在MIC通道号中找开话筒ID号
			{
				if (SlaverMicOnList[i] == UnitMicNumList[j])
				{
					UnitMicNumList[j] = 0;
					WriteFpgaREG((REG_Channel0ID + j), 0x0000);
				}
			}
			SlaverMicOnList[i] = 0;		// 当前发言MIC 的ID列表
		}
	}

	SysInfo.MicOnSlaveNum 	= 0;
	EpiTxFrameBuff[TxWriteCount].FpgaDest=0xffff;
	EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(dest_id);
	EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0x00 | (dest_id >> 8))<<8)|MaClsPri);
	__disable_irq();
	TxWriteCount++;
	if (TxWriteCount>=TX_Frame_MAX_SIZE)
	{
		TxWriteCount = 0; 
	}
	EpiTxFrameCount++;
	__enable_irq();
	WifiTxBuff[WifiTxWrite].FpgaDest= 0xffff;
	WifiTxBuff[WifiTxWrite].Type_AddrL= (uint8)(dest_id);
	WifiTxBuff[WifiTxWrite].Para_CmdL= (((0x00 | (dest_id >> 8))<<8)|MaClsPri);
	__disable_irq();
	WifiTxWrite++;
	if (WifiTxWrite >= RX_Frame_MAX_SIZE){
		WifiTxWrite = 0;	
	}
	__enable_irq();

	// 系统当前开启的话筒数
	CmdModeFlag = 0;
	EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
	CmdModeFlag = flag;
	U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
	U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);	// 摄像跟踪处理
	CameraSendOpenCode(dest_id, 1, 0);
}
/*********************************************************************************************************
** Function name:       InterpreterMicOpenHandle
** Descriptions: 译员机申请开话筒。根据系统的话筒数量限制，话筒开启模式来对此ID的话筒开申请进行处理
**************** 1.第一次打开话筒。最后一次打开话筒。
**************** 2.相同Room,不同out.
**************** 3.相同Room,相同Out.
**************** 4.不同Room,不同Out.
**************** 5.不同Room.相同Out.             
*********************************************************************************************************/
void InterpreterMicOpenHandle(uint16 id, uint8 Ver2)
{
	uint8 i = 0,j=0;
	uint8 OutChannleTemp = 0;
	uint16 Temp = 0;
	uint8 mm=0;
			 //临时缓冲存储通道号和翻译间 然后查询建立到输出通道列表中
	for (i=0;i<=5;i++)
	{
		if (TempInterBuff[i].TempMicid==id)
		{
			mm=TempInterBuff[i].TempMicRoom;	//翻译间
			InterMicOpenBuff[mm][Ver2]=id;
			TempInterBuff[i].TempMicid = 0x00;
			TempInterBuff[i].TempMicRoom = 0x00;
		}
	}			 
			
	// 此通道号占用，先关闭它再打开
	OutChannleTemp = CheckInterOutNum(Ver2);	
	if (OutChannleTemp > 0)
	{
		EPI_SntBuffInit(InterMicOnList[OutChannleTemp], InterMicOnList[OutChannleTemp] >> 8, InterMicClose);

		//以下为我加的，不同翻译间开启相同通道时互锁，如果该翻译间没有开启其它通道则开启扬声器
		for (i=0;i<InterMaxNum;i++)
		{
			if (InterMicOpenBuff[i][OutChannleTemp]!=0)  //该通道存在其他开话筒译员
			{
				if (i==mm){	//同翻译间关扬声器  查找相同通道号所在的翻译间
					EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|mm);
					InterMicOpenBuff[i][OutChannleTemp]=id;	 // 
				}
				else{	//不同翻译间继续查找该翻译间有无其他输出通道号
					mm=i;
					InterMicOpenBuff[i][OutChannleTemp]=0; //该翻译间的输出通道号关闭
					break;
				}
			}
		}	

		if (mm<InterMaxNum)
		{
			for (j=0;j<InterMaxNum;j++)
			{
				if(InterMicOpenBuff[mm][j]!=0){	 //该翻译间有输出通道号
					EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|mm);
					break;
				}
		  	}
		}
		if (j>=InterMaxNum)
			EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x40|mm);


		// 通知FPGA关通道
		if (OutChannleTemp>=16){
			Temp = (1<<(OutChannleTemp-16));
			InterruptChannleFlag2 &= (~Temp);
			WriteFpgaREG(REG_Default6,InterruptChannleFlag2);	
		}			
		else {
			Temp = (1<<OutChannleTemp);
			InterruptChannleFlag &= (~Temp);
			WriteFpgaREG(REG_Interpreter,InterruptChannleFlag);	
		}
		InterMicOnList[OutChannleTemp] = 0;
		InterMicOutList[OutChannleTemp] = 0;	
	}
	// 保存输出通道号，通道输出通道号找出对应ID号来关闭它。
	InterMicOutList[Ver2] = Ver2;
	InterMicOnList[Ver2] = id;		
	
	
	
	for(i = 0; i < InterMicBuffSize; i++) 
	{
		if(InterMicBuff[i].OpenId == id) break;
	}

	// 关闭相同翻译间MIC
	OutChannleTemp = CheckInterRoomNum(InterMicBuff[i].Room);	
	if (OutChannleTemp > 0)	
	{
		EPI_SntBuffInit(InterMic[OutChannleTemp].OpenId, InterMic[OutChannleTemp].OpenId >> 8, InterMicClose);
		// 通知FPGA关通道
		if (OutChannleTemp >= 16){
			Temp = (1<<(OutChannleTemp-16));
			InterruptChannleFlag2 &= (~Temp);
			WriteFpgaREG(REG_Default6,InterruptChannleFlag2);				
		}
		else {
			Temp = (1<<OutChannleTemp);
			InterruptChannleFlag &= (~Temp);
			WriteFpgaREG(REG_Interpreter,InterruptChannleFlag);				
		}

		InterMic[OutChannleTemp].OpenId = id;
		InterMic[OutChannleTemp].Room = OutChannleTemp;	
	}
					
	// 关闭相同翻译间扬声器,只要有上翻译间号就会执行
	if (i<InterMicBuffSize)
	{
		EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|InterMicBuff[i].Room);	
	}

	// 允许开MIC
	EPI_SntBuffInit(id, id >> 8, InterMicOpen);	
	SendToPcBuff(InterMicOpen,(uint8)(((id >> 4) & 0xF0) | 0x00),(uint8)id);
	
	// 通知FPGA开通道
	if (Ver2 >= 16){
		Temp = (1<<(Ver2-16));
		InterruptChannleFlag2 |= (Temp);
		WriteFpgaREG(REG_Default6,InterruptChannleFlag2);				
	}
	else {
		Temp = (1<<Ver2);
		InterruptChannleFlag |= (Temp);
		WriteFpgaREG(REG_Interpreter,InterruptChannleFlag);				
	}
}
/*--------------------------------------------------------------------
函数功能: 译员机关话筒根据系统的话筒数量限制，话筒开启模式来对此ID的话筒关申请进行处理
输入说明: id : 关闭话筒的ID  
返回 值 : 
说    明: 
1. 相同room，不同通道，打开二个，先关一个，再关一个。         
--------------------------------------------------------------------*/
void InterpreterMicCloseHandle(uint16 id,uint8 Ver2)
{
	uint8 i = 0,j=0,InterRoom=0;
	uint16 Temp = 0;

	// 保存输出通道号，通道输出通道号找出对应ID号来关闭它。
	InterMicOutList[Ver2] = 0;
	InterMicOnList[Ver2] = 0;	
	InterMic[Ver2].OpenId = 0;
	InterMic[Ver2].Room = 0;	
	// 只打开本ID打开所对应的翻译扬声器
	// 允许关MIC
	EPI_SntBuffInit(id, id >> 8, InterMicClose);
	for(i = 0; i < InterMicBuffSize; i++){ 
		if(InterMicBuff[i].OpenId == id) break;
	}
			
	if (i<InterMicBuffSize)
	{
		EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x40|InterMicBuff[i].Room);					
	}
	else 
		EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|InterMicBuff[i].Room);
				
	SendToPcBuff(InterMicClose,(uint8)(((id >> 4) & 0xF0) | 0x00),(uint8)id);
								
	// 通知FPGA关通道
	if (Ver2 >= 16){
		Temp = 1<<(Ver2-16);
		InterruptChannleFlag2 &= (~Temp);
		WriteFpgaREG(REG_Default6,InterruptChannleFlag2);	
	}
	else {
		Temp = 1<<Ver2;
		InterruptChannleFlag &= (~Temp);
		WriteFpgaREG(REG_Interpreter,InterruptChannleFlag);	
	}			
										
	//相应ID清零，并查询此翻译间是否有其他机器开启话筒，有就关闭扬声器，否则开启扬声器
	for (i=0;i<InterMaxNum;i++)
	{
		for (j=0;j<InterMaxNum;j++)
		{
			if (InterMicOpenBuff[i][j]==id)
			{
				InterMicOpenBuff[i][j]=0;
				InterRoom=i;			//id所在翻译间
				break;
			}
		}  
	}
	//以下为我加的，首先查询ID的翻译间号并把数组清零，然后查询此翻译间是否有其他ID开启话筒
	//如果有开话筒关闭扬声器，否则开启该翻译间的扬声器
	for (i=0;i<InterMaxNum;i++){
		if (InterMicOpenBuff[InterRoom][i]!=0)  //翻译间存在其他开话筒译员
		{
			EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|InterRoom);
			break;
		}
	}	
	if(i>=InterMaxNum){
		EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x40|InterRoom);
	}
}
void	InterExtendOpenHandle(uint16 id, uint8 Ver2)
{
	uint8 i = 0,j=0;
	uint8 OutChannleTemp = 0;
	uint16 Temp = 0;
	uint8 mm=0;
			 //临时缓冲存储通道号和翻译间 然后查询建立到输出通道列表中
	for (i=0;i<=5;i++)
	{
		if (TempInterBuff[i].TempMicid==id)
		{
			mm=TempInterBuff[i].TempMicRoom;	//翻译间
			InterMicOpenBuff[mm][Ver2]=id;
			TempInterBuff[i].TempMicid = 0x00;
			TempInterBuff[i].TempMicRoom = 0x00;
		}
	}			 
			
	// 此通道号占用，先关闭它再打开
	OutChannleTemp = CheckInterOutNum(Ver2);	
	if (OutChannleTemp > 0)					
	{
		EPI_SntBuffInit(InterMicOnList[OutChannleTemp], InterMicOnList[OutChannleTemp] >> 8, InterMicClose);

		//以下为我加的，不同翻译间开启相同通道时互锁，如果该翻译间没有开启其它通道则开启扬声器
	     for (i=0;i<InterMaxNum;i++)
		   {
		    if (InterMicOpenBuff[i][OutChannleTemp]!=0)  //该通道存在其他开话筒译员
				{
				 if (i==mm)	//同翻译间关扬声器  查找相同通道号所在的翻译间
				 {
					EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|mm);
					InterMicOpenBuff[i][OutChannleTemp]=id;	 // 

				 }
				 else	//不同翻译间继续查找该翻译间有无其他输出通道号
				 {
					mm=i;
					InterMicOpenBuff[i][OutChannleTemp]=0; //该翻译间的输出通道号关闭
					break;
				 }
				}
		  }	

		  if (mm<InterMaxNum)
		  {
       for (j=0;j<InterMaxNum;j++)
		   {
			 if(InterMicOpenBuff[mm][j]!=0)	 //该翻译间有输出通道号
			 {
			  //InterMicOpenBuff[mm][OutChannleTemp]=0;
			  EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|mm);
			  break;
			 }
		   }
		  }
		  if (j>=InterMaxNum)
		   EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x40|mm);

		// 通知FPGA关通道
		if (OutChannleTemp >= 16){
			InterruptChannleFlag2 &= (~(1<<(OutChannleTemp-16)));
			WriteFpgaREG(REG_Default6,InterruptChannleFlag2);	
		}
		else{
			Temp = (1<<OutChannleTemp);
			InterruptChannleFlag &= (~Temp);
			WriteFpgaREG(REG_Interpreter,InterruptChannleFlag);	
		}

		InterMicOnList[OutChannleTemp] = 0;
		InterMicOutList[OutChannleTemp] = 0;	
	}
	// 保存输出通道号，通道输出通道号找出对应ID号来关闭它。
	InterMicOutList[Ver2] = Ver2;
	InterMicOnList[Ver2] = id;		
	
	
	
	for(i = 0; i < InterMicBuffSize; i++) 
	{
		if(InterMicBuff[i].OpenId == id) break;
	}

	// 关闭相同翻译间MIC
	OutChannleTemp = CheckInterRoomNum(InterMicBuff[i].Room);	
	if (OutChannleTemp > 0)	
	{
		EPI_SntBuffInit(InterMic[OutChannleTemp].OpenId, InterMic[OutChannleTemp].OpenId >> 8, InterMicClose);
		// 通知FPGA关通道
		if (OutChannleTemp >= 16){
			InterruptChannleFlag2 &= (~(1<<(OutChannleTemp-16)));
			WriteFpgaREG(REG_Default6,InterruptChannleFlag2);				
		}
		else{
			Temp = (1<<OutChannleTemp);
			InterruptChannleFlag &= (~Temp);
			WriteFpgaREG(REG_Interpreter,InterruptChannleFlag);				
		}

		InterMic[OutChannleTemp].OpenId = id;
		InterMic[OutChannleTemp].Room = OutChannleTemp;	
	}
					
	// 关闭相同翻译间扬声器,只要有上翻译间号就会执行
	if (i<InterMicBuffSize)
	{
		EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|InterMicBuff[i].Room);	

	}

	// 允许开MIC
	EPI_SntBuffInit(id, id >> 8, InterMicOpen);	
	SendToPcBuff(InterMicOpen,(uint8)(((id >> 4) & 0xF0) | 0x00),(uint8)id);
	
	// 通知FPGA开通道
	if (Ver2 >= 16){
		InterruptChannleFlag2 |= ((1<<(Ver2-16)));
		WriteFpgaREG(REG_Default6,InterruptChannleFlag2);		
	}
	else {
		Temp = (1<<Ver2);
		InterruptChannleFlag |= (Temp);
		WriteFpgaREG(REG_Interpreter,InterruptChannleFlag);			
	}
}
/*********************************************************************************************************
** Function name:       CheckInterOutNum
** Descriptions: 查找此译员机输出通道号是否被占用    开启话筒的输出通道号(1-15)   0:说明此通道号没占用，其它则返回被占用通道保存位置   
*********************************************************************************************************/
uint8 CheckInterOutNum(uint8 OutNum)
{
	uint8 i = 0;

	for (i=0;i<INTER_MAX_SPK_NUM;i++)
	{
		if (InterMicOutList[i] == OutNum)
		{
			return i;
		}
	}
	return 0;
}
/*********************************************************************************************************
** Function name:       CheckInterRoomNum
** Descriptions: 译员机翻译间号            
*********************************************************************************************************/
uint8 CheckInterRoomNum(uint8 RoomNum)
{
	uint8 i = 0;

	for (i=0;i<INTER_MAX_SPK_NUM;i++)
	{
		if (InterMic[i].Room == RoomNum)
		{
			return i;
		}
	}
	return 0;
}
/*********************************************************************************************************
** Function name:       InterrupChannleUpLan
** Descriptions: 更新译员机语言通道          
*********************************************************************************************************/
void InterrupChannleUpLan(void)
{	
	WriteFpgaREG(REG_Lan1Lan2,(SysInfo.ChannelLan[1]<<8)|SysInfo.ChannelLan[2]);
	WriteFpgaREG(REG_Lan3Lan4,(SysInfo.ChannelLan[3]<<8)|SysInfo.ChannelLan[4]);
	WriteFpgaREG(REG_Lan5Lan6,(SysInfo.ChannelLan[5]<<8)|SysInfo.ChannelLan[6]);
	WriteFpgaREG(REG_Lan7Lan8,(SysInfo.ChannelLan[7]<<8)|SysInfo.ChannelLan[8]);
	WriteFpgaREG(REG_Lan9Lan10,(SysInfo.ChannelLan[9]<<8)|SysInfo.ChannelLan[10]);
	WriteFpgaREG(REG_Lan11Lan12,(SysInfo.ChannelLan[11]<<8)|SysInfo.ChannelLan[12]);
	WriteFpgaREG(REG_Lan13Lan14,(SysInfo.ChannelLan[13]<<8)|SysInfo.ChannelLan[14]);			
	WriteFpgaREG(REG_Lan15Default,(SysInfo.ChannelLan[15]<<8)|0xaa);	
	WriteFpgaREG(REG_Lan16Lan17,(SysInfo.ChannelLan[16]<<8)|SysInfo.ChannelLan[17]);
	WriteFpgaREG(REG_Lan18Lan19,(SysInfo.ChannelLan[18]<<8)|SysInfo.ChannelLan[19]);
	WriteFpgaREG(REG_Lan20Lan21,(SysInfo.ChannelLan[20]<<8)|SysInfo.ChannelLan[21]);
	WriteFpgaREG(REG_Lan22Lan23,(SysInfo.ChannelLan[22]<<8)|SysInfo.ChannelLan[23]);
	WriteFpgaREG(REG_Lan24Lan25,(SysInfo.ChannelLan[24]<<8)|SysInfo.ChannelLan[25]);
	WriteFpgaREG(REG_Lan26Lan27,(SysInfo.ChannelLan[26]<<8)|SysInfo.ChannelLan[27]);
	WriteFpgaREG(REG_Lan28Lan29,(SysInfo.ChannelLan[28]<<8)|SysInfo.ChannelLan[29]);		
	WriteFpgaREG(REG_Lan30Lan31,(SysInfo.ChannelLan[30]<<8)|SysInfo.ChannelLan[31]);			
}
/*********************************************************************************************************
** Function name:       SlaveMicInvitePrg
** Descriptions: 设置代表机ID的话筒操作开状态，同时初始化UART1的发送缓冲区, 并且发送信息给PC id : 开启话筒的ID             
*********************************************************************************************************/
void SlaveMicInvitePrg(uint16_t DestId)
{  
	uint8_t temp = CmdModeFlag;
	// 打开指定id的代表机话筒
 	uint8_t AddrL, Ver;
	
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	CmdModeFlag = 0;
	EPI_SntBuffInit(AddrL, Ver, DeMicOpen);
	CmdModeFlag = 1;
	EPI_SntBuffInit(AddrL, Ver, DeMicOpen);

	// 把打开话筒指定id的信息传给PC机
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x00),(uint8)DestId);
	CmdModeFlag = temp;
}
/*********************************************************************************************************
** Function name:       FineIdPositionHandle
** Descriptions:  DestId:目的ID       查找目的ID的位置(采用二分查找算法)    当查找到后，返回正确的位置,否则返回125  
*********************************************************************************************************/
uint16_t FineIdPositionHandle(uint16_t DestId)
{
	if ((DelegInfoTable[DestId]>>8)!=0x00)
		return DestId;
	else
		return (MAX_DELEG_NUM + 1);
}
/*********************************************************************************************************
** Function name:       DestIdListHandle
** Descriptions:   代表机MIC操作信息的操作          
*********************************************************************************************************/
void DestIdListHandle(uint16 dest_id)
{
	uint16 i;
	
	i = FineIdPositionHandle(dest_id);
	if (i > MAX_DELEG_NUM) // 此ID不在ID列表中,临时接上了一台代表机
	{					
		InsertOnliSlrIdHandle(dest_id);
	}
}
/*********************************************************************************************************
** Function name:       SlaveCallChair
** Descriptions:   代表机向主席机申请内部通话      
*********************************************************************************************************/
void SlaveCallChair(uint16 dest_id)
{
	if (SysInfo.MeetSta == MEET_STATE)
	{
	    if (CallChairOnList[0]==dest_id)
			EPI_SntBuffInit(0xDE, ((uint8)(dest_id >> 4) & 0xF0) | 0x0F, (uint8)(dest_id));
	    else if (OnlineCallChairNum<CALL_CHAIR_ON_NUM)		// 申请列表没满
		{
			CallChairOnList[OnlineCallChairNum++] = dest_id;
			// 发给主席机,有代表机申请内部通话
			EPI_SntBuffInit(0xDE, ((uint8)(dest_id >> 4) & 0xF0) | 0x0F, (uint8)(dest_id));	
		}
		else
		{
			EPI_SntBuffInit(dest_id, dest_id>>8, CallChairFull);	
		}
	}
}
/*********************************************************************************************************
** Function name:       SetSlaveStaticPrg	  DestId : 目的ID..CmdL：参数
** Descriptions:      设置某个代表机的状态(只有挂在此主机/扩展主机下的代表机/主席机才有效果)             
*********************************************************************************************************/
void SetSlaveStaticPrg(uint16 DestId, uint8 CmdL)
{
	switch (CmdL) {
		case YES:
		case ABS:
		case NO:
		   	DelegInfoTable[DestId] &= (VNC+0xff00);
		   	DelegInfoTable[DestId] |= ((CmdL - 0x24) << 4);			    
			break;
		case AA_SER:
		case A_SER:
		case O_SER:
		case F_SER:
		case FF_SER:
		   	DelegInfoTable[DestId] &= (VNC+0xff00);
		   	DelegInfoTable[DestId] |= ((CmdL - 0x44) << 4);			    
			break;
		case PerOne:
		case PerTwo:
		case PerThree:
		case PerFour:
		case PerFive:
		   	DelegInfoTable[DestId] &= (VNC+0xff00);
		   	DelegInfoTable[DestId] |= ((CmdL - 0x64) << 4);			    
			break;
		case DisWork:
		    DelegInfoTable[DestId] |= DelegUnit;		
			break;
		case EnWork:
	     	DelegInfoTable[DestId] &= (~DelegUnit+0xff00);	
			break;
		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       HEX_Send
** Descriptions:        单元机代码HEX文件提取与打包               
*********************************************************************************************************/
void HEX_Send(void)
{
	//////////////////////////////////////////////////////////////////////
	unsigned char surveyF,surveyB;

	if (((unsigned char *)NetRxPoint != NetRxProcess1) && 
		((unsigned char *)NetRxPoint != NetRxProcess1 + 1))
	{
		surveyF = *(NetRxProcess1 + 1);
		surveyB = *NetRxProcess1;
	//	UARTCharPut(UART0_BASE, surveyF);
	//	UARTCharPut(UART0_BASE, surveyB);
	}
	else
	{
		return;
	}

	if ((surveyF == 0x0D) && (surveyB == 0x3A))
	{
		tempNUM = 0;
		NetRxProcess1++;
		NetRxProcess1++;
		if (NetRxProcess1 >= ((unsigned char *)(NetRxBuff+NET_RX_BUFF_SIZE)))
		{
			NetRxProcess1 = (unsigned char *)NetRxBuff;
		}
		return;
	}
	else if ((surveyF == 0x3A) || ((surveyF == 0x0D) && (surveyB == 0x0A)))
	{
		switch(surveyF)
		{
			case 0x3A:
			{
				*(unsigned short *)SMS_WR_Point = 0xFFFF;				//第一个双字节为目标地址
				SMS_WR_Point += 2;
				if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
				{
					SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
				}

				AddrPoint = SMS_WR_Point;								//把用于存字节数大小的指针地址保存
				*(SMS_WR_Point+1) = 0x00;								//第二个双字节为字节大小
				SMS_WR_Point += 2;
				if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
				{
					SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
				}

				*(unsigned short *)SMS_WR_Point = 0x0400;				//第三个双字节为控制字节
				SMS_WR_Point += 2;
				if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
				{
					SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
				}

				*SMS_WR_Point = surveyB;
				SMS_WR_Point += 1;
				if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
				{
					SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
				}
				tempNUM = 7;
				break;
			}
			case 0x0D:
			{
				*(unsigned short *)SMS_WR_Point = 0xFCFC;
				tempNUM += 2;
				SMS_WR_Point += 2;
				if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
				{
					SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
				}

				*AddrPoint = tempNUM-4;
				SMS_TxSizeCount += (tempNUM / 2);
				tempNUM = 0;
				FramePoint = SMS_WR_Point;
				break;
			}
			default:
				break;
		}
	}
	else if (((surveyB == 0x3A) && (surveyF == 0x0A)) || ((surveyB == 0x0D) && (surveyF == 0x0A)) || (surveyB == 0x0D))
	{
		switch(surveyB)
			{
				case 0x3A:
				{
					*(unsigned short *)SMS_WR_Point = 0xFFFF;				//第一个双字节为目标地址
					SMS_WR_Point += 2;
					if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
					{
						SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
					}

					AddrPoint = SMS_WR_Point;								//把用于存字节数大小的指针地址保存
					*(SMS_WR_Point+1) = 0x00;								//第二个双字节为字节大小
					SMS_WR_Point += 2;
					if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
					{
						SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
					}

					*(unsigned short *)SMS_WR_Point = 0x0400;				//第三个双字节为控制字节
					SMS_WR_Point += 2;
					if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
					{
						SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
					}
					tempNUM = 6;
					break;
				}
				case 0x0D:
				{
					if (surveyF == 0x0A)
					{
						break;
					}

					*SMS_WR_Point = surveyF;

					tempNUM++;
					SMS_WR_Point += 1;
					if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
					{
						SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
					}

					*(unsigned short *)SMS_WR_Point = 0xFCFC;
					tempNUM += 2;
					SMS_WR_Point += 2;
					if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
					{
						SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
					}
				
					*AddrPoint = tempNUM-4;
					SMS_TxSizeCount += (tempNUM / 2);
					tempNUM = 0;
					FramePoint = SMS_WR_Point;	
					break;
				}
				default:
					break;
			}	
	}
	else if ((surveyF == 0xFC) && (surveyB == 0xFC))
	{

		if (SMS_WR_Point == (unsigned char *)SMS_Tx_Buff)
		{
			if ((*(SMS_Tx_Buff + SMS_TX_BUF_SIZE - 2) & 0x00F0) == 0x30)
		 	{
		 		if ((*(SMS_Tx_Buff + SMS_TX_BUF_SIZE - 1) & 0x0100) == 0)
				{
					NetRxTail = (unsigned short *)(NetRxProcess1 + 2);
					NetRxProcess = NetRxTail;
					DonwLoader = 0;
					RxTime = 0;
					tempNUM = 0;
				} 
		 	}
		}
		else
		{
			if ((SMS_WR_Point-2) == (unsigned char *)SMS_Tx_Buff)
			{
				if ((*(SMS_Tx_Buff + SMS_TX_BUF_SIZE - 1) & 0x00F0) == 0x30)
				{
					if (*SMS_Tx_Buff &0x0100 == 0x0000)
					{
						NetRxTail = (unsigned short *)(NetRxProcess1 + 2);
						NetRxProcess = NetRxTail;
						DonwLoader = 0;
						RxTime = 0;
						tempNUM = 0;	
					}
				}
			}
			else
			{
				 if ((*(SMS_WR_Point - 3) & 0xF0) == 0x30)
				 {
		 			if ((*(SMS_WR_Point - 2) & 0x01) == 0)
					{
						NetRxTail = (unsigned short *)(NetRxProcess1 + 2);
						NetRxProcess = NetRxTail;
						DonwLoader = 0;
						RxTime = 0;
						tempNUM = 0;
					} 
		 		}	
			}
		}
		SMS_WR_Point = FramePoint;	
	}
	else
	{
		*SMS_WR_Point = surveyF;//假如不是这些控制字节存到打包里的数据区域；
		SMS_WR_Point += 1;
		if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
		{
			SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
		}
		*SMS_WR_Point = surveyB;
		SMS_WR_Point += 1;
		if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
		{
			SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
		}
		tempNUM += 2;
	}


	NetRxProcess1++;
	NetRxProcess1++;
	if (NetRxProcess1 >= ((unsigned char *)(NetRxBuff+NET_RX_BUFF_SIZE)))
	{
		NetRxProcess1 = (unsigned char *)NetRxBuff;
	}
}
/*********************************************************************************************************
** Function name:       NetRxDataProcess
** Descriptions:        接收网络数据处理               
*********************************************************************************************************/
void NetRxDataProcess(void)
{
	uint16_t KeyData = 0;	
	
//	if (NetRxRelay == 0){
//		NetRxTail = NetRxProcess;	
//	}
	
	if (NetRxPoint != NetRxProcess){
		KeyData = *NetRxProcess;
	}
	else {
		return;
	}

	NetRxProcess++;
	if (NetRxProcess>=(NetRxBuff+NET_RX_BUFF_SIZE))
	{
		NetRxProcess = NetRxBuff;
	}
	NetRxProcess1 = (unsigned char *)NetRxProcess;
	
	/*if((KeyData>0x0)&&(KeyData<0x80)&&(!NetRxBuff_Type_DWIN)){
		NetRxBuff_Size = ((KeyData/2)-2);
	}else{
		NetRxBuff_Type = KeyData;
		if((!NetRxBuff_Type_DWIN)&&(NetRxBuff_Type>=0xA000)&&(NetRxBuff_Type<=0xA00F))			//DWIN的类型,下个包开始根据数据大小判断DWIN的格式结尾0xFCFC
		{
			NetRxBuff_Type_DWIN=0x01;
			
		}else if(NetRxBuff_Type_DWIN){															//本包开始根据数据大小判断DWIN的格式结尾0xFCFC
			if(NetRxBuff_Size_i>=NetRxBuff_Size){
				NetRxBuff_Size_i = 0x0;
				NetRxBuff_Type_DWIN = 0x0;
				NetRxBuff_Size = 0x0;
				if (KeyData == 0xFCFC)
				{
					PC_NetProcess(NetRxTail);							// 接收PC机信息处理		
					NetRxTail = NetRxProcess;
					NetRxProcess1 = (unsigned char *)NetRxProcess;
				}
			}else{
				NetRxBuff_Size_i++;
			}
		}else{*/					//之前非DWIN数据
			if (KeyData == 0xFCFC)
			{
				PC_NetProcess(NetRxTail);							// 接收PC机信息处理		
				NetRxTail = NetRxProcess;
				NetRxProcess1 = (unsigned char *)NetRxProcess;
			}
	//	}
	//}
	//if (KeyData == 0xFCFC)
	//{
	//	PC_NetProcess(NetRxTail);							// 接收PC机信息处理		
	//	NetRxTail = NetRxProcess;
	//	NetRxProcess1 = (unsigned char *)NetRxProcess;
	//}
}
/*********************************************************************************************************
** Function name:       MasterSpcCmdHandle
** Descriptions: 主席机特殊命令处理      dest_id ：目的ID..Para	：命令参数        
*********************************************************************************************************/
void MasterSpcCmdHandle(uint16 dest_id, uint8 Para)
{
	switch (Para) 
	{
		case MSignStart :	// 主席机请求发起签到
			{	
				if ((SignFuncFlag == FALSE)||(StartScanOnID > 0)) break;  	// 如果签到功能和不在扫描单元没有开放,则退出
				if (SysInfo.MeetSta == MEET_STATE) 
				{
					// 应答主席机
					CmdModeFlag = 0;
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead + (uint8)((dest_id >> 8) & 0x0F), 0x0E);
					CmdModeFlag = 1;
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead + (uint8)((dest_id >> 8) & 0x0F), 0x0E);			        
					// 下发现在发起签到的ID号
					WriteFpgaREG(REG_MySignStart,dest_id);
					MasterSigneId = dest_id;
					SignMaxNum 		= 1000;				// 签到总数为1000
					WriteFpgaREG(REG_SignMaxNum,SignMaxNum);
					WifiInfor.wififlag.wififlag.wifisign = 1;
					WriteFpgaREG(REG_WifiControl,(WifiInfor.wififlag.controlwifi & WIFIDEBUG));
					WifiStateInit();
					SysModeSwitch(SIGN_IN_STATE);
										
					SignStartFlag 	= TRUE;				// 开始签到标志
					// 发送应到总人数给带屏的主席机
					CmdModeFlag = 0;
					EPI_SntBuffInit(0xD9, 0x3F, 0xE8);
					CmdModeFlag = 1;
					EPI_SntBuffInit(0xD9, 0x3F, 0xE8);			        

					// 向PC机返回签到数量
					SendToPcBuff(0x03,0x32,0xE8);
					// 开始进行签到
					CmdModeFlag = 0;
				    EPI_SntBuffInit(0xD2, SignCmdHead | 0x0F, SignStart);
					CmdModeFlag = 1;
				    EPI_SntBuffInit(0xD2, SignCmdHead | 0x0F, SignStart); 
				}
			}
			break;
		case MVoteStart : 	// 主席机请求发起表决
			{
				if ((VoteFuncFlag == FALSE)||(StartScanOnID > 0)) break;		// 如果没有启动表决功能，则退出
				if (SysInfo.MeetSta == MEET_STATE) {
					// 应答主席机
					EPI_SntBuffInit((uint8)(dest_id), VoteCmdHead + (uint8)((dest_id >> 8) & 0x0F), 0x2E);			        
					U_REG_MyVoteStart.U_REG_MyVoteStart.Id = dest_id;
					U_REG_MyVoteStart.U_REG_MyVoteStart.LookRes = 0;
					WriteFpgaREG(REG_MyVoteStart,U_REG_MyVoteStart.REG_MyVoteStartVal);
					VoteTimeState = 0;
					VoteSignedFlag = FALSE;					// 不需要签到就可以表决
					VoteResTypeFlag = FALSE;				// 最后一次有效(客观)
					// 应答PC
					SendToPcBuff(0x02,0x03,0x00);
					// 向PC机器返回表决类型
					SysInfo.VoteType = VOTE_MODE;
					WifiInfor.wififlag.wififlag.votefirst = 0;
					WifiInfor.wififlag.wififlag.votesigned = 0;
					WifiInfor.wififlag.wififlag.votemode = SysInfo.VoteType;
					WifiInfor.wififlag.wififlag.wifivote = 1;
					WriteFpgaREG(REG_WifiControl,(WifiInfor.wififlag.controlwifi & WIFIDEBUG));
					WifiStateInit();
					SendToPcBuff(0x02,0x00,0x00);
					SysModeSwitch(VOTE_STATE);		
				    // 返回PC表决已经开始
					gbit_start_vote = TRUE;			// 表决开始
					SendToPcBuff(0x02,0x02,0x00);
				}
			}
			break;
		case MSignOver:		// 主席机请求停止签到
			{
		 	    // 所有机器结束签到
		 	    if (VoteRdPnt == VoteWrPnt) //	接收的数据处理完才能退出
		 	    {
					// 应答主席机
					CmdModeFlag = 0;
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);	
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);
					CmdModeFlag = 1;
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);	
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);
					// 返回PC签到已经结束
					SendToPcBuff(0x03,0x01,0x00);										
		 	   	 	WifiInfor.wififlag.wififlag.wifisign = 0;
					WriteFpgaREG(REG_WifiControl,(WifiInfor.wififlag.controlwifi & WIFIDEBUG));					
					SysModeSwitch(MEET_STATE);
					ICStateUpdate();
		 	    }
				else
				{
					SignExitTimeStart = 1;
				}
			}
			break;
		case MVotePause : 	// 主席机请求停止表决
			if(gbit_start_vote == TRUE) 
			{
				if (VoteRdPnt == VoteWrPnt) //	接收的数据处理完才能退出
				{
					QuitVoteFlag	= FALSE; 		// 停止表决后,不直接退出表决
					VoteResRtnEnable= TRUE;			// 使能返回表决结果计数器				
					// 应答主席机
					CmdModeFlag = 0;
					EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x30);
					EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x30);
					CmdModeFlag = 1;
					EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x30);
					EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x30);
				}
				else
				{
					SignExitTimeStart = 1;
				}
				if (U_REG_MyVoteStart.U_REG_MyVoteStart.Id == dest_id)
				{
					VoteLookres = 1;
//					U_REG_MyVoteStart.U_REG_MyVoteStart.LookRes = 1;
//					WriteFpgaREG(REG_MyVoteStart,U_REG_MyVoteStart.REG_MyVoteStartVal);
				}
			}
			break;
		case MVoteOver:
			if (gbit_start_vote == TRUE) 
			{
				gbit_start_vote = FALSE;
				QuitVoteFlag	= TRUE; 		// 直接退出表决

				WifiInfor.wififlag.wififlag.wifivote = 0;
				WriteFpgaREG(REG_WifiControl,(WifiInfor.wififlag.controlwifi & WIFIDEBUG));
				ICStateUpdate();
				// 返回PC表决已经结束
				SendToPcBuff(0x02,0x01,0x00);
				// 应答主席机
				CmdModeFlag = 0;
				EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x2F);
				EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x2F);
				CmdModeFlag = 1;
				EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x2F);
				EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x2F);
				U_REG_MyVoteStart.U_REG_MyVoteStart.LookRes = 0;
				U_REG_MyVoteStart.U_REG_MyVoteStart.Id = 0;
				WriteFpgaREG(REG_MyVoteStart,U_REG_MyVoteStart.REG_MyVoteStartVal);
			}
			break;
		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       SlaveMicStop
** Descriptions:设置此ID的话筒关操作状态，同时初始化UART1的发送缓冲区, 并且发送信息给PC
*********************************************************************************************************/
void SlaveMicStop(uint16 DestId)
{
	uint8 AddrL, Ver;
	uint8_t temp = CmdModeFlag;
	// 关闭指定id的代表机话筒
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	CmdModeFlag = 0;
	EPI_SntBuffInit(AddrL, Ver, DeMicClose);
	CmdModeFlag = 1;
	EPI_SntBuffInit(AddrL, Ver, DeMicClose);

	// 把关闭话筒指定id的信息传给PC机
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x01),(uint8)DestId);
	CmdModeFlag = temp;
}
/*********************************************************************************************************
** Function name:       MicCloseChannleNumber
** Descriptions:		给关闭话筒的单元分配声音通道0-7  
*********************************************************************************************************/
uint8_t MicCloseChannleNumber(uint16 DestId)
{
	uint8 i = 0, temp = 8;
	
	for (i=0; i<8; i++)
	{
		if (UnitMicNumList[i] == DestId)
		{
			UnitMicNumList[i] = 0;
			WriteFpgaREG((REG_Channel0ID + i), 0x0000);
			temp = i;
			break;
		}
	}
	return (temp);
}
/*********************************************************************************************************
** Function name:       SlaveMicEnWait
** Descriptions:		等待的ID   
*********************************************************************************************************/
void SlaveMicEnWait(uint16 DestId)
{
	uint8 AddrL, Ver;
	
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	EPI_SntBuffInit(AddrL, Ver, DeMicWait);

	// 把撤消话筒等待的id信息传给PC机
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x02),(uint8)DestId);
}
/*********************************************************************************************************
** Function name:       SlaveMicDisWait
** Descriptions:	撤消等待的ID         
*********************************************************************************************************/
void SlaveMicDisWait(uint16 DestId)
{
	uint8 AddrL, Ver;
	uint8_t temp = CmdModeFlag;
	
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	CmdModeFlag = 0;
	EPI_SntBuffInit(AddrL, Ver, DeMicDisWait);
	CmdModeFlag = 1;
	EPI_SntBuffInit(AddrL, Ver, DeMicDisWait);
	CmdModeFlag = temp;
	// 把撤消话筒等待的id信息传给PC机
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x03),(uint8)DestId);
}
/*********************************************************************************************************
** Function name:       MicOpenWaitHandle
** Descriptions:	对话筒开启的等待进行处理           
*********************************************************************************************************/
void MicOpenWaitHandle(uint16 id) 
{
	uint8 i;
	
	// Normal 模式下，如果已经满了，则进行等待处理
	if (SysInfo.MicWaitNum > 0) {						// 当前等待列表中有话筒等待
		for (i = 0; i < SysInfo.MicWaitNum; i++) {
			if (WaitMicList[i] == id) break;
		}
		if (i < SysInfo.MicWaitNum) {					// 此ID已经在等待列表中
			for (; i < SysInfo.MicWaitNum - 1; i++){
				WaitMicList[i] = WaitMicList[i + 1];
				WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
			}
			WaitMicList[SysInfo.MicWaitNum - 1] = 0;
			WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x00);
			SysInfo.MicWaitNum--;
			
			// 发送撤消其等待状态
			SlaveMicDisWait(id);
		}
		else if (SysInfo.MicWaitNum < SysInfo.SpkNum) {// 等待列表还没有满
			// 发送使能其等待状态
			SlaveMicEnWait(id);
			WaitMicList[SysInfo.MicWaitNum++] = id;		// 处理等待列表
			WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), id);
		}
		else {									// 等待列表已满
			// 发送关闭话筒命令
			MicCloseChannleNumber(id);
			SlaveMicStop(id);
			
		}
	}
	else {										// 当前等待列表为空
		// 发送使能其等待状态					
		SlaveMicEnWait(id);
		WaitMicList[SysInfo.MicWaitNum++] = id;			// 处理等待列表
		WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), id);
	}
}
/*********************************************************************************************************
** Function name:       SlaveMIcApplyOpen
** Descriptions:根据系统的话筒数量限制，话筒开启模式来对此ID的话筒开申请进行处理  代表机成功申请开后，再关闭话筒,则不需要申请           
*********************************************************************************************************/
void SlaveMIcApplyOpen(uint16 id)
{
	uint8 i;
	
	// 接收到了代表机的发言声请，将命令发给主席机裁决
	for (i = 0; i < SysInfo.MicOnSlaveNum; i++){  // 查找发言列表中是否有此ID
		if (SlaverMicOnList[i] == id) break;
	}

	if (i < SysInfo.MicOnSlaveNum) {	   		// 此ID处于开启话筒列表
		MicOnChannleNumber(id);
		EPI_SntBuffInit(id % 256, id / 256, DeMicOpen);
	}
	else if (SysInfo.MicWaitNum > 0) 
	{								// 当前有话筒正在申请发言
		for (i = 0; i < SysInfo.MicWaitNum; i++) {
			if (WaitMicList[i] == id) break;
		}
		if (i < SysInfo.MicWaitNum) // 此ID已经在申请发言列表中
		{			
			for (; i < (SysInfo.MicWaitNum - 1); i++){
				WaitMicList[i] = WaitMicList[i + 1];
				WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
			}
			WaitMicList[i] = 0;
			WriteFpgaREG((REG_Wait0ID + i), 0x0000);
			SysInfo.MicWaitNum--;						
			SlaveMicDisWait(id);		// 发送撤消其等待状态

			// 代表机撤消声请开话筒, 发送给带屏的主席机
			__disable_irq();
			if (CmdModeFlag == 1){
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDC, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDC, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
			}
			else if (CmdModeFlag == 0){
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDC, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDC, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
			}
			__enable_irq();
		}
		else 
		{							// 此ID不在申请列表中
			if (SysInfo.MicWaitNum < (SysInfo.SpkNum - SysInfo.MicOnSlaveNum)) 
			{	// 等待列表还没有满
				SlaveMicEnWait(id);	// 先进入等待模式
				WaitMicList[SysInfo.MicWaitNum++] = id;
				WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), id);
				// 代表机声请开话筒, 发送给带屏的主席机
				__disable_irq();
				if (CmdModeFlag == 1){
					CmdModeFlag = 0;
					EPI_SntBuffInit(0xDB, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
					CmdModeFlag = 1;
					EPI_SntBuffInit(0xDB, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				}
				else if (CmdModeFlag == 0){
					CmdModeFlag = 1;
					EPI_SntBuffInit(0xDB, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
					CmdModeFlag = 0;
					EPI_SntBuffInit(0xDB, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				}
				__enable_irq();
			}
			else 
			{						// 申请列表已满
				MicCloseChannleNumber(id);
				SlaveMicStop(id);	// 关闭话筒
			}
		}
	}
	else if (SysInfo.MicWaitNum == 0) 
	{
		if (SysInfo.MicOnSlaveNum < SysInfo.SpkNum) 
		{		// 当前开启的话筒数量已经达到额定值
			SlaveMicEnWait(id);	// 先进入等待模式			
			WaitMicList[SysInfo.MicWaitNum++] = id;
			WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), id);
			// 代表机声请开话筒, 发送给带屏的主席机
			__disable_irq();
			if (CmdModeFlag == 1){
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDB, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDB, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
			}
			else if (CmdModeFlag == 0){
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDB, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDB, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
			}
			__enable_irq();
		}
		else 
		{							// 申请列表已满
			MicCloseChannleNumber(id);
			SlaveMicStop(id);	// 关闭话筒
		}
	}
}
/*********************************************************************************************************
** Function name:       SlaveMicOpenHandle
** Descriptions:   代表机申请开话筒。根据系统的话筒数量限制，话筒开启模式来对此ID的话筒开申请进行处理             
*********************************************************************************************************/
void SlaveMicOpenHandle(uint16 id)
{
	uint8 SlaveMic_i, ChannelTemp=0,ChannelNum=0;
	uint8_t flag = CmdModeFlag;
	
	switch (SysInfo.MicMode) 
	{
		case APPLY:			// MIC开启模式为申请模式，主机只起转发的作用，由主席机决定是否开启
			if (SysInfo.MeetSta == MEET_STATE){ // 会议模式下才处理话筒申请
				SlaveMIcApplyOpen(id);		// 代表机MIC 开申请处理
			}
			else {							// 其他模式下,代表机申请开话筒,主机则强制行的关闭
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
			}
			break;
			
		case FREE:			// MIC声请模式为FREE 自由开启
			for (SlaveMic_i = 0; SlaveMic_i < SysInfo.MicOnSlaveNum; SlaveMic_i++){ 	// 查找,如果处于打开就返回不处理
				if (FreeMicOnList[SlaveMic_i] == id) break;
			}
			if (SlaveMic_i < SysInfo.MicOnSlaveNum){ 									// 处于开启话筒列表
				MicOnChannleNumber(id);
				EPI_SntBuffInit(id % 256, id / 256, DeMicOpen);				
			}
			else {
				if (SysInfo.MicOnSlaveNum < SysInfo.MicFreeOnNum) {				// 列表还未满
								
					ChannelTemp = MicOnChannleNumber(id);
					if (ChannelTemp == 1){
						SlaveMicInvitePrg(id);							//开启话筒的ID 					
						FreeMicOnList[SysInfo.MicOnSlaveNum++] = id;								
						// 系统当前开启的话筒数
						CmdModeFlag = 0;
						EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
						CmdModeFlag = flag;
						U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
						U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
					 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);	
						CameraSendOpenCode(id, 0, 0);						// 摄像跟踪处理
					}
					else{
						EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
					}
				}
				else {												// 列表已经满
					MicCloseChannleNumber(id);
					SlaveMicStop(id);
				}											
			}
			break;
			
		case NORMAL:
			for(SlaveMic_i = 0; SlaveMic_i < SysInfo.MicOnSlaveNum; SlaveMic_i++){ // 查询当前ID是不是在发言人列表里
				if(SlaverMicOnList[SlaveMic_i] == id) break;
			}
			if (SlaveMic_i < SysInfo.MicOnSlaveNum) {						// 所申请MIC开的ID处于发言列表中
				MicOnChannleNumber(id);
				EPI_SntBuffInit(id % 256, id / 256, DeMicOpen);
			}
			else  {														 	// 所申请MIC开的ID不处于发言列表中
				if(SysInfo.MicOnSlaveNum < SysInfo.SpkNum){ 		// 允许发言人数还没有满	
					ChannelTemp = MicOnChannleNumber(id);
					if (ChannelTemp == 1){
						SlaverMicOnList[SysInfo.MicOnSlaveNum++] = id;
						SlaveMicInvitePrg(id);					
						CameraSendOpenCode(id, 0, 0);// 摄像跟踪处理
					}
					else{
						EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
					}
				}
				else {
					MicOpenWaitHandle(id);
				}
			}
			
			// 系统当前开启的话筒数, 发送给有屏幕的代表机/主席机
			CmdModeFlag = 0;
			EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
			CmdModeFlag = flag;
			U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
			U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
		 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);	
			break;

		case FIFO:
			for(SlaveMic_i = 0; SlaveMic_i < SysInfo.MicOnSlaveNum; SlaveMic_i++){ // 查询当前ID是不是在发言人列表里
				if(SlaverMicOnList[SlaveMic_i] == id) break;
			}
			
			if (SlaveMic_i < SysInfo.MicOnSlaveNum) {							// 所声请MIC开的ID处于发言列表中
				MicOnChannleNumber(id);
				EPI_SntBuffInit(id % 256, id / 256, DeMicOpen);
			}
			else {																// 所声请MIC开的ID不处于发言列表中
				if(SysInfo.MicOnSlaveNum < SysInfo.SpkNum) {					// 允许发言人数还没有满	
					ChannelTemp = MicOnChannleNumber(id);					    //先选择通道再发开话筒
					if (ChannelTemp == 1){
						SlaverMicOnList[SysInfo.MicOnSlaveNum++] = id;
						SlaveMicInvitePrg(id);														
						CameraSendOpenCode(id, 0, 0);// 摄像跟踪处理
					}
					else{
						EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
					}
				}
				else{ 										// 发言列表已经满
					if (SysInfo.MicOnSlaveNum>0){
						ChannelNum = MicCloseChannleNumber(SlaverMicOnList[0]);
						ChannelTemp = MicOnChannleNumber(id);						//先选择通道再发开话筒
						if (ChannelTemp == 1){
							CameraSendOpenCode(id, 0, 1);// 摄像跟踪处理
							
							SlaveMicStop(SlaverMicOnList[0]);		// 按FIFO的规则最前的id停止					
							SlaveMicInvitePrg(id);						// 开启当前申请开的ID
							//开话筒列表数据向后移动一位					
							for(SlaveMic_i = 0; SlaveMic_i < (SysInfo.MicOnSlaveNum - 1); SlaveMic_i++)
							{
								SlaverMicOnList[SlaveMic_i] = SlaverMicOnList[SlaveMic_i + 1];
							}
							SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = id;		//新开的话筒加入列表
						}
						else{
							if (ChannelNum < 8){
								UnitMicNumList[ChannelNum] = SlaverMicOnList[0];
								WriteFpgaREG((REG_Channel0ID + ChannelNum), SlaverMicOnList[0]);
							}
						}			
					}
				}
			}
			
			// 系统当前开启的话筒数, 发送给有屏幕的代表机/主席机
			CmdModeFlag = 0;
			EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
			CmdModeFlag = flag;
			U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
			U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
		 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);			
			break;

		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       SlaveMIcApplyClose
** Descriptions: 根据系统的话筒数量限制，话筒开启模式来对此ID的话筒开申请进行处理 代表机成功申请开后，再关闭话筒,则不需要申请         
*********************************************************************************************************/
void SlaveMIcApplyClose(uint16 id)
{
	uint8 i;
	uint8_t flag = CmdModeFlag;
	for (i = 0; i < SysInfo.MicOnSlaveNum; i++) 
	{				// 查找
		if (SlaverMicOnList[i] == id) break;
	}
	if (i < SysInfo.MicOnSlaveNum) 
	{								// 处于开启话筒列表
		MicCloseChannleNumber(id);
		SlaveMicStop(id);							// 关闭话筒
		
		for(; i < (SysInfo.MicOnSlaveNum - 1); i++)
			SlaverMicOnList[i] = SlaverMicOnList[i + 1];
		SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = 0;
		if(SysInfo.MicOnSlaveNum > 0) SysInfo.MicOnSlaveNum--;

		// 系统当前开启的话筒数
		CmdModeFlag = 0;
		EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
		CmdModeFlag = flag;
		U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
		U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
	 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);		// 摄像跟踪处理
		CameraSendClsCode(id, 0);
	}
	else 
	{
		CmdModeFlag = 0;
		EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
		CmdModeFlag = 1;
		EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
		CmdModeFlag = flag;
	}			
}
/*********************************************************************************************************
** Function name:       SlaveMicCloseHandle
** Descriptions:  代表机关话筒根据系统的话筒数量限制，话筒开启模式来对此ID的话筒关申请进行处理           
*********************************************************************************************************/
void SlaveMicCloseHandle(uint16 id)
{
	uint8 i,channeltemp=0;
	uint8_t temp = CmdModeFlag;
	
	switch (SysInfo.MicMode) 
	{
		case APPLY:			// MIC声请模式, 主机只起转发的作用
			SlaveMIcApplyClose(id);
			break;
			
		case FREE:			// MIC声请模式为FREE 自由关闭
			for (i = 0; i < SysInfo.MicOnSlaveNum; i++) {				// 查找
				if (FreeMicOnList[i] == id) break;
			}
			
			if (i < SysInfo.MicOnSlaveNum) {								// 处于开启话筒列表
				MicCloseChannleNumber(id);
				SlaveMicStop(id);
				
				for (; i < (SysInfo.MicOnSlaveNum - 1); i++)
				{
					FreeMicOnList[i] = FreeMicOnList[i + 1];
				}
				FreeMicOnList[i] = 0;
				SysInfo.MicOnSlaveNum--;
				// 系统当前开启的话筒数
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
				CmdModeFlag = temp;
				U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
				U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
			 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);				// 摄像跟踪处理
				CameraSendClsCode(id, 0);
			}
			else 
			{
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
			}			
			break;
			
		case NORMAL:
			for(i = 0; i < SysInfo.MicOnSlaveNum; i++) // 查询当前ID是不是在发言人列表里
			{	
				if(SlaverMicOnList[i] == id) break;
			}
			
			if (i < SysInfo.MicOnSlaveNum) // 所声请MIC关的ID处于发言列表中
			{				
				MicCloseChannleNumber(id);
				SlaveMicStop(id);
				
				for(; i < (SysInfo.MicOnSlaveNum - 1); i++)
				{
					SlaverMicOnList[i] = SlaverMicOnList[i + 1];
				}
				SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = 0;
				SysInfo.MicOnSlaveNum--;
				if (SysInfo.MicWaitNum > 0) // 当前有话筒等待
				{			
					channeltemp = MicOnChannleNumber(WaitMicList[0]);
					if (channeltemp == 1){
						SlaveMicInvitePrg(WaitMicList[0]);
						
						SlaverMicOnList[SysInfo.MicOnSlaveNum++] = WaitMicList[0];
						// 摄像跟踪处理
						CameraSendWaitCode(id);
	
						for (i = 0; i < (SysInfo.MicWaitNum - 1); i++)
						{
							WaitMicList[i] = WaitMicList[i + 1];
							WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
						}
						WaitMicList[SysInfo.MicWaitNum - 1] = 0;
						WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x0000);
						SysInfo.MicWaitNum--;
					}
					else{
						CmdModeFlag = 1;
						EPI_SntBuffInit((WaitMicList[0]) % 256, (WaitMicList[0]) / 256, DeMicClose);
						CmdModeFlag = 0;
						EPI_SntBuffInit((WaitMicList[0]) % 256, (WaitMicList[0]) / 256, DeMicClose);
						for (i = 0; i < (SysInfo.MicWaitNum - 1); i++)
						{
							WaitMicList[i] = WaitMicList[i + 1];
							WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
						}
						WaitMicList[SysInfo.MicWaitNum - 1] = 0;
						WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x0000);
						SysInfo.MicWaitNum--;
					}
				}
				else 
				{
					// 摄像跟踪处理
					CameraSendClsCode(id, 0);
				}
			}
			else 						// 所声请MIC开的ID不处于发言列表中
			{
				// 处理好等待队列
				for (i = 0; i < SysInfo.MicWaitNum; i++) {    //修复开启6个单元,再开第七个,又关后,没有清除对应等待的缓存,使得再关闭一个单元，原来等待的单元又会开启。解决：如果关闭的是等待的单元,则将等待队列数减一_xavior
					if (WaitMicList[i] == id) break;
				}
				if (i < SysInfo.MicWaitNum)                   //修复开启6个单元,再开第七个,又关后,没有清除对应等待的缓存,使得再关闭一个单元，原来等待的单元又会开启。解决：如果关闭的是等待的单元,则将等待队列数减一_xavior
				{		// 此ID已经在等待列表中
					for (; i < (SysInfo.MicWaitNum - 1); i++){
						WaitMicList[i] = WaitMicList[i + 1];
						WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
					}
					WaitMicList[i] = 0;
					WriteFpgaREG((REG_Wait0ID + i), 0x0000);
					SysInfo.MicWaitNum--;						
					// 代表机撤消声请开话筒, 发送给带屏的主席机
					//CmdModeFlag = 1;
					//EPI_SntBuffInit(0xDC, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
					//CmdModeFlag = 0;
					//EPI_SntBuffInit(0xDC, ((uint8)(id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				}
				
				CmdModeFlag = 1;
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
				CmdModeFlag = 0;
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
			}
			
			// 系统当前开启的话筒数
			CmdModeFlag = 0;
			EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
			CmdModeFlag = temp;	
			U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
			U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
		 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);			
			break;

		case FIFO:
			for(i = 0; i < SysInfo.MicOnSlaveNum; i++) {	// 查询当前ID是不是在发言人列表里
				if(SlaverMicOnList[i] == id) break;
			}
			
			if (i < SysInfo.MicOnSlaveNum) // 所声请MIC关的ID处于发言列表中
			{								
				SlaveMicStop(id);
				MicCloseChannleNumber(id);
				for(; i < SysInfo.MicOnSlaveNum - 1; i++)
				{
					SlaverMicOnList[i] = SlaverMicOnList[i + 1];
				}
				SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = 0;
				SysInfo.MicOnSlaveNum--;
				
				CameraSendClsCode(id, 0);// 摄像跟踪处理
			}
			else 
			{								// 所声请MIC开的ID不处于发言列表中
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
			}
			
			// 系统当前开启的话筒数
			CmdModeFlag = 0;
			EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
			CmdModeFlag = temp;	
			U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
			U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
		 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);			
			break;

		default :
			break;
	}
	CmdModeFlag = temp;
}
/*********************************************************************************************************
** Function name:       MasterMicInvitePrg
** Descriptions:      设置主席机ID的话筒操作开状态，并且发送信息给PC        
*********************************************************************************************************/
void MasterMicInvitePrg(uint16 DestId)
{  
	// 打开指定id的代表机话筒
 	uint8 AddrL, Ver;
	
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	EPI_SntBuffInit(AddrL, Ver, MaMicOpen);

	// 把打开话筒指定id的信息传给PC机
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x00),(uint8)DestId);
}
/*********************************************************************************************************
** Function name:       MasterMicStopPrg
** Descriptions:   设置主席机话筒关操作状态，并且发送信息给PC          
*********************************************************************************************************/
void MasterMicStopPrg(uint16 DestId)
{
	uint8 AddrL, Ver;
	
	// 关闭指定id的代表机话筒
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	EPI_SntBuffInit(AddrL, Ver, MaMicClose);

	// 把关闭话筒指定id的信息传给PC机
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x01),(uint8)DestId);
}
/*********************************************************************************************************
** Function name:       MasterApplyHandle
** Descriptions:  根据系统的话筒数量限制，话筒开启模式来对此ID的话筒开申请进行处理         
*********************************************************************************************************/
void MasterApplyHandle(uint16 id, uint8 EnDis)
{
	uint8 i,channeltemp=0;
	uint8_t flag = CmdModeFlag;
	if (SysInfo.MicWaitNum==0x00)
	{
		CmdModeFlag = 0;
		EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
		CmdModeFlag = 1;
		EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
		CmdModeFlag = flag;
		return;
	}
	for (i = 0; i < SysInfo.MicWaitNum; i++) 
	{
		if (WaitMicList[i] == id) break;
	}
	if (i>=SysInfo.MicWaitNum)   //不在等待列表中
	{
		CmdModeFlag = 0;
		EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
		CmdModeFlag = 1;
		EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
		CmdModeFlag = flag;	 
		return;
	}

	switch (EnDis) 
	{
		case 0x00:
			// 接收到了主席机的命令，代表机的发言申请得到了同意
			for(i = 0; i < SysInfo.MicOnSlaveNum; i++) 
			{			// 查询当前ID是不是在发言人列表里
				if(SlaverMicOnList[i] == id) break;
			}
						
			if (i < SysInfo.MicOnSlaveNum) 
			{						// 已经开启
				
			}
			else if(SysInfo.MicOnSlaveNum < SysInfo.SpkNum) 
			{	
				// 发送信息给主席机，以确认收到主席机的同意指令
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));	
				// 允许发言人数还没有满
				channeltemp = MicOnChannleNumber(id);
				if (channeltemp == 1){	
					SlaverMicOnList[SysInfo.MicOnSlaveNum++] = id;
//					MicOnChannleNumber(id);
					SlaveMicInvitePrg(id);
				}
				else{
					CmdModeFlag = 0;
					EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
					CmdModeFlag = 1;
					EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
				}
								
				// 处理好等待队列
				for (i = 0; i < SysInfo.MicWaitNum; i++) 
				{
					if (WaitMicList[i] == id) break;
				}
				if (i < SysInfo.MicWaitNum) 
				{					// 此ID已经在等待列表中
					for (; i < (SysInfo.MicWaitNum - 1); i++)
					{
						WaitMicList[i] = WaitMicList[i + 1];
						WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
					}
					WaitMicList[SysInfo.MicWaitNum - 1] = 0;
					WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x0000);
					SysInfo.MicWaitNum--;
				}

				// 系统当前开启的话筒数
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
				U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
				U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
			 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);				// 摄像跟踪处理
				if (channeltemp == 1){
					CameraSendOpenCode(id, 0, 0);
				}
			}
			else 
			{						// 发言人数此时已经达到满的状态
				// 发送信息给主席机，以确认收到主席机的同意指令
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));			
				// 采用FIFO的模式处理
				MicCloseChannleNumber(SlaverMicOnList[0]);
				SlaveMicStop(SlaverMicOnList[0]);					// 按FIFO的规则最前的id停止
				
				channeltemp = MicOnChannleNumber(id);
				if (channeltemp == 1){
					SlaveMicInvitePrg(id);
					for(i = 0; i < (SysInfo.MicOnSlaveNum - 1); i++)
					{
						SlaverMicOnList[i] = SlaverMicOnList[i + 1];
					}
					SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = id; 
				}
				else{
					CmdModeFlag = 0;
					EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
					CmdModeFlag = 1;
					EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
				}

				// 处理好等待队列
				for (i = 0; i < SysInfo.MicWaitNum; i++) 
				{
					if (WaitMicList[i] == id) break;
				}
				if (i < SysInfo.MicWaitNum) {	// 此ID已经在等待列表中
					for (; i < (SysInfo.MicWaitNum - 1); i++)
					{
						WaitMicList[i] = WaitMicList[i + 1];
						WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
					}
					WaitMicList[SysInfo.MicWaitNum - 1] = 0;
					WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x0000);
					SysInfo.MicWaitNum--;
				}

				// 摄像跟踪处理
				if (channeltemp == 1){
					CameraSendOpenCode(id, 0, 1);
				}
			}
			break;
		case 0x01:		// 反对开启
			for (i = 0; i < SysInfo.MicWaitNum; i++) 
			{
				if (WaitMicList[i] == id) break;
			}
			// 此ID已经在申请列表中, PC机已发关闭命令
			if (i < SysInfo.MicWaitNum) 
			{			
				for (; i < (SysInfo.MicWaitNum - 1); i++)
				{
					WaitMicList[i] = WaitMicList[i + 1];
					WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
				}
				WaitMicList[SysInfo.MicWaitNum - 1] = 0;
				WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x0000);
				SysInfo.MicWaitNum--;
				SlaveMicDisWait(id);		// 发送撤消其等待状态

				// 代表机撤消申请开话筒, 发送给带屏的主席机
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				break;
			}

			for (i = 0; i < SysInfo.MicOnSlaveNum; i++) 
			{				// 查找
				if (SlaverMicOnList[i] == id) break;
			}
			if (i < SysInfo.MicOnSlaveNum) 
			{								// 处于开启话筒列表
				MicCloseChannleNumber(id);
				SlaveMicStop(id);
				
				for(; i < (SysInfo.MicOnSlaveNum - 1); i++)
				{
					SlaverMicOnList[i] = SlaverMicOnList[i + 1];
				}
				SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = 0;
				if(SysInfo.MicOnSlaveNum > 0) 
				{
					SysInfo.MicOnSlaveNum--;
				}

				// 系统当前开启的话筒数
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
				CmdModeFlag = flag;
				U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
				U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
			 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);				// 摄像跟踪处理
				CameraSendClsCode(id, 0);
			}
			else 
			{
				CmdModeFlag = 0;
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
				CmdModeFlag = 1;
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
			}			
			break;
		default :
			break;
	}
	CmdModeFlag = flag;	
}
/*********************************************************************************************************
** Function name:       MasterOpenMicHandle
** Descriptions:        主席机开启话筒             
*********************************************************************************************************/
void MasterOpenMicHandle(uint16 destid)
{
	uint8 i = 0,temp = 0;

	for(i = 0; i < SysInfo.MicOnMasterNum; i++) // 查询当前ID是不是在发言人列表里
	{			
		if(MasterMicOnList[i] == destid) break;
	}
	
	if (i < SysInfo.MicOnMasterNum) 						// 所声请MIC开的ID处于发言列表中
	{						
		MicOnChannleNumber(destid);
		EPI_SntBuffInit(destid % 256, destid / 256, MaMicOpen);
	}
	else 
	{											// 所声请MIC开的ID不处于发言列表中
		if(SysInfo.MicOnMasterNum < (SysInfo.MicChannelNum-SysInfo.SpkNum)) 		// 允许发言人数还没有满	
		{	
			temp = MicOnChannleNumber(destid);
			if (temp == 1){
				MasterMicOnList[SysInfo.MicOnMasterNum++] = destid;
				MasterMicInvitePrg(destid);
				// 摄像跟踪处理
				CameraSendOpenCode(destid, 1, 0);
			}
			else{
				EPI_SntBuffInit(destid % 256, destid / 256, MaMicClose);
			}
		}
		else
		{
		    MicCloseChannleNumber(destid);
			MasterMicStopPrg(destid);
		}
	}
}
/*********************************************************************************************************
** Function name:       MasterClsMicHandle
** Descriptions:         主席机关闭话筒             
*********************************************************************************************************/
void MasterClsMicHandle(uint16 destid)
{
	uint8 i = 0;
	
	for(i = 0; i < SysInfo.MicOnMasterNum; i++) // 查询当前ID是不是在发言人列表里
	{	
		if(MasterMicOnList[i] == destid) break;
	}
	
	if (i < SysInfo.MicOnMasterNum) // 所声请MIC关的ID处于发言列表中
	{	

		MicCloseChannleNumber(destid);
		MasterMicStopPrg(destid);
		
		for(; i < (SysInfo.MicOnMasterNum - 1); i++)
		{
			MasterMicOnList[i] = MasterMicOnList[i + 1];
		}
		MasterMicOnList[SysInfo.MicOnMasterNum - 1] = 0;
		SysInfo.MicOnMasterNum--;						
		CameraSendClsCode(destid, 1);// 摄像跟踪处理
			
	}
	else 						// 所声请MIC开的ID不处于发言列表中
	{								
		EPI_SntBuffInit(destid % 256, destid / 256, MaMicClose);
	}
}
/*********************************************************************************************************
** Function name:       SlaveFrameProcess
** Descriptions:        从机信息帧接收处理              
*********************************************************************************************************/
void SlaveFrameProcess(void)
{	
	uint16 KeyData = 0;								
	uint8 i = 0;
	uint8 ToBuff[40] = {0};
	CmdModeFlag = 0;
	
	if(RxWriteCount != RxReadCount) 
	{	
		SlaveMsgProcess(&EpiRxFrameBuff[RxReadCount]);
		EpiRxFrameBuff[RxReadCount].AddrL = 0;
		EpiRxFrameBuff[RxReadCount].Para = 0;
		EpiRxFrameBuff[RxReadCount].Cmmd = 0;
		RxReadCount++;		
		if(RxReadCount >= RX_Frame_MAX_SIZE)
			RxReadCount = 0;			
	}
	
	
	CmdModeFlag = 0;
}
/*********************************************************************************************************
** Function name:       WifiFrameProcess
** Descriptions:        WIFI信息帧接收处理              
*********************************************************************************************************/
void WifiFrameProcess(void)
{
	if (WifiRxWrite == WifiRxRead)	return;
	CmdModeFlag = 1;
	SlaveMsgProcess(&WifiRxBuff[WifiRxRead]);
	__disable_irq();
	WifiRxRead++;
	if (WifiRxRead>=RX_Frame_MAX_SIZE){
		WifiRxRead = 0;
	}
	__enable_irq();
	if (ChkOnlineIdFlag == FALSE){	
		MicInforProcess();
	}
	CmdModeFlag = 0;
}
/*********************************************************************************************************
** Function name:  OpenToJoin
** Descriptions:   WIFI单元开关话筒操作即加入单元列表(扫描单元还没扫到，开了话筒没有声音)
** explain: 
*********************************************************************************************************/
void DelegateOpenToJoin(uint16_t id)
{
	uint16_t i=0;
	if (CmdModeFlag == 1){
		if ((DelegInfoTable[id]>>8) == 0x00) {
			DelegInfoTable[id] = 0xF200;
			DelegInfoTable[id] |= DelegUnit | ONLINE;
			OnlineSlaveNum++;
			OnlineWifiNum++;
			for (i=0;i<=OnlineNum;i++){	  //查询ID列表
				if (OnliSlaverId[i] == id)  break;
			}
			if (i>OnlineNum){
				OnliSlaverId[OnlineNum] = id;	// 在线代表机/主席机ID
				OnlineNum++;
				RefreshFlag = 1;
				if (OnlineNum<EPI_SCAN_TX_MAX_SIZE){	   
					EpiTxFrameMax_Scan = OnlineNum;
				}
				else{   
					EpiTxFrameMax_Scan = EPI_SCAN_TX_MAX_SIZE;
				}
			}
		}
	}
}
void ChairOpenToJoin(uint16_t id)
{
	uint16_t i=0;
	if (CmdModeFlag == 1){
		if ((DelegInfoTable[id]>>8) == 0x00) {
			DelegInfoTable[id] = 0xF300;
			DelegInfoTable[id] |= ChairUnit | ONLINE;
			OnlineChairNum++;
			OnlineWifiNum++;
			for (i=0;i<=OnlineNum;i++){	  //查询ID列表
				if (OnliSlaverId[i] == id)  break;
			}
			if (i>OnlineNum){
				OnliSlaverId[OnlineNum] = id;	// 在线代表机/主席机ID
				OnlineNum++;//
				RefreshFlag = 1;
				if (OnlineNum<EPI_SCAN_TX_MAX_SIZE){	   
					EpiTxFrameMax_Scan = OnlineNum;
				}
				else{   
					EpiTxFrameMax_Scan = EPI_SCAN_TX_MAX_SIZE;
				}
			}
		}
	}
}
/*********************************************************************************************************
** Function name:  CheckChannelWifi     
** Descriptions :  检查话筒8个通道中前6个通道（WIFI只用前6个通道）是否被全部占用          
*********************************************************************************************************/
uint8_t CheckChannelWifi(void)
{
	uint8_t	i;

	for (i=0; i<6; i++){
		if (UnitMicNumList[i] == 0x0000)	return (1);
	}
	return (0);
}
/*********************************************************************************************************
** Function name:       MicInforProcess
** Descriptions: 主席机和代表机MIC操作信息的操作
** explain:      主席单元除具有代表单元的全部功能外，还有以下 优先权功能/发言/控制代表单元话筒 功能       
*********************************************************************************************************/
void MicInforProcess(void)
{
	uint8 ver2,Ver2Temp;
	uint16 dest_id;
	uint8 i = 0,channeltemp=0;
	
	if(MicRdPnt == MicWrPnt) 
	{
		return;
	}	
		
	dest_id = MicOprInforBuff[MicRdPnt].Oprid;
	ver2 	= MicOprInforBuff[MicRdPnt].OprMod;
	
	MicRdPnt++;
	if (MicRdPnt >= MIC_INFOR_BUF_SIZE)
	{
		MicRdPnt = 0x00;
	}

	Ver2Temp = ver2 & 0x7F;
	switch (Ver2Temp) 
	{
		case DeMicOpen:						// 代表机申请开话筒			
			if (ChairKeyMaskSts == FALSE) {	// 没有主席机的屏蔽优先级功能执行
				DelegateOpenToJoin(dest_id);
				SlaveMicOpenHandle(dest_id);//((DelegInfoTable[DestId] & 0xF000) != 0xF000)				
			}
			break;
		case DeMicClose:				// 代表机申请关话筒			
			if (ChairKeyMaskSts == FALSE) {
				DelegateOpenToJoin(dest_id);
				SlaveMicCloseHandle(dest_id);	
			}
			break;
		case MaMicOpen:				// 主席机开话筒, 只要有空闲话筒就开
			ChairOpenToJoin(dest_id);
			MasterOpenMicHandle(dest_id);
			break;
		case MaMicClose:				// 主席机关话筒
			ChairOpenToJoin(dest_id);
			MasterClsMicHandle(dest_id);
			break;			

		case MaPausePri:				// 执行主席机优先权屏蔽命令
			break;
		case MaDisPausePri:				// 由本机发起的屏蔽撤销主席机优先权命令 
			break;
		case MaClsPri:					// 主席机关闭所有代表机话筒命令
			ChairOpenToJoin(dest_id);
			for(i = 0; i < SysInfo.MicOnMasterNum; i++){ // 查询当前ID是不是在发言人列表里
				if(MasterMicOnList[i] == dest_id) break;
			}
			if (i<SysInfo.MicOnMasterNum)	//在发言人列表中
			{
				if (SysInfo.MicWaitNum > 0) {		// 有等待事件
					MasterClsPriWtHandle(dest_id);
				}
				else {						// 无等待事件
					MasterClsPriHandle(dest_id);
				}			

				if ((ver2 & 0x80) == 0x00) {
					DestIdListHandle(dest_id);						// 代表机/主席机ID列表处理
				}
			}
			else 
			{
				if(SysInfo.MicOnMasterNum < (SysInfo.MicChannelNum-SysInfo.SpkNum)) //不在发言人列表中并且主席发言未满
				{
					channeltemp = MicOnChannleNumber(dest_id);
					if (channeltemp == 1){
					MasterMicOnList[SysInfo.MicOnMasterNum++] = dest_id;
//					MicOnChannleNumber(dest_id);		
					if (SysInfo.MicWaitNum > 0) {		// 有等待事件
						MasterClsPriWtHandle(dest_id);
					}
					else {						// 无等待事件
						MasterClsPriHandle(dest_id);
					}
					}			
				}
			}
			break;
		case ChairAgree:				// 主席机同意代表机开启话筒
			if ((ChairKeyMaskSts == FALSE) && (SysInfo.MicMode == APPLY)) {
				MasterApplyHandle(dest_id, 0);	
			}
			break;
		case ChairAbject:				// 主席机反对代表机开启话筒
			if ((ChairKeyMaskSts == FALSE) && (SysInfo.MicMode == APPLY)) {
				MasterApplyHandle(dest_id, 1);	
			}
			break;
		case ApplyService:				// 代表/主席单元申请茶水服务请求命令
		case CancleService:				// 代表/主席单元撤消茶水服务请求命令
			// 把申请茶水的id传给PC机
			SendToPcBuff(0x0F,(uint8)(((dest_id >> 4) & 0xF0) | ((ver2 & 0x7F) - ApplyService)),(uint8)dest_id);
			break;
		case CallChair:					// 代表机向主席机发出内部通话申请
			SlaveCallChair(dest_id);
			break;
		case CanclChair:				// 代表机向主席机撤销内部通话申请			
			if (OnlineCallChairNum>0)
			{	
				for (i=0; i<CALL_CHAIR_ON_NUM; i++){
					if (dest_id == CallChairOnList[i])	break;
				}

				if (i<CALL_CHAIR_ON_NUM){		// 申请的ID在列表中
					CallChairOnList[0] = 0;
					OnlineCallChairNum--;
					// 通知所有主席机
					EPI_SntBuffInit(0xD2, MeetCmdHead | 0x0F, CanclChair);
				}
				else {
					EPI_SntBuffInit(dest_id, dest_id>>8, CanclChair);
				}
			}
			break;
		case ChairAgreeCall:
			EPI_SntBuffInit(0xD2, MeetCmdHead | 0x0F, ChairAgreeCall);
		case ApplyHelp:							// 译员机申请帮助
		case ApplyHelpAsk:
			SendToPcBuff(0x15,(uint8)(((dest_id >> 4) & 0xF0) | ((ver2 & 0x7F) - ApplyHelp)),(uint8)dest_id);			
			break;
		case CardOn:
			SendToPcBuff(0x19,(uint8)(((dest_id >> 4) & 0xF0) | 0x00),(uint8)dest_id);
			break;
		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       ResultSendBuffInit
** Descriptions:签到/表决ID及结果发送缓冲区初始化  MeetMode, 系统模式; dest_id : 已签到/表决ID, Res: 表决结果             
*********************************************************************************************************/
void ResultSendBuffInit(uint8 MeetMode, uint16 dest_id, uint8 Res)
{
	switch (MeetMode) 
	{
		case MEET_STATE :
			MicOprInforBuff[MicWrPnt].Oprid  = dest_id;	
			MicOprInforBuff[MicWrPnt].OprMod = Res;
			MicWrPnt++;
			if(MicWrPnt >= MIC_INFOR_BUF_SIZE)
				MicWrPnt = 0;
			break;
		case VOTE_STATE :
			SignVoteIdBuf[VoteWrPnt] = dest_id;
			VoteResultBuf[VoteWrPnt] = Res;
			VoteWrPnt++;
			if(VoteWrPnt >= VOTE_SENT_BUF_SIZE) {
				VoteWrPnt = 0;
				StopScanFlag = TRUE;
			}
			VoteSendPcFlag = TRUE;
			break;
		case SIGN_IN_STATE:
			SignVoteIdBuf[VoteWrPnt] = dest_id;
			VoteWrPnt++;
			if(VoteWrPnt >= VOTE_SENT_BUF_SIZE) 
			{
				VoteWrPnt = 0;
				StopScanFlag = TRUE;
			}
			VoteSendPcFlag = TRUE;
			break;
		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       SignInHandle
** Descriptions:   签到处理         dest_id : 目的ID..Para ：接收到的代表机信息帧协议参数低8位      
*********************************************************************************************************/
void SignInHandle(uint16 dest_id, uint8 Para)
{   	
	if (Para == SignOk) // 应答数据正确
	{	
		ResultSendBuffInit(SIGN_IN_STATE, dest_id, 0);
	}
	else if (Para == MSignOver) // 主席机请求停止签到
	{				
		MasterSpcCmdHandle(dest_id, MSignOver);
	}
}
/*********************************************************************************************************
** Function name:       VoteHandle
** Descriptions:  无记名表决处理    dest_id : 目的ID..ver2 ：接收到的代表机信息帧协议参数低8位           
*********************************************************************************************************/
void VoteHandle(uint16 dest_id, uint8 ver2)
{
	uint8 k, p;
	uint8 NoteVer, temp=0;
	uint16 i = 0;
	
	// 查找出当前ID的位置
	i = FineIdPositionHandle(dest_id);

	switch (SysInfo.VoteType) 
	{
		case VOTE_MODE:
			
			if((ver2 >= YES) && (ver2 <= NO)) 
			{
				k = (uint8)((dest_id - 1) % 8);
				p = (uint8)((dest_id - 1) >> 0x03);
				
				// 保存具体的表决结果
		    	NoteVer = ((DelegInfoTable[i] >> 4) & 0x07);		// 当前ID的代表机投票状态
			    temp = ver2 - 0x24;
							    
			    if (NoteVer == 0x00) 								// 当前机器还没有表决
				{								
					VoteResult[temp - 1]++;
					SetSlaveStaticPrg(i, ver2);	
					ResultSendBuffInit(VOTE_STATE, dest_id, temp);
					if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    }
			    else 
				{												// 当前机器已经表决了
			    	// 已经改变投票结果,同时最后一次表决有效,才上发给PC
			    	if ((NoteVer != temp) && (VoteResTypeFlag == FALSE)) 
					{					
						VoteResult[NoteVer - 1]--;
						VoteResult[temp - 1]++;
						SetSlaveStaticPrg(i, ver2);					
						SignVoteSendList[p] &= ~(0x01 << k);
						ResultSendBuffInit(VOTE_STATE, dest_id, temp);
						if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    	}
			    }
			}
			else if ((ver2 == MVoteOver) || (ver2 == MVotePause)) 
			{	
						
				MasterSpcCmdHandle(dest_id, ver2);
			}
			break;
		case SELECT_MODE:
			if((ver2 >= PerFive) && (ver2 <= PerOne)) {
				k = (uint8)((dest_id - 1) % 8);
				p = (uint8)((dest_id - 1) >> 0x03);	

				// 保存具体的选举结果
		    	NoteVer = ((DelegInfoTable[i] >> 4) & 0x07);	// 当前ID的代表机投票状态
			    temp = ver2 - 0x64;
			    
			    if (NoteVer == 0x00) {								// 当前机器还没有表决
					VoteResult[temp - 1]++;
					SetSlaveStaticPrg(i, ver2);
					ResultSendBuffInit(VOTE_STATE, dest_id, temp);
					if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    }
			    else {												// 当前机器已经表决
			    	// 已经改变投票结果,同时最后一次表决有效
			    	if ((NoteVer != temp) && (VoteResTypeFlag == FALSE)) {					
						VoteResult[NoteVer - 1]--;
						VoteResult[temp - 1]++;
						SetSlaveStaticPrg(i, ver2);
						SignVoteSendList[p] &= ~(0x01 << k);
						ResultSendBuffInit(VOTE_STATE, dest_id, temp);
						if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    	}
			    }
			}
			break;
		case EVALUA_MODE:
			if((ver2 >= AA_SER) && (ver2 <= FF_SER)) {
				// 保存已经表决的ID
				k = (uint8)((dest_id - 1) % 8);
				p = (uint8)((dest_id - 1) >> 0x03);

				// 保存具体的评级结果
		    	NoteVer = ((DelegInfoTable[i] >> 4) & 0x07);	// 当前ID的代表机投票状态
			    temp = ver2 - 0x44;
			    
			    if (NoteVer == 0x00) {								// 当前机器还没有表决
					VoteResult[temp - 1]++;
					SetSlaveStaticPrg(i, ver2);

					ResultSendBuffInit(VOTE_STATE, dest_id, temp);
					if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    }
			    else {												// 当前机器已经表决了
			    	// 已经改变投票结果,同时最后一次表决有效
			    	if ((NoteVer != temp) && (VoteResTypeFlag == FALSE)) {					
						VoteResult[NoteVer - 1]--;
						VoteResult[temp - 1]++;
						SetSlaveStaticPrg(i, ver2);
						SignVoteSendList[p] &= ~(0x01 << k);
						ResultSendBuffInit(VOTE_STATE, dest_id, temp);
						if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    	}
			    }
			}
			break;
		default :
			break;
	}

	if (SysInfo.VoteType==VOTE_MODE) 
		EPI_SntBuffInit(dest_id, (dest_id>>8)|VoteCmdHead, 0xF1+temp);
	else
		EPI_SntBuffInit(dest_id, (dest_id>>8)|VoteCmdHead, 0xF0+temp);
}
/*********************************************************************************************************
** Function name:       SetIdHandle
** Descriptions:        dest_id : ID  ver2：设置ID的命令           
*********************************************************************************************************/
void SetIdHandle(uint16 dest_id, uint8 ver2)
{
	if ((ver2==AckId)&&(dest_id == Setdestid))	//ID有冲突
	{
		IDRepeat=TRUE;
		Repeat_ID=dest_id;
	}
	if ((ver2 == AckId) && (SysInfo.CurrSetId == dest_id)) 
	{
		if (SysInfo.CurrSetId<(SYS_MAX_DELEG_NUM + MAX_EXMAC_NUM))//我加的
		{
			Setdestid=SysInfo.CurrSetId;
			KeyOpr=TRUE;
			SetIDDelay=0;
		}
	}
}
/*********************************************************************************************************
** Function name:       InterpreterOutNumLoad
** Descriptions:    译员机输出通道号       Adr : 地址 Cmmd:命令        
*********************************************************************************************************/
void InterpreterOutNumLoad(uint16 dest_id,uint8 Cmmd)
{
	uint8 Ver;
	uint8 i;
	
	Ver = Cmmd & 0xF0;
	switch (Ver) 
	{
		case 0x10:			// 译员机打开输出通道号
			Ver = Cmmd & 0x0F;
			if ((Ver>0)&&(Ver<INTER_MAX_SPK_NUM))
			{								
				InterpreterMicOpenHandle(dest_id,Ver);
			}		
			break;
		case 0x20:			// 译员机关话筒输出通道号
			Ver = Cmmd & 0x0F;
			if ((Ver>0)&&(Ver<INTER_MAX_SPK_NUM))
			{	
				InterpreterMicCloseHandle(dest_id,Ver);	
			}	
			break;
		case 0x80:			// 译员上传自己当前的翻译间号(1-16)
			Ver = Cmmd & 0x1F;
			if ((Ver>0)&&(Ver<17))
			{
				for(i = 0; i < InterMicBuffSize; i++) 
				{
					if(InterMicBuff[i].OpenId == dest_id) break;
				}

				if (i==InterMicBuffSize)	// 此ID不存在
				{
					InterMicBuff[CurrentCount].OpenId = dest_id;
					InterMicBuff[CurrentCount].Room = Ver;
					CurrentCount++;
					if (CurrentCount>=InterMicBuffSize)
					{
						CurrentCount = 0;
					}
				}
				else
				{
					InterMicBuff[i].OpenId = dest_id;
					InterMicBuff[i].Room = Ver;		
				}
		   
				for (i=0;i<5;i++)   //临时Buff保存ID和翻译间
				{
					if (TempInterBuff[i].TempMicid==0x00)
					{
						TempInterBuff[i].TempMicid = dest_id;
						TempInterBuff[i].TempMicRoom = Ver;
						break;
					}
				}
				if(i>=5)
				{
					TempInterBuff[5].TempMicid = dest_id;
					TempInterBuff[5].TempMicRoom = Ver;
				}		   
			}
			break;
		case 0x90:
			Ver = Cmmd & 0x0F;
			if (Ver<INTER_MAX_SPK_NUM)
			{								
				InterExtendOpenHandle(dest_id,Ver+16);
			}
			break;
		case 0xA0:
			Ver = Cmmd & 0x0F;
			if (Ver<INTER_MAX_SPK_NUM)
			{	
				InterpreterMicCloseHandle(dest_id,Ver+16);	
			}					
			break;
		case 0xB0:
			Ver = Cmmd & 0x1F;
			if (Ver<16)
			{
				Ver = Ver + 16;
				for(i = 0; i < InterMicBuffSize; i++) 
				{
					if(InterMicBuff[i].OpenId == dest_id) break;
				}

				if (i==InterMicBuffSize)	// 此ID不存在
				{
					InterMicBuff[CurrentCount].OpenId = dest_id;
					InterMicBuff[CurrentCount].Room = Ver;
					CurrentCount++;
					if (CurrentCount>=InterMicBuffSize)
					{
						CurrentCount = 0;
					}
				}
				else
				{
					InterMicBuff[i].OpenId = dest_id;
					InterMicBuff[i].Room = Ver;		
				}
		   
				for (i=0;i<5;i++)   //临时Buff保存ID和翻译间
				{
					if (TempInterBuff[i].TempMicid==0x00)
					{
						TempInterBuff[i].TempMicid = dest_id;
						TempInterBuff[i].TempMicRoom = Ver;
						break;
					}
			   }
			   if(i>=5)
			   {
				  TempInterBuff[5].TempMicid = dest_id;
				  TempInterBuff[5].TempMicRoom = Ver;
			   }		   
			}				
			break;
		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       InterUpdata
** Descriptions:  译员机通道号，翻译间信息帧处理  DestId : 目的ID..CmdL：参数 
** explain:		  备份主机启动后，译员机的相关信息帧处理             
*********************************************************************************************************/
void InterUpdata(uint16 dest_id,uint8 ver2)
{
    uint8 temp,i,mm;
	temp=ver2&0xF0;
	if (temp==0x50)
	{
		mm=ver2&0x07;

		if ((ver2&0x08)==0x00) //代表机开话筒
		{
			for(i = 0; i < SysInfo.MicOnSlaveNum; i++){ // 查询当前ID是不是在发言人列表里
				if(SlaverMicOnList[i] == dest_id) break;
			}
			if (i < SysInfo.MicOnSlaveNum) {			// 所申请MIC开的ID处于发言列表中
				return;	
			}
			UnitMicNumList[mm]=dest_id;
			WriteFpgaREG((REG_Channel0ID + mm), dest_id);
			SysInfo.MicOnSlaveNum++;
			if (SysInfo.MicMode==FREE){
				for (i=0;i<8;i++){
		 			if(FreeMicOnList[i]==0){	  //正在发言列表
						FreeMicOnList[i]=dest_id;
						break;
					}
				}
			}
			else {
				for (i=0;i<8;i++){
					if(SlaverMicOnList[i]==0){	  //正在发言列表
						SlaverMicOnList[i]=dest_id;
						break;
					}
				}
			}
		}
		else
		{
			for(i = 0; i < SysInfo.MicOnMasterNum; i++){ // 查询当前ID是不是在发言人列表里
				if(MasterMicOnList[i] == dest_id) break;
			}
			if (i < SysInfo.MicOnMasterNum) {			// 所申请MIC开的ID处于发言列表中
				return;						
			}
			UnitMicNumList[mm]=dest_id;
			WriteFpgaREG((REG_Channel0ID + mm), dest_id);
			SysInfo.MicOnMasterNum++;
			for (i=0;i<8;i++) {
				if (MasterMicOnList[i]==0){	  //正在发言列表
					MasterMicOnList[i]=dest_id;
					break;
				}
			}
		}
	}
	else if (temp==0x00){			  //把译员机通道号存入相应列表中
		SysInfo.MicOnInterNum++;
		if (ver2<INTER_MAX_SPK_NUM)	{   
			InterMicOutList[ver2]=ver2;
			InterMicOnList[ver2]=dest_id;
		}  
	}
	else{								   //译员机翻译间信息
		InterMic[CurrentCount].OpenId = dest_id;
		InterMic[CurrentCount].Room = ver2;
		CurrentCount++;
	}
}
/*********************************************************************************************************
** Function name:       WritePara
** Descriptions:        备份主机初始化写相关寄存器和变量               
*********************************************************************************************************/
void WritePara(void)
{
	switch (SysInfo.MeetSta)
	{
		case MEET_STATE:
			break;
		case VOTE_STATE:
			gbit_start_vote = TRUE;
			if (SendResult==0x00){	
				VoteResult[0]=0;
				VoteResult[1]=0;
				VoteResult[2]=0;
				VoteResult[3]=0;
				VoteResult[4]=0;
			}
			else{				//主机处于结果查看模式
				VoteResRtnStart = TRUE;
				WriteFpgaREG(REG_VoteRes1,VoteResult[0]);
				WriteFpgaREG(REG_VoteRes2,VoteResult[1]);
				WriteFpgaREG(REG_VoteRes3,VoteResult[2]);
				WriteFpgaREG(REG_VoteRes4,VoteResult[3]);
				WriteFpgaREG(REG_VoteRes5,VoteResult[4]);
				U_REG_MyVoteStart.U_REG_MyVoteStart.LookRes = 1;
				WriteFpgaREG(REG_MyVoteStart,U_REG_MyVoteStart.REG_MyVoteStartVal);
			}
			break;
		case SIGN_IN_STATE:
			WriteFpgaREG(REG_MySignStart,0x8000|MasterSigneId);
			WriteFpgaREG(REG_SignNum,0);     	//签到人数清零
			SignStartFlag = TRUE;				// 开始签到标志
			break;
		case SET_ID_STATE:
			if (SetIDNumber==0)
				SetIDNumber=1;
			SysInfo.CurrSetId=SetIDNumber;
			break;
		default:
			break;
	}
	WriteFpgaREG(REG_Default1,RegValue);	  //恢复话筒等相关信息
    SysInfo.SpkNum = ((RegValue>>8)&0x07);    //最大发言人
	if ((SysInfo.SpkNum > 6)|(SysInfo.SpkNum==0)){ 
		SysInfo.SpkNum = 1;
	}
	WriteFpgaREG(REG_Interpreter,InterPara);
	WriteFpgaREG(REG_Other2,InterVal);
}
/*********************************************************************************************************
** Function name:       GetStatus
** Descriptions:                      
*********************************************************************************************************/
void GetStatus(void)
{
	uint16 temp,mm;
	temp=ReadFpgaREG(REG_Default3);
	if (temp==0x00)  return;
	SysInfo.MicMode=((temp&0x180)>>7);
	SysInfo.MeetSta=temp&0x07;
	InterruptChannleFlag=ReadFpgaREG(REG_Interpreter);
	SysInfo.SpkNum=((temp>>9)&0x07)+1;
	if ((SysInfo.SpkNum>6)|(SysInfo.SpkNum==0))
		SysInfo.SpkNum=1;
	if((temp&0x2000)==0x2000)
		BidFlag = TRUE;
	else 
		BidFlag = FALSE;
	switch (SysInfo.MeetSta)
	{
		case MEET_STATE:
			VoteResRtnOk	= TRUE;
			gbit_start_vote = FALSE;
			VoteResRtnStart = FALSE;
			SignStartFlag = FALSE;
			VoteRtnFlag = FALSE;
			VoteMode=0x00;
			break;
		case  VOTE_STATE:
			VoteMode= ReadFpgaREG(REG_Default4)&0xFF;
			SysInfo.VoteType=VoteMode/4;
			VoteResTypeFlag=((VoteMode>>1)&0x01);
			mm= ReadFpgaREG(REG_MyVoteStart);
   			if ((temp&0x1000)==0x00)	   //未有发送结果
			{
				VoteResRtnStart= FALSE;
				VoteResRtnOk	= TRUE;
				VoteRtnFlag = FALSE;
			}
			else 
			{
				VoteResRtnStart= TRUE;
				VoteResRtnOk	= FALSE;
				VoteRtnFlag = TRUE;
			}
			SendResult=((mm>>14)&0x01);
			if (SendResult!=0x00)
			{
				VoteResult[0] = ReadFpgaREG(REG_VoteRes1);
				VoteResult[1] = ReadFpgaREG(REG_VoteRes2);
				VoteResult[2] = ReadFpgaREG(REG_VoteRes3);
				VoteResult[3] = ReadFpgaREG(REG_VoteRes4);
				VoteResult[4] = ReadFpgaREG(REG_VoteRes5);
			}
			break;
		case  SIGN_IN_STATE:
			MasterSigneId=ReadFpgaREG(REG_MySignStart)|0x8000;
			break;
		case  SET_ID_STATE:
			SetIDNumber=((ReadFpgaREG(REG_Default3)&0x78)>>3);
			break;
		default:
			break;
	}
	RegValue=ReadFpgaREG(REG_Default1);
	InterPara=ReadFpgaREG(REG_Interpreter);
	InterVal=ReadFpgaREG(REG_Other2);
	SysInfo.InterChanTotalNum=((ReadFpgaREG(REG_Default5)&0x3E0)>>5);
}
/*********************************************************************************************************
** Function name:       BuildOnliIdListPrg
** Descriptions:        系统扫描在线ID后，建立一个在线的从机ID列表              
*********************************************************************************************************/
void BuildOnliIdListPrg(void)		
{
	uint16_t i = 0;
//	uint8_t Buff[4]={0,0,0,0};

	SysInfo.PollId = 1;

	// 在线最大单元
	#if (DEBUG_DOG == 1) 
	{
//		UARTCharPut(UART0_BASE, 0xdd);
//		UARTCharPut(UART0_BASE, 0xdd);
//		UARTCharPut(UART0_BASE, OnlineNum>>8);
//		UARTCharPut(UART0_BASE, OnlineNum);
	}
	#endif
	
	for (i=0; i<OnlineNum;i++)
	{
		if(DelegInfoTable[i] & ONLINE)
		{
			#if (DEBUG_DOG == 1) 
			{
//				UARTCharPut(UART0_BASE, OnliSlaverId[i]>>8);
//				UARTCharPut(UART0_BASE, OnliSlaverId[i]);
			}
			#endif
		}			
	}
	if (OnlineNum<EPI_SCAN_TX_MAX_SIZE)
	{
		EpiTxFrameMax_Scan = OnlineNum;
	}
	else
	{
		EpiTxFrameMax_Scan = EPI_SCAN_TX_MAX_SIZE;
	}
	// 列出在线主席机单元ID
	#if (DEBUG_DOG == 1) 
	{
//	UARTCharPut(UART0_BASE, 0xd1);
//	UARTCharPut(UART0_BASE, 0xd1);
//	UARTCharPut(UART0_BASE, OnlineChairNum>>8);
//	UARTCharPut(UART0_BASE, OnlineChairNum);
	}
	#endif
	
	for (i=0; i<OnlineNum;i++)
	{
		if(DelegInfoTable[i] & ChairUnit)
		{
			#if (DEBUG_DOG == 1) 
			{
//			UARTCharPut(UART0_BASE, OnliSlaverId[i]>>8);
//			UARTCharPut(UART0_BASE, OnliSlaverId[i]);
			}
			#endif
		}			
	}
	
	// 列出在线代表机单元ID
	#if (DEBUG_DOG == 1) 
	{
//	UARTCharPut(UART0_BASE, 0xd2);
//	UARTCharPut(UART0_BASE, 0xd2);
//	UARTCharPut(UART0_BASE, OnlineSlaveNum>>8);
//	UARTCharPut(UART0_BASE, OnlineSlaveNum);
	}
	#endif
	
	for (i=0; i<OnlineNum;i++)
	{
		if(DelegInfoTable[i] & DelegUnit)
		{
			#if (DEBUG_DOG == 1) 
			{
//			UARTCharPut(UART0_BASE, OnliSlaverId[i]>>8);
//			UARTCharPut(UART0_BASE, OnliSlaverId[i]);
			}
			#endif
		}			
	}	

	// 列出在线译员机单元ID
	#if (DEBUG_DOG == 1) 
	{
//		UARTCharPut(UART0_BASE, 0xd3);
//		UARTCharPut(UART0_BASE, 0xd3);
//		UARTCharPut(UART0_BASE, OnlineInterNum>>8);
//		UARTCharPut(UART0_BASE, OnlineInterNum);
	}
	#endif
	
	for (i=0; i<OnlineNum;i++)
	{
		if(DelegInfoTable[i] & InterUnit)
		{
			#if (DEBUG_DOG == 1) 
			{
//			UARTCharPut(UART0_BASE, OnliSlaverId[i]>>8);
//			UARTCharPut(UART0_BASE, OnliSlaverId[i]);
			}
			#endif
		}			
	}
	
	ScanData.State = 4;

	if (StartScanOnID == 2)
	{
		StartScanOnID = 0;
		PowerUpState = 2;
		SendToPcBuff(0x05, ((OnlineNum>>4)&0xF0)|0x01, OnlineNum&0xFF);
		SendToPcBuff(0x05, ((OnlineChairNum>>4)&0xF0)|0x02, OnlineChairNum&0xFF);
		SendToPcBuff(0x05, ((OnlineSlaveNum>>4)&0xF0)|0x03, OnlineSlaveNum&0xFF);
		SendToPcBuff(0x05, ((OnlineInterNum>>4)&0xF0)|0x04, OnlineInterNum&0xFF);		
	}
	RefreshFlag = 1;
	if ((PowerUpState == 1)&&(StartScanOnID==0))		// 仅上电扫完才执行
	{
		PowerUpState = 2;
		// 通知单元退出扫描在线单元
		EPI_SntBuffInit(0xD2, MeetCmdHead | 0x0F, ScanIDStop);	
		WriteFpgaREG(REG_Timer,0x0001);
	}
}
/*********************************************************************************************************
** Function name:    HostScanPrg
** Descriptions:     主机依据在线ID列表，来扫描在线的ID 
****************     会议下广播所有在线和不在线的单元，要让它们上码来。
****************   	 其它模式下只扫描在线的ID单元。
*********************************************************************************************************/
void HostScanPrg(void)
{	
	uint16_t dest_id = 0;
	uint8_t	 temp=CmdModeFlag;
	switch (SysInfo.MeetSta) 
	{
		case MEET_STATE:
			//if (StartScan==TRUE) break;		  //进入扫描单元 则停止扫描
		   // if (StopScanFlag == TRUE)  break;    //发送缓冲区慢暂停扫描
			//此处EpiTxFrameMax_Scan由改成EPI_SCAN_TX_MAX_SIZE因为做备份主机 时EpiTxFrameMax_Scan为0
			if ((EpiTxFrameCount<3)&&(DonwLoader == 0))//EpiTxFrameMax_Scan) 
			{	
				EPI_SntBuffInit( CurrID&0xff,(CurrID>>8)&0x0f|0x00, 0x24);
				//CurrID=0x03;
				CurrID++;
				//if(CurrID > SYS_MAX_DELEG_NUM)  		//扫描系统所有机器  从1--SYS_MAX_DELEG_NUM
				//				CurrID = 1;
				if(CurrID > 0x05)	
	       	    	CurrID = 0x03;
			}
			break;
		/*case VOTE_STATE:
			
			if (StopScanFlag == TRUE) break;					// 缓冲区满,则暂停扫描
			
			if (EpiTxFrameCount<EpiTxFrameMax_Scan)//EPI_SCAN_TX_MAX_SIZE)
			{	
	       	    dest_id = OnliSlaverId[SysInfo.PollId - 1];	// 搜索主席、代表、译员机的信息
				
				EpiTxFrameBuff[TxWriteCount].FpgaDest = dest_id;
				EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(dest_id);
				EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0xE0 | (dest_id >> 8))<<8)|(PollVote + VoteMode));
					
				__disable_irq();			
				TxWriteCount++;
				if (TxWriteCount>=TX_Frame_MAX_SIZE){
					TxWriteCount = 0; 
				}
				EpiTxFrameCount++;	
				__enable_irq();

		  		SysInfo.PollId++;
	       		if(SysInfo.PollId > OnlineNum)  					// 124台代表机/主席机 + 10台扩展主机
	       	    	SysInfo.PollId = 1;				
			}
			break;
		case SIGN_IN_STATE:

			if (StopScanFlag == TRUE) break;
			
			if (EpiTxFrameCount<EpiTxFrameMax_Scan)					//EPI_SCAN_TX_MAX_SIZE)
			{	
	       		if(SysInfo.PollId > OnlineNum)  					// 124台代表机/主席机 + 10台扩展主机
	       		{
					SysInfo.PollId = 1;	
	       		}

	       	    dest_id = OnliSlaverId[SysInfo.PollId - 1];			// 搜索主席、代表、译员机的信息
				
				EpiTxFrameBuff[TxWriteCount].FpgaDest = dest_id;
				EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(dest_id);
				EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0xE0 | (dest_id >> 8))<<8)|PollSign);				
				__disable_irq();
				TxWriteCount++;
				if (TxWriteCount>=TX_Frame_MAX_SIZE)
				{
					TxWriteCount = 0; 
				}
				EpiTxFrameCount++;	
				__enable_irq();
		  		SysInfo.PollId++;
			}
			  	
			break;
		case SET_ID_STATE:

			if (EpiTxFrameCount<2)//EPI_SCAN_TX_MAX_SIZE)
			{	
				EpiTxFrameBuff[TxWriteCount].FpgaDest = 0XFFFF;
				EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(SysInfo.CurrSetId);
				EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0xE0 | (SysInfo.CurrSetId >> 8))<<8)|PollSetId);
				__disable_irq();				
				TxWriteCount++;
				if (TxWriteCount>=TX_Frame_MAX_SIZE){
					TxWriteCount = 0; 
				}
				EpiTxFrameCount++;			
				__enable_irq();

				SysInfo.PollId++;
				if (SysInfo.PollId>(SYS_MAX_DELEG_NUM + MAX_EXMAC_NUM))// 1240台代表机/主席机+10台扩展主机 
				{
					SysInfo.PollId = 1;
				}
			}
			break;*/
		default :
			break;
	}
	CmdModeFlag = temp;
}
/*********************************************************************************************************
** Function name:    SendStatus
** Descriptions:     主机扫描完成后发送代表机组合信息        
*********************************************************************************************************/
void SendStatus(void)
{
	uint16_t dest_id;
	uint8_t temp;
	if (StopScanFlag == TRUE) return;	   //扫描缓冲区满

	if (EpiTxFrameCount<EpiTxFrameMax_Scan)
	{	
		dest_id = OnliSlaverId[SysInfo.PollId - 1];
		temp = DelegInfoTable[dest_id]>>8;					
		if ((temp&0xC0)!=0x00)
		{    	    
			EpiTxFrameBuff[TxWriteCount].FpgaDest = dest_id;
			EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(dest_id);
			EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0xB0 | (dest_id >> 8))<<8)|temp);
			__disable_irq();
			TxWriteCount++;
			if (TxWriteCount>=TX_Frame_MAX_SIZE) {
		    	TxWriteCount = 0; 
			}
			EpiTxFrameCount++;
			__enable_irq();
		}	
		SysInfo.PollId++;		
	}

	if(SysInfo.PollId > OnlineNum)
	{
		SysInfo.PollId = 1;
		ScanEnd=0;
	}
}
/*********************************************************************************************************
** Function name:    CompareIndex
** Descriptions:             
*********************************************************************************************************/
uint8_t CompareIndex(void)
{
	uint8_t num=0;
	num = (WifiTxWrite >= WifiTxRead) ? (WifiTxWrite - WifiTxRead) : (RX_Frame_MAX_SIZE - WifiTxRead + WifiTxWrite);
	return num;
}
/*********************************************************************************************************
** Function name:    HostScanOnliIdPrg
** Descriptions:     扫描当前主机下的所有ID范围，检测有哪些ID在线，用来建立一个在线ID列表(代表机/扩展主机)        
*********************************************************************************************************/
void HostScanOnliIdPrg(void)
{
	uint16_t dest_id,comparenum=0;
	static uint16_t numid=1;

    if ((SysInfo.MeetSta == MEET_STATE) &&(EpiTxFrameCount<16))//EPI_SCAN_TX_MAX_SIZE)) // 主机没有下发的控制信息，这时需要下发相应模式下的轮询码
	{ 			
		if (CheckOnliIdCnt > 0) // 还在扫描       
		{	
       	    dest_id = SysInfo.PollId;									// 搜索主席、代表在线信息
			comparenum = CompareIndex();
			if (comparenum < 32){
				if (numid <= 240){
					WifiTxBuff[WifiTxWrite].FpgaDest= numid;
					WifiTxBuff[WifiTxWrite].Type_AddrL= (uint8)(numid);
					WifiTxBuff[WifiTxWrite].Para_CmdL= (((0xE0 | (numid >> 8))<<8)|PollOnlineMeet);
					__disable_irq();
					WifiTxWrite++;
					if (WifiTxWrite >= RX_Frame_MAX_SIZE){
						WifiTxWrite = 0;	
					}
					numid++;
					__enable_irq();
				}
			}
			EpiTxFrameBuff[TxWriteCount].FpgaDest = dest_id;
			EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(dest_id);
			EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0xE0 | (dest_id >> 8))<<8)|PollOnlineMeet);
			__disable_irq();
			TxWriteCount++;
			if (TxWriteCount>=TX_Frame_MAX_SIZE)
			{
				TxWriteCount = 0; 
			}
			EpiTxFrameCount++;
			__enable_irq();
			SysInfo.PollId++;

   		    if(SysInfo.PollId > (SYS_MAX_DELEG_NUM + MAX_EXMAC_NUM))// 1240台代表机/主席机+10台扩展主机 
			{ 	
       	    	SysInfo.PollId = 1;		// 重新扫描
       	    	CheckOnliIdCnt--;
				numid = 1;		
       		}
		}
		else 
		{
			if (ScanData.State == 0)
			{
				ScanData.State = 1;				// 开始延时
			}
			else if(ScanData.State == 2)		// 延时时间到
			{
				ScanData.State = 3;				
				ChkOnlineIdFlag = FALSE;		// 在线ID扫描处理完毕
				BuildOnliIdListPrg();			// 建立在线ID列表
				ScanEnd=1;                      //扫描结束
				StartScan=FALSE;
				KeyLockStatus=FALSE;
			}
		}
	}
}
/*********************************************************************************************************
** Function name:       HostScan
** Descriptions:        主机扫描          
*********************************************************************************************************/
void HostScan(void)
{
	/*if (HostOnline==0x02){
		if (HostStart==TRUE){
			if (ChkOnlineIdFlag){ 
				HostScanOnliIdPrg();			// 扫描所有机用来建立在线ID列表
			}	
			else {
				if(ScanEnd==0){*/
					HostScanPrg();				// 主机轮询在线的ID,让它们回码
				/*}
			}
			if ((ScanEnd==1)&&(ChkOnlineIdFlag == FALSE)){
				SendStatus();
			}
		}
	}*/
}
/*********************************************************************************************************
** Function name:       ResetApplyMicInfoPrg
** Descriptions:        主机进入签到/表决/上电启动清除MIC申请列表           
*********************************************************************************************************/
void ResetApplyMicInfoPrg(void)
{
	uint8_t i;
	
	if (SysInfo.MicMode == APPLY)
	{
		for(i = 0; i < MAX_MIC_WAIT_NUM; i++)// 当前的等待列表
		{
			WaitMicList[i] = 0;
			WriteFpgaREG((REG_Wait0ID + i), 0x0000);		
		}
		SysInfo.MicWaitNum	 = 0;

		for(i = 0; i < MAX_MIC_ON_NUM; i++)
		{
			SlaverMicOnList[i] = 0;			// 当前发言代表机MIC 的ID列表
//			WriteFpgaREG((REG_Channel0ID + i), 0);
		}
		for (i=0; i<8; i++){
			if (UnitMicNumList[i] != 0){
				if (DelegInfoTable[UnitMicNumList[i]] != 0x10){
					if ((DelegInfoTable[UnitMicNumList[i]]&0x0001) == 0x00){
//						if ((DelegInfoTable[UnitMicNumList[i]]&0x00FE) == 0xF2){
							WriteFpgaREG((REG_Channel0ID + i), 0);
//						}
						UnitMicNumList[i] = 0;
					}
				}
			}
		}
		SysInfo.MicOnSlaveNum = 0;
		EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60);
		U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
		U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
	 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);
	}
	if (BidFlag == TRUE)	//禁止开话筒
	{
		for(i = 0; i < MAX_MIC_WAIT_NUM; i++)// 当前的等待列表
		{
			WaitMicList[i] = 0;
			WriteFpgaREG((REG_Wait0ID + i), 0x0000);		
		}
		SysInfo.MicWaitNum	 = 0;
	}
	 
	if (OnlineCallChairNum>0)
	{
		OnlineCallChairNum = 0;
		for(i = 0; i < CALL_CHAIR_ON_NUM; i++)
		{
			CallChairOnList[i] = 0;
		}
	}
}
/*********************************************************************************************************
** Function name:       ScanOnliIdInit
** Descriptions:        备份主机初始化写相关寄存器和变量               
*********************************************************************************************************/
void ScanOnliIdInit(uint8_t Mode)
{
	uint16_t i = 0;
	// 初始化在线检测ID
	ChkOnlineIdFlag 	= Mode;
	CheckOnliIdCnt		= Mode * 1;			// 一定要扫描2遍，扫描一次就是不回码
	SysInfo.PollId 	= 1;
	ScanData.State = 0; 
	ScanData.DelayCount = 0;
	CheckRepeatIdFlag = 0;			       //清除ID冲突标志

	OnlineNum = 0;
	for (i = 0; i < MAX_DELEG_NUM; i++){
		DelegInfoTable[i] = 0x00;
		OnliSlaverId[i] = 0X00;
	}
	OnlineWifiNum  = 0;
	OnlineSlaveNum = 0;
	OnlineChairNum = 0;
	OnlineInterNum = 0;
}
/*********************************************************************************************************
** Function name:       SysModeSwitch
** Descriptions:        系统工作模式转换     ver : 模式编码               
*********************************************************************************************************/
void SysModeSwitch(uint8_t ver)
{
	switch (ver) 
	{
		case MEET_STATE:			//  会议模式  						
	    	if(SysInfo.MeetSta != MEET_STATE) 
			{				
			 	if(SysInfo.MeetSta == SIGN_IN_STATE) 
				{
			 	    // 所有机器结束签到
					SignStartFlag = FALSE;
					WriteFpgaREG(REG_SignNum,0);
					CmdModeFlag = 0;
					EPI_SntBuffInit(0xD2, SignCmdHead | 0x0F, SignOver);
					EPI_SntBuffInit(0xD2, SignCmdHead | 0x0F, SignOver);
					CmdModeFlag = 1;
					EPI_SntBuffInit(0xD2, SignCmdHead | 0x0F, SignOver);
					EPI_SntBuffInit(0xD2, SignCmdHead | 0x0F, SignOver);
				}
				else if(SysInfo.MeetSta == VOTE_STATE) 
				{
					
		 		    // 让所有机器投票结束
					VoteResRtnStart = FALSE;	// 停止返回数据PC机
					VoteResRtnOk	= TRUE;
					VoteResRtnEnable= FALSE;
					CmdModeFlag = 0;		
					EPI_SntBuffInit(0xD2, VoteCmdHead | 0x0F, VoteOver);
					EPI_SntBuffInit(0xD2, VoteCmdHead | 0x0F, VoteOver);
					CmdModeFlag = 1;		
					EPI_SntBuffInit(0xD2, VoteCmdHead | 0x0F, VoteOver);
					EPI_SntBuffInit(0xD2, VoteCmdHead | 0x0F, VoteOver);
				}
				else if(SysInfo.MeetSta == SET_ID_STATE) 
				{
					SysInfo.SetStartIdNum = 1;
				}				
				WriteFpgaREG(REG_MySignStart,0x0000);
				WriteFpgaREG(REG_MyVoteStart,0x0000);
			  	SysInfo.MeetSta = MEET_STATE;
			  	SysInfo.PollId = 1;
				if (bConnectedPcFlag == TRUE) 
				{
					ConnectedPcFlag = TRUE;			// 主机启动在线检测PC
				}
			  	Setdestid=0;

			  	//  让所有机器进入会议模式
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, 0x0F, 0x01);	
				EPI_SntBuffInit(0xD2, 0x0F, 0x01);
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xD2, 0x0F, 0x01);	
				EPI_SntBuffInit(0xD2, 0x0F, 0x01);
				RefreshFlag = 1;	
			}
			// 返回应答信息给PC
			SendToPcBuff(0x00,0x00,MEET_STATE);
			break;
		case VOTE_STATE:			//  表决模式
			if (VoteFuncFlag == FALSE)	break;		// 如果没有启动表决功能，则退出
			
			// 必须是会议模式下才能发起表决
			if(SysInfo.MeetSta == MEET_STATE) 
			{
				ConnectedPcFlag = FALSE;			// 主机启动在线检测PC
				PcConnectCnt	= 0x00;
				PcConnectCntBuf	= 0x00;

				VoteMode = SysInfo.VoteType * 4 + (uint8)VoteResTypeFlag * 2 + (uint8)VoteSignedFlag * 1;
			  	SysInfo.MeetSta = VOTE_STATE;
				SysInfo.PollId = 1;
				// 表决模式下的相关变量初始化
				VoteModeValInit();
				ResetApplyMicInfoPrg();

				// 所有机器开始投票
				gbit_start_vote = TRUE;				// 表决开始
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, VoteCmdHead | 0x0F, VoteStart + VoteMode);
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xD2, VoteCmdHead | 0x0F, VoteStart + VoteMode);
				
				// 先返回应答信息给PC
				SendToPcBuff(0x00,0x00,VOTE_STATE);
				RefreshFlag = 1;
			}
			break;
		case SIGN_IN_STATE:			// 签到模式
			if (SignFuncFlag == FALSE)	break;  	// 如果签到功能没有开放,则退出

			if(SysInfo.MeetSta == MEET_STATE) 
			{
				ConnectedPcFlag = FALSE;		// 主机启动在线检测PC
				PcConnectCnt	= 0x00;
				PcConnectCntBuf	= 0x00;

			  	SysInfo.MeetSta = SIGN_IN_STATE;
				SysInfo.PollId = 1;
				// 签到模式下的相关变量初始化
				SignModeValInit();				
				ResetApplyMicInfoPrg();
				
				WriteFpgaREG(REG_SignMaxNum,SignMaxNum);
				WriteFpgaREG(REG_SignNum,SignedNum);
			 	// 应答PC机
			 	SendToPcBuff(0x00,0x00,SIGN_IN_STATE);
				RefreshFlag = 1;
			}
			break;
		case SET_ID_STATE:		// 对代表机进行ID编辑			
			if(SysInfo.MeetSta != SET_ID_STATE) 
			{
				ConnectedPcFlag = FALSE;			// 主机启动在线检测PC
				PcConnectCnt	= 0x00;
				PcConnectCntBuf	= 0x00;
				SysInfo.PollId = 1;

				SysInfo.MeetSta = SET_ID_STATE;
				SysInfo.CurrSetId = SysInfo.SetStartIdNum;
				// 发送进入设置ID广播
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, SetIdCmdHead | 0x0F, EnterId);
				WifiInfor.wifiID.wifiID.currentID = SysInfo.CurrSetId;
				WifiInfor.wifiID.wifiID.setIDflag = 1;	
				WriteFpgaREG(REG_WifiSetID, WifiInfor.wifiID.currentIDwifi);
				// 清除所有代表机/主席机话筒信息
				ResetMicInfoPrg();
			}
			// 返回信息给PC
			SendToPcBuff(0x00,0x00,SET_ID_STATE);
			break;
		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       ReturnSignResult
** Descriptions:   找到已经签到但是没有把结果发送给PC的ID，并且发送签到结果给PC              
*********************************************************************************************************/
void ReturnSignResult(void)
{
	uint8 k, p, temp;
	uint16  dest_id;
	
	// 只有当有发送请求 && 已到人数小于应到人数时才返回签到数据给PC机
	if (VoteSendPcFlag == TRUE) 
	{
		if (EpiTxFrameCount<TX_Frame_MAX_SIZE)
		{
					
			dest_id = SignVoteIdBuf[VoteRdPnt];
			VoteRdPnt++;
			if (VoteRdPnt >= VOTE_SENT_BUF_SIZE)
			{
				VoteRdPnt = 0x00;
			}

			k = (uint8)((dest_id - 1) & 0x07);			// 8bit标志位置
			p = (uint8)((dest_id - 1) >> 0x03);			// ID号的位置
			temp = SignVoteSendList[p] & (0x01 << k);
			
			// 还没有发送签到数据给PC机
			if (temp == 0x00) 						
			{							
			    // 实时的将ID和表决结果发送给PC机
			    SendToPcBuff(0x03,((uint8)(dest_id >> 4) & 0xF0) | 0x03,(uint8)dest_id);
				EPI_SntBuffInit(dest_id, (dest_id>>8)|SignCmdHead, 0x05);	// 回应上来签到的单元
				SignedNum++; 
				// 发送实到人数给带屏的主席机
				
				if (dest_id < 241)	WifiStateUpdate(dest_id, 0x01);
				WriteFpgaREG(REG_SignNum,SignedNum);	
				SignVoteSendList[p] |= (0x01 << k);
				if (MasterSigneId == dest_id)
				{
					WriteFpgaREG(REG_MySignStart,0x8000|dest_id);
				}
			}
			else		  //已经签到 应答签到单元
				EPI_SntBuffInit(dest_id, (dest_id>>8)|SignCmdHead, 0x05);

			if (VoteRdPnt == VoteWrPnt) 
			{
				VoteSendPcFlag = FALSE;
				StopScanFlag   = FALSE;
					 	   	 	
				if (SignExitTimeStart == 1)		// 退出签到
				{
					SignExitTimeStart = 0;
					// 应答主席机
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);		
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);
					// 返回PC签到已经结束
					SendToPcBuff(0x03,0x01,0x00);					
					SysModeSwitch(MEET_STATE);
				}
			}
		}
	}
}
/*********************************************************************************************************
** Function name:       ReturnVoteNamedId
** Descriptions:     实时的返回记名表决模式下，已经表决的ID和表决选项              
*********************************************************************************************************/
void ReturnVoteNamedId(void)
{
	uint8 Res, temp;
	uint8 k, p, VoteRes;
	uint16 dest_id;
	
	if ((VoteSendPcFlag == TRUE)&&(EpiTxFrameCount<TX_Frame_MAX_SIZE))
	{
		dest_id = SignVoteIdBuf[VoteRdPnt];
		Res		= VoteResultBuf[VoteRdPnt];
		VoteRdPnt++;
		if (VoteRdPnt >= VOTE_SENT_BUF_SIZE)
			VoteRdPnt = 0x00;

			
	    // 实时的将ID和表决结果发送给PC机
		k = (uint8)((dest_id - 1) % 8);
		p = (uint8)((dest_id - 1) >> 0x03);			// 标志位置
		if ((Res & 0x08) == 0x08) 					// 扩展主机下代表机/主席机最后一次表决有效
		{					
			SignVoteSendList[p] &= ~(0x01 << k);
		}
		Res &= 0x07;								// 屏蔽表决结果的无效位
		temp = SignVoteSendList[p] & (0x01 << k);

		if((temp == 0) && (Res <= 0x05)) 
		{
		    VoteRes = ((dest_id >> 4) & 0xF0);
		    VoteRes |= Res;
	
		    // 实时的将ID和表决结果发送给PC机
		    SendToPcBuff(0x12,VoteRes,(uint8)(dest_id));
			SignVoteSendList[p] |= (0x01 << k);		
		}
		
		if (VoteRdPnt == VoteWrPnt) 
		{
			VoteSendPcFlag = FALSE;
			StopScanFlag   = FALSE;
			
			if (SignExitTimeStart == 1)		// 退出签到
			{
				SignExitTimeStart = 0;
				QuitVoteFlag	= FALSE; 		// 停止表决后,不直接退出表决
				VoteResRtnEnable= TRUE;			// 使能返回表决结果计数器				
				// 应答主席机
				EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x30);
				EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x30);
			}
		}
	}
}
/*********************************************************************************************************
** Function name:       ReturnVoteResult
** Descriptions:        广播投票结果                
*********************************************************************************************************/
void ReturnVoteResult(void)
{
	uint8 i;
	
	// 将这5种票数发给有屏的主席机
	switch (SysInfo.VoteType) {
		case SELECT_MODE:
		case EVALUA_MODE:
			// 计算K5的票数
			i = (VoteResult[4] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD7, i | 0x0F, (uint8)(VoteResult[4] & 0xFF));
			WriteFpgaREG(REG_VoteRes5,VoteResult[4]);
			SendToPcBuff(0x13,i|0x05,(uint8)(VoteResult[4] & 0xFF));
			// 计算K4的票数
			i = (VoteResult[3] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD6, i | 0x0F, (uint8)(VoteResult[3] & 0xFF));
			WriteFpgaREG(REG_VoteRes4,VoteResult[3]);
			SendToPcBuff(0x13,i|0x04,(uint8)(VoteResult[3] & 0xFF));
		case VOTE_MODE:
			// 计算K3的票数
			i = (VoteResult[2] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD5, i | 0x0F, (uint8)(VoteResult[2] & 0xFF));
			WriteFpgaREG(REG_VoteRes3,VoteResult[2]);
			SendToPcBuff(0x13,i|0x03,(uint8)(VoteResult[2] & 0xFF));
			// 计算K2的票数
			i = (VoteResult[1] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD4, i | 0x0F, (uint8)(VoteResult[1] & 0xFF));
			WriteFpgaREG(REG_VoteRes2,VoteResult[1]);
			SendToPcBuff(0x13,i|0x02,(uint8)(VoteResult[1] & 0xFF));
			// 计算K1的票数
			i = (VoteResult[0] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD3, i | 0x0F, (uint8)(VoteResult[0] & 0xFF));
			WriteFpgaREG(REG_VoteRes1,VoteResult[0]);
			SendToPcBuff(0x13,i|0x01,(uint8)(VoteResult[0] & 0xFF));
			break;
		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       ReturnPcResultPrg
** Descriptions:        返回签到表决下的结果给PC                
*********************************************************************************************************/
void ReturnPcResultPrg(void)
{
	// 返回签到结果
	switch (SysInfo.MeetSta) 
	{
		case SIGN_IN_STATE:
			if (SignStartFlag == TRUE)	// 已经启动了签到
			{			
				ReturnSignResult();					// 返回签到人数
			}
			break;
		case VOTE_STATE:							// 实时的返回表决人数
			if (gbit_start_vote == TRUE)            // 已经开始表决
			{			
				ReturnVoteNamedId();				// 返回表决ID,和表决结果
			}
			if (VoteResRtnStart == TRUE) // 发送结果
			{
				// 返回最终的表决结果(各项表决票数，及总表决人数)
				if (VoteResRtnOk == TRUE) 
				{
					ReturnVoteResult();
					VoteResRtnOk = FALSE;
					VoteRtnFlag = TRUE;
					if (VoteLookres == 1){
						U_REG_MyVoteStart.U_REG_MyVoteStart.LookRes = 1;
						WriteFpgaREG(REG_MyVoteStart,U_REG_MyVoteStart.REG_MyVoteStartVal);
						VoteLookres = 0;
					}
				}
				if (QuitVoteFlag == TRUE) // 直接退出表决模式
				{	
					// 进入会议模式
					SysModeSwitch(MEET_STATE);
					VoteRtnFlag = FALSE;
				}
			}
			break;
		default :
			break;
	}
} 
/*********************************************************************************************************
** Function name:       SonyCameraDeal
** Descriptions:      Sony摄像球数据接收处理              
*********************************************************************************************************/
void SonyCameraDeal(void)
{
	uint8 TempBuff[12],i;
	uint16 buffid=0;
	
	if (Camera_Buffer[Camera_Info.CameraRD].RecvBuff[1] != 0x50)			//收到的未其他命令直接返回
	{
		for (i=0; i<11; i++){
			Camera_Buffer[Camera_Info.CameraRD].RecvBuff[i] = 0;
		}
		__disable_irq();
		Camera_Info.CameraRD++;
		if (Camera_Info.CameraRD>9)
			Camera_Info.CameraRD = 0;
		__enable_irq();
		return;
	}
	if (FullViewFlag == 1)
		buffid = 0;
	else {
		buffid = Camera_Info.Camera_PreID%127;
		if (buffid == 0)
				buffid = 127;
		else
				buffid = buffid;
	}
	if (FullViewFlag == 1){
		if (Camera_Buffer[Camera_Info.CameraRD].RecvBuff[10] == 0xFF){	//Pan/Title data
			for (i=0; i<8; i++){
				FullView_Con[i] = Camera_Buffer[Camera_Info.CameraRD].RecvBuff[i+2];
			}
//		RW24XX(FullView_Con,8,PrePos_ADDR,AT24C08_Write_Addr);
		I2C_WriteNByte(0xAE, 2, PrePos_ADDR, FullView_Con, 8);
//		if (Camera_Info.Camera_Set == 1)
//			{
//				if (SysInfo.Languge == LAN_CN)
//					LcdDisplayString(48, 184,12, 0, "全景设置完成");
//				else if (SysInfo.Languge == LAN_EN)
//					LcdDisplayString(48, 184,16, 0, "FullviewFinished");
//		}			
	}
		else if (Camera_Buffer[Camera_Info.CameraRD].RecvBuff[6] == 0xFF){	//Zoom Data
			for (i=0; i<4; i++){
				FullView_Con[8+i] = Camera_Buffer[Camera_Info.CameraRD].RecvBuff[i+2];
			}
//			RW24XX(&FullView_Con[8],4,PrePos_ADDR+Camera_Zoom,AT24C08_Write_Addr);
			I2C_WriteNByte(0xAE, 2, PrePos_ADDR+Camera_Zoom, &FullView_Con[8], 4);
		}

	}
	else {
		if (Camera_Buffer[Camera_Info.CameraRD].RecvBuff[10] == 0xFF){	//Pan/Title data

			for (i=0; i<8; i++){
				TempBuff[i] = Camera_Buffer[Camera_Info.CameraRD].RecvBuff[i+2];
			}
//			RW24XX(TempBuff,8,PrePos_ADDR+buffid*16,AT24C08_Write_Addr);	
			I2C_WriteNByte(0xAE, 2, PrePos_ADDR+buffid*16, TempBuff, 8);
//			if (Camera_Info.Camera_Set == 1){
//				if (SysInfo.Languge == LAN_CN)
//					LcdDisplayString(48, 208,8, 0, "设置完成");
//				else if (SysInfo.Languge == LAN_EN)
//					LcdDisplayString(48, 208,12, 0, "  Finished  ");
//				inttostr(Camera_Info.Camera_PreID,Buff,4);
//				LcdDisplayString(48,176,4,0, Buff);	
//			}			
		}
		else if (Camera_Buffer[Camera_Info.CameraRD].RecvBuff[6] == 0xFF){	//Zoom Data

			for (i=0; i<4; i++){
				TempBuff[i] = Camera_Buffer[Camera_Info.CameraRD].RecvBuff[i+2];
			}
//			RW24XX(TempBuff,4,PrePos_ADDR+buffid*16+Camera_Zoom,AT24C08_Write_Addr);
			I2C_WriteNByte(0xAE, 2, PrePos_ADDR+buffid*16+Camera_Zoom, TempBuff, 4);				
		}	
	}
	for (i=0; i<11; i++){
		Camera_Buffer[Camera_Info.CameraRD].RecvBuff[i] = 0;
	}
	__disable_irq();
	Camera_Info.CameraRD++;
	if (Camera_Info.CameraRD>9)
		Camera_Info.CameraRD = 0;	
	__enable_irq();
}
/*********************************************************************************************************
** Function name:       ArryDataDeal
** Descriptions:     矩阵数据切换处理             
*********************************************************************************************************/
void ArryDataDeal(void)
{
	uint8 i;
	if (Camera_Info.Camera_Set == 0) {
		if (Arry_Buffer[Arry_RDCount].RecvBuff[7] != 0x0A){
			for (i=0; i<11; i++){
				Arry_Buffer[Arry_RDCount].RecvBuff[i] = 0;
			}
			__disable_irq();
			Arry_RDCount++;
			if (Arry_RDCount>4)
				Arry_RDCount = 0;
			__enable_irq();
			return;
		}
		else {
			for (i=0; i<16; i++)
				if (Camera_Map[i].Map_Addr == Camera_Info.Out_Chan) break;
			if (i<16){
				if (Arry_Buffer[Arry_RDCount].RecvBuff[2]  != (Camera_Map[i].Map_Chan+48)){
//					UARTCharPut(UART0_BASE,Camera_Map[i].Map_Chan+48);
//					UARTCharPut(UART0_BASE,'V');
//					UARTCharPut(UART0_BASE,'1');
//					UARTCharPut(UART0_BASE,'.');
					uart0SendByte (Camera_Map[i].Map_Chan+48);
					uart0SendByte ('V');
					uart0SendByte ('1');
					uart0SendByte ('.');
				}
					
			}
				
			for (i=0; i<11; i++){
				Arry_Buffer[Arry_RDCount].RecvBuff[i] = 0;
			}
			__disable_irq();
			Arry_RDCount++;
			if (Arry_RDCount > 4)
				Arry_RDCount = 0;
			__enable_irq();
			return;	
		}
	}	
}
/*********************************************************************************************************
** Function name:       replytoPC
** Descriptions:                        
*********************************************************************************************************/
void replytoPC(void)
{
	if (LangSend == 0){
		SendToPcBuff(0x0D,0x04,0x05);
		SMS_Tx_State = 0;
	}	
	else if (LangSend == 1){
		SendToPcBuff(0x0D,0x04,0x07);
		SMS_Tx_State = 0;
		LangSend = 0;
	}
	else if (LangSend == 2){
		SendToPcBuff(0x0D,0x04,0x08);
		SMS_Tx_State = 0;
		LangSend = 0;
	}
	else if (LangSend == 3){
		SendToPcBuff(0x0D,0x04,0x09);
		SMS_Tx_State = 0;
		LangSend = 0;
	}	
}
/*********************************************************************************************************
** Function name:       PwrOffHandle
** Descriptions:        系统掉电处理                
*********************************************************************************************************/
void PwrOffHandle(void)
{	
	uint8_t temp = CmdModeFlag;
	if (BackupHostOnline==FALSE){
		CmdModeFlag = 0;
		EPI_SntBuffInit(0xD2, MeetCmdHead| 0x0F, PowerDown);		// 关机
		CmdModeFlag = 1;
		EPI_SntBuffInit(0xD2, MeetCmdHead| 0x0F, PowerDown);		// 关机
	}
	RelayOut_OFF;						// 断开扬声器的音频继电器
	CmdModeFlag = temp;
}

#define   PHY_CRS            8  
#define   PHY_CRS_High()     LPC_GPIO1->FIODIR|=(1<<PHY_CRS);LPC_GPIO1->FIOSET|=(1<<PHY_CRS);
#define   PHY_CRS_Low()      LPC_GPIO1->FIODIR|=(1<<PHY_CRS);LPC_GPIO1->FIOCLR|=(1<<PHY_CRS);

void delay_ms(uint16_t time)
{
  uint32_t i ,j;
     for(i=0;i<time;i++)
       for(j=25000;j;j--);
}

/*********************************************************************************************************
** Function name:       delay
** Descriptions:        参数为1时，延时为1ms                
** input parameters:    每加一延时多0.3ms
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void delay(int16_t time)
{
	int16_t i,j;
	for (i=0; i<5900; i++)
		for (j=0; j<time; j++);
}

void InitAddFlag(void)
{
	App_boot_pSysInfo = (App_cSysInfo *)&App_boot_SysInfo; 
	memset(App_boot_pSysInfo, 0x00, sizeof(App_cSysInfo)); 
	  
//	memcpy(App_boot_pSysInfo, (App_cSysInfo *)FLASH_APP_ADDR, sizeof(App_cSysInfo));
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:                        
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main (void)
{
	uint8_t uartBuffer[16], len=0;
	uint8_t tempDate=0;
	uint8_t konggejian[] = "\r\ninput a command\r\n";
	uint8_t update_app[] = "\r\nthe file will be sent...\r\n";
	
	//PHY_CRS_Low();
	delay_ms(200);
	__enable_irq();
	/*硬件初始化*/
	SystemInit();                                                          /* 系统初始化                  */
	InitDevices();
	/*变量初始化*/
	VariableInit();
	PwrOnSetup();
	BackupHostInit();	
	CameraSetAddr();
	WDTInit();
	//ChkOnlineIdFlag 	= 0;
	//InitAddFlag();
	while (1){
		/*********************ID模式下广播冲突ID*******************/
		if ((IDRepeat==TRUE)&&(SysInfo.MeetSta==SET_ID_STATE)){ 		  
			EpiTxFrameBuff[TxWriteCount].FpgaDest=0xffff;
			EpiTxFrameBuff[TxWriteCount].Type_AddrL = (uint8)(Repeat_ID);
			EpiTxFrameBuff[TxWriteCount].Para_CmdL = (((0x00 | (Repeat_ID >> 8))<<8)|0x35);
			__disable_irq();
			TxWriteCount++;
			if (TxWriteCount>=TX_Frame_MAX_SIZE){
				TxWriteCount = 0; 
			}
			EpiTxFrameCount++;
			__enable_irq();
			IDRepeat = FALSE;
    	}
		
		/******************状态切换 相关变量初始化*****************/
		if ((HostTemp!=HostSta)&&(BackupStart==FALSE)){		  
			ParameterInit();
			HostTemp=HostSta;
		}

		/*******************备份主机启动扫描单元*******************/
		if ((HostOnline==0x02)&&(ScanOnlineId!=0x00)&&(BackupStart==FALSE)){	
			ScanUint();
		}
		
		/***************备份主机启动相关变量初始化*****************/
		if ((HostOnline==0x02)&&(ParaInit==TRUE)&&(BackupStart==FALSE)){	
			WritePara();
			ParaInit=FALSE;
		}

		/**********************按键数据扫描************************/
		if ((KeyLockStatus==FALSE)&&(DonwLoader == 0)){	//主机在线 按键锁定
			KeyScanPrg();								// 按键检测及处理
		}
		FuncKeyPrg();									// 键功能处理
		SetIdKeyPrg();									// 进入设置ID

		/********************扫描在线ID及轮询**********************/
		if (HostSta==0x01)	HostScan();
		delay(1200);
		/*********************网络数据接收************************/
		UipPolling();
		if (DonwLoader == 1)	HEX_Send();
		else					NetRxDataProcess();	// 处理网络数据包
		if (HostSta==0x01)		ReturnPcResultPrg();// 返回结果给PC机
		
		/********************1602显示刷新**********************/
		RefreshShow();
		/********************单元机信息处理**********************/
		SlaveFrameProcess();	// 接收主席代表机译员机信息处理
		if (ChkOnlineIdFlag == FALSE)	
			MicInforProcess();	// 处理会议模式下命令
		
		/********************WIFI信息处理**********************/
		WifiFrameProcess();
		/***********************摄像头控制************************/
		if (Camera_Info.CameraRD != Camera_Info.CameraWR)//Sony摄像球处理
			SonyCameraDeal();	
		if (Arry_RDCount != Arry_WRCount)
			ArryDataDeal();
		
		/*****************定时给PC发送在线应答*******************/
		HostCheckPcPrg();
		
		/*******************短消息发送完成应答*******************/
		if (SMS_Tx_State == 2)	replytoPC();

		/************************发送PC**************************/
		Lm75_Check();
		ADC_Check();
		/************************喂狗**************************/
		WDTFeed();
		/******串口发送update,回车回到boot,等待用户输入update更新应用层******/
		/*if(u8UARTReceive(&tempDate)>0)		//先屏蔽不使用
		{
			if(tempDate==0x20)              //空格键
			{
					len = 0;
					vUARTSend(konggejian, sizeof(konggejian));           //please input a update command
			}
			else if(tempDate==0x0D)         //回车键 
			{
					if((memcmp(uartBuffer,"update",6)==0)||(memcmp(uartBuffer,"Update",6)==0))
					{
						vUARTSend(update_app, sizeof(update_app));
						App_boot_pSysInfo->download = 5;
						//u32IAP_CopyRAMToFlash(FLASH_APP_ADDR,(uint32_t)App_boot_pSysInfo,sizeof(App_cSysInfo));
						SaveParameter();	
						SCB->AIRCR = (0x05fa << 16) + 1;
					}
			}
			else
			{
					vUARTSendChar(tempDate);
					uartBuffer[len++]= tempDate;
					if(len>15)
						len= 0;
			}
		}*/
	}
}
/*void uip_log(char *m)		//20241029_clone分支修改+other person remove update function
{
	;
	//printf("无用，单纯测试");
}*/
/*void last_func(void)
{
	//printf("send debug message\n");
}*/
/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
