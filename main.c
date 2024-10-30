#define	_MAIN_GLOBAL_
/*********************************************************************************************************
  ͷ�ļ�                           
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"                                                    /* LPC17xx����Ĵ���            */
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
��������: WindowInit
����˵��: ��������ʼ��
*********************************************************************************************************/
void WindowInit(void)
{
	ChooseNum 	= 1; CurrentPage	= 0; BeInFlag  = 0;
	RefreshFlag	= 1; EnterFlag		= 0; ParaIndex = 1;
	/*MODE*/
	MenuWindow[mode].currentWindow  = &DisplayMicModeWnd;					/* Mic Mode menu	*/
	MenuWindow[mode].nextFlag		= 0;									/* û���¼��˵� 	*/
	MenuWindow[mode].currentPara	= &SysInfo.MicMode;						/* ����			 	*/
	MenuWindow[mode].paraRange		= 3;									/* ������Χ			*/
	MenuWindow[mode].modifyType		= ModeType;								/* �޸�MODE����		*/
	MenuWindow[mode].enterFlag		= 0;									/*�����¼��˵���־	*/
	/*ACTIVE*/
	MenuWindow[active].currentWindow = &DisplayActiveMicNumWnd;				/* Active Mode menu */
	MenuWindow[active].nextFlag		 = 0;									/* û���¼��˵�  	*/
	MenuWindow[active].currentPara	 = &MicNum;								/* ����Ҫת��SysInfo.SpkNumֵ*/
	MenuWindow[active].paraRange	 = 3;									/* ������Χ		 	*/
	MenuWindow[active].modifyType	 = ActiveType;							/* �޸�Active����	*/
	MenuWindow[active].enterFlag	 = 0;									/*���뵱ǰ�˵���־	*/
	/*VOLUME*/
	MenuWindow[volume].currentWindow = &DisplayVolumeWnd;					/* volume menu 		*/
	MenuWindow[volume].nextFlag		 = 1;									/*  ���¼��˵�  	*/
	MenuWindow[volume].currentPara	 = &ChooseNum;							/* ����				*/
	MenuWindow[volume].paraRange	 = 6;									/* ������Χ		 	*/
	MenuWindow[volume].modifyType	 = NoneType;							/* ���Ҽ��޸�����	*/
	MenuWindow[volume].enterFlag	 = 0;									/* ���뵱ǰ�˵���־	*/
	MenuWindow[volume].nextPage		 = VolumeMenu;							/* ָ���Ӳ˵��ṹ�� */
														  					/*�˽ṹ��ָ����ѡ��˵����ʱ��Ҫת��*/
	VolumeMenu[linein].currentWindow = &DisplayLineIn;						/* ������ʾ����		*/
	VolumeMenu[linein].nextFlag		 = 0;									/* û���¼��˵�		*/
	VolumeMenu[linein].currentPara	 = &SysInfo.LineInVol;					/* ����				*/
	VolumeMenu[linein].paraRange	 = 41;									/* ����������С		*/
	VolumeMenu[linein].modifyType	 = LineinType;							/* �޸���������		*/
	VolumeMenu[linein].enterFlag	 = 0;

	VolumeMenu[lineout].currentWindow = &DisplayLineOut;					/* �߳���ʾ����		*/
	VolumeMenu[lineout].nextFlag	  = 0;									/* û���¼��˵�		*/
	VolumeMenu[lineout].currentPara	  = &SysInfo.LinOutVol;					/* ����				*/
	VolumeMenu[lineout].paraRange	  = 41;									/* �߳�������С		*/
	VolumeMenu[lineout].modifyType	  = LineoutType;						/* �޸��߳�����		*/
	VolumeMenu[lineout].enterFlag	  = 0;

	VolumeMenu[unit].currentWindow = &DisplayUnit;							/* ��Ԫ�߳���ʾ����	*/
	VolumeMenu[unit].nextFlag	   = 0;										/* û���¼��˵�		*/
	VolumeMenu[unit].currentPara   = &SysInfo.UintOutVol;					/* ����				*/
	VolumeMenu[unit].paraRange	   = 41;									/* 	������С		*/
	VolumeMenu[unit].modifyType	   = UnitoutType;							/* �޸ĵ�Ԫ�߳�		*/
	VolumeMenu[unit].enterFlag	   = 0;

	VolumeMenu[treble].currentWindow = &DisplayTreble;						/* ������ʾ����		*/
	VolumeMenu[treble].nextFlag	     = 0;									/* û���¼��˵�		*/
	VolumeMenu[treble].currentPara   = &SysInfo.TrebleVol;					/* ����				*/
	VolumeMenu[treble].paraRange	 = 24;									/* 	������С		*/
	VolumeMenu[treble].modifyType	 = TrebleType;							/* �޸ĸ���			*/
	VolumeMenu[treble].enterFlag	 = 0;

	VolumeMenu[bass].currentWindow	= &DisplayBass;							/* ������ʾ����		*/
	VolumeMenu[bass].nextFlag		= 0;									/* û���¼��˵�		*/
	VolumeMenu[bass].currentPara   	= &SysInfo.BassVol;						/* ����				*/
	VolumeMenu[bass].paraRange		= 24;									/* 	������С		*/
	VolumeMenu[bass].modifyType		= BassType;								/* �޸ĵ���			*/
	VolumeMenu[bass].enterFlag		= 0;

	VolumeMenu[remotein].currentWindow = &DisplayRemote;						/* Զ�̻�����ʾ����	*/
	VolumeMenu[remotein].nextFlag	   = 0;									/* û���¼��˵�		*/
	VolumeMenu[remotein].currentPara   = &SysInfo.LinkOutVol;				/* ����				*/
	VolumeMenu[remotein].paraRange	   = 41;								/* 	������С		*/
	VolumeMenu[remotein].modifyType	   = remoteType;						/* �޸�Զ������		*/
	VolumeMenu[remotein].enterFlag	   = 0;

	VolumeMenu[speaker].currentWindow = &DisplaySpeaker;					/* ������ʾ����		*/
	VolumeMenu[speaker].nextFlag	  = 0;									/* û���¼��˵�		*/
	VolumeMenu[speaker].currentPara   = NULL;								/*U_REG_Default1.U_REG_Default1.Speaker*/
	VolumeMenu[speaker].paraRange	 = 1;									/* 	������С		*/
	VolumeMenu[speaker].modifyType	 = SpeakerType;							/* �޸�����			*/
	VolumeMenu[speaker].enterFlag	 = 0;
	/*EQ*/
	MenuWindow[eq].currentWindow 	= &DisplayEQWnd;						/* EQ		 menu	*/
	MenuWindow[eq].nextFlag			= 1;									/* û���¼��˵� 	*/
	MenuWindow[eq].currentPara		= &ChooseNum;							/* ����			 	*/
	MenuWindow[eq].paraRange		= 2;									/* ������Χ			*/
	MenuWindow[eq].modifyType		= NoneType;								/* û���޸�����		*/
	MenuWindow[eq].enterFlag		= 0;
	MenuWindow[eq].nextPage		 	= EQMenu; 

	EQMenu[effet].currentWindow 	= &DisplayEffect;						/* ��Ч��ʾ����		*/
	EQMenu[effet].nextFlag	  		= 0;									/* û���¼��˵�		*/
	EQMenu[effet].currentPara   	= &ParaIndex;							/* ENTER->SysInfo.EQSelect*/
	EQMenu[effet].paraRange	 		= 8;									/* 	8��ѡ��			*/
	EQMenu[effet].modifyType	 	= NoneType;								/* �޸���Ч			*/
	EQMenu[effet].enterFlag	 		= 0;

	EQMenu[custom].currentWindow 	= &DisplayEqCustom;						/* ��Ч�Զ�����ʾ	*/
	EQMenu[custom].nextFlag	  		= 1;									/* û���¼��˵�		*/
	EQMenu[custom].currentPara   	= &ParaIndex;				
	EQMenu[custom].paraRange	 	= 7;									/* 	8��ѡ��			*/
	EQMenu[custom].modifyType	 	= NoneType;								/* �޸�				*/
	EQMenu[custom].enterFlag	 	= 0;
	EQMenu[custom].nextPage		 	= SoundCustomMenu;

	SoundCustomMenu[band1].currentWindow = &DisplayBand;
	SoundCustomMenu[band1].enterFlag	 = 0;								/* û���¼��˵�		*/
	SoundCustomMenu[band1].currentPara	 = &SysInfo.Band1;					/* ������ 1��		*/
	SoundCustomMenu[band1].paraRange	 = 41;
	SoundCustomMenu[band1].modifyType	 = Band1Type;

	SoundCustomMenu[band2].currentWindow = &DisplayBand;
	SoundCustomMenu[band2].enterFlag	 = 0;								/* û���¼��˵�		*/
	SoundCustomMenu[band2].currentPara	 = &SysInfo.Band2;					/* ������ 2��		*/
	SoundCustomMenu[band2].paraRange	 = 41;
	SoundCustomMenu[band2].modifyType	 = Band2Type;

	SoundCustomMenu[band3].currentWindow = &DisplayBand;
	SoundCustomMenu[band3].enterFlag	 = 0;								/* û���¼��˵�		*/
	SoundCustomMenu[band3].currentPara	 = &SysInfo.Band3;					/* ������ 3��		*/
	SoundCustomMenu[band3].paraRange	 = 41;
	SoundCustomMenu[band3].modifyType	 = Band3Type;

	SoundCustomMenu[band4].currentWindow = &DisplayBand;
	SoundCustomMenu[band4].enterFlag	 = 0;								/* û���¼��˵�		*/
	SoundCustomMenu[band4].currentPara	 = &SysInfo.Band4;					/* ������ 4��		*/
	SoundCustomMenu[band4].paraRange	 = 41;
	SoundCustomMenu[band4].modifyType	 = Band4Type;

	SoundCustomMenu[band5].currentWindow = &DisplayBand;
	SoundCustomMenu[band5].enterFlag	 = 0;								/* û���¼��˵�		*/
	SoundCustomMenu[band5].currentPara	 = &SysInfo.Band5;					/* ������ 5��		*/
	SoundCustomMenu[band5].paraRange	 = 41;
	SoundCustomMenu[band5].modifyType	 = Band5Type;

	SoundCustomMenu[band6].currentWindow = &DisplayBand;
	SoundCustomMenu[band6].enterFlag	 = 0;								/* û���¼��˵�		*/
	SoundCustomMenu[band6].currentPara	 = &SysInfo.Band6;					/* ������ 6��		*/
	SoundCustomMenu[band6].paraRange	 = 41;
	SoundCustomMenu[band6].modifyType	 = Band6Type;

	SoundCustomMenu[band7].currentWindow = &DisplayBand;
	SoundCustomMenu[band7].enterFlag	 = 0;								/* û���¼��˵�		*/
	SoundCustomMenu[band7].currentPara	 = &SysInfo.Band7;					/* ������ 7��		*/
	SoundCustomMenu[band7].paraRange	 = 41;
	SoundCustomMenu[band7].modifyType	 = Band7Type;
	/*ZONE*/
	MenuWindow[zone].currentWindow = &DisplayZoneWnd;						/* zone menu 		*/
	MenuWindow[zone].nextFlag	   = 1;										/*  ���¼��˵�  	*/
	MenuWindow[zone].currentPara   = &ChooseNum;							/* ����				*/
	MenuWindow[zone].paraRange	   = 5;										/* ������Χ		 	*/
	MenuWindow[zone].modifyType	   = NoneType;								/* ���Ҽ��޸�����	*/
	MenuWindow[zone].enterFlag	   = 0;										/* ���뵱ǰ�˵���־	*/
	MenuWindow[zone].nextPage	   = ZoneMenu;

	ZoneMenu[speaker1].currentWindow	= &DisplayZonePage;					/* ZONE1��ʾ		*/
	ZoneMenu[speaker1].nextFlag	  		= 1;								/* û���¼��˵�		*/
	ZoneMenu[speaker1].currentPara   	= &ParaIndex;				
	ZoneMenu[speaker1].paraRange	 	= 5;								/* 	8��ѡ��			*/
	ZoneMenu[speaker1].modifyType	 	= NoneType;							/* �޸�				*/
	ZoneMenu[speaker1].enterFlag	 	= 0;
	ZoneMenu[speaker1].nextPage			= zone1Page;

	zone1Page[endid].currentWindow  = &DisplayZoneID;
	zone1Page[endid].enterFlag	    = 0;									/* û���¼��˵�		    */
	zone1Page[endid].currentPara	= NULL;									/*������ΧΪ4000���⴦��*/
	zone1Page[endid].paraRange	 	= 0;									
	zone1Page[endid].modifyType	 	= Zone1IDType;

	zone1Page[gain01].currentWindow	= &DisplayZoneGain1;
	zone1Page[gain01].enterFlag		= 0;									/* û���¼��˵�		  */
	zone1Page[gain01].currentPara	= &HornZone.SoundZone[0].gain1;			/* 4������������⴦��*/
	zone1Page[gain01].paraRange		= 41;								
	zone1Page[gain01].modifyType	= Zone1GainType;
	zone1Page[gain02].currentWindow	= &DisplayZoneGain1;
	zone1Page[gain02].enterFlag		= 0;									/* û���¼��˵�		  */
	zone1Page[gain02].currentPara	= &HornZone.SoundZone[1].gain1;			/* 4������������⴦��*/
	zone1Page[gain02].paraRange		= 41;								
	zone1Page[gain02].modifyType	= Zone1GainType;
	zone1Page[gain03].currentWindow	= &DisplayZoneGain1;
	zone1Page[gain03].enterFlag		= 0;									/* û���¼��˵�		  */
	zone1Page[gain03].currentPara	= &HornZone.SoundZone[2].gain1;			/* 4������������⴦��*/
	zone1Page[gain03].paraRange		= 41;								
	zone1Page[gain03].modifyType	= Zone1GainType;
	zone1Page[gain04].currentWindow	= &DisplayZoneGain1;
	zone1Page[gain04].enterFlag		= 0;									/* û���¼��˵�		  */
	zone1Page[gain04].currentPara	= &HornZone.SoundZone[3].gain1;			/* 4������������⴦��*/
	zone1Page[gain04].paraRange		= 41;								
	zone1Page[gain04].modifyType	= Zone1GainType;

	ZoneMenu[speaker2].currentWindow	= &DisplayZonePage;					/* ZONE2��ʾ		*/
	ZoneMenu[speaker2].nextFlag	  		= 1;								/* û���¼��˵�		*/
	ZoneMenu[speaker2].currentPara   	= &ParaIndex;				
	ZoneMenu[speaker2].paraRange	 	= 5;								/* 	8��ѡ��			*/
	ZoneMenu[speaker2].modifyType	 	= NoneType;							/* �޸�				*/
	ZoneMenu[speaker2].enterFlag	 	= 0;
	ZoneMenu[speaker2].nextPage			= zone2Page;

	zone2Page[endid].currentWindow  = &DisplayZoneID;
	zone2Page[endid].enterFlag	    = 0;									/* û���¼��˵�		    */
	zone2Page[endid].currentPara	= NULL;									/*������ΧΪ4000���⴦��*/
	zone2Page[endid].paraRange	 	= 0;									
	zone2Page[endid].modifyType	 	= Zone2IDType;

	zone2Page[gain01].currentWindow	= &DisplayZoneGain2;
	zone2Page[gain01].enterFlag		= 0;									/* û���¼��˵�		  */
	zone2Page[gain01].currentPara	= &HornZone.SoundZone[0].gain2;			/* 4������������⴦��*/
	zone2Page[gain01].paraRange		= 41;								
	zone2Page[gain01].modifyType	= Zone2GainType;
	zone2Page[gain02].currentWindow	= &DisplayZoneGain2;
	zone2Page[gain02].enterFlag		= 0;									/* û���¼��˵�		  */
	zone2Page[gain02].currentPara	= &HornZone.SoundZone[1].gain2;			/* 4������������⴦��*/
	zone2Page[gain02].paraRange		= 41;								
	zone2Page[gain02].modifyType	= Zone2GainType;
	zone2Page[gain03].currentWindow	= &DisplayZoneGain2;
	zone2Page[gain03].enterFlag		= 0;									/* û���¼��˵�		  */
	zone2Page[gain03].currentPara	= &HornZone.SoundZone[2].gain2;			/* 4������������⴦��*/
	zone2Page[gain03].paraRange		= 41;								
	zone2Page[gain03].modifyType	= Zone2GainType;
	zone2Page[gain04].currentWindow	= &DisplayZoneGain2;
	zone2Page[gain04].enterFlag		= 0;									/* û���¼��˵�		  */
	zone2Page[gain04].currentPara	= &HornZone.SoundZone[3].gain2;			/* 4������������⴦��*/
	zone2Page[gain04].paraRange		= 41;								
	zone2Page[gain04].modifyType	= Zone2GainType;

	ZoneMenu[speaker3].currentWindow	= &DisplayZonePage;					/* ZONE3��ʾ		*/
	ZoneMenu[speaker3].nextFlag	  		= 1;								/* û���¼��˵�		*/
	ZoneMenu[speaker3].currentPara   	= &ParaIndex;				
	ZoneMenu[speaker3].paraRange	 	= 5;								/* 	8��ѡ��			*/
	ZoneMenu[speaker3].modifyType	 	= NoneType;							/* �޸�				*/
	ZoneMenu[speaker3].enterFlag	 	= 0;
	ZoneMenu[speaker3].nextPage			= zone3Page;

	zone3Page[endid].currentWindow  = &DisplayZoneID;
	zone3Page[endid].enterFlag	    = 0;									/* û���¼��˵�		    */
	zone3Page[endid].currentPara	= NULL;									/*������ΧΪ4000���⴦��*/
	zone3Page[endid].paraRange	 	= 0;									
	zone3Page[endid].modifyType	 	= Zone3IDType;

	zone3Page[gain01].currentWindow	= &DisplayZoneGain3;
	zone3Page[gain01].enterFlag		= 0;									/* û���¼��˵�		  */
	zone3Page[gain01].currentPara	= &HornZone.SoundZone[0].gain3;			/* 4������������⴦��*/
	zone3Page[gain01].paraRange		= 41;								
	zone3Page[gain01].modifyType	= Zone3GainType;
	zone3Page[gain02].currentWindow	= &DisplayZoneGain3;
	zone3Page[gain02].enterFlag		= 0;									/* û���¼��˵�		  */
	zone3Page[gain02].currentPara	= &HornZone.SoundZone[1].gain3;			/* 4������������⴦��*/
	zone3Page[gain02].paraRange		= 41;								
	zone3Page[gain02].modifyType	= Zone3GainType;
	zone3Page[gain03].currentWindow	= &DisplayZoneGain3;
	zone3Page[gain03].enterFlag		= 0;									/* û���¼��˵�		  */
	zone3Page[gain03].currentPara	= &HornZone.SoundZone[2].gain3;			/* 4������������⴦��*/
	zone3Page[gain03].paraRange		= 41;								
	zone3Page[gain03].modifyType	= Zone3GainType;
	zone3Page[gain04].currentWindow	= &DisplayZoneGain3;
	zone3Page[gain04].enterFlag		= 0;									/* û���¼��˵�		  */
	zone3Page[gain04].currentPara	= &HornZone.SoundZone[3].gain3;			/* 4������������⴦��*/
	zone3Page[gain04].paraRange		= 41;								
	zone3Page[gain04].modifyType	= Zone3GainType;

	ZoneMenu[speaker4].currentWindow	= &DisplayZonePage;					/* ZONE4��ʾ		*/
	ZoneMenu[speaker4].nextFlag	  		= 1;								/* û���¼��˵�		*/
	ZoneMenu[speaker4].currentPara   	= &ParaIndex;				
	ZoneMenu[speaker4].paraRange	 	= 2;								/* 	8��ѡ��			*/
	ZoneMenu[speaker4].modifyType	 	= NoneType;							/* �޸�				*/
	ZoneMenu[speaker4].enterFlag	 	= 0;								
	ZoneMenu[speaker4].nextPage			= zone4Page;

	zone4Page[endid].currentWindow  = &DisplayZoneID;
	zone4Page[endid].enterFlag	    = 0;									/* û���¼��˵�		    */
	zone4Page[endid].currentPara	= NULL;									/*������ΧΪ4000���⴦��*/
	zone4Page[endid].paraRange	 	= 0;									
	zone4Page[endid].modifyType	 	= Zone4IDType;

	zone4Page[gain01].currentWindow	= &DisplayZoneGain4;
	zone4Page[gain01].enterFlag		= 0;										/* û���¼��˵�		  */
	zone4Page[gain01].currentPara	= &HornZone.SoundZone[0].gain4;			/* 4������������⴦��*/
	zone4Page[gain01].paraRange		= 41;								
	zone4Page[gain01].modifyType	= Zone4GainType;
	zone4Page[gain02].currentWindow	= &DisplayZoneGain4;
	zone4Page[gain02].enterFlag		= 0;										/* û���¼��˵�		  */
	zone4Page[gain02].currentPara	= &HornZone.SoundZone[1].gain4;			/* 4������������⴦��*/
	zone4Page[gain02].paraRange		= 41;								
	zone4Page[gain02].modifyType	= Zone4GainType;
	zone4Page[gain03].currentWindow	= &DisplayZoneGain4;
	zone4Page[gain03].enterFlag		= 0;										/* û���¼��˵�		  */
	zone4Page[gain03].currentPara	= &HornZone.SoundZone[2].gain4;			/* 4������������⴦��*/
	zone4Page[gain03].paraRange		= 41;								
	zone4Page[gain03].modifyType	= Zone4GainType;
	zone4Page[gain04].currentWindow	= &DisplayZoneGain4;
	zone4Page[gain04].enterFlag		= 0;										/* û���¼��˵�		  */
	zone4Page[gain04].currentPara	= &HornZone.SoundZone[3].gain4;			/* 4������������⴦��*/
	zone4Page[gain04].paraRange		= 41;								
	zone4Page[gain04].modifyType	= Zone4GainType;

	ZoneMenu[zonesw].currentWindow	= &DisplaySwPage;						/* ZONE SW��ʾ		*/
	ZoneMenu[zonesw].nextFlag	  	= 1;									/* û���¼��˵�		*/
	ZoneMenu[zonesw].currentPara   	= &ParaIndex;				
	ZoneMenu[zonesw].paraRange	 	= 2;								
	ZoneMenu[zonesw].modifyType	 	= NoneType;								/* �޸�����			*/
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

	ZoneMenu[remoteout].currentWindow	= &DisplayRemoteOut;				/* ZONE SW��ʾ		*/
	ZoneMenu[remoteout].nextFlag	  	= 0;								/* û���¼��˵�		*/
	ZoneMenu[remoteout].currentPara   	= &ParaIndex;				
	ZoneMenu[remoteout].paraRange	 	= 4;								
	ZoneMenu[remoteout].modifyType	 	= NoneType;							/* �޸�����			*/
	ZoneMenu[remoteout].enterFlag	 	= 0;

	/*UNIT SCAN*/
	MenuWindow[manage].currentWindow = &DisplayUnitScanWnd;					/* unit scan menu	*/
	MenuWindow[manage].nextFlag		 = 1;									/* û���¼��˵� 	*/
	MenuWindow[manage].currentPara	 = NULL;								/* ����			 	*/
	MenuWindow[manage].paraRange	 = 0;									/* ������Χ			*/
	MenuWindow[manage].modifyType	 = ScanType;							/* �޸�				*/
	MenuWindow[manage].enterFlag	 = 0;
	/*NET WORK*/
	MenuWindow[net].currentWindow = &DisplayNetWnd;							/* net work menu	*/
	MenuWindow[net].nextFlag	  = 1;								
	MenuWindow[net].currentPara	  = &ChooseNum;								/* ����			 	*/
	MenuWindow[net].paraRange	  = 2;										/* ������Χ			*/
	MenuWindow[net].modifyType	  = NoneType;								/* �޸�				*/
	MenuWindow[net].enterFlag	  = 0;
	MenuWindow[net].nextPage	  = NetMenu;

	NetMenu[aip].currentWindow	= &DisplayIPPage;							/* IP����			 */							
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

	NetMenu[amac].currentWindow	= &DisplayMACPage;							/* MAC����			 */							
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
	/*���������*/
	MenuWindow[trans].currentWindow = &DisplayTransWnd;						
	MenuWindow[trans].nextFlag		= 1;								
	MenuWindow[trans].currentPara	= &ChooseNum;								/* ����			 	*/
	MenuWindow[trans].paraRange		= 5;										/* ������Χ			*/
	MenuWindow[trans].modifyType	= NoneType;									/* �޸�				*/
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
	/*����ͷ����*/
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
	MenuWindow[misc].nextFlag		 = 1;								/*  ���¼��˵�  	*/
	MenuWindow[misc].currentPara	 = &ChooseNum;						/* ����				*/
	MenuWindow[misc].paraRange	 	 = 10;								/* ������Χ		 	*/
	MenuWindow[misc].modifyType	 	 = NoneType;						/* ���Ҽ��޸�����	*/
	MenuWindow[misc].enterFlag	 	 = 0;								/* ���뵱ǰ�˵���־	*/
	MenuWindow[misc].nextPage		 = MiscMenu;						/* ָ���Ӳ˵��ṹ�� */
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
��������: MainValInit
����˵��: ��������ʼ��
*********************************************************************************************************/
void MainValInit(void)
{
	uint16 i;
	DonwLoader = 0;			//���ظ��´����ʶ

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

	// MIC��Ϣ����������
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
	SysInfo.PollId = 1;						// ��ѯID��ʼ��Ϊ0	

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
** Descriptions:        ���ģʽ����ر�����ʼ��
*********************************************************************************************************/
void VoteModeValInit(void)
{
	uint16_t i;
	
	// ���ͶƱ���
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
	    SignVoteIdBuf[i] = 0;		// ���ǩ��/���ID���ͻ�����
	    VoteResultBuf[i] = 0;		// ������������ͻ�����
	}
	for(i = 0; i < sizeof(SignVoteSendList); i++) 
	{
		SignVoteSendList[i] = 0;	// ���ǩ��/���ID��־�б�
	}
	VoteWrPnt	  	= 0x00;
	VoteRdPnt	  	= 0x00;
	VoteSendPcFlag	= FALSE;
	StopScanFlag	= FALSE;

	VoteResRtnOk	= TRUE;
	QuitVoteFlag	= FALSE;
	
	VoteResRtnCnt	= 0x00;
    VoteResRtnEnable= FALSE;	
	VoteResRtnStart = FALSE;   		// ���ͽ����־λ
}
/*********************************************************************************************************
** Function name:       SignModeValInit
** Descriptions:        ǩ��ģʽ����ر�����ʼ��
*********************************************************************************************************/
void SignModeValInit(void)
{
	uint16_t i;

	for(i = 0; i < VOTE_SENT_BUF_SIZE; i++) 
	{
	    SignVoteIdBuf[i] = 0;		// ���ǩ��/���ID���ͻ�����
	}
	for(i = 0; i < sizeof(SignVoteSendList); i++) 
	{
		SignVoteSendList[i] = 0;	// ���ǩ��/���ID��־�б�
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
** Descriptions:        TCP/IP��ر�����ʼ��
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
** Descriptions:        ������ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void VariableInit(void)
{
	WindowInit();
	MainValInit();
	KeyValInit();
	SoftUartValInit();			// ģ�⴮��ģ����ر�����ʼ��
	HardUart0ValInit();			// Ӳ������0ģ����ر�����ʼ��
	HardUart1ValInit();			// Ӳ������0ģ����ر�����ʼ��
	NetValInit();
	VoteModeValInit();			// ���ģʽ����ر�����ʼ��
	SignModeValInit();			// ǩ��ģʽ����ر�����ʼ��
	ResetMicInfoPrg();
	Lm75Init();
	CurrID=0x01;
	IDRepeat=FALSE;          //ID��ͻ��־λ 
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
** Descriptions:       PC�����ȡ��������ڲ���ʾ����
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
** Descriptions:       ����������ر�����ʼ��
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
		ScanOnliIdInit(1);   //ɨ�赥Ԫ
		SysInfo.MeetSta = MEET_STATE;
		// ֪ͨ���л����������ģʽ
		EPI_SntBuffInit(0xD2, MeetCmdHead | 0x0F, MEET_STATE);		
	    // ��Ͳ����ģʽ
		EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x20 + SysInfo.MicMode);	
	    // �������
		EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x40 + SysInfo.SpkNum);	
 	    // ������Ա������
		EPI_SntBuffInit(0xD2, InterLoadCmd | 0x0F, 0x80|SysInfo.InterRoomNum);	
		NormalChannel();
    }
}
/*********************************************************************************************************
** Function name:       ParameterInit
** Descriptions:        ״̬�л� ��ر�����ʼ��
*********************************************************************************************************/
void ParameterInit(void)
{
	uint16_t temp;
	uint8_t i;
	switch (HostSta)
	{
		case 0x00:	   //��������״̬
//		case 0x01:
			temp=ReadFpgaREG(REG_ByPass);	  //Power off�л�
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

			ResetMicInfoPrg();	   //���MIC�б��ʼ��
			break;
		default:
			break;
	}
}
/*********************************************************************************************************
��������: ��������������MIC��Ϣ�͵�ǰ���ԵĴ����ID
����˵��: 
*********************************************************************************************************/
void ResetMicInfoPrg(void)
{
	uint16_t i;

	for(i = 0; i < MAX_MIC_ON_NUM; i++)
	{
		SlaverMicOnList[i] = 0x00;			// ��ǰ���Դ����MIC ��ID�б�
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
		MasterMicOnList[i] = 0x00;			// ��ǰ���Դ����MIC ��ID�б�
	}

	for(i = 0; i < INTER_MAX_SPK_NUM; i++)
	{
		InterMicOnList[i] = 0x00;			// 
	}
	
	for(i = 0; i < MAX_MIC_WAIT_NUM; i++)
	{
		WaitMicList[i] = 0x00;				// ��ǰ�ĵȴ��б�
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
** Descriptions:        ������������ ��ɨ�����ߵ�Ԫ���ϴ�����������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ScanUint(void)
{
	uint16_t dest_id;

	if (EpiTxFrameCount<EPI_SCAN_TX_MAX_SIZE) // ����û���·��Ŀ�����Ϣ����ʱ��Ҫ�·���Ӧģʽ�µ���ѯ��
	{ 			
		if (ScanOnlineId > 0) // ����ɨ��       
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

			if(ScanID > (SYS_MAX_DELEG_NUM + MAX_EXMAC_NUM))// 1240̨�����/��ϯ��+10̨��չ���� 
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
** Descriptions:        Ӳ����ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitDevices(void)
{
	FanInit();											 // ���ȿ��ƣ���
	LED1Init();											 // LED1GPIO��ʼ��
	RelayOutInit();										 // ��ʱ�����ʼ��
	BlinkInit();										 // 595GPIO��ʼ��
	KeyInit();											 // ����GPIO��ʼ��
	
	LEDAGC_ON();
	LEDANC_ON();
	LEDAFC_ON();
	Blinkled(0xFFFF);									 // �ϵ����ǰ��ȫ��LED

	SPIInit();											 // ��ʼ������
	WriteFpgaREG(REG_ByPass,0x00);
	/******************************************************************************************/
///	timer0Init();										 // Timer Init
///	GPIOInit();											 //FPGA�������ж�
	I2C1Init(100000);									 // IIC��ʼ��

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
	
	// ״̬��Ϣ��Ĭ��ֵ	 
	MicNum = 0;
	SysInfo.SpkNum = 1;
	SysInfo.MicMode    = FIFO;
	SysInfo.VoteType   = VOTE_MODE;
	
	// ����Щֵ��ŵ�EEPROM��   
	WriteEeprom(SysInfo.SpkNum, SPEAKER_NUM_ADDR);
	WriteEeprom(SysInfo.MicMode, MIC_MODE_ADDR);
	WriteEeprom(0x00, SPK_ADDR);		// Ĭ��Ϊ����
	U_REG_Default1.U_REG_Default1.Speaker = 1;
 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);
	EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x20 + SysInfo.MicMode);
	EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x40 + SysInfo.SpkNum);

	// ���ñ�ǣ���ʾ�Ѿ����ù�״̬��Ϣ	    
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
	WriteFpgaREG(REG_Other2,RegOther2.RegOther2Val);		//�Զ�0ͨ���򿪡�
	WriteEeprom(0x01, AUTO_ZERO_ADDR);	
	SignFuncFlag = TRUE;					// Ĭ��Ϊ�ر�ǩ������
	VoteFuncFlag = TRUE;					// Ĭ��Ϊ�������
	SysInfo.Languge = LAN_CN;					// Ĭ��Ϊ����
	WriteEeprom(SysInfo.Languge, LANG_ADDR);		
	RegIR.RegIR.IRLight = 15;
	WriteFpgaREG(REG_IRLight,RegIR.REG_IRLightVal);
	WriteEeprom(15, IRLIGHT_ADDR);
	//-------------------------------------------------------
	//��Ա��ͨ������
	for (i=0; i<32; i++)
	{
		SysInfo.ChannelLan[i] = i;
		WriteEeprom(SysInfo.ChannelLan[i],INTER_LANG_ADDR+i);
	}
	
	// ���Ե�Ԫ����������

	//------------------------------------------------------------		
	SysInfo.LineInVol = 0;		
	WriteEeprom(SysInfo.LineInVol, LINEIN_ADDR);
	
	SysInfo.LinkOutVol = 0;
	WriteEeprom(SysInfo.LinkOutVol, LinkVol_ADDR);
	WriteFpgaREG(REG_LineIn,(((uint16_t)(DbToGain[SysInfo.LinkOutVol]))<<8)|DbToGain[SysInfo.LineInVol]);		
	
	SysInfo.BassVol = 12;		// ȡ�м�ֵ
	WriteEeprom(SysInfo.BassVol, BASS_ADDR);
	
	SysInfo.TrebleVol = 12;		// ȡ�м�ֵ
	WriteEeprom(SysInfo.TrebleVol, TREBLE_ADDR);		

	SysInfo.LinOutVol = 0;		
	WriteEeprom(SysInfo.LinOutVol, VOLUME_ADDR);

	SysInfo.UintOutVol = 0;		
	WriteEeprom(SysInfo.UintOutVol, UnitVol_ADDR);
	WriteFpgaREG(REG_Volume, ((uint16_t)DbToGain[SysInfo.LinOutVol]<<8)|DbToGain[SysInfo.UintOutVol]);

	SysInfo.IR_ChanVol = 6;		// ȡ�м�ֵ
	WriteEeprom(SysInfo.IR_ChanVol, IR_CHAN_ADDR);		

	SysInfo.Band1 = 20;		// ȡ�м�ֵ
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
	//�ر���Ч��1
	ByPassValue &= ~(ByPass_AGC_Bit);LEDAGC_OFF();
	SysInfo.Sound_AFNGC1 &= ~0x04;
	//WriteEeprom(SysInfo.Sound_AFNGC1, AFNGC1_Add);
	ByPassValue &= ~(ByPass_ANC_Bit);LEDANC_OFF();
	SysInfo.Sound_AFNGC1 &= ~0x02;
	//WriteEeprom(SysInfo.Sound_AFNGC1, AFNGC1_Add);
	ByPassValue &= ~(ByPass_AFC_Bit);LEDAFC_OFF();
	SysInfo.Sound_AFNGC1 &= ~0x01;
	WriteEeprom(SysInfo.Sound_AFNGC1, AFNGC1_Add);
	//�ر���Ч��2
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
	//�ر�3108
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
	/*******************����������************************/
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

	HornZone.reg_zoon.SwitchZone = 0x0000;	//Զ�̻���رգ��������ܿ���DSP����Ϊ1
	WriteEeprom(HornZone.reg_zoon.SwitchZone, FanZone_ADDR);
	WriteEeprom(HornZone.reg_zoon.SwitchZone>>8, FanZone_ADDR+1);
	WriteFpgaREG(REG_SwitchZone,HornZone.reg_zoon.SwitchZone);
	
	RegTrackBack.RegTrackBackVal = 0x1000;
	WriteFpgaREG(REG_IR_Track,RegTrackBack.RegTrackBackVal);
	WriteEeprom(0x01, Track_Add);
	/***************************KEY��SSID�ָ�Ĭ��************************************/
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
	// ������Ϣ
	NetDefault();

	ICStateInit();
	WifiStateInit();
	Blinkled(0X0000);LEDAGC_OFF();LEDANC_OFF();LEDAFC_OFF();
}
/*********************************************************************************************************
��������: PwrOnSetup
����˵��: ϵͳ�ϵ����ã���ȡEEPROM���ò���
���� ֵ : 
˵    ��: 
*********************************************************************************************************/
void PwrOnSetup(void)
{
	uint8_t 	temp = 0,delaytime1 = 0;
	uint16_t 	i = 0;
	uint8_t 	buff[64];

	SignFuncFlag = TRUE;
	VoteFuncFlag = TRUE;
	
	temp = ReadEeprom(LANG_ADDR);
	if (temp == 0x00)			//��LCD��ʾ���Ա�־,0x00����
	{
		SysInfo.Languge = LAN_CN;				// Ĭ��Ϊ����
		U_REG_Default1.U_REG_Default1.Language = LAN_CN;
	}
	else if (temp == 0x01)		//��LCD��ʾ���Ա�־,0x00����
	{
		SysInfo.Languge = LAN_EN;
		U_REG_Default1.U_REG_Default1.Language = LAN_EN;
	}
	else 
	{
		SysInfo.Languge = LAN_CN;				// Ĭ��Ϊ����
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
	// �Զ���ͨ��
	temp = ReadEeprom(AUTO_ZERO_ADDR);
	if (temp == 1) // ��
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
	/******����ΪSony������ĳ�ʼ�����̣���ȡ��صĲ���*******************************/
	Camera_Info.Camera_Type = ReadEeprom(CameraID);

	if (Camera_Info.Camera_Type > CUSTOM)
	{
		Camera_Info.Camera_Type=SAMSUNG;		//use sansung	protocol
	}
	UARTInit();
	UART1Init();	
	SoftUartValInit();	 
	SonyCameraInit();
	//��ȡȫ��
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
	Init8306();												// ��λ���1S������
	//--------------------------------------------------------------------------
	// ��ȡEEpron��Ϣ�ɹ�
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
		// ���Ե�Ԫ����������
		if (ReadEeprom(SPK_ADDR) > 0x00) // 1, ��; 0, ��
		{
			WriteEeprom(0x01, SPK_ADDR);		// Ĭ��Ϊ����
			U_REG_Default1.U_REG_Default1.Speaker = 0;
		}
		else
		{
			U_REG_Default1.U_REG_Default1.Speaker = 1;			
		}

		WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);
		/**************************��Ͳģʽ���������*****************************/
		SysInfo.SpkNum 	= ReadEeprom(SPEAKER_NUM_ADDR);
		SysInfo.MicMode    = ReadEeprom(MIC_MODE_ADDR);	

		// ��ʼ��MIC ����
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

		// ��ʼ�����ģʽģʽ
		if (SysInfo.VoteType > EVALUA_MODE)
		{
			SysInfo.VoteType = VOTE_MODE;
		}
		// ��ʼ��MIC ģʽ
		if (SysInfo.MicMode > APPLY) 
		{
			SysInfo.MicMode = FIFO;
			WriteEeprom(SysInfo.MicMode, MIC_MODE_ADDR);
		}
	
		//------------------------------------------------------------			
		SysInfo.LineInVol  = ReadEeprom(LINEIN_ADDR);		// ��ȡ��������	
		SysInfo.UintOutVol  = ReadEeprom(UnitVol_ADDR);		// ��ȡ��������
		SysInfo.LinkOutVol  = ReadEeprom(LinkVol_ADDR);	
		SysInfo.BassVol = ReadEeprom(BASS_ADDR);			// ��ȡ����ֵ			
		SysInfo.TrebleVol = ReadEeprom(TREBLE_ADDR);		// ��ȡ����ֵ		
		SysInfo.LinOutVol = ReadEeprom(VOLUME_ADDR);		// ��ȡ������ֵ	
		SysInfo.IR_ChanVol = ReadEeprom(IR_CHAN_ADDR);		// ��ȡ��������ֵ	
		SysInfo.Band1 = ReadEeprom(Band1_Addr);
		SysInfo.Band2 = ReadEeprom(Band2_Addr);
		SysInfo.Band3 = ReadEeprom(Band3_Addr);
		SysInfo.Band4 = ReadEeprom(Band4_Addr);
		SysInfo.Band5 = ReadEeprom(Band5_Addr);
		SysInfo.Band6 = ReadEeprom(Band6_Addr);
		SysInfo.Band7 = ReadEeprom(Band7_Addr);
		
		if (SysInfo.LinkOutVol  > 41) 				// ֵ������Χ
		{		
			SysInfo.LinkOutVol = 41;						// ȡ�м�ֵ
			WriteEeprom(SysInfo.LinkOutVol, LinkVol_ADDR);
		}
		if (SysInfo.LineInVol  > 41) 				// ֵ������Χ
		{		
			SysInfo.LineInVol = 41;						// ȡ�м�ֵ
			WriteEeprom(SysInfo.LineInVol, LINEIN_ADDR);
		}
		WriteFpgaREG(REG_LineIn,(((uint16_t)(DbToGain[SysInfo.LinkOutVol]))<<8)|DbToGain[SysInfo.LineInVol]);
				
		if (SysInfo.BassVol > MaxBass) 					// ֵ������Χ
		{		
			SysInfo.BassVol = 12;							// ȡ�м�ֵ
			WriteEeprom(SysInfo.BassVol, BASS_ADDR);
		}		
		if (SysInfo.TrebleVol > MaxTreble) 				// ֵ������Χ
		{		
			SysInfo.TrebleVol = 12;						// ȡ�м�ֵ
			WriteEeprom(SysInfo.TrebleVol, TREBLE_ADDR);
		}		
//		if (SysInfo.LinOutVol > MaxVolume) 				// ֵ������Χ
//		{		
//			SysInfo.LinOutVol = 6;						// ȡ�м�ֵ
//			WriteEeprom(SysInfo.LinOutVol, VOLUME_ADDR);
//		}		
		if (SysInfo.IR_ChanVol > MaxVolume) 			// ֵ������Χ
		{		
			SysInfo.IR_ChanVol = 6;					// ȡ�м�ֵ
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
				
		// 3108����	
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

		if (SysInfo.InterRoomNum > TranslateRoomMax)// �������
		{
			SysInfo.InterRoomNum = TranslateRoomMax;
		}
		else if (SysInfo.InterRoomNum < 2){
			SysInfo.InterRoomNum = 2;
		}		
		EPI_SntBuffInit(0xD2, InterLoadCmd | 0x0F, 0x80|SysInfo.InterRoomNum);
		for (i=0; i<32; i++)							// ��Ա��ͨ������
		{
			SysInfo.ChannelLan[i] = ReadEeprom(INTER_LANG_ADDR+i);
			if (SysInfo.ChannelLan[i]>178)
			{
				SysInfo.ChannelLan[i] = 1;
			}			
		}
		/****************************����WIFI��ʱ***************************/
		delaytime1 = ReadEeprom(Delay_ADDR);
		if (delaytime1 > 0x0F){
			delaytime1 = 0x08;
			WriteEeprom(8, Delay_ADDR);
			WifiInfor.controlwifi[2] = 8;
		}
		WifiInfor.wififlag.wififlag.timedelay = delaytime1;
		WifiInfor.controlwifi[2] = delaytime1;
		WriteFpgaREG(REG_WifiDelay,WifiInfor.DelayTime[delaytime1]);	
		/****************************����������*****************************/
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
		/**********************************************����KEY��SSID************************************************/
		for (i=0; i<64; i++){
			WifiInfor.KeyContent[i+6] = ReadEeprom(Key_ADDR+i);
		}
		for (i=0; i<32; i++){
			WifiInfor.SsidContent[i+6] = ReadEeprom(Ssid_ADDR+i);
		}
		//-------------------------------------------------------
		// ������Ϣ
		
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
	//�����ʼ��
	EtheApp_Init();

	//---------------------------------------------------------------
	U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
	U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);

	timer0Init();										 // Timer Init
	GPIOInit();											 //FPGA�������ж�
	RelayOut_ON;
	Blinkled(0X0000);LEDAGC_OFF();LEDANC_OFF();LEDAFC_OFF();
	adcInit();
}
/*********************************************************************************************************
** Function name:       MicOnChannleNumber
** Descriptions:        ������Ͳ�ĵ�Ԫ��������ͨ��0-7              
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
** Descriptions:        ��OnliSlaverId�б��в���һ�������/��ϯ��ID          
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

	//����EpiTxFrameMax_Scan��ֵ�����ڱ������������������ͶƱǩ��ģʽ�²���ɨ�赥Ԫ
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
** Descriptions: ����ϯ���ر����д�����Ļ�Ͳʱ�������ʱ�ȴ��б����еȴ����ԵĻ�ͲID�������ȴ����Ե�ID(�еȴ���Ͳ)           
*********************************************************************************************************/
void MasterClsPriWtHandle(uint16_t dest_id)
{
	uint8_t i, j, k =0,temp=0,MicFlag=0,channeltemp=0,waittemp=0;
	uint16_t buffer[7];
	uint8_t flag = CmdModeFlag;
	// �Ѵ򿪻�Ͳָ��id����Ϣ����PC��
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
	
	if (MicFlag == 1){		//��ϯ�ڿ���Ͳ�б���
		for (i=0; i<CameraPnt; i++){
			if (CameraList[i] == dest_id) {
				for (j=i; j<(CameraPnt-1); j++)
					CameraList[j] = CameraList[j+1];
				CameraList[j] = dest_id;
				break;
			}
		}
	}

	// �еȴ����Ե�ID
	for(i = 0; i < SysInfo.MicChannelNum; i++)// ��ǰ����MIC ��ID�б�
	{
		for(j = 0; j < SysInfo.MicChannelNum; j++) 	// ��MICͨ�������ҿ���ͲID��
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

	// ֻ����NORMALģʽ�²Ż��еȴ��¼�����
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
	
	// ϵͳ��ǰ�����Ļ�Ͳ��
	CmdModeFlag = 0;
	EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
	CmdModeFlag = flag;
	U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
	U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);	// ������ٴ���
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
** Function name:       MasterClsPriHandle			  dest_id : ִ�йرչ��ܵ���ϯ��ID 
** Descriptions: ����ϯ���ر����д�����Ļ�Ͳʱ�������ʱ�ȴ��б����еȴ����ԵĻ�ͲID�������ȴ����Ե�ID(�޵ȴ���Ͳ)             
*********************************************************************************************************/
void MasterClsPriHandle(uint16_t destid)
{
	uint8_t i, j, k=0, temp=0,MicFlag=0;
	uint16_t dest_id,buffer[7];
	uint8_t flag = CmdModeFlag;
	dest_id = destid;
	// ��ִ������Ȩ��id��Ϣ����PC��
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
	
	if (MicFlag == 1){		//��ϯ�ڿ���Ͳ�б���
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
			for(j = 0; j < SysInfo.MicChannelNum; j++) 	// ��MICͨ�������ҿ���ͲID��
			{
				if (FreeMicOnList[i] == UnitMicNumList[j])
				{
					UnitMicNumList[j] = 0;
					WriteFpgaREG((REG_Channel0ID + j), 0x0000);
				}
			}			
			FreeMicOnList[i] = 0;			// ��ǰ����MIC ��ID�б�
		}
	}
	else 
	{
		for(i = 0; i < SysInfo.MicChannelNum; i++) 
		{	
			for(j = 0; j < SysInfo.MicChannelNum; j++) 	// ��MICͨ�������ҿ���ͲID��
			{
				if (SlaverMicOnList[i] == UnitMicNumList[j])
				{
					UnitMicNumList[j] = 0;
					WriteFpgaREG((REG_Channel0ID + j), 0x0000);
				}
			}
			SlaverMicOnList[i] = 0;		// ��ǰ����MIC ��ID�б�
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

	// ϵͳ��ǰ�����Ļ�Ͳ��
	CmdModeFlag = 0;
	EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
	CmdModeFlag = flag;
	U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
	U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);	// ������ٴ���
	CameraSendOpenCode(dest_id, 1, 0);
}
/*********************************************************************************************************
** Function name:       InterpreterMicOpenHandle
** Descriptions: ��Ա�����뿪��Ͳ������ϵͳ�Ļ�Ͳ�������ƣ���Ͳ����ģʽ���Դ�ID�Ļ�Ͳ��������д���
**************** 1.��һ�δ򿪻�Ͳ�����һ�δ򿪻�Ͳ��
**************** 2.��ͬRoom,��ͬout.
**************** 3.��ͬRoom,��ͬOut.
**************** 4.��ͬRoom,��ͬOut.
**************** 5.��ͬRoom.��ͬOut.             
*********************************************************************************************************/
void InterpreterMicOpenHandle(uint16 id, uint8 Ver2)
{
	uint8 i = 0,j=0;
	uint8 OutChannleTemp = 0;
	uint16 Temp = 0;
	uint8 mm=0;
			 //��ʱ����洢ͨ���źͷ���� Ȼ���ѯ���������ͨ���б���
	for (i=0;i<=5;i++)
	{
		if (TempInterBuff[i].TempMicid==id)
		{
			mm=TempInterBuff[i].TempMicRoom;	//�����
			InterMicOpenBuff[mm][Ver2]=id;
			TempInterBuff[i].TempMicid = 0x00;
			TempInterBuff[i].TempMicRoom = 0x00;
		}
	}			 
			
	// ��ͨ����ռ�ã��ȹر����ٴ�
	OutChannleTemp = CheckInterOutNum(Ver2);	
	if (OutChannleTemp > 0)
	{
		EPI_SntBuffInit(InterMicOnList[OutChannleTemp], InterMicOnList[OutChannleTemp] >> 8, InterMicClose);

		//����Ϊ�Ҽӵģ���ͬ����俪����ͬͨ��ʱ����������÷����û�п�������ͨ������������
		for (i=0;i<InterMaxNum;i++)
		{
			if (InterMicOpenBuff[i][OutChannleTemp]!=0)  //��ͨ��������������Ͳ��Ա
			{
				if (i==mm){	//ͬ������������  ������ͬͨ�������ڵķ����
					EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|mm);
					InterMicOpenBuff[i][OutChannleTemp]=id;	 // 
				}
				else{	//��ͬ�����������Ҹ÷���������������ͨ����
					mm=i;
					InterMicOpenBuff[i][OutChannleTemp]=0; //�÷��������ͨ���Źر�
					break;
				}
			}
		}	

		if (mm<InterMaxNum)
		{
			for (j=0;j<InterMaxNum;j++)
			{
				if(InterMicOpenBuff[mm][j]!=0){	 //�÷���������ͨ����
					EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|mm);
					break;
				}
		  	}
		}
		if (j>=InterMaxNum)
			EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x40|mm);


		// ֪ͨFPGA��ͨ��
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
	// �������ͨ���ţ�ͨ�����ͨ�����ҳ���ӦID�����ر�����
	InterMicOutList[Ver2] = Ver2;
	InterMicOnList[Ver2] = id;		
	
	
	
	for(i = 0; i < InterMicBuffSize; i++) 
	{
		if(InterMicBuff[i].OpenId == id) break;
	}

	// �ر���ͬ�����MIC
	OutChannleTemp = CheckInterRoomNum(InterMicBuff[i].Room);	
	if (OutChannleTemp > 0)	
	{
		EPI_SntBuffInit(InterMic[OutChannleTemp].OpenId, InterMic[OutChannleTemp].OpenId >> 8, InterMicClose);
		// ֪ͨFPGA��ͨ��
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
					
	// �ر���ͬ�����������,ֻҪ���Ϸ����žͻ�ִ��
	if (i<InterMicBuffSize)
	{
		EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|InterMicBuff[i].Room);	
	}

	// ����MIC
	EPI_SntBuffInit(id, id >> 8, InterMicOpen);	
	SendToPcBuff(InterMicOpen,(uint8)(((id >> 4) & 0xF0) | 0x00),(uint8)id);
	
	// ֪ͨFPGA��ͨ��
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
��������: ��Ա���ػ�Ͳ����ϵͳ�Ļ�Ͳ�������ƣ���Ͳ����ģʽ���Դ�ID�Ļ�Ͳ��������д���
����˵��: id : �رջ�Ͳ��ID  
���� ֵ : 
˵    ��: 
1. ��ͬroom����ͬͨ�����򿪶������ȹ�һ�����ٹ�һ����         
--------------------------------------------------------------------*/
void InterpreterMicCloseHandle(uint16 id,uint8 Ver2)
{
	uint8 i = 0,j=0,InterRoom=0;
	uint16 Temp = 0;

	// �������ͨ���ţ�ͨ�����ͨ�����ҳ���ӦID�����ر�����
	InterMicOutList[Ver2] = 0;
	InterMicOnList[Ver2] = 0;	
	InterMic[Ver2].OpenId = 0;
	InterMic[Ver2].Room = 0;	
	// ֻ�򿪱�ID������Ӧ�ķ���������
	// �����MIC
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
								
	// ֪ͨFPGA��ͨ��
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
										
	//��ӦID���㣬����ѯ�˷�����Ƿ�����������������Ͳ���о͹ر���������������������
	for (i=0;i<InterMaxNum;i++)
	{
		for (j=0;j<InterMaxNum;j++)
		{
			if (InterMicOpenBuff[i][j]==id)
			{
				InterMicOpenBuff[i][j]=0;
				InterRoom=i;			//id���ڷ����
				break;
			}
		}  
	}
	//����Ϊ�Ҽӵģ����Ȳ�ѯID�ķ����Ų����������㣬Ȼ���ѯ�˷�����Ƿ�������ID������Ͳ
	//����п���Ͳ�ر����������������÷�����������
	for (i=0;i<InterMaxNum;i++){
		if (InterMicOpenBuff[InterRoom][i]!=0)  //����������������Ͳ��Ա
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
			 //��ʱ����洢ͨ���źͷ���� Ȼ���ѯ���������ͨ���б���
	for (i=0;i<=5;i++)
	{
		if (TempInterBuff[i].TempMicid==id)
		{
			mm=TempInterBuff[i].TempMicRoom;	//�����
			InterMicOpenBuff[mm][Ver2]=id;
			TempInterBuff[i].TempMicid = 0x00;
			TempInterBuff[i].TempMicRoom = 0x00;
		}
	}			 
			
	// ��ͨ����ռ�ã��ȹر����ٴ�
	OutChannleTemp = CheckInterOutNum(Ver2);	
	if (OutChannleTemp > 0)					
	{
		EPI_SntBuffInit(InterMicOnList[OutChannleTemp], InterMicOnList[OutChannleTemp] >> 8, InterMicClose);

		//����Ϊ�Ҽӵģ���ͬ����俪����ͬͨ��ʱ����������÷����û�п�������ͨ������������
	     for (i=0;i<InterMaxNum;i++)
		   {
		    if (InterMicOpenBuff[i][OutChannleTemp]!=0)  //��ͨ��������������Ͳ��Ա
				{
				 if (i==mm)	//ͬ������������  ������ͬͨ�������ڵķ����
				 {
					EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|mm);
					InterMicOpenBuff[i][OutChannleTemp]=id;	 // 

				 }
				 else	//��ͬ�����������Ҹ÷���������������ͨ����
				 {
					mm=i;
					InterMicOpenBuff[i][OutChannleTemp]=0; //�÷��������ͨ���Źر�
					break;
				 }
				}
		  }	

		  if (mm<InterMaxNum)
		  {
       for (j=0;j<InterMaxNum;j++)
		   {
			 if(InterMicOpenBuff[mm][j]!=0)	 //�÷���������ͨ����
			 {
			  //InterMicOpenBuff[mm][OutChannleTemp]=0;
			  EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|mm);
			  break;
			 }
		   }
		  }
		  if (j>=InterMaxNum)
		   EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x40|mm);

		// ֪ͨFPGA��ͨ��
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
	// �������ͨ���ţ�ͨ�����ͨ�����ҳ���ӦID�����ر�����
	InterMicOutList[Ver2] = Ver2;
	InterMicOnList[Ver2] = id;		
	
	
	
	for(i = 0; i < InterMicBuffSize; i++) 
	{
		if(InterMicBuff[i].OpenId == id) break;
	}

	// �ر���ͬ�����MIC
	OutChannleTemp = CheckInterRoomNum(InterMicBuff[i].Room);	
	if (OutChannleTemp > 0)	
	{
		EPI_SntBuffInit(InterMic[OutChannleTemp].OpenId, InterMic[OutChannleTemp].OpenId >> 8, InterMicClose);
		// ֪ͨFPGA��ͨ��
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
					
	// �ر���ͬ�����������,ֻҪ���Ϸ����žͻ�ִ��
	if (i<InterMicBuffSize)
	{
		EPI_SntBuffInit(0xD2, CloseRoomCmd | 0x0F, 0x20|InterMicBuff[i].Room);	

	}

	// ����MIC
	EPI_SntBuffInit(id, id >> 8, InterMicOpen);	
	SendToPcBuff(InterMicOpen,(uint8)(((id >> 4) & 0xF0) | 0x00),(uint8)id);
	
	// ֪ͨFPGA��ͨ��
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
** Descriptions: ���Ҵ���Ա�����ͨ�����Ƿ�ռ��    ������Ͳ�����ͨ����(1-15)   0:˵����ͨ����ûռ�ã������򷵻ر�ռ��ͨ������λ��   
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
** Descriptions: ��Ա��������            
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
** Descriptions: ������Ա������ͨ��          
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
** Descriptions: ���ô����ID�Ļ�Ͳ������״̬��ͬʱ��ʼ��UART1�ķ��ͻ�����, ���ҷ�����Ϣ��PC id : ������Ͳ��ID             
*********************************************************************************************************/
void SlaveMicInvitePrg(uint16_t DestId)
{  
	uint8_t temp = CmdModeFlag;
	// ��ָ��id�Ĵ������Ͳ
 	uint8_t AddrL, Ver;
	
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	CmdModeFlag = 0;
	EPI_SntBuffInit(AddrL, Ver, DeMicOpen);
	CmdModeFlag = 1;
	EPI_SntBuffInit(AddrL, Ver, DeMicOpen);

	// �Ѵ򿪻�Ͳָ��id����Ϣ����PC��
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x00),(uint8)DestId);
	CmdModeFlag = temp;
}
/*********************************************************************************************************
** Function name:       FineIdPositionHandle
** Descriptions:  DestId:Ŀ��ID       ����Ŀ��ID��λ��(���ö��ֲ����㷨)    �����ҵ��󣬷�����ȷ��λ��,���򷵻�125  
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
** Descriptions:   �����MIC������Ϣ�Ĳ���          
*********************************************************************************************************/
void DestIdListHandle(uint16 dest_id)
{
	uint16 i;
	
	i = FineIdPositionHandle(dest_id);
	if (i > MAX_DELEG_NUM) // ��ID����ID�б���,��ʱ������һ̨�����
	{					
		InsertOnliSlrIdHandle(dest_id);
	}
}
/*********************************************************************************************************
** Function name:       SlaveCallChair
** Descriptions:   ���������ϯ�������ڲ�ͨ��      
*********************************************************************************************************/
void SlaveCallChair(uint16 dest_id)
{
	if (SysInfo.MeetSta == MEET_STATE)
	{
	    if (CallChairOnList[0]==dest_id)
			EPI_SntBuffInit(0xDE, ((uint8)(dest_id >> 4) & 0xF0) | 0x0F, (uint8)(dest_id));
	    else if (OnlineCallChairNum<CALL_CHAIR_ON_NUM)		// �����б�û��
		{
			CallChairOnList[OnlineCallChairNum++] = dest_id;
			// ������ϯ��,�д���������ڲ�ͨ��
			EPI_SntBuffInit(0xDE, ((uint8)(dest_id >> 4) & 0xF0) | 0x0F, (uint8)(dest_id));	
		}
		else
		{
			EPI_SntBuffInit(dest_id, dest_id>>8, CallChairFull);	
		}
	}
}
/*********************************************************************************************************
** Function name:       SetSlaveStaticPrg	  DestId : Ŀ��ID..CmdL������
** Descriptions:      ����ĳ���������״̬(ֻ�й��ڴ�����/��չ�����µĴ����/��ϯ������Ч��)             
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
** Descriptions:        ��Ԫ������HEX�ļ���ȡ����               
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
				*(unsigned short *)SMS_WR_Point = 0xFFFF;				//��һ��˫�ֽ�ΪĿ���ַ
				SMS_WR_Point += 2;
				if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
				{
					SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
				}

				AddrPoint = SMS_WR_Point;								//�����ڴ��ֽ�����С��ָ���ַ����
				*(SMS_WR_Point+1) = 0x00;								//�ڶ���˫�ֽ�Ϊ�ֽڴ�С
				SMS_WR_Point += 2;
				if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
				{
					SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
				}

				*(unsigned short *)SMS_WR_Point = 0x0400;				//������˫�ֽ�Ϊ�����ֽ�
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
					*(unsigned short *)SMS_WR_Point = 0xFFFF;				//��һ��˫�ֽ�ΪĿ���ַ
					SMS_WR_Point += 2;
					if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
					{
						SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
					}

					AddrPoint = SMS_WR_Point;								//�����ڴ��ֽ�����С��ָ���ַ����
					*(SMS_WR_Point+1) = 0x00;								//�ڶ���˫�ֽ�Ϊ�ֽڴ�С
					SMS_WR_Point += 2;
					if (SMS_WR_Point >=(unsigned char *)(SMS_Tx_Buff+SMS_TX_BUF_SIZE))
					{
						SMS_WR_Point = (unsigned char *)SMS_Tx_Buff;
					}

					*(unsigned short *)SMS_WR_Point = 0x0400;				//������˫�ֽ�Ϊ�����ֽ�
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
		*SMS_WR_Point = surveyF;//���粻����Щ�����ֽڴ浽��������������
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
** Descriptions:        �����������ݴ���               
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
		if((!NetRxBuff_Type_DWIN)&&(NetRxBuff_Type>=0xA000)&&(NetRxBuff_Type<=0xA00F))			//DWIN������,�¸�����ʼ�������ݴ�С�ж�DWIN�ĸ�ʽ��β0xFCFC
		{
			NetRxBuff_Type_DWIN=0x01;
			
		}else if(NetRxBuff_Type_DWIN){															//������ʼ�������ݴ�С�ж�DWIN�ĸ�ʽ��β0xFCFC
			if(NetRxBuff_Size_i>=NetRxBuff_Size){
				NetRxBuff_Size_i = 0x0;
				NetRxBuff_Type_DWIN = 0x0;
				NetRxBuff_Size = 0x0;
				if (KeyData == 0xFCFC)
				{
					PC_NetProcess(NetRxTail);							// ����PC����Ϣ����		
					NetRxTail = NetRxProcess;
					NetRxProcess1 = (unsigned char *)NetRxProcess;
				}
			}else{
				NetRxBuff_Size_i++;
			}
		}else{*/					//֮ǰ��DWIN����
			if (KeyData == 0xFCFC)
			{
				PC_NetProcess(NetRxTail);							// ����PC����Ϣ����		
				NetRxTail = NetRxProcess;
				NetRxProcess1 = (unsigned char *)NetRxProcess;
			}
	//	}
	//}
	//if (KeyData == 0xFCFC)
	//{
	//	PC_NetProcess(NetRxTail);							// ����PC����Ϣ����		
	//	NetRxTail = NetRxProcess;
	//	NetRxProcess1 = (unsigned char *)NetRxProcess;
	//}
}
/*********************************************************************************************************
** Function name:       MasterSpcCmdHandle
** Descriptions: ��ϯ�����������      dest_id ��Ŀ��ID..Para	���������        
*********************************************************************************************************/
void MasterSpcCmdHandle(uint16 dest_id, uint8 Para)
{
	switch (Para) 
	{
		case MSignStart :	// ��ϯ��������ǩ��
			{	
				if ((SignFuncFlag == FALSE)||(StartScanOnID > 0)) break;  	// ���ǩ�����ܺͲ���ɨ�赥Ԫû�п���,���˳�
				if (SysInfo.MeetSta == MEET_STATE) 
				{
					// Ӧ����ϯ��
					CmdModeFlag = 0;
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead + (uint8)((dest_id >> 8) & 0x0F), 0x0E);
					CmdModeFlag = 1;
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead + (uint8)((dest_id >> 8) & 0x0F), 0x0E);			        
					// �·����ڷ���ǩ����ID��
					WriteFpgaREG(REG_MySignStart,dest_id);
					MasterSigneId = dest_id;
					SignMaxNum 		= 1000;				// ǩ������Ϊ1000
					WriteFpgaREG(REG_SignMaxNum,SignMaxNum);
					WifiInfor.wififlag.wififlag.wifisign = 1;
					WriteFpgaREG(REG_WifiControl,(WifiInfor.wififlag.controlwifi & WIFIDEBUG));
					WifiStateInit();
					SysModeSwitch(SIGN_IN_STATE);
										
					SignStartFlag 	= TRUE;				// ��ʼǩ����־
					// ����Ӧ������������������ϯ��
					CmdModeFlag = 0;
					EPI_SntBuffInit(0xD9, 0x3F, 0xE8);
					CmdModeFlag = 1;
					EPI_SntBuffInit(0xD9, 0x3F, 0xE8);			        

					// ��PC������ǩ������
					SendToPcBuff(0x03,0x32,0xE8);
					// ��ʼ����ǩ��
					CmdModeFlag = 0;
				    EPI_SntBuffInit(0xD2, SignCmdHead | 0x0F, SignStart);
					CmdModeFlag = 1;
				    EPI_SntBuffInit(0xD2, SignCmdHead | 0x0F, SignStart); 
				}
			}
			break;
		case MVoteStart : 	// ��ϯ����������
			{
				if ((VoteFuncFlag == FALSE)||(StartScanOnID > 0)) break;		// ���û������������ܣ����˳�
				if (SysInfo.MeetSta == MEET_STATE) {
					// Ӧ����ϯ��
					EPI_SntBuffInit((uint8)(dest_id), VoteCmdHead + (uint8)((dest_id >> 8) & 0x0F), 0x2E);			        
					U_REG_MyVoteStart.U_REG_MyVoteStart.Id = dest_id;
					U_REG_MyVoteStart.U_REG_MyVoteStart.LookRes = 0;
					WriteFpgaREG(REG_MyVoteStart,U_REG_MyVoteStart.REG_MyVoteStartVal);
					VoteTimeState = 0;
					VoteSignedFlag = FALSE;					// ����Ҫǩ���Ϳ��Ա��
					VoteResTypeFlag = FALSE;				// ���һ����Ч(�͹�)
					// Ӧ��PC
					SendToPcBuff(0x02,0x03,0x00);
					// ��PC�������ر������
					SysInfo.VoteType = VOTE_MODE;
					WifiInfor.wififlag.wififlag.votefirst = 0;
					WifiInfor.wififlag.wififlag.votesigned = 0;
					WifiInfor.wififlag.wififlag.votemode = SysInfo.VoteType;
					WifiInfor.wififlag.wififlag.wifivote = 1;
					WriteFpgaREG(REG_WifiControl,(WifiInfor.wififlag.controlwifi & WIFIDEBUG));
					WifiStateInit();
					SendToPcBuff(0x02,0x00,0x00);
					SysModeSwitch(VOTE_STATE);		
				    // ����PC����Ѿ���ʼ
					gbit_start_vote = TRUE;			// �����ʼ
					SendToPcBuff(0x02,0x02,0x00);
				}
			}
			break;
		case MSignOver:		// ��ϯ������ֹͣǩ��
			{
		 	    // ���л�������ǩ��
		 	    if (VoteRdPnt == VoteWrPnt) //	���յ����ݴ���������˳�
		 	    {
					// Ӧ����ϯ��
					CmdModeFlag = 0;
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);	
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);
					CmdModeFlag = 1;
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);	
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);
					// ����PCǩ���Ѿ�����
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
		case MVotePause : 	// ��ϯ������ֹͣ���
			if(gbit_start_vote == TRUE) 
			{
				if (VoteRdPnt == VoteWrPnt) //	���յ����ݴ���������˳�
				{
					QuitVoteFlag	= FALSE; 		// ֹͣ�����,��ֱ���˳����
					VoteResRtnEnable= TRUE;			// ʹ�ܷ��ر�����������				
					// Ӧ����ϯ��
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
				QuitVoteFlag	= TRUE; 		// ֱ���˳����

				WifiInfor.wififlag.wififlag.wifivote = 0;
				WriteFpgaREG(REG_WifiControl,(WifiInfor.wififlag.controlwifi & WIFIDEBUG));
				ICStateUpdate();
				// ����PC����Ѿ�����
				SendToPcBuff(0x02,0x01,0x00);
				// Ӧ����ϯ��
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
** Descriptions:���ô�ID�Ļ�Ͳ�ز���״̬��ͬʱ��ʼ��UART1�ķ��ͻ�����, ���ҷ�����Ϣ��PC
*********************************************************************************************************/
void SlaveMicStop(uint16 DestId)
{
	uint8 AddrL, Ver;
	uint8_t temp = CmdModeFlag;
	// �ر�ָ��id�Ĵ������Ͳ
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	CmdModeFlag = 0;
	EPI_SntBuffInit(AddrL, Ver, DeMicClose);
	CmdModeFlag = 1;
	EPI_SntBuffInit(AddrL, Ver, DeMicClose);

	// �ѹرջ�Ͳָ��id����Ϣ����PC��
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x01),(uint8)DestId);
	CmdModeFlag = temp;
}
/*********************************************************************************************************
** Function name:       MicCloseChannleNumber
** Descriptions:		���رջ�Ͳ�ĵ�Ԫ��������ͨ��0-7  
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
** Descriptions:		�ȴ���ID   
*********************************************************************************************************/
void SlaveMicEnWait(uint16 DestId)
{
	uint8 AddrL, Ver;
	
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	EPI_SntBuffInit(AddrL, Ver, DeMicWait);

	// �ѳ�����Ͳ�ȴ���id��Ϣ����PC��
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x02),(uint8)DestId);
}
/*********************************************************************************************************
** Function name:       SlaveMicDisWait
** Descriptions:	�����ȴ���ID         
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
	// �ѳ�����Ͳ�ȴ���id��Ϣ����PC��
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x03),(uint8)DestId);
}
/*********************************************************************************************************
** Function name:       MicOpenWaitHandle
** Descriptions:	�Ի�Ͳ�����ĵȴ����д���           
*********************************************************************************************************/
void MicOpenWaitHandle(uint16 id) 
{
	uint8 i;
	
	// Normal ģʽ�£�����Ѿ����ˣ�����еȴ�����
	if (SysInfo.MicWaitNum > 0) {						// ��ǰ�ȴ��б����л�Ͳ�ȴ�
		for (i = 0; i < SysInfo.MicWaitNum; i++) {
			if (WaitMicList[i] == id) break;
		}
		if (i < SysInfo.MicWaitNum) {					// ��ID�Ѿ��ڵȴ��б���
			for (; i < SysInfo.MicWaitNum - 1; i++){
				WaitMicList[i] = WaitMicList[i + 1];
				WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
			}
			WaitMicList[SysInfo.MicWaitNum - 1] = 0;
			WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x00);
			SysInfo.MicWaitNum--;
			
			// ���ͳ�����ȴ�״̬
			SlaveMicDisWait(id);
		}
		else if (SysInfo.MicWaitNum < SysInfo.SpkNum) {// �ȴ��б�û����
			// ����ʹ����ȴ�״̬
			SlaveMicEnWait(id);
			WaitMicList[SysInfo.MicWaitNum++] = id;		// ����ȴ��б�
			WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), id);
		}
		else {									// �ȴ��б�����
			// ���͹رջ�Ͳ����
			MicCloseChannleNumber(id);
			SlaveMicStop(id);
			
		}
	}
	else {										// ��ǰ�ȴ��б�Ϊ��
		// ����ʹ����ȴ�״̬					
		SlaveMicEnWait(id);
		WaitMicList[SysInfo.MicWaitNum++] = id;			// ����ȴ��б�
		WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), id);
	}
}
/*********************************************************************************************************
** Function name:       SlaveMIcApplyOpen
** Descriptions:����ϵͳ�Ļ�Ͳ�������ƣ���Ͳ����ģʽ���Դ�ID�Ļ�Ͳ��������д���  ������ɹ����뿪���ٹرջ�Ͳ,����Ҫ����           
*********************************************************************************************************/
void SlaveMIcApplyOpen(uint16 id)
{
	uint8 i;
	
	// ���յ��˴�����ķ������룬���������ϯ���þ�
	for (i = 0; i < SysInfo.MicOnSlaveNum; i++){  // ���ҷ����б����Ƿ��д�ID
		if (SlaverMicOnList[i] == id) break;
	}

	if (i < SysInfo.MicOnSlaveNum) {	   		// ��ID���ڿ�����Ͳ�б�
		MicOnChannleNumber(id);
		EPI_SntBuffInit(id % 256, id / 256, DeMicOpen);
	}
	else if (SysInfo.MicWaitNum > 0) 
	{								// ��ǰ�л�Ͳ�������뷢��
		for (i = 0; i < SysInfo.MicWaitNum; i++) {
			if (WaitMicList[i] == id) break;
		}
		if (i < SysInfo.MicWaitNum) // ��ID�Ѿ������뷢���б���
		{			
			for (; i < (SysInfo.MicWaitNum - 1); i++){
				WaitMicList[i] = WaitMicList[i + 1];
				WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
			}
			WaitMicList[i] = 0;
			WriteFpgaREG((REG_Wait0ID + i), 0x0000);
			SysInfo.MicWaitNum--;						
			SlaveMicDisWait(id);		// ���ͳ�����ȴ�״̬

			// ������������뿪��Ͳ, ���͸���������ϯ��
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
		{							// ��ID���������б���
			if (SysInfo.MicWaitNum < (SysInfo.SpkNum - SysInfo.MicOnSlaveNum)) 
			{	// �ȴ��б�û����
				SlaveMicEnWait(id);	// �Ƚ���ȴ�ģʽ
				WaitMicList[SysInfo.MicWaitNum++] = id;
				WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), id);
				// ��������뿪��Ͳ, ���͸���������ϯ��
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
			{						// �����б�����
				MicCloseChannleNumber(id);
				SlaveMicStop(id);	// �رջ�Ͳ
			}
		}
	}
	else if (SysInfo.MicWaitNum == 0) 
	{
		if (SysInfo.MicOnSlaveNum < SysInfo.SpkNum) 
		{		// ��ǰ�����Ļ�Ͳ�����Ѿ��ﵽ�ֵ
			SlaveMicEnWait(id);	// �Ƚ���ȴ�ģʽ			
			WaitMicList[SysInfo.MicWaitNum++] = id;
			WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), id);
			// ��������뿪��Ͳ, ���͸���������ϯ��
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
		{							// �����б�����
			MicCloseChannleNumber(id);
			SlaveMicStop(id);	// �رջ�Ͳ
		}
	}
}
/*********************************************************************************************************
** Function name:       SlaveMicOpenHandle
** Descriptions:   ��������뿪��Ͳ������ϵͳ�Ļ�Ͳ�������ƣ���Ͳ����ģʽ���Դ�ID�Ļ�Ͳ��������д���             
*********************************************************************************************************/
void SlaveMicOpenHandle(uint16 id)
{
	uint8 SlaveMic_i, ChannelTemp=0,ChannelNum=0;
	uint8_t flag = CmdModeFlag;
	
	switch (SysInfo.MicMode) 
	{
		case APPLY:			// MIC����ģʽΪ����ģʽ������ֻ��ת�������ã�����ϯ�������Ƿ���
			if (SysInfo.MeetSta == MEET_STATE){ // ����ģʽ�²Ŵ���Ͳ����
				SlaveMIcApplyOpen(id);		// �����MIC �����봦��
			}
			else {							// ����ģʽ��,��������뿪��Ͳ,������ǿ���еĹر�
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
			}
			break;
			
		case FREE:			// MIC����ģʽΪFREE ���ɿ���
			for (SlaveMic_i = 0; SlaveMic_i < SysInfo.MicOnSlaveNum; SlaveMic_i++){ 	// ����,������ڴ򿪾ͷ��ز�����
				if (FreeMicOnList[SlaveMic_i] == id) break;
			}
			if (SlaveMic_i < SysInfo.MicOnSlaveNum){ 									// ���ڿ�����Ͳ�б�
				MicOnChannleNumber(id);
				EPI_SntBuffInit(id % 256, id / 256, DeMicOpen);				
			}
			else {
				if (SysInfo.MicOnSlaveNum < SysInfo.MicFreeOnNum) {				// �б�δ��
								
					ChannelTemp = MicOnChannleNumber(id);
					if (ChannelTemp == 1){
						SlaveMicInvitePrg(id);							//������Ͳ��ID 					
						FreeMicOnList[SysInfo.MicOnSlaveNum++] = id;								
						// ϵͳ��ǰ�����Ļ�Ͳ��
						CmdModeFlag = 0;
						EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
						CmdModeFlag = flag;
						U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
						U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
					 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);	
						CameraSendOpenCode(id, 0, 0);						// ������ٴ���
					}
					else{
						EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
					}
				}
				else {												// �б��Ѿ���
					MicCloseChannleNumber(id);
					SlaveMicStop(id);
				}											
			}
			break;
			
		case NORMAL:
			for(SlaveMic_i = 0; SlaveMic_i < SysInfo.MicOnSlaveNum; SlaveMic_i++){ // ��ѯ��ǰID�ǲ����ڷ������б���
				if(SlaverMicOnList[SlaveMic_i] == id) break;
			}
			if (SlaveMic_i < SysInfo.MicOnSlaveNum) {						// ������MIC����ID���ڷ����б���
				MicOnChannleNumber(id);
				EPI_SntBuffInit(id % 256, id / 256, DeMicOpen);
			}
			else  {														 	// ������MIC����ID�����ڷ����б���
				if(SysInfo.MicOnSlaveNum < SysInfo.SpkNum){ 		// ������������û����	
					ChannelTemp = MicOnChannleNumber(id);
					if (ChannelTemp == 1){
						SlaverMicOnList[SysInfo.MicOnSlaveNum++] = id;
						SlaveMicInvitePrg(id);					
						CameraSendOpenCode(id, 0, 0);// ������ٴ���
					}
					else{
						EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
					}
				}
				else {
					MicOpenWaitHandle(id);
				}
			}
			
			// ϵͳ��ǰ�����Ļ�Ͳ��, ���͸�����Ļ�Ĵ����/��ϯ��
			CmdModeFlag = 0;
			EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
			CmdModeFlag = flag;
			U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
			U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
		 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);	
			break;

		case FIFO:
			for(SlaveMic_i = 0; SlaveMic_i < SysInfo.MicOnSlaveNum; SlaveMic_i++){ // ��ѯ��ǰID�ǲ����ڷ������б���
				if(SlaverMicOnList[SlaveMic_i] == id) break;
			}
			
			if (SlaveMic_i < SysInfo.MicOnSlaveNum) {							// ������MIC����ID���ڷ����б���
				MicOnChannleNumber(id);
				EPI_SntBuffInit(id % 256, id / 256, DeMicOpen);
			}
			else {																// ������MIC����ID�����ڷ����б���
				if(SysInfo.MicOnSlaveNum < SysInfo.SpkNum) {					// ������������û����	
					ChannelTemp = MicOnChannleNumber(id);					    //��ѡ��ͨ���ٷ�����Ͳ
					if (ChannelTemp == 1){
						SlaverMicOnList[SysInfo.MicOnSlaveNum++] = id;
						SlaveMicInvitePrg(id);														
						CameraSendOpenCode(id, 0, 0);// ������ٴ���
					}
					else{
						EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
					}
				}
				else{ 										// �����б��Ѿ���
					if (SysInfo.MicOnSlaveNum>0){
						ChannelNum = MicCloseChannleNumber(SlaverMicOnList[0]);
						ChannelTemp = MicOnChannleNumber(id);						//��ѡ��ͨ���ٷ�����Ͳ
						if (ChannelTemp == 1){
							CameraSendOpenCode(id, 0, 1);// ������ٴ���
							
							SlaveMicStop(SlaverMicOnList[0]);		// ��FIFO�Ĺ�����ǰ��idֹͣ					
							SlaveMicInvitePrg(id);						// ������ǰ���뿪��ID
							//����Ͳ�б���������ƶ�һλ					
							for(SlaveMic_i = 0; SlaveMic_i < (SysInfo.MicOnSlaveNum - 1); SlaveMic_i++)
							{
								SlaverMicOnList[SlaveMic_i] = SlaverMicOnList[SlaveMic_i + 1];
							}
							SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = id;		//�¿��Ļ�Ͳ�����б�
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
			
			// ϵͳ��ǰ�����Ļ�Ͳ��, ���͸�����Ļ�Ĵ����/��ϯ��
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
** Descriptions: ����ϵͳ�Ļ�Ͳ�������ƣ���Ͳ����ģʽ���Դ�ID�Ļ�Ͳ��������д��� ������ɹ����뿪���ٹرջ�Ͳ,����Ҫ����         
*********************************************************************************************************/
void SlaveMIcApplyClose(uint16 id)
{
	uint8 i;
	uint8_t flag = CmdModeFlag;
	for (i = 0; i < SysInfo.MicOnSlaveNum; i++) 
	{				// ����
		if (SlaverMicOnList[i] == id) break;
	}
	if (i < SysInfo.MicOnSlaveNum) 
	{								// ���ڿ�����Ͳ�б�
		MicCloseChannleNumber(id);
		SlaveMicStop(id);							// �رջ�Ͳ
		
		for(; i < (SysInfo.MicOnSlaveNum - 1); i++)
			SlaverMicOnList[i] = SlaverMicOnList[i + 1];
		SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = 0;
		if(SysInfo.MicOnSlaveNum > 0) SysInfo.MicOnSlaveNum--;

		// ϵͳ��ǰ�����Ļ�Ͳ��
		CmdModeFlag = 0;
		EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
		CmdModeFlag = flag;
		U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
		U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
	 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);		// ������ٴ���
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
** Descriptions:  ������ػ�Ͳ����ϵͳ�Ļ�Ͳ�������ƣ���Ͳ����ģʽ���Դ�ID�Ļ�Ͳ��������д���           
*********************************************************************************************************/
void SlaveMicCloseHandle(uint16 id)
{
	uint8 i,channeltemp=0;
	uint8_t temp = CmdModeFlag;
	
	switch (SysInfo.MicMode) 
	{
		case APPLY:			// MIC����ģʽ, ����ֻ��ת��������
			SlaveMIcApplyClose(id);
			break;
			
		case FREE:			// MIC����ģʽΪFREE ���ɹر�
			for (i = 0; i < SysInfo.MicOnSlaveNum; i++) {				// ����
				if (FreeMicOnList[i] == id) break;
			}
			
			if (i < SysInfo.MicOnSlaveNum) {								// ���ڿ�����Ͳ�б�
				MicCloseChannleNumber(id);
				SlaveMicStop(id);
				
				for (; i < (SysInfo.MicOnSlaveNum - 1); i++)
				{
					FreeMicOnList[i] = FreeMicOnList[i + 1];
				}
				FreeMicOnList[i] = 0;
				SysInfo.MicOnSlaveNum--;
				// ϵͳ��ǰ�����Ļ�Ͳ��
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
				CmdModeFlag = temp;
				U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
				U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
			 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);				// ������ٴ���
				CameraSendClsCode(id, 0);
			}
			else 
			{
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
			}			
			break;
			
		case NORMAL:
			for(i = 0; i < SysInfo.MicOnSlaveNum; i++) // ��ѯ��ǰID�ǲ����ڷ������б���
			{	
				if(SlaverMicOnList[i] == id) break;
			}
			
			if (i < SysInfo.MicOnSlaveNum) // ������MIC�ص�ID���ڷ����б���
			{				
				MicCloseChannleNumber(id);
				SlaveMicStop(id);
				
				for(; i < (SysInfo.MicOnSlaveNum - 1); i++)
				{
					SlaverMicOnList[i] = SlaverMicOnList[i + 1];
				}
				SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = 0;
				SysInfo.MicOnSlaveNum--;
				if (SysInfo.MicWaitNum > 0) // ��ǰ�л�Ͳ�ȴ�
				{			
					channeltemp = MicOnChannleNumber(WaitMicList[0]);
					if (channeltemp == 1){
						SlaveMicInvitePrg(WaitMicList[0]);
						
						SlaverMicOnList[SysInfo.MicOnSlaveNum++] = WaitMicList[0];
						// ������ٴ���
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
					// ������ٴ���
					CameraSendClsCode(id, 0);
				}
			}
			else 						// ������MIC����ID�����ڷ����б���
			{
				// ����õȴ�����
				for (i = 0; i < SysInfo.MicWaitNum; i++) {    //�޸�����6����Ԫ,�ٿ����߸�,�ֹغ�,û�������Ӧ�ȴ��Ļ���,ʹ���ٹر�һ����Ԫ��ԭ���ȴ��ĵ�Ԫ�ֻῪ�������������رյ��ǵȴ��ĵ�Ԫ,�򽫵ȴ���������һ_xavior
					if (WaitMicList[i] == id) break;
				}
				if (i < SysInfo.MicWaitNum)                   //�޸�����6����Ԫ,�ٿ����߸�,�ֹغ�,û�������Ӧ�ȴ��Ļ���,ʹ���ٹر�һ����Ԫ��ԭ���ȴ��ĵ�Ԫ�ֻῪ�������������رյ��ǵȴ��ĵ�Ԫ,�򽫵ȴ���������һ_xavior
				{		// ��ID�Ѿ��ڵȴ��б���
					for (; i < (SysInfo.MicWaitNum - 1); i++){
						WaitMicList[i] = WaitMicList[i + 1];
						WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
					}
					WaitMicList[i] = 0;
					WriteFpgaREG((REG_Wait0ID + i), 0x0000);
					SysInfo.MicWaitNum--;						
					// ������������뿪��Ͳ, ���͸���������ϯ��
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
			
			// ϵͳ��ǰ�����Ļ�Ͳ��
			CmdModeFlag = 0;
			EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
			CmdModeFlag = temp;	
			U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
			U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
		 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);			
			break;

		case FIFO:
			for(i = 0; i < SysInfo.MicOnSlaveNum; i++) {	// ��ѯ��ǰID�ǲ����ڷ������б���
				if(SlaverMicOnList[i] == id) break;
			}
			
			if (i < SysInfo.MicOnSlaveNum) // ������MIC�ص�ID���ڷ����б���
			{								
				SlaveMicStop(id);
				MicCloseChannleNumber(id);
				for(; i < SysInfo.MicOnSlaveNum - 1; i++)
				{
					SlaverMicOnList[i] = SlaverMicOnList[i + 1];
				}
				SlaverMicOnList[SysInfo.MicOnSlaveNum - 1] = 0;
				SysInfo.MicOnSlaveNum--;
				
				CameraSendClsCode(id, 0);// ������ٴ���
			}
			else 
			{								// ������MIC����ID�����ڷ����б���
				EPI_SntBuffInit(id % 256, id / 256, DeMicClose);
			}
			
			// ϵͳ��ǰ�����Ļ�Ͳ��
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
** Descriptions:      ������ϯ��ID�Ļ�Ͳ������״̬�����ҷ�����Ϣ��PC        
*********************************************************************************************************/
void MasterMicInvitePrg(uint16 DestId)
{  
	// ��ָ��id�Ĵ������Ͳ
 	uint8 AddrL, Ver;
	
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	EPI_SntBuffInit(AddrL, Ver, MaMicOpen);

	// �Ѵ򿪻�Ͳָ��id����Ϣ����PC��
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x00),(uint8)DestId);
}
/*********************************************************************************************************
** Function name:       MasterMicStopPrg
** Descriptions:   ������ϯ����Ͳ�ز���״̬�����ҷ�����Ϣ��PC          
*********************************************************************************************************/
void MasterMicStopPrg(uint16 DestId)
{
	uint8 AddrL, Ver;
	
	// �ر�ָ��id�Ĵ������Ͳ
	AddrL 	= (uint8)(DestId);
	Ver		= (uint8)(DestId >> 8);
	EPI_SntBuffInit(AddrL, Ver, MaMicClose);

	// �ѹرջ�Ͳָ��id����Ϣ����PC��
	SendToPcBuff(0x11,(uint8)(((DestId >> 4) & 0xF0) | 0x01),(uint8)DestId);
}
/*********************************************************************************************************
** Function name:       MasterApplyHandle
** Descriptions:  ����ϵͳ�Ļ�Ͳ�������ƣ���Ͳ����ģʽ���Դ�ID�Ļ�Ͳ��������д���         
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
	if (i>=SysInfo.MicWaitNum)   //���ڵȴ��б���
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
			// ���յ�����ϯ�������������ķ�������õ���ͬ��
			for(i = 0; i < SysInfo.MicOnSlaveNum; i++) 
			{			// ��ѯ��ǰID�ǲ����ڷ������б���
				if(SlaverMicOnList[i] == id) break;
			}
						
			if (i < SysInfo.MicOnSlaveNum) 
			{						// �Ѿ�����
				
			}
			else if(SysInfo.MicOnSlaveNum < SysInfo.SpkNum) 
			{	
				// ������Ϣ����ϯ������ȷ���յ���ϯ����ͬ��ָ��
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));	
				// ������������û����
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
								
				// ����õȴ�����
				for (i = 0; i < SysInfo.MicWaitNum; i++) 
				{
					if (WaitMicList[i] == id) break;
				}
				if (i < SysInfo.MicWaitNum) 
				{					// ��ID�Ѿ��ڵȴ��б���
					for (; i < (SysInfo.MicWaitNum - 1); i++)
					{
						WaitMicList[i] = WaitMicList[i + 1];
						WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
					}
					WaitMicList[SysInfo.MicWaitNum - 1] = 0;
					WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x0000);
					SysInfo.MicWaitNum--;
				}

				// ϵͳ��ǰ�����Ļ�Ͳ��
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
				U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
				U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
			 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);				// ������ٴ���
				if (channeltemp == 1){
					CameraSendOpenCode(id, 0, 0);
				}
			}
			else 
			{						// ����������ʱ�Ѿ��ﵽ����״̬
				// ������Ϣ����ϯ������ȷ���յ���ϯ����ͬ��ָ��
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));			
				// ����FIFO��ģʽ����
				MicCloseChannleNumber(SlaverMicOnList[0]);
				SlaveMicStop(SlaverMicOnList[0]);					// ��FIFO�Ĺ�����ǰ��idֹͣ
				
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

				// ����õȴ�����
				for (i = 0; i < SysInfo.MicWaitNum; i++) 
				{
					if (WaitMicList[i] == id) break;
				}
				if (i < SysInfo.MicWaitNum) {	// ��ID�Ѿ��ڵȴ��б���
					for (; i < (SysInfo.MicWaitNum - 1); i++)
					{
						WaitMicList[i] = WaitMicList[i + 1];
						WriteFpgaREG((REG_Wait0ID + i), WaitMicList[i]);
					}
					WaitMicList[SysInfo.MicWaitNum - 1] = 0;
					WriteFpgaREG((REG_Wait0ID + SysInfo.MicWaitNum - 1), 0x0000);
					SysInfo.MicWaitNum--;
				}

				// ������ٴ���
				if (channeltemp == 1){
					CameraSendOpenCode(id, 0, 1);
				}
			}
			break;
		case 0x01:		// ���Կ���
			for (i = 0; i < SysInfo.MicWaitNum; i++) 
			{
				if (WaitMicList[i] == id) break;
			}
			// ��ID�Ѿ��������б���, PC���ѷ��ر�����
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
				SlaveMicDisWait(id);		// ���ͳ�����ȴ�״̬

				// ������������뿪��Ͳ, ���͸���������ϯ��
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xDC, (uint8)((id >> 4) & 0xF0) | 0x0F, (uint8)(id));
				break;
			}

			for (i = 0; i < SysInfo.MicOnSlaveNum; i++) 
			{				// ����
				if (SlaverMicOnList[i] == id) break;
			}
			if (i < SysInfo.MicOnSlaveNum) 
			{								// ���ڿ�����Ͳ�б�
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

				// ϵͳ��ǰ�����Ļ�Ͳ��
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, LoadCmdHead | 0x0F, 0x60 + SysInfo.MicOnSlaveNum);
				CmdModeFlag = flag;
				U_REG_Default1.U_REG_Default1.MaxSpkNum = SysInfo.SpkNum;
				U_REG_Default1.U_REG_Default1.MicOnNum = SysInfo.MicOnSlaveNum;
			 	WriteFpgaREG(REG_Default1,U_REG_Default1.REG_Default1Val);				// ������ٴ���
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
** Descriptions:        ��ϯ��������Ͳ             
*********************************************************************************************************/
void MasterOpenMicHandle(uint16 destid)
{
	uint8 i = 0,temp = 0;

	for(i = 0; i < SysInfo.MicOnMasterNum; i++) // ��ѯ��ǰID�ǲ����ڷ������б���
	{			
		if(MasterMicOnList[i] == destid) break;
	}
	
	if (i < SysInfo.MicOnMasterNum) 						// ������MIC����ID���ڷ����б���
	{						
		MicOnChannleNumber(destid);
		EPI_SntBuffInit(destid % 256, destid / 256, MaMicOpen);
	}
	else 
	{											// ������MIC����ID�����ڷ����б���
		if(SysInfo.MicOnMasterNum < (SysInfo.MicChannelNum-SysInfo.SpkNum)) 		// ������������û����	
		{	
			temp = MicOnChannleNumber(destid);
			if (temp == 1){
				MasterMicOnList[SysInfo.MicOnMasterNum++] = destid;
				MasterMicInvitePrg(destid);
				// ������ٴ���
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
** Descriptions:         ��ϯ���رջ�Ͳ             
*********************************************************************************************************/
void MasterClsMicHandle(uint16 destid)
{
	uint8 i = 0;
	
	for(i = 0; i < SysInfo.MicOnMasterNum; i++) // ��ѯ��ǰID�ǲ����ڷ������б���
	{	
		if(MasterMicOnList[i] == destid) break;
	}
	
	if (i < SysInfo.MicOnMasterNum) // ������MIC�ص�ID���ڷ����б���
	{	

		MicCloseChannleNumber(destid);
		MasterMicStopPrg(destid);
		
		for(; i < (SysInfo.MicOnMasterNum - 1); i++)
		{
			MasterMicOnList[i] = MasterMicOnList[i + 1];
		}
		MasterMicOnList[SysInfo.MicOnMasterNum - 1] = 0;
		SysInfo.MicOnMasterNum--;						
		CameraSendClsCode(destid, 1);// ������ٴ���
			
	}
	else 						// ������MIC����ID�����ڷ����б���
	{								
		EPI_SntBuffInit(destid % 256, destid / 256, MaMicClose);
	}
}
/*********************************************************************************************************
** Function name:       SlaveFrameProcess
** Descriptions:        �ӻ���Ϣ֡���մ���              
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
** Descriptions:        WIFI��Ϣ֡���մ���              
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
** Descriptions:   WIFI��Ԫ���ػ�Ͳ���������뵥Ԫ�б�(ɨ�赥Ԫ��ûɨ�������˻�Ͳû������)
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
			for (i=0;i<=OnlineNum;i++){	  //��ѯID�б�
				if (OnliSlaverId[i] == id)  break;
			}
			if (i>OnlineNum){
				OnliSlaverId[OnlineNum] = id;	// ���ߴ����/��ϯ��ID
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
			for (i=0;i<=OnlineNum;i++){	  //��ѯID�б�
				if (OnliSlaverId[i] == id)  break;
			}
			if (i>OnlineNum){
				OnliSlaverId[OnlineNum] = id;	// ���ߴ����/��ϯ��ID
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
** Descriptions :  ��黰Ͳ8��ͨ����ǰ6��ͨ����WIFIֻ��ǰ6��ͨ�����Ƿ�ȫ��ռ��          
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
** Descriptions: ��ϯ���ʹ����MIC������Ϣ�Ĳ���
** explain:      ��ϯ��Ԫ�����д���Ԫ��ȫ�������⣬�������� ����Ȩ����/����/���ƴ���Ԫ��Ͳ ����       
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
		case DeMicOpen:						// ��������뿪��Ͳ			
			if (ChairKeyMaskSts == FALSE) {	// û����ϯ�����������ȼ�����ִ��
				DelegateOpenToJoin(dest_id);
				SlaveMicOpenHandle(dest_id);//((DelegInfoTable[DestId] & 0xF000) != 0xF000)				
			}
			break;
		case DeMicClose:				// ���������ػ�Ͳ			
			if (ChairKeyMaskSts == FALSE) {
				DelegateOpenToJoin(dest_id);
				SlaveMicCloseHandle(dest_id);	
			}
			break;
		case MaMicOpen:				// ��ϯ������Ͳ, ֻҪ�п��л�Ͳ�Ϳ�
			ChairOpenToJoin(dest_id);
			MasterOpenMicHandle(dest_id);
			break;
		case MaMicClose:				// ��ϯ���ػ�Ͳ
			ChairOpenToJoin(dest_id);
			MasterClsMicHandle(dest_id);
			break;			

		case MaPausePri:				// ִ����ϯ������Ȩ��������
			break;
		case MaDisPausePri:				// �ɱ�����������γ�����ϯ������Ȩ���� 
			break;
		case MaClsPri:					// ��ϯ���ر����д������Ͳ����
			ChairOpenToJoin(dest_id);
			for(i = 0; i < SysInfo.MicOnMasterNum; i++){ // ��ѯ��ǰID�ǲ����ڷ������б���
				if(MasterMicOnList[i] == dest_id) break;
			}
			if (i<SysInfo.MicOnMasterNum)	//�ڷ������б���
			{
				if (SysInfo.MicWaitNum > 0) {		// �еȴ��¼�
					MasterClsPriWtHandle(dest_id);
				}
				else {						// �޵ȴ��¼�
					MasterClsPriHandle(dest_id);
				}			

				if ((ver2 & 0x80) == 0x00) {
					DestIdListHandle(dest_id);						// �����/��ϯ��ID�б���
				}
			}
			else 
			{
				if(SysInfo.MicOnMasterNum < (SysInfo.MicChannelNum-SysInfo.SpkNum)) //���ڷ������б��в�����ϯ����δ��
				{
					channeltemp = MicOnChannleNumber(dest_id);
					if (channeltemp == 1){
					MasterMicOnList[SysInfo.MicOnMasterNum++] = dest_id;
//					MicOnChannleNumber(dest_id);		
					if (SysInfo.MicWaitNum > 0) {		// �еȴ��¼�
						MasterClsPriWtHandle(dest_id);
					}
					else {						// �޵ȴ��¼�
						MasterClsPriHandle(dest_id);
					}
					}			
				}
			}
			break;
		case ChairAgree:				// ��ϯ��ͬ������������Ͳ
			if ((ChairKeyMaskSts == FALSE) && (SysInfo.MicMode == APPLY)) {
				MasterApplyHandle(dest_id, 0);	
			}
			break;
		case ChairAbject:				// ��ϯ�����Դ����������Ͳ
			if ((ChairKeyMaskSts == FALSE) && (SysInfo.MicMode == APPLY)) {
				MasterApplyHandle(dest_id, 1);	
			}
			break;
		case ApplyService:				// ����/��ϯ��Ԫ�����ˮ������������
		case CancleService:				// ����/��ϯ��Ԫ������ˮ������������
			// �������ˮ��id����PC��
			SendToPcBuff(0x0F,(uint8)(((dest_id >> 4) & 0xF0) | ((ver2 & 0x7F) - ApplyService)),(uint8)dest_id);
			break;
		case CallChair:					// ���������ϯ�������ڲ�ͨ������
			SlaveCallChair(dest_id);
			break;
		case CanclChair:				// ���������ϯ�������ڲ�ͨ������			
			if (OnlineCallChairNum>0)
			{	
				for (i=0; i<CALL_CHAIR_ON_NUM; i++){
					if (dest_id == CallChairOnList[i])	break;
				}

				if (i<CALL_CHAIR_ON_NUM){		// �����ID���б���
					CallChairOnList[0] = 0;
					OnlineCallChairNum--;
					// ֪ͨ������ϯ��
					EPI_SntBuffInit(0xD2, MeetCmdHead | 0x0F, CanclChair);
				}
				else {
					EPI_SntBuffInit(dest_id, dest_id>>8, CanclChair);
				}
			}
			break;
		case ChairAgreeCall:
			EPI_SntBuffInit(0xD2, MeetCmdHead | 0x0F, ChairAgreeCall);
		case ApplyHelp:							// ��Ա���������
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
** Descriptions:ǩ��/���ID��������ͻ�������ʼ��  MeetMode, ϵͳģʽ; dest_id : ��ǩ��/���ID, Res: ������             
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
** Descriptions:   ǩ������         dest_id : Ŀ��ID..Para �����յ��Ĵ������Ϣ֡Э�������8λ      
*********************************************************************************************************/
void SignInHandle(uint16 dest_id, uint8 Para)
{   	
	if (Para == SignOk) // Ӧ��������ȷ
	{	
		ResultSendBuffInit(SIGN_IN_STATE, dest_id, 0);
	}
	else if (Para == MSignOver) // ��ϯ������ֹͣǩ��
	{				
		MasterSpcCmdHandle(dest_id, MSignOver);
	}
}
/*********************************************************************************************************
** Function name:       VoteHandle
** Descriptions:  �޼����������    dest_id : Ŀ��ID..ver2 �����յ��Ĵ������Ϣ֡Э�������8λ           
*********************************************************************************************************/
void VoteHandle(uint16 dest_id, uint8 ver2)
{
	uint8 k, p;
	uint8 NoteVer, temp=0;
	uint16 i = 0;
	
	// ���ҳ���ǰID��λ��
	i = FineIdPositionHandle(dest_id);

	switch (SysInfo.VoteType) 
	{
		case VOTE_MODE:
			
			if((ver2 >= YES) && (ver2 <= NO)) 
			{
				k = (uint8)((dest_id - 1) % 8);
				p = (uint8)((dest_id - 1) >> 0x03);
				
				// �������ı�����
		    	NoteVer = ((DelegInfoTable[i] >> 4) & 0x07);		// ��ǰID�Ĵ����ͶƱ״̬
			    temp = ver2 - 0x24;
							    
			    if (NoteVer == 0x00) 								// ��ǰ������û�б��
				{								
					VoteResult[temp - 1]++;
					SetSlaveStaticPrg(i, ver2);	
					ResultSendBuffInit(VOTE_STATE, dest_id, temp);
					if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    }
			    else 
				{												// ��ǰ�����Ѿ������
			    	// �Ѿ��ı�ͶƱ���,ͬʱ���һ�α����Ч,���Ϸ���PC
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

				// ��������ѡ�ٽ��
		    	NoteVer = ((DelegInfoTable[i] >> 4) & 0x07);	// ��ǰID�Ĵ����ͶƱ״̬
			    temp = ver2 - 0x64;
			    
			    if (NoteVer == 0x00) {								// ��ǰ������û�б��
					VoteResult[temp - 1]++;
					SetSlaveStaticPrg(i, ver2);
					ResultSendBuffInit(VOTE_STATE, dest_id, temp);
					if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    }
			    else {												// ��ǰ�����Ѿ����
			    	// �Ѿ��ı�ͶƱ���,ͬʱ���һ�α����Ч
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
				// �����Ѿ������ID
				k = (uint8)((dest_id - 1) % 8);
				p = (uint8)((dest_id - 1) >> 0x03);

				// ���������������
		    	NoteVer = ((DelegInfoTable[i] >> 4) & 0x07);	// ��ǰID�Ĵ����ͶƱ״̬
			    temp = ver2 - 0x44;
			    
			    if (NoteVer == 0x00) {								// ��ǰ������û�б��
					VoteResult[temp - 1]++;
					SetSlaveStaticPrg(i, ver2);

					ResultSendBuffInit(VOTE_STATE, dest_id, temp);
					if (dest_id < 241)	WifiStateUpdate(dest_id, temp);
			    }
			    else {												// ��ǰ�����Ѿ������
			    	// �Ѿ��ı�ͶƱ���,ͬʱ���һ�α����Ч
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
** Descriptions:        dest_id : ID  ver2������ID������           
*********************************************************************************************************/
void SetIdHandle(uint16 dest_id, uint8 ver2)
{
	if ((ver2==AckId)&&(dest_id == Setdestid))	//ID�г�ͻ
	{
		IDRepeat=TRUE;
		Repeat_ID=dest_id;
	}
	if ((ver2 == AckId) && (SysInfo.CurrSetId == dest_id)) 
	{
		if (SysInfo.CurrSetId<(SYS_MAX_DELEG_NUM + MAX_EXMAC_NUM))//�Ҽӵ�
		{
			Setdestid=SysInfo.CurrSetId;
			KeyOpr=TRUE;
			SetIDDelay=0;
		}
	}
}
/*********************************************************************************************************
** Function name:       InterpreterOutNumLoad
** Descriptions:    ��Ա�����ͨ����       Adr : ��ַ Cmmd:����        
*********************************************************************************************************/
void InterpreterOutNumLoad(uint16 dest_id,uint8 Cmmd)
{
	uint8 Ver;
	uint8 i;
	
	Ver = Cmmd & 0xF0;
	switch (Ver) 
	{
		case 0x10:			// ��Ա�������ͨ����
			Ver = Cmmd & 0x0F;
			if ((Ver>0)&&(Ver<INTER_MAX_SPK_NUM))
			{								
				InterpreterMicOpenHandle(dest_id,Ver);
			}		
			break;
		case 0x20:			// ��Ա���ػ�Ͳ���ͨ����
			Ver = Cmmd & 0x0F;
			if ((Ver>0)&&(Ver<INTER_MAX_SPK_NUM))
			{	
				InterpreterMicCloseHandle(dest_id,Ver);	
			}	
			break;
		case 0x80:			// ��Ա�ϴ��Լ���ǰ�ķ�����(1-16)
			Ver = Cmmd & 0x1F;
			if ((Ver>0)&&(Ver<17))
			{
				for(i = 0; i < InterMicBuffSize; i++) 
				{
					if(InterMicBuff[i].OpenId == dest_id) break;
				}

				if (i==InterMicBuffSize)	// ��ID������
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
		   
				for (i=0;i<5;i++)   //��ʱBuff����ID�ͷ����
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

				if (i==InterMicBuffSize)	// ��ID������
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
		   
				for (i=0;i<5;i++)   //��ʱBuff����ID�ͷ����
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
** Descriptions:  ��Ա��ͨ���ţ��������Ϣ֡����  DestId : Ŀ��ID..CmdL������ 
** explain:		  ����������������Ա���������Ϣ֡����             
*********************************************************************************************************/
void InterUpdata(uint16 dest_id,uint8 ver2)
{
    uint8 temp,i,mm;
	temp=ver2&0xF0;
	if (temp==0x50)
	{
		mm=ver2&0x07;

		if ((ver2&0x08)==0x00) //���������Ͳ
		{
			for(i = 0; i < SysInfo.MicOnSlaveNum; i++){ // ��ѯ��ǰID�ǲ����ڷ������б���
				if(SlaverMicOnList[i] == dest_id) break;
			}
			if (i < SysInfo.MicOnSlaveNum) {			// ������MIC����ID���ڷ����б���
				return;	
			}
			UnitMicNumList[mm]=dest_id;
			WriteFpgaREG((REG_Channel0ID + mm), dest_id);
			SysInfo.MicOnSlaveNum++;
			if (SysInfo.MicMode==FREE){
				for (i=0;i<8;i++){
		 			if(FreeMicOnList[i]==0){	  //���ڷ����б�
						FreeMicOnList[i]=dest_id;
						break;
					}
				}
			}
			else {
				for (i=0;i<8;i++){
					if(SlaverMicOnList[i]==0){	  //���ڷ����б�
						SlaverMicOnList[i]=dest_id;
						break;
					}
				}
			}
		}
		else
		{
			for(i = 0; i < SysInfo.MicOnMasterNum; i++){ // ��ѯ��ǰID�ǲ����ڷ������б���
				if(MasterMicOnList[i] == dest_id) break;
			}
			if (i < SysInfo.MicOnMasterNum) {			// ������MIC����ID���ڷ����б���
				return;						
			}
			UnitMicNumList[mm]=dest_id;
			WriteFpgaREG((REG_Channel0ID + mm), dest_id);
			SysInfo.MicOnMasterNum++;
			for (i=0;i<8;i++) {
				if (MasterMicOnList[i]==0){	  //���ڷ����б�
					MasterMicOnList[i]=dest_id;
					break;
				}
			}
		}
	}
	else if (temp==0x00){			  //����Ա��ͨ���Ŵ�����Ӧ�б���
		SysInfo.MicOnInterNum++;
		if (ver2<INTER_MAX_SPK_NUM)	{   
			InterMicOutList[ver2]=ver2;
			InterMicOnList[ver2]=dest_id;
		}  
	}
	else{								   //��Ա���������Ϣ
		InterMic[CurrentCount].OpenId = dest_id;
		InterMic[CurrentCount].Room = ver2;
		CurrentCount++;
	}
}
/*********************************************************************************************************
** Function name:       WritePara
** Descriptions:        ����������ʼ��д��ؼĴ����ͱ���               
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
			else{				//�������ڽ���鿴ģʽ
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
			WriteFpgaREG(REG_SignNum,0);     	//ǩ����������
			SignStartFlag = TRUE;				// ��ʼǩ����־
			break;
		case SET_ID_STATE:
			if (SetIDNumber==0)
				SetIDNumber=1;
			SysInfo.CurrSetId=SetIDNumber;
			break;
		default:
			break;
	}
	WriteFpgaREG(REG_Default1,RegValue);	  //�ָ���Ͳ�������Ϣ
    SysInfo.SpkNum = ((RegValue>>8)&0x07);    //�������
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
   			if ((temp&0x1000)==0x00)	   //δ�з��ͽ��
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
** Descriptions:        ϵͳɨ������ID�󣬽���һ�����ߵĴӻ�ID�б�              
*********************************************************************************************************/
void BuildOnliIdListPrg(void)		
{
	uint16_t i = 0;
//	uint8_t Buff[4]={0,0,0,0};

	SysInfo.PollId = 1;

	// �������Ԫ
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
	// �г�������ϯ����ԪID
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
	
	// �г����ߴ������ԪID
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

	// �г�������Ա����ԪID
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
	if ((PowerUpState == 1)&&(StartScanOnID==0))		// ���ϵ�ɨ���ִ��
	{
		PowerUpState = 2;
		// ֪ͨ��Ԫ�˳�ɨ�����ߵ�Ԫ
		EPI_SntBuffInit(0xD2, MeetCmdHead | 0x0F, ScanIDStop);	
		WriteFpgaREG(REG_Timer,0x0001);
	}
}
/*********************************************************************************************************
** Function name:    HostScanPrg
** Descriptions:     ������������ID�б���ɨ�����ߵ�ID 
****************     �����¹㲥�������ߺͲ����ߵĵ�Ԫ��Ҫ��������������
****************   	 ����ģʽ��ֻɨ�����ߵ�ID��Ԫ��
*********************************************************************************************************/
void HostScanPrg(void)
{	
	uint16_t dest_id = 0;
	uint8_t	 temp=CmdModeFlag;
	switch (SysInfo.MeetSta) 
	{
		case MEET_STATE:
			//if (StartScan==TRUE) break;		  //����ɨ�赥Ԫ ��ֹͣɨ��
		   // if (StopScanFlag == TRUE)  break;    //���ͻ���������ͣɨ��
			//�˴�EpiTxFrameMax_Scan�ɸĳ�EPI_SCAN_TX_MAX_SIZE��Ϊ���������� ʱEpiTxFrameMax_ScanΪ0
			if ((EpiTxFrameCount<3)&&(DonwLoader == 0))//EpiTxFrameMax_Scan) 
			{	
				EPI_SntBuffInit( CurrID&0xff,(CurrID>>8)&0x0f|0x00, 0x24);
				//CurrID=0x03;
				CurrID++;
				//if(CurrID > SYS_MAX_DELEG_NUM)  		//ɨ��ϵͳ���л���  ��1--SYS_MAX_DELEG_NUM
				//				CurrID = 1;
				if(CurrID > 0x05)	
	       	    	CurrID = 0x03;
			}
			break;
		/*case VOTE_STATE:
			
			if (StopScanFlag == TRUE) break;					// ��������,����ͣɨ��
			
			if (EpiTxFrameCount<EpiTxFrameMax_Scan)//EPI_SCAN_TX_MAX_SIZE)
			{	
	       	    dest_id = OnliSlaverId[SysInfo.PollId - 1];	// ������ϯ��������Ա������Ϣ
				
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
	       		if(SysInfo.PollId > OnlineNum)  					// 124̨�����/��ϯ�� + 10̨��չ����
	       	    	SysInfo.PollId = 1;				
			}
			break;
		case SIGN_IN_STATE:

			if (StopScanFlag == TRUE) break;
			
			if (EpiTxFrameCount<EpiTxFrameMax_Scan)					//EPI_SCAN_TX_MAX_SIZE)
			{	
	       		if(SysInfo.PollId > OnlineNum)  					// 124̨�����/��ϯ�� + 10̨��չ����
	       		{
					SysInfo.PollId = 1;	
	       		}

	       	    dest_id = OnliSlaverId[SysInfo.PollId - 1];			// ������ϯ��������Ա������Ϣ
				
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
				if (SysInfo.PollId>(SYS_MAX_DELEG_NUM + MAX_EXMAC_NUM))// 1240̨�����/��ϯ��+10̨��չ���� 
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
** Descriptions:     ����ɨ����ɺ��ʹ���������Ϣ        
*********************************************************************************************************/
void SendStatus(void)
{
	uint16_t dest_id;
	uint8_t temp;
	if (StopScanFlag == TRUE) return;	   //ɨ�軺������

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
** Descriptions:     ɨ�赱ǰ�����µ�����ID��Χ���������ЩID���ߣ���������һ������ID�б�(�����/��չ����)        
*********************************************************************************************************/
void HostScanOnliIdPrg(void)
{
	uint16_t dest_id,comparenum=0;
	static uint16_t numid=1;

    if ((SysInfo.MeetSta == MEET_STATE) &&(EpiTxFrameCount<16))//EPI_SCAN_TX_MAX_SIZE)) // ����û���·��Ŀ�����Ϣ����ʱ��Ҫ�·���Ӧģʽ�µ���ѯ��
	{ 			
		if (CheckOnliIdCnt > 0) // ����ɨ��       
		{	
       	    dest_id = SysInfo.PollId;									// ������ϯ������������Ϣ
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

   		    if(SysInfo.PollId > (SYS_MAX_DELEG_NUM + MAX_EXMAC_NUM))// 1240̨�����/��ϯ��+10̨��չ���� 
			{ 	
       	    	SysInfo.PollId = 1;		// ����ɨ��
       	    	CheckOnliIdCnt--;
				numid = 1;		
       		}
		}
		else 
		{
			if (ScanData.State == 0)
			{
				ScanData.State = 1;				// ��ʼ��ʱ
			}
			else if(ScanData.State == 2)		// ��ʱʱ�䵽
			{
				ScanData.State = 3;				
				ChkOnlineIdFlag = FALSE;		// ����IDɨ�账�����
				BuildOnliIdListPrg();			// ��������ID�б�
				ScanEnd=1;                      //ɨ�����
				StartScan=FALSE;
				KeyLockStatus=FALSE;
			}
		}
	}
}
/*********************************************************************************************************
** Function name:       HostScan
** Descriptions:        ����ɨ��          
*********************************************************************************************************/
void HostScan(void)
{
	/*if (HostOnline==0x02){
		if (HostStart==TRUE){
			if (ChkOnlineIdFlag){ 
				HostScanOnliIdPrg();			// ɨ�����л�������������ID�б�
			}	
			else {
				if(ScanEnd==0){*/
					HostScanPrg();				// ������ѯ���ߵ�ID,�����ǻ���
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
** Descriptions:        ��������ǩ��/���/�ϵ��������MIC�����б�           
*********************************************************************************************************/
void ResetApplyMicInfoPrg(void)
{
	uint8_t i;
	
	if (SysInfo.MicMode == APPLY)
	{
		for(i = 0; i < MAX_MIC_WAIT_NUM; i++)// ��ǰ�ĵȴ��б�
		{
			WaitMicList[i] = 0;
			WriteFpgaREG((REG_Wait0ID + i), 0x0000);		
		}
		SysInfo.MicWaitNum	 = 0;

		for(i = 0; i < MAX_MIC_ON_NUM; i++)
		{
			SlaverMicOnList[i] = 0;			// ��ǰ���Դ����MIC ��ID�б�
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
	if (BidFlag == TRUE)	//��ֹ����Ͳ
	{
		for(i = 0; i < MAX_MIC_WAIT_NUM; i++)// ��ǰ�ĵȴ��б�
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
** Descriptions:        ����������ʼ��д��ؼĴ����ͱ���               
*********************************************************************************************************/
void ScanOnliIdInit(uint8_t Mode)
{
	uint16_t i = 0;
	// ��ʼ�����߼��ID
	ChkOnlineIdFlag 	= Mode;
	CheckOnliIdCnt		= Mode * 1;			// һ��Ҫɨ��2�飬ɨ��һ�ξ��ǲ�����
	SysInfo.PollId 	= 1;
	ScanData.State = 0; 
	ScanData.DelayCount = 0;
	CheckRepeatIdFlag = 0;			       //���ID��ͻ��־

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
** Descriptions:        ϵͳ����ģʽת��     ver : ģʽ����               
*********************************************************************************************************/
void SysModeSwitch(uint8_t ver)
{
	switch (ver) 
	{
		case MEET_STATE:			//  ����ģʽ  						
	    	if(SysInfo.MeetSta != MEET_STATE) 
			{				
			 	if(SysInfo.MeetSta == SIGN_IN_STATE) 
				{
			 	    // ���л�������ǩ��
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
					
		 		    // �����л���ͶƱ����
					VoteResRtnStart = FALSE;	// ֹͣ��������PC��
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
					ConnectedPcFlag = TRUE;			// �����������߼��PC
				}
			  	Setdestid=0;

			  	//  �����л����������ģʽ
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, 0x0F, 0x01);	
				EPI_SntBuffInit(0xD2, 0x0F, 0x01);
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xD2, 0x0F, 0x01);	
				EPI_SntBuffInit(0xD2, 0x0F, 0x01);
				RefreshFlag = 1;	
			}
			// ����Ӧ����Ϣ��PC
			SendToPcBuff(0x00,0x00,MEET_STATE);
			break;
		case VOTE_STATE:			//  ���ģʽ
			if (VoteFuncFlag == FALSE)	break;		// ���û������������ܣ����˳�
			
			// �����ǻ���ģʽ�²��ܷ�����
			if(SysInfo.MeetSta == MEET_STATE) 
			{
				ConnectedPcFlag = FALSE;			// �����������߼��PC
				PcConnectCnt	= 0x00;
				PcConnectCntBuf	= 0x00;

				VoteMode = SysInfo.VoteType * 4 + (uint8)VoteResTypeFlag * 2 + (uint8)VoteSignedFlag * 1;
			  	SysInfo.MeetSta = VOTE_STATE;
				SysInfo.PollId = 1;
				// ���ģʽ�µ���ر�����ʼ��
				VoteModeValInit();
				ResetApplyMicInfoPrg();

				// ���л�����ʼͶƱ
				gbit_start_vote = TRUE;				// �����ʼ
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, VoteCmdHead | 0x0F, VoteStart + VoteMode);
				CmdModeFlag = 1;
				EPI_SntBuffInit(0xD2, VoteCmdHead | 0x0F, VoteStart + VoteMode);
				
				// �ȷ���Ӧ����Ϣ��PC
				SendToPcBuff(0x00,0x00,VOTE_STATE);
				RefreshFlag = 1;
			}
			break;
		case SIGN_IN_STATE:			// ǩ��ģʽ
			if (SignFuncFlag == FALSE)	break;  	// ���ǩ������û�п���,���˳�

			if(SysInfo.MeetSta == MEET_STATE) 
			{
				ConnectedPcFlag = FALSE;		// �����������߼��PC
				PcConnectCnt	= 0x00;
				PcConnectCntBuf	= 0x00;

			  	SysInfo.MeetSta = SIGN_IN_STATE;
				SysInfo.PollId = 1;
				// ǩ��ģʽ�µ���ر�����ʼ��
				SignModeValInit();				
				ResetApplyMicInfoPrg();
				
				WriteFpgaREG(REG_SignMaxNum,SignMaxNum);
				WriteFpgaREG(REG_SignNum,SignedNum);
			 	// Ӧ��PC��
			 	SendToPcBuff(0x00,0x00,SIGN_IN_STATE);
				RefreshFlag = 1;
			}
			break;
		case SET_ID_STATE:		// �Դ��������ID�༭			
			if(SysInfo.MeetSta != SET_ID_STATE) 
			{
				ConnectedPcFlag = FALSE;			// �����������߼��PC
				PcConnectCnt	= 0x00;
				PcConnectCntBuf	= 0x00;
				SysInfo.PollId = 1;

				SysInfo.MeetSta = SET_ID_STATE;
				SysInfo.CurrSetId = SysInfo.SetStartIdNum;
				// ���ͽ�������ID�㲥
				CmdModeFlag = 0;
				EPI_SntBuffInit(0xD2, SetIdCmdHead | 0x0F, EnterId);
				WifiInfor.wifiID.wifiID.currentID = SysInfo.CurrSetId;
				WifiInfor.wifiID.wifiID.setIDflag = 1;	
				WriteFpgaREG(REG_WifiSetID, WifiInfor.wifiID.currentIDwifi);
				// ������д����/��ϯ����Ͳ��Ϣ
				ResetMicInfoPrg();
			}
			// ������Ϣ��PC
			SendToPcBuff(0x00,0x00,SET_ID_STATE);
			break;
		default :
			break;
	}
}
/*********************************************************************************************************
** Function name:       ReturnSignResult
** Descriptions:   �ҵ��Ѿ�ǩ������û�аѽ�����͸�PC��ID�����ҷ���ǩ�������PC              
*********************************************************************************************************/
void ReturnSignResult(void)
{
	uint8 k, p, temp;
	uint16  dest_id;
	
	// ֻ�е��з������� && �ѵ�����С��Ӧ������ʱ�ŷ���ǩ�����ݸ�PC��
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

			k = (uint8)((dest_id - 1) & 0x07);			// 8bit��־λ��
			p = (uint8)((dest_id - 1) >> 0x03);			// ID�ŵ�λ��
			temp = SignVoteSendList[p] & (0x01 << k);
			
			// ��û�з���ǩ�����ݸ�PC��
			if (temp == 0x00) 						
			{							
			    // ʵʱ�Ľ�ID�ͱ��������͸�PC��
			    SendToPcBuff(0x03,((uint8)(dest_id >> 4) & 0xF0) | 0x03,(uint8)dest_id);
				EPI_SntBuffInit(dest_id, (dest_id>>8)|SignCmdHead, 0x05);	// ��Ӧ����ǩ���ĵ�Ԫ
				SignedNum++; 
				// ����ʵ����������������ϯ��
				
				if (dest_id < 241)	WifiStateUpdate(dest_id, 0x01);
				WriteFpgaREG(REG_SignNum,SignedNum);	
				SignVoteSendList[p] |= (0x01 << k);
				if (MasterSigneId == dest_id)
				{
					WriteFpgaREG(REG_MySignStart,0x8000|dest_id);
				}
			}
			else		  //�Ѿ�ǩ�� Ӧ��ǩ����Ԫ
				EPI_SntBuffInit(dest_id, (dest_id>>8)|SignCmdHead, 0x05);

			if (VoteRdPnt == VoteWrPnt) 
			{
				VoteSendPcFlag = FALSE;
				StopScanFlag   = FALSE;
					 	   	 	
				if (SignExitTimeStart == 1)		// �˳�ǩ��
				{
					SignExitTimeStart = 0;
					// Ӧ����ϯ��
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);		
					EPI_SntBuffInit((uint8)(dest_id), SignCmdHead | (uint8)(dest_id >> 8), 0x0F);
					// ����PCǩ���Ѿ�����
					SendToPcBuff(0x03,0x01,0x00);					
					SysModeSwitch(MEET_STATE);
				}
			}
		}
	}
}
/*********************************************************************************************************
** Function name:       ReturnVoteNamedId
** Descriptions:     ʵʱ�ķ��ؼ������ģʽ�£��Ѿ������ID�ͱ��ѡ��              
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

			
	    // ʵʱ�Ľ�ID�ͱ��������͸�PC��
		k = (uint8)((dest_id - 1) % 8);
		p = (uint8)((dest_id - 1) >> 0x03);			// ��־λ��
		if ((Res & 0x08) == 0x08) 					// ��չ�����´����/��ϯ�����һ�α����Ч
		{					
			SignVoteSendList[p] &= ~(0x01 << k);
		}
		Res &= 0x07;								// ���α���������Чλ
		temp = SignVoteSendList[p] & (0x01 << k);

		if((temp == 0) && (Res <= 0x05)) 
		{
		    VoteRes = ((dest_id >> 4) & 0xF0);
		    VoteRes |= Res;
	
		    // ʵʱ�Ľ�ID�ͱ��������͸�PC��
		    SendToPcBuff(0x12,VoteRes,(uint8)(dest_id));
			SignVoteSendList[p] |= (0x01 << k);		
		}
		
		if (VoteRdPnt == VoteWrPnt) 
		{
			VoteSendPcFlag = FALSE;
			StopScanFlag   = FALSE;
			
			if (SignExitTimeStart == 1)		// �˳�ǩ��
			{
				SignExitTimeStart = 0;
				QuitVoteFlag	= FALSE; 		// ֹͣ�����,��ֱ���˳����
				VoteResRtnEnable= TRUE;			// ʹ�ܷ��ر�����������				
				// Ӧ����ϯ��
				EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x30);
				EPI_SntBuffInit((uint8)(dest_id), (uint8)(dest_id >> 8) | VoteCmdHead, 0x30);
			}
		}
	}
}
/*********************************************************************************************************
** Function name:       ReturnVoteResult
** Descriptions:        �㲥ͶƱ���                
*********************************************************************************************************/
void ReturnVoteResult(void)
{
	uint8 i;
	
	// ����5��Ʊ��������������ϯ��
	switch (SysInfo.VoteType) {
		case SELECT_MODE:
		case EVALUA_MODE:
			// ����K5��Ʊ��
			i = (VoteResult[4] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD7, i | 0x0F, (uint8)(VoteResult[4] & 0xFF));
			WriteFpgaREG(REG_VoteRes5,VoteResult[4]);
			SendToPcBuff(0x13,i|0x05,(uint8)(VoteResult[4] & 0xFF));
			// ����K4��Ʊ��
			i = (VoteResult[3] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD6, i | 0x0F, (uint8)(VoteResult[3] & 0xFF));
			WriteFpgaREG(REG_VoteRes4,VoteResult[3]);
			SendToPcBuff(0x13,i|0x04,(uint8)(VoteResult[3] & 0xFF));
		case VOTE_MODE:
			// ����K3��Ʊ��
			i = (VoteResult[2] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD5, i | 0x0F, (uint8)(VoteResult[2] & 0xFF));
			WriteFpgaREG(REG_VoteRes3,VoteResult[2]);
			SendToPcBuff(0x13,i|0x03,(uint8)(VoteResult[2] & 0xFF));
			// ����K2��Ʊ��
			i = (VoteResult[1] >> 4) & 0xF0;
			EPI_SntBuffInit(0xD4, i | 0x0F, (uint8)(VoteResult[1] & 0xFF));
			WriteFpgaREG(REG_VoteRes2,VoteResult[1]);
			SendToPcBuff(0x13,i|0x02,(uint8)(VoteResult[1] & 0xFF));
			// ����K1��Ʊ��
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
** Descriptions:        ����ǩ������µĽ����PC                
*********************************************************************************************************/
void ReturnPcResultPrg(void)
{
	// ����ǩ�����
	switch (SysInfo.MeetSta) 
	{
		case SIGN_IN_STATE:
			if (SignStartFlag == TRUE)	// �Ѿ�������ǩ��
			{			
				ReturnSignResult();					// ����ǩ������
			}
			break;
		case VOTE_STATE:							// ʵʱ�ķ��ر������
			if (gbit_start_vote == TRUE)            // �Ѿ���ʼ���
			{			
				ReturnVoteNamedId();				// ���ر��ID,�ͱ�����
			}
			if (VoteResRtnStart == TRUE) // ���ͽ��
			{
				// �������յı�����(������Ʊ�������ܱ������)
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
				if (QuitVoteFlag == TRUE) // ֱ���˳����ģʽ
				{	
					// �������ģʽ
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
** Descriptions:      Sony���������ݽ��մ���              
*********************************************************************************************************/
void SonyCameraDeal(void)
{
	uint8 TempBuff[12],i;
	uint16 buffid=0;
	
	if (Camera_Buffer[Camera_Info.CameraRD].RecvBuff[1] != 0x50)			//�յ���δ��������ֱ�ӷ���
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
//					LcdDisplayString(48, 184,12, 0, "ȫ���������");
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
//					LcdDisplayString(48, 208,8, 0, "�������");
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
** Descriptions:     ���������л�����             
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
** Descriptions:        ϵͳ���紦��                
*********************************************************************************************************/
void PwrOffHandle(void)
{	
	uint8_t temp = CmdModeFlag;
	if (BackupHostOnline==FALSE){
		CmdModeFlag = 0;
		EPI_SntBuffInit(0xD2, MeetCmdHead| 0x0F, PowerDown);		// �ػ�
		CmdModeFlag = 1;
		EPI_SntBuffInit(0xD2, MeetCmdHead| 0x0F, PowerDown);		// �ػ�
	}
	RelayOut_OFF;						// �Ͽ�����������Ƶ�̵���
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
** Descriptions:        ����Ϊ1ʱ����ʱΪ1ms                
** input parameters:    ÿ��һ��ʱ��0.3ms
** output parameters:   ��
** Returned value:      ��
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
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
	/*Ӳ����ʼ��*/
	SystemInit();                                                          /* ϵͳ��ʼ��                  */
	InitDevices();
	/*������ʼ��*/
	VariableInit();
	PwrOnSetup();
	BackupHostInit();	
	CameraSetAddr();
	WDTInit();
	//ChkOnlineIdFlag 	= 0;
	//InitAddFlag();
	while (1){
		/*********************IDģʽ�¹㲥��ͻID*******************/
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
		
		/******************״̬�л� ��ر�����ʼ��*****************/
		if ((HostTemp!=HostSta)&&(BackupStart==FALSE)){		  
			ParameterInit();
			HostTemp=HostSta;
		}

		/*******************������������ɨ�赥Ԫ*******************/
		if ((HostOnline==0x02)&&(ScanOnlineId!=0x00)&&(BackupStart==FALSE)){	
			ScanUint();
		}
		
		/***************��������������ر�����ʼ��*****************/
		if ((HostOnline==0x02)&&(ParaInit==TRUE)&&(BackupStart==FALSE)){	
			WritePara();
			ParaInit=FALSE;
		}

		/**********************��������ɨ��************************/
		if ((KeyLockStatus==FALSE)&&(DonwLoader == 0)){	//�������� ��������
			KeyScanPrg();								// ������⼰����
		}
		FuncKeyPrg();									// �����ܴ���
		SetIdKeyPrg();									// ��������ID

		/********************ɨ������ID����ѯ**********************/
		if (HostSta==0x01)	HostScan();
		delay(1200);
		/*********************�������ݽ���************************/
		UipPolling();
		if (DonwLoader == 1)	HEX_Send();
		else					NetRxDataProcess();	// �����������ݰ�
		if (HostSta==0x01)		ReturnPcResultPrg();// ���ؽ����PC��
		
		/********************1602��ʾˢ��**********************/
		RefreshShow();
		/********************��Ԫ����Ϣ����**********************/
		SlaveFrameProcess();	// ������ϯ�������Ա����Ϣ����
		if (ChkOnlineIdFlag == FALSE)	
			MicInforProcess();	// �������ģʽ������
		
		/********************WIFI��Ϣ����**********************/
		WifiFrameProcess();
		/***********************����ͷ����************************/
		if (Camera_Info.CameraRD != Camera_Info.CameraWR)//Sony��������
			SonyCameraDeal();	
		if (Arry_RDCount != Arry_WRCount)
			ArryDataDeal();
		
		/*****************��ʱ��PC��������Ӧ��*******************/
		HostCheckPcPrg();
		
		/*******************����Ϣ�������Ӧ��*******************/
		if (SMS_Tx_State == 2)	replytoPC();

		/************************����PC**************************/
		Lm75_Check();
		ADC_Check();
		/************************ι��**************************/
		WDTFeed();
		/******���ڷ���update,�س��ص�boot,�ȴ��û�����update����Ӧ�ò�******/
		/*if(u8UARTReceive(&tempDate)>0)		//�����β�ʹ��
		{
			if(tempDate==0x20)              //�ո��
			{
					len = 0;
					vUARTSend(konggejian, sizeof(konggejian));           //please input a update command
			}
			else if(tempDate==0x0D)         //�س��� 
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
/*void uip_log(char *m)		//20241029_clone��֧�޸�+other person remove update function
{
	;
	//printf("���ã���������");
}*/
/*void last_func(void)
{
	//printf("send debug message\n");
}*/
/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
