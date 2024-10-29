#ifndef MAIN_H
#define MAIN_H
#include "stdint.h"

#ifndef _MAIN_GLOBAL_
	#define	MAIN_GLOBAL	extern
#else
	#define	MAIN_GLOBAL
#endif

#include "define.h"

typedef struct 
{														/* ����һ���˵���־ΪTRUEʱ������Ϊ�¼��˵���ڲ��� */
	void (* currentWindow)(void);						/* ��ǰ������ʾ����      */
	uint8_t	 enterFlag;									/* ���뵱ǰ�˵���־	 	 */
	uint8_t  *currentPara;								/* ��ǰ�˵����ò���      */
	uint8_t	 paraRange;									/* ����ֵ��Χ			 */
	uint8_t  modifyType;								/* ���Ҽ��޸�����		 */
}floorWindow;
typedef struct 
{														/* ����һ���˵���־ΪTRUEʱ������Ϊ�¼��˵���ڲ��� */
	void (* currentWindow)(void);						/* ��ǰ������ʾ����      */
	uint8_t	 nextFlag;									/* �Ƿ�����һ���˵���־  */
	uint8_t *currentPara;								/* ��ǰ�˵����ò���      */
	uint8_t	 paraRange;									/* ����ֵ��Χ			 */
	uint8_t  modifyType;								/* ���Ҽ��޸�����		 */
	uint8_t	 enterFlag;									/* ���뵱ǰ�˵���־	 	 */
	floorWindow * nextPage;								/* �¼��˵����ָ��		 */
}sonWindow;
typedef struct 
{														/* ����һ���˵���־ΪTRUEʱ������Ϊ�¼��˵���ڲ��� */
	void (* currentWindow)(void);						/* ��ǰ������ʾ����      */
	uint8_t	 nextFlag;									/* �Ƿ�����һ���˵���־  */
	uint8_t *currentPara;								/* ��ǰ�˵����ò���      */
	uint8_t	 paraRange;									/* ����ֵ��Χ			 */
	uint8_t  modifyType;								/* ���Ҽ��޸�����		 */
	uint8_t	 enterFlag;									/* ���뵱ǰ�˵���־	     */
	sonWindow * nextPage;								/* �¼��˵����ָ��		 */
}window;
MAIN_GLOBAL window MenuWindow[13];						/* ʮ�������˵�			 */

MAIN_GLOBAL	sonWindow VolumeMenu[7]; 					/* VOLUME�˵���6���Ӳ˵� */

MAIN_GLOBAL	sonWindow EQMenu[2]; 						/* EQ�˵���2���Ӳ˵�	 */
MAIN_GLOBAL	floorWindow SoundCustomMenu[7]; 			/* �Զ���7�ξ�����		 */

MAIN_GLOBAL	sonWindow ZoneMenu[6];						/* zone��һ����5���˵�	 */
MAIN_GLOBAL	floorWindow zone1Page[5];					/* ÿ��zone������ѡ��	 */
MAIN_GLOBAL	floorWindow zone2Page[5];
MAIN_GLOBAL	floorWindow zone3Page[5];
MAIN_GLOBAL	floorWindow zone4Page[5];
MAIN_GLOBAL	floorWindow zoneSw[4];
MAIN_GLOBAL	sonWindow NetMenu[2];						/* ��������				 */
MAIN_GLOBAL	floorWindow ipPage[5];
MAIN_GLOBAL	floorWindow macPage[7];
MAIN_GLOBAL	sonWindow TransMenu[5];						/* ���������			 */
MAIN_GLOBAL	floorWindow roomPage[2];
MAIN_GLOBAL	sonWindow CameraMenu[3];					/* ����������			 */
MAIN_GLOBAL	floorWindow mapPage[3];
MAIN_GLOBAL	sonWindow MiscMenu[9];						/* ���� WIFI CONTROL	 */

MAIN_GLOBAL uint8_t RefreshFlag;						/* ����ˢ�±�־			 */
MAIN_GLOBAL uint8_t BeInFlag;
MAIN_GLOBAL uint8_t	EnterFlag;							/* ������������ò˵���־*/
MAIN_GLOBAL uint8_t	CurrentPage;						/* ��ǰ�˵�����			 */
MAIN_GLOBAL uint8_t ChooseNum;							/* ѡ��λ��ѡ��			 */
MAIN_GLOBAL uint8_t ParaIndex;							/* ����λ��ѡ��			 */
MAIN_GLOBAL uint8_t NumDsp;
//-----------------------------------------------------------------------------------
MAIN_GLOBAL unsigned long TheSysClock;					// ϵͳʱ�ӱ���
static volatile unsigned short *pEpiFpgaPort;			// ����ָ��
MAIN_GLOBAL uint8_t 	SysFaultAnalyse;					// оƬ���Ϸ���
MAIN_GLOBAL uint8_t 	PowerUpState;						// �ϵ�״̬ 1:�ϵ� 2:ɨ�����
MAIN_GLOBAL	uint8_t	MicNum;								// ����PC��Ͳ���� 0.1.2.3.4
MAIN_GLOBAL uint16_t  CurrID;
MAIN_GLOBAL	uint8_t   CmdModeFlag;						// 0:DIG   1:WIFI   
MAIN_GLOBAL	uint8_t   ModeSwitch;                         //PC�л�����
MAIN_GLOBAL	uint8_t   ModeSwDelay;                        //�����л���ʱ
MAIN_GLOBAL	uint8_t   VersionDis;                         //�汾��ʾ����
MAIN_GLOBAL	uint8_t   VerDelay;                           //�汾�л���ʱ
MAIN_GLOBAL	uint8_t	BackupInit;							//��������������ر�����ʼ��
MAIN_GLOBAL	uint8_t   HostOnline;                         //00 ���������� 01 ��������  02�������� 
MAIN_GLOBAL	uint8_t   HostFlag;
MAIN_GLOBAL	uint8_t   HostSta;                            //����״̬  00����������״̬  01������״̬
MAIN_GLOBAL	uint8_t   HostTemp;
MAIN_GLOBAL	uint16_t	ScanID;                             //��ѯID��
MAIN_GLOBAL	uint8_t   ScanOnlineId;						//ɨ������ID
MAIN_GLOBAL	uint8_t   HostStart;							//��������������ʼ��  TRUE����ʼ�����  FALSE�����ڳ�ʼ��
MAIN_GLOBAL	uint8_t	HostCheckDelay;                     //�������߼��
MAIN_GLOBAL	uint8_t   AlarmCount;                         //������������
MAIN_GLOBAL	uint8_t   StaDelay;
MAIN_GLOBAL	uint16_t	Mic_Buff[12];						//Mic��Ϣ���»���
MAIN_GLOBAL	uint8_t	MicUpdata;                          //Mic���±�־λ
MAIN_GLOBAL	uint8_t	MicWaitUpdata;						//Mic�ȴ��б����
MAIN_GLOBAL	uint8_t	KeyLockStatus;						//����������־
MAIN_GLOBAL uint8_t   IDRepeat;                          //ID��ͻ��־
MAIN_GLOBAL uint16_t  Repeat_ID;						   //�ظ�ID
MAIN_GLOBAL uint8_t	KeyOpr;  
MAIN_GLOBAL	uint8_t	VoteRtnFlag; 					   //���ͽ����־
MAIN_GLOBAL uint8_t	SetIDDelay;	
MAIN_GLOBAL uint16_t  PowerOnDelay;                   //�����ϵ���ʱ
MAIN_GLOBAL uint8_t   PowerOnFlag;
MAIN_GLOBAL uint8_t   PowerFlag;                      //�ϵ���ɱ�־
//MAIN_GLOBAL uint8_t   WaitUpdata;                     //�ϵ���µȴ��б�
MAIN_GLOBAL uint8_t   BackupHostOnline;
MAIN_GLOBAL uint8_t	MicStaUpdata;
MAIN_GLOBAL uint8_t   OnlineDelay;
MAIN_GLOBAL uint8_t   CheckHostDelay;
MAIN_GLOBAL uint16_t	InterPara;                     //��Ա�������Ϣ����
MAIN_GLOBAL uint16_t  InterVal;
MAIN_GLOBAL uint8_t   ModeStaDelay;
MAIN_GLOBAL uint8_t   ModeSta;
MAIN_GLOBAL uint8_t   StaCount;
MAIN_GLOBAL uint16_t	Setdestid;  
MAIN_GLOBAL	uint16_t  SignNumber;                         //ǩ������
MAIN_GLOBAL	uint16_t  SetIDNumber;                        //�·���ID��
MAIN_GLOBAL	uint16_t  RegValue;							//��ȡ�Ĵ���ֵ
MAIN_GLOBAL	uint8_t   SendResult;                         //�·�ͶƱ���
MAIN_GLOBAL	uint8_t   ParaInit;                           //������������������ʼ��
MAIN_GLOBAL	uint8_t	BlinkFlag;                          //��˸��־λ
//MAIN_GLOBAL uint8_t	Mic_Wait_Flag;                      //Mic�ȴ��б���±�־
MAIN_GLOBAL uint16_t	Mic_Wait_Buff[12];					//Mic�ȴ��б��ͻ���
MAIN_GLOBAL uint8_t   Mic_WaitTx_Updata;
MAIN_GLOBAL uint8_t   BackupStart;
MAIN_GLOBAL uint8_t	BidFlag;                          //ͶƱǩ����ͲȨ�ޱ�־
MAIN_GLOBAL uint8_t	TypeID;
MAIN_GLOBAL uint8_t	InterRoomSet;
MAIN_GLOBAL uint8_t	InterRoomFlag;
MAIN_GLOBAL uint8_t	RangeBit;  							//0:û�й��  1����λ  2��ʮλ  3����λ  4ǧλ
MAIN_GLOBAL char	Idtemp[6];
MAIN_GLOBAL uint8_t	Scanflag;
typedef struct 
{
	uint16_t	tos;		//����  ������
	uint16_t	thyst;	//����  �ط���
	uint8_t	control;
	uint8_t	flag;
}Lm75Struct;
MAIN_GLOBAL	Lm75Struct FanControl;
MAIN_GLOBAL uint16_t	CurrentTemple;

typedef struct 
{
	uint8_t		gain1;
	uint8_t		gain2;
	uint8_t		gain3;
	uint8_t		gain4;
//	uint16_t	beginID;
	uint16_t	endID;
}ZoneInfor;
typedef struct 
{
	ZoneInfor	SoundZone[4];
//	uint16_t	SwitchZone;	/* bit(4)  1:�������������ܿ�  0:�������������ܹ�  bit(3--0)DSP����   0:����1  1:����2  2:����3  3:����4 */
	union
	{
		uint16_t	SwitchZone;
		struct
		{
			unsigned Dsp1Out	:4;
			unsigned Dsp2Out	:4;
			unsigned VideoOut	:4;
			unsigned Switch		:1;
			unsigned Defaul		:3;	
		}reg_zoon;	
	}reg_zoon;
}SoundZoneInfor;
MAIN_GLOBAL	SoundZoneInfor HornZone;
MAIN_GLOBAL	uint8_t 	   DbToGain[42];
typedef struct
{
	uint8_t KeyContent[72];
	uint8_t	SsidContent[40];
	uint8_t	wifibuffer[96];
	uint8_t	DebugResult[6];
	uint16_t	DelayTime[16];
	uint8_t controlwifi[7];
	uint8_t	onesecondflag;
	uint8_t onesecondcount;
	union
	{
		uint16_t currentIDwifi;
		struct
		{
			unsigned currentID	:15;/* ��ǰID */
			unsigned setIDflag	:1;	/* 1:��ʼ��ID   0:������ID */
		}wifiID;
	}wifiID;
	union
	{
		uint16_t controlwifi;
		struct
		{
			unsigned debugstart	:1;/* 1:��ʼ����  0:��������*/
			unsigned clearflag	:1;/* ��λ������Խ�� */
			unsigned modeflag	:1;/* 1:ʡ��ģʽ  0:����ģʽ*/
			unsigned wifisign	:1;/* 1:��ʼǩ��  0:����ǩ��*/
			unsigned wifivote	:1;/* 1:��ʼ���  0:�������  2:�鿴������*/
			unsigned votemode	:2;/* 00:������� 01:5��ѡ��1 2 3 4 5   10:5������-- - 0 + ++*/
			unsigned votefirst	:1;/* 1:����״���Ч  0:������һ����Ч*/
			unsigned votesigned	:1;/* 0:�������ǩ��  1:�����Ҫǩ��*/
			unsigned updatwifi	:1;/* 1:��ʼ����KEY��SSID  0:��������*/
			unsigned rfflag		:1;/* 1:RF��ʼ���, 0:RF�������*/
			unsigned rebootflag :1;/* ����KEY��SSID����һ���½���ʹWIFI��Ԫ����*/
			unsigned timedelay	:4;/* 0:10ms 1:15ms 2:18ms 3:20ms 4:25ms 5:30ms  ������ʱʱ�䣬Ĭ��Ϊ18ms*/
//			unsigned defaul		:1;
		}wififlag;
	}wififlag;
}WifiInfor_R;
MAIN_GLOBAL	WifiInfor_R WifiInfor;
MAIN_GLOBAL	uint8_t RFDelayFlag;/* 0:3.1s�͵�ƽ��ʱ  1:15.1s�ߵ�ƽ��ʱ*/
MAIN_GLOBAL	uint16_t RfTime;
typedef struct{
	union{
		uint16_t WifiStaReg1Value;
		struct{
			unsigned id1state	:3;/*id1  3bit  state  */
			unsigned id2state	:3;/*id2  3bit  state  */
			unsigned id3state	:3;/*id3  3bit  state  */
			unsigned id4state	:3;/*id4  3bit  state  */
			unsigned id5state	:3;/*id5  3bit  state  */
			unsigned id6state	:1;/*id6  1LSB  state  */
		}WifiStaReg1;
	}WifiStaReg1;
	union{
		uint16_t WifiStaReg2Value;
		struct{
			unsigned id6state	:2;/*id6  2MSB  state  */
			unsigned id7state	:3;/*id7  3bit  state  */
			unsigned id8state	:3;/*id8  3bit  state  */
			unsigned id9state	:3;/*id9  3bit  state  */
			unsigned id10state	:3;/*id10  3bit  state */
			unsigned id11state	:2;/*id11  2LSB  state */
		}WifiStaReg2;
	}WifiStaReg2;
	union{
		uint16_t WifiStaReg3Value;
		struct{
			unsigned id11state	:1;/*id11  1MSB  state */
			unsigned id12state	:3;/*id12  3bit  state */
			unsigned id13state	:3;/*id13  3bit  state */
			unsigned id14state	:3;/*id14  3bit  state */
			unsigned id15state	:3;/*id15  3bit  state */
			unsigned id16state	:3;/*id16  3bit  state */
		}WifiStaReg3;
	}WifiStaReg3;
}WifiState_R;
MAIN_GLOBAL	WifiState_R WifiState[15];
typedef struct{
	uint16_t  ICState1;
	uint16_t  ICState2;
	uint16_t  ICState3;
}ICState_R;
MAIN_GLOBAL	ICState_R ICState[15];
typedef struct 
{
	uint8_t SpkNum;				// ���������	
	uint8_t MicMode;				// 0:FIFO 	1:Normal	2:Apply
	uint8_t MicWaitNum;			// ���뿪��Ͳ����
	uint8_t MicOnMasterNum;		// ���ڷ�����ϯ������,���Ʋ�ѯ�����б�
	uint8_t MicOnSlaveNum;		// ���ڷ��Դ��������	
	uint8_t MicOnInterNum;		// ���ڷ�����Ա������
	uint8_t MicChannelNum;		// MICͨ��Ϊ6��WIFI���ƣ���MICͨ��Ϊ8(ԭDIG����)
	uint8_t MicFreeOnNum;		// FREE_MIC_ON_NUM

	uint8_t MeetSta;				// ����״̬
	uint8_t VoteType;				// ѡ������
	uint16_t PollId;        		// ��ǰ��ѯ��id
	uint16_t CurrSetId; 			// ��ǰSET ID 
	uint16_t SetStartIdNum;		// ���ÿ�ʼID
	
	uint8_t LineInVol;			// ��·��������
	uint8_t IR_ChanVol;			// ����ͨ������
	uint8_t BassVol;				// ��������
	uint8_t TrebleVol;			// ��������
	uint8_t LinOutVol;			// ��·�������
	uint8_t UintOutVol;			// ��Ԫ�������
	uint8_t	LinkOutVol;
	uint8_t	Band1;					//EQ�����߸�Ƶ��1---7
	uint8_t Band2;
	uint8_t Band3;
	uint8_t Band4;
	uint8_t	Band5;
	uint8_t	Band6;
	uint8_t	Band7;
	uint8_t Soft;
	uint8_t Strength;
	uint8_t Classic;
	uint8_t	EQSelect;

	uint8_t InterChanTotalNum;	// ����ͨ������
	uint8_t CurrentLanNum;		// ��ǰ��ʾ�����Ժ�
	uint8_t CurrentLanVal;		// ��ǰ��ʾ������ֵ
	uint8_t InterRoomNum;			// �������		

	uint8_t Languge;
	uint8_t BackUpFlag;			// �ȱ��ݿ��ر�־
	uint8_t Sound_AFNGC1;
	uint8_t Sound_AFNGC2;

	uint8_t IPAddValue[4];		// IP��ַ
	uint8_t SubMaskValue[4];		// ��������
	uint8_t GatewayValue[4];		// Ĭ������
	
	uint8_t ChannelLan[32];     	// ÿ��ͨ����Ӧ��������Ϣ0-178..ԭ������

	uint8_t DefaultValue;			// Ĭ��

	
} MainInfoStruct;
MAIN_GLOBAL	MainInfoStruct SysInfo;

MAIN_GLOBAL union
{
	uint16_t	RegTrackBackVal;
	struct
	{
		unsigned BackID		:12;	//������ٹ����У���һ����Ͳ�ر�ʱ������ǰһ��Ԥ��λ��ID
		unsigned TrackSw	:1;		//WIFI��Ԫ�������LED����ʹ��
		unsigned Nc			:3;		//Ԥ��
	}RegTrackBack;
}RegTrackBack;

MAIN_GLOBAL union
{
	uint16_t RegOther2Val;

	struct
	{		
		unsigned HostMode	:4;// �ȱ��� 0101:����	1010:�ӻ�
		unsigned Default	:1;// 
		unsigned HostOnline	:1;// ��̨��������
		unsigned AutoZero	:1;// �Զ�0ͨ�� 0: �ر� 1: ��
		unsigned Speaker 	:1;// ������ 1: ��  0:�ر� û��
		unsigned Language   :2;// �˵���ʾ����0: ���� 1: Ӣ�� 2: ����
		unsigned ChangeHost	:2;// 10ʱ��ʾҪ���л���������01��ʾҪ���л��ɴӻ�
		unsigned SyaTick	:4;// ���ӻ�����
	}RegOther2;
}RegOther2;

MAIN_GLOBAL union
{
	uint16_t REG_IRLightVal;
	struct
	{		
		unsigned IRLight	:4;// ��������ȵ���
		unsigned other		:12;//û��
	}RegIR;
}RegIR;
/*
B15��		0���������򿪡�1���������رա�
B14-B13��	00���������ԡ�01��Ӣ�ġ�10�����塣
B12-B11��	��ʾ����01����ʱ�������ʱ��10����ʱ���Ե���ʱ��
B10-B8��	���Ͳ����

B7-B3		
B2-B0		���ڷ�������

*/
MAIN_GLOBAL union
{
	uint16_t REG_Default1Val;
	struct
	{
		unsigned MicOnNum	:3;
		unsigned PC_Online	:5;		// PC�ھ�
		unsigned MaxSpkNum	:3;
		unsigned Default 	:2;		// ��ʾ��
		unsigned Language 	:2;		
		unsigned Speaker	:1;		// 0���������򿪡�1���������رա�
	}U_REG_Default1;
	
}U_REG_Default1;

// ��ϯ����������־
MAIN_GLOBAL union
{
	uint16_t	REG_MyVoteStartVal;
	struct
	{
		unsigned Id			:12;	// ID��
		unsigned Default 	:2;		// û��
		unsigned LookRes	:1;		// �鿴������		
		unsigned Ask		:1;		// �������		
	}U_REG_MyVoteStart;
	
}U_REG_MyVoteStart;

//-----------------------------------------------------------------------
// �������͸����������Ϣ֡
typedef struct 
{
	uint16_t FpgaDest;			//Ŀ���ַ
	//ushort Byte;				//���͵��ֽ���
	uint16_t Type_AddrL;			//�������ͺ�Ŀ���ַ��8λ
	uint16_t Para_CmdL;			//�����4λ&��ַ��4λ,�����8λ
	//ushort CRC;				//У����0xFCFC
}EpiTxFrameStruct;
MAIN_GLOBAL EpiTxFrameStruct EpiTxFrameBuff[TX_Frame_MAX_SIZE+1];
MAIN_GLOBAL	uint8_t TxWriteCount;
MAIN_GLOBAL	uint8_t TxReadCount;	
MAIN_GLOBAL uint8_t EpiTxFrameCount;		//�����ٶ�,���Ͳ�����.//����Ҫ���͵�֡��,��ͨʹ�����ͬʱ����16֡(4*16=64),����Ϣ���120���ֽ�.
// ��������ID��ȷ�����ɨ��һ֡��������ID�����ڵ���35ʱ��ɨ��35֡��
MAIN_GLOBAL uint8_t EpiTxFrameMax_Scan;		// �����������15֡����ÿ��ֻ���Ϸ�һ֡������ֻ�ܽ���һ֡���ݴ���
MAIN_GLOBAL	EpiTxFrameStruct WifiTxBuff[RX_Frame_MAX_SIZE];
MAIN_GLOBAL	uint8_t WifiTxWrite;
MAIN_GLOBAL	uint8_t WifiTxRead;
MAIN_GLOBAL	uint16_t WifiPollID;
//-----------------------------------------------------------------------
// �������մ������Ϣ֡
typedef struct  
{
	uint8_t AddrL;
	uint8_t Para;
	uint8_t Cmmd;
} RxFrameStruct; 
MAIN_GLOBAL	RxFrameStruct EpiRxFrameBuff[RX_Frame_MAX_SIZE];
MAIN_GLOBAL	uint8_t RxWriteCount;
MAIN_GLOBAL	uint8_t RxReadCount;
MAIN_GLOBAL	RxFrameStruct WifiRxBuff[RX_Frame_MAX_SIZE];
MAIN_GLOBAL	uint8_t WifiRxWrite;
MAIN_GLOBAL	uint8_t WifiRxRead;
MAIN_GLOBAL	uint8_t TxFlag;
MAIN_GLOBAL	uint8_t Set_UNIT;

typedef struct {
	uint8_t	Camera_Set;
	uint8_t Camera_Type;
	uint16_t Camera_PreID;
	uint8_t	RecvCount;	
	uint8_t CameraRD;
	uint8_t CameraWR;
	//uint8_t	Camera_Num;		//ѡ�����NUmber
	//uint8_t Camera_Chan;
	uint8_t	Out_Chan;
	uint8_t Set_Num;
}Camera_struct;

typedef struct {
	uint8_t RecvBuff[11];
}Camera_Buff;

MAIN_GLOBAL	Camera_Buff		Camera_Buffer[10];

MAIN_GLOBAL	uint8_t Arry_RecvCount;
MAIN_GLOBAL	uint8_t	Arry_RDCount;
MAIN_GLOBAL	uint8_t	Arry_WRCount;
MAIN_GLOBAL	Camera_Buff		Arry_Buffer[5];

typedef struct {
	uint8_t Map_Chan;
	uint8_t Map_Addr;
}Chan_Buf;
MAIN_GLOBAL uint8_t Camera_Serial;
MAIN_GLOBAL	Chan_Buf	Camera_Map[16];

//MAIN_GLOBAL uint8_t Camera_Chan[16];

MAIN_GLOBAL	Camera_struct	Camera_Info;
MAIN_GLOBAL	uint16_t	CameraIDList[128];
MAIN_GLOBAL	uint8_t FullView_Con[12];
MAIN_GLOBAL uint8_t FullViewFlag;

//MAIN_GLOBAL unsigned short EPI_RX_Buff[EPI_RX_BUFF_SIZE];	//���ջ���

//-----------------------------------------------------------------------
// ����������Ϣ��������������
MAIN_GLOBAL	uint16_t  SMS_Tx_Buff[SMS_TX_BUF_SIZE];	
MAIN_GLOBAL uint8_t 	SMS_TxSizeCount;				// ���ζ���Ϣ�����ֽ���
MAIN_GLOBAL uint8_t	SMS_Tx_State;					// 0:û�����ݷ��� 1:�ж���Ϣ���� 2:���ŷ����� 3:
MAIN_GLOBAL unsigned char * SMS_WR_Point;
MAIN_GLOBAL unsigned char * SMS_RE_Point;
MAIN_GLOBAL unsigned char * AddrPoint;
MAIN_GLOBAL unsigned char * FramePoint;
MAIN_GLOBAL unsigned char FrameCount;

//MAIN_GLOBAL uint8_t 	SMS_Tx_DelayCount;				// ������������ж�ʮ���ֽ����ϣ����ͺ�Ҫ�ȴ�10ms�������������ݣ�Ҫ��Ȼ���ǡ�
//-----------------------------------------------------------------------
// ��Ԫ�ϴ�IC��������
typedef	struct 
{
	uint16_t Buff[CARD_RX_BUFF_SIZE];
	uint16_t *Point;
	uint16_t *Process;
	uint16_t *Tail;
	uint8_t Delay;
}CardRxStruct;
MAIN_GLOBAL CardRxStruct CardRx;

//-----------------------------------------------------------------------
// ����ģʽ����ر�������
MAIN_GLOBAL	uint16_t 	FreeMicOnList[FREE_MIC_ON_NUM];				// ���ɿ���ģʽ�£����MIC��ID�б�
MAIN_GLOBAL	uint16_t 	SlaverMicOnList[MAX_MIC_ON_NUM];			// ���ڷ����б�,�����
MAIN_GLOBAL	uint16_t 	MasterMicOnList[MAX_MIC_ON_NUM];			// ���ڷ����б�,��ϯ��
MAIN_GLOBAL	uint16_t 	InterMicOnList[INTER_MAX_SPK_NUM];			// ���ڷ����б�,��Ա��.��ѯ��ǰID�ǲ����ڷ������б���
MAIN_GLOBAL	uint16_t 	WaitMicList[MAX_MIC_WAIT_NUM];				// ���Եȴ��б�
MAIN_GLOBAL	uint16_t 	UnitMicNumList[MAX_MIC_ON_NUM];				// ���ڴ򿪵Ļ�Ͳ���롣(0-7),�����ID��
MAIN_GLOBAL uint8_t  	InterMicOutList[INTER_MAX_SPK_NUM];  		// ��Ա�����ͨ����(1-15)
MAIN_GLOBAL	uint16_t  OnliSlaverId[MAX_DELEG_NUM]; 				// ���ߵĴ������ID
MAIN_GLOBAL	uint16_t  OnlineNum;     								// �������ߵ�Ԫ
MAIN_GLOBAL	uint16_t  OnlineChairNum;     						// ���ߵ���ϯ����
MAIN_GLOBAL	uint16_t  OnlineSlaveNum;     						// ���ߵĴ������
MAIN_GLOBAL	uint8_t   OnlineWifiNum;
MAIN_GLOBAL	uint16_t	OnlineInterNum;								// ���ߵ���Ա��
MAIN_GLOBAL uint16_t	Mic_Wait_Buff[12];				          	//Mic�ȴ��б��ͻ���

MAIN_GLOBAL uint16_t	PauseID;						
MAIN_GLOBAL uint8_t	CheckRepeatIdFlag;	
MAIN_GLOBAL uint8_t   PauseOnList;						
//MAIN_GLOBAL	uint16_t  OnliSlaverIdBuff[MAX_DELEG_NUM]; 			// ���ߵĴ������ID,��ID�Ŵ�����Ӧλ��

//MAIN_GLOBAL	uint8_t	UnitType[MAX_DELEG_NUM];                   //����������ɨ�軺��

								
MAIN_GLOBAL	uint16_t   DelegInfoTable[MAX_DELEG_NUM];				// �����״̬��
//   D15 D14        D13~D8      D7    D6 D5 D4   D3     D2       D1      D0 
//  ������ϸ���    ����������    NC     Vote      NC   Online   Dis/En    M/S  

#define			CALL_CHAIR_ON_NUM		1
MAIN_GLOBAL	uint16_t 	CallChairOnList[CALL_CHAIR_ON_NUM];			// ��������ϯ���ڲ�ͨ���б�
MAIN_GLOBAL	uint8_t  OnlineCallChairNum;     						// ������������ϯ���ڲ�ͨ����

// ɨ������ر���
MAIN_GLOBAL uint8_t 	StartScanOnID;							// ɨ�����ߵ�Ԫ��־ 0: û��ɨ�� 1: ����ɨ�� 2: ��ʼɨ��
MAIN_GLOBAL uint16_t 	ScanOnIDCount;							
MAIN_GLOBAL	uint8_t   PollTargetFlag;			  					// ��ѯĿ���־λ 0������� 1����չ����
MAIN_GLOBAL	uint8_t   ChkOnlineIdFlag;		    				// ��ѯ���ߴ������־λ
MAIN_GLOBAL	uint8_t  	CheckOnliIdCnt;								// ��ѯ���ߵ�Ԫѭ������
MAIN_GLOBAL	uint8_t	ScanEnd;                                   //ɨ�����
MAIN_GLOBAL	uint8_t	StartScan;
MAIN_GLOBAL	uint8_t	LangSend;

union RegData{
	uint16_t	value;
	struct {
		unsigned vfdmove	:2;
		unsigned blacklight	:1;	
		unsigned frontlight	:1;
		unsigned ChannelFlag :1;
		unsigned	TotalChannel :5;
		unsigned default1	:1;
		unsigned alcfun		:1;
		unsigned RTCFlag		:1;
	}other;
};
MAIN_GLOBAL	union RegData	RegData5;


//��ʱ20ms���ٴ��������Ϸ�������
typedef struct
{
	uint8_t State;		// 0:û���ݴ��� 1.��ʼ��ʱ 2:�ռ�ɨ�������� 3:���Դ������� 4:�ռ��������
	uint8_t DelayCount;	// ÿ��һ��10ms
}ScanDataStruct;
MAIN_GLOBAL ScanDataStruct ScanData;


// MIC������Ϣ֡����
typedef struct  
{ 
    uint8_t OprMod;              	 	// MIC������ʽ(��/��)
    uint16_t  Oprid;                    // ��ҪMIC������ID
} MicOprFrameStruct;

MAIN_GLOBAL	MicOprFrameStruct   MicOprInforBuff[MIC_INFOR_BUF_SIZE];
MAIN_GLOBAL	uint8_t	 	MicWrPnt;								// MIC��Ϣдָ��
MAIN_GLOBAL	uint8_t	 	MicRdPnt;								// MIC��Ϣ��ָ��
//MAIN_GLOBAL	uint8_t		MicInforOkFlag;	

// ���
MAIN_GLOBAL	uint8_t	VoteLookres;
MAIN_GLOBAL	uint8_t	VoteFuncFlag;								// ������ܱ�־
MAIN_GLOBAL	uint8_t	gbit_start_vote;							// ���ģʽ������־
MAIN_GLOBAL	uint8_t 	VoteResTypeFlag;      						// �����͹�ͶƱ��־λ
MAIN_GLOBAL	uint8_t	VoteSignedFlag;      						// ��Ҫǩ�������ͶƱ��־λ
MAIN_GLOBAL	uint8_t	VoteResRtnOk;								// ����������OK��־
MAIN_GLOBAL	uint8_t	QuitVoteFlag;								// �˳������־
MAIN_GLOBAL	uint8_t   VoteResRtnStart;      						// ���ͽ����־λ
MAIN_GLOBAL	uint8_t   VoteResRtnEnable;  							// 
MAIN_GLOBAL	uint16_t  VoteResRtnCnt;				      			// ���ͽ����ʱ����
MAIN_GLOBAL	uint8_t 	VoteMode;
MAIN_GLOBAL	uint16_t  VoteResult[5];								// 10 ̨��չ���� + 1̨���� �ı�����
MAIN_GLOBAL	uint16_t  VoteResultFlag;								// ��չ�������ر�������ܽ����־

MAIN_GLOBAL	uint16_t	VoteTimer;									// ��ʱ�����ʱ��
MAIN_GLOBAL uint8_t	VoteTimeState;								// ��ʱ���״̬ 0: �رն�ʱ�����1��ʹ�ܶ�ʱ�����2: ��ʼ��ʱ�����	
MAIN_GLOBAL uint8_t 	Vote_10ms;									// 10ms����������ˢ����ʾʱ��

// ǩ��
MAIN_GLOBAL	uint8_t   SignFuncFlag;								// ǩ�����ܱ�־����
MAIN_GLOBAL	uint8_t   SignStartFlag;								// ǩ����ʼ��־
MAIN_GLOBAL	uint16_t  SignedNum;									// ��ǩ������
MAIN_GLOBAL	uint16_t  SignMaxNum;									// ���ǩ������,10̨��չ���� + 1̨���� * 125
//MAIN_GLOBAL	uchar	  SignFullFlag;							// ǩ������־
MAIN_GLOBAL uint16_t  MasterSigneId;								// ��ϯ������ǩ����ID��

MAIN_GLOBAL	uint16_t  SignVoteIdBuf[VOTE_SENT_BUF_SIZE];  		// �Ѿ�ǩ��/�����ID(���͸�PC���Ļ�����)
MAIN_GLOBAL	uint16_t   VoteResultBuf[VOTE_SENT_BUF_SIZE];			// �Ѿ������ID�Ľ��
// ÿ̨ID�ѷ���ǩ�����������־λ��ÿ������8��λ������������8��ID��־
MAIN_GLOBAL	uint16_t  SignVoteSendList[SYS_MAX_DELEG_NUM/8+1]; // �Ѿ�����ǩ������б�,�Ѿ�����ǩ������б�
MAIN_GLOBAL	uint8_t   VoteWrPnt;									// ���͸�PC��дָ��
MAIN_GLOBAL	uint8_t   VoteRdPnt;									// ���͸�PC�Ķ�ָ��
MAIN_GLOBAL	uint8_t	VoteSendPcFlag;								// �������ݸ�PC��־
MAIN_GLOBAL	uint8_t	StopScanFlag;								// ���ͻ��������󣬽�ֹɨ��
MAIN_GLOBAL unsigned char DonwLoader;
MAIN_GLOBAL	uint8_t	SignExitTimeStart;							// ǩ��ʱ���˳�������������û���겻�����Ͻ������
//------------------------------------------------
//TCP/IP����
MAIN_GLOBAL uint16_t 	NetRxBuff[NET_RX_BUFF_SIZE];				// 
MAIN_GLOBAL uint16_t 	*NetRxPoint;									// ����֡ͷ
MAIN_GLOBAL uint16_t 	*NetRxProcess;									// ���մ������
MAIN_GLOBAL uint16_t 	*NetRxTail;										// ����ָ������
MAIN_GLOBAL uint8_t 	NetRxRelay;										// ����ʱ���ڴ�����,��Ȼ�ͻᶪ����������
MAIN_GLOBAL unsigned char	*NetRxPoint1;
MAIN_GLOBAL unsigned char 	*NetRxProcess1;
MAIN_GLOBAL	unsigned char tempNUM;
MAIN_GLOBAL unsigned char 	*Ptobuff;
MAIN_GLOBAL unsigned char RxTime;
MAIN_GLOBAL unsigned char RxTimeDelay;
MAIN_GLOBAL unsigned char PCLoad;
MAIN_GLOBAL unsigned char PCLoadDelay;
MAIN_GLOBAL uint16_t 	NetRxBuff_Size;								//�˴����ݴ�С,����PC��ʽ�ж�,�������ͼƬ�����ͼƬ����Ҳ��0xFCFC�����
MAIN_GLOBAL uint16_t 	NetRxBuff_Type;								//�˴���������
MAIN_GLOBAL uint16_t 	NetRxBuff_Type_DWIN;					//�˴�����ΪDWIN����
MAIN_GLOBAL uint16_t 	NetRxBuff_Size_i;							//�˴����ݽ��մ���

// ������PC���ӱ�־
MAIN_GLOBAL	uint8_t	bConnectedPcFlag;							// ��־λ����
MAIN_GLOBAL	uint8_t   ConnectedPcFlag;							// �������߼��PC
MAIN_GLOBAL uint16_t	PcConnectCnt;								// ���Ӽ�����
MAIN_GLOBAL	uint16_t	PcConnectCntBuf;							// ���Ӽ�����������
MAIN_GLOBAL uint8_t  	DisconnectPc;

//������Ϣ��ر���
MAIN_GLOBAL	uint8_t   ChairKeyMaskSts;			// ��ϯ���Ƿ����˴������Ͳ�����μ� 0��û�У�1����
MAIN_GLOBAL	uint8_t	SetIdKeyPressed;
MAIN_GLOBAL	uint8_t   QuitDelay;                  //�˳���ID��ʱ
MAIN_GLOBAL	uint8_t   QuitFlag;                   //�˳���ID��־
MAIN_GLOBAL	uint8_t  	SetIdFlag;
MAIN_GLOBAL uint8_t 	SetIdKeyCount;				//��������
MAIN_GLOBAL	uint8_t	KeyOprOkFlag;
//MAIN_GLOBAL	uint8_t   LcdReturnFlag;
//MAIN_GLOBAL	uint16_t  LcdReturnDelay;
MAIN_GLOBAL	uint16_t	KeyValue;
MAIN_GLOBAL	uint8_t	KeyIndex;
MAIN_GLOBAL	uint8_t	sKeyIndex;
MAIN_GLOBAL	uint8_t	KeyTypeFlag;				// �������ͣ�0x00���̰���0xCC������		
MAIN_GLOBAL	uint8_t	KeyPushPopFlg;				// ��������/�����־ 1�����£�0������
MAIN_GLOBAL	uint8_t 	KeyDelay10Ms;
//MAIN_GLOBAL	uint8_t	KeyLockStatus;				// ��������״̬
// �����ֿ�
MAIN_GLOBAL uint8_t  eng_line_bytes; 
MAIN_GLOBAL uint8_t  eng_char_bytes;
MAIN_GLOBAL uint8_t  chn_line_bytes;
MAIN_GLOBAL uint8_t  chn_char_bytes;
MAIN_GLOBAL uint8_t  font_pointer[4];


#define	UartTempSize		100

MAIN_GLOBAL uint8_t UartTempRx[UartTempSize];
MAIN_GLOBAL uint8_t UartRxDelay;
MAIN_GLOBAL uint8_t *UartRxPoint;
MAIN_GLOBAL uint8_t *UartRxProcess;
MAIN_GLOBAL uint8_t *UartRxTail;

MAIN_GLOBAL uint8_t	Fpga2Port1Value;
MAIN_GLOBAL uint8_t	Fpga2Port2Value;
MAIN_GLOBAL uint16_t	BlinkPortValue;
MAIN_GLOBAL uint16_t 	InterruptChannleFlag;
MAIN_GLOBAL uint16_t 	InterruptChannleFlag2;

MAIN_GLOBAL uint8_t 	NetSetBuff[4];				// ����������Ϣ��ʱ��
MAIN_GLOBAL uint8_t 	Buff1K[1024];				//
MAIN_GLOBAL uint8_t 	NetSetBuff[4];				// ����������Ϣ��ʱ��
MAIN_GLOBAL	uint8_t 	MACAddrBuff[6];
#define InterMicBuffSize		5
// ������Ա������Ͳ�б���ֹ�Ϸ����ķ�������ͨ���Ų�һ��
typedef struct
{
	uint16_t OpenId;				// ��Ա�����뿪��Ͳ��ID
	uint8_t Room;					// ��Ա�����ڵķ���
}InterMicStruct;

MAIN_GLOBAL uint8_t CurrentCount;		// 
MAIN_GLOBAL InterMicStruct InterMicBuff[InterMicBuffSize];// �ݴ淭����
MAIN_GLOBAL InterMicStruct InterMic[INTER_MAX_SPK_NUM];

typedef struct
{
  uint16_t TempMicid;
  uint8_t TempMicRoom;
 //uint8_t TempOutChannel;
}TempInterMic;
MAIN_GLOBAL TempInterMic   TempInterBuff[6];

#define InterMaxNum  32
MAIN_GLOBAL	uint8_t InterRoomBuff[InterMaxNum];
MAIN_GLOBAL	uint16_t InterMicOpenBuff[InterMaxNum][InterMaxNum];

MAIN_GLOBAL uint8_t	SysTickCount;			// ϵͳ���ģ��������ӻ�ͨѶ�á�
MAIN_GLOBAL uint8_t	SysTickValue;			// ����������
MAIN_GLOBAL uint8_t	ChangToHostCount;		// �л�������ģʽ����.��Ϊ5Sʱ���Զ��л�������

#define		ByPass_3108_Bit				0x0001
#define		ByPass_AFC_Bit				0x0002
#define		ByPass_ANC_Bit				0x0004
#define		ByPass_AGC_Bit				0x0008
#define		ByPass2_AFC_Bit				0x0040
#define		ByPass2_ANC_Bit				0x0080
#define		ByPass2_AGC_Bit				0x0100


MAIN_GLOBAL uint16_t	ByPassValue;			// Bit0=1 ����3108��Bit1=1 ������Ч
MAIN_GLOBAL uint16_t	Startup_3108;     //�����ϵ�ʱ�ж϶ϵ�ǰ�Ƿ�����Ч�ı�־
// �������
//MAIN_GLOBAL	uint8_t   SBUF2;
MAIN_GLOBAL	uint8_t   cmd_control[15];
MAIN_GLOBAL	uint8_t		Sony_Zoom[9];
//MAIN_GLOBAL	uchar   cmd_control[11] = {0xa0,0x10,0x10,0x00,0x07,0x00,0xff,0xff,0xff,0xaf,0xdb};
//MAIN_GLOBAL	uchar code camera_code[11] 	= {0xa0,0x10,0x10,0x00,0x07,0x00,0xff,0xff,0xff,0xaf,0xdb};
//MAIN_GLOBAL	uchar code result_code[11] 	= {0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0xff,0xff,0xff,0xff,0xff};

// ����2����
//MAIN_GLOBAL	uint8_t  	uart2_index;
//MAIN_GLOBAL	uint8_t  	uart2_cnt;
//MAIN_GLOBAL	uint8_t   SoftUartBusy;

// ��һ�����뷢�Ե�id
MAIN_GLOBAL	uint16_t  CameraList[CEMARE_SERIES];
MAIN_GLOBAL	uint8_t 	CameraPnt;
MAIN_GLOBAL	uint8_t   ChairPosition;
MAIN_GLOBAL	uint8_t   debugstart_Count;		//���Ʒ��Ͷ���Ϣ��WIFI6302K

//----------------------------------------------------------------
MAIN_GLOBAL void WifiStateInit(void);
MAIN_GLOBAL void ICStateUpdate(void);
MAIN_GLOBAL void ICStateVolume(uint8_t id);
MAIN_GLOBAL void WifiStateUpdate(uint8_t id, uint8_t state);
MAIN_GLOBAL void delay(int16_t time);
MAIN_GLOBAL void watchdog_init(void);	
MAIN_GLOBAL void InitDevices(void);
MAIN_GLOBAL void MainValInit(void);
MAIN_GLOBAL void CheckChipFault(void);
MAIN_GLOBAL void SysValInit(void);
MAIN_GLOBAL void PwrOnSetup(void);
MAIN_GLOBAL void HardUart1ValInit(void);
MAIN_GLOBAL void NetValInit(void);
MAIN_GLOBAL void VoteModeValInit(void);
MAIN_GLOBAL void SignModeValInit(void);
MAIN_GLOBAL void GetStatus(void);
MAIN_GLOBAL uint16_t FineIdPositionHandle(uint16_t DestId);
MAIN_GLOBAL void InsertOnliSlrIdHandle(uint16_t DestId);
MAIN_GLOBAL void SlaveMicEnWait(uint16_t DestId);
MAIN_GLOBAL void SlaveMicDisWait(uint16_t DestId);
MAIN_GLOBAL void SlaveMicInvitePrg(uint16_t DestId);
MAIN_GLOBAL void SlaveMicStop(uint16_t DestId);
MAIN_GLOBAL void MasterMicInvitePrg(uint16_t DestId);
MAIN_GLOBAL void MasterMicStopPrg(uint16_t DestId);
MAIN_GLOBAL void MasterApplyHandle(uint16_t id, uint8_t EnDis);
MAIN_GLOBAL void SlaveMIcApplyOpen(uint16_t id);
MAIN_GLOBAL void SlaveMIcApplyClose(uint16_t id);
MAIN_GLOBAL void MicOpenWaitHandle(uint16_t id);
MAIN_GLOBAL void SlaveMicOpenHandle(uint16_t id);
MAIN_GLOBAL void SlaveMicCloseHandle(uint16_t id);
MAIN_GLOBAL void MasterOpenMicHandle(uint16_t destid);
MAIN_GLOBAL void MasterClsMicHandle(uint16_t destid);
MAIN_GLOBAL void MasterPausePriHandle(uint16_t destid);
MAIN_GLOBAL void MasterDisPausePriHandle(uint16_t destid);
MAIN_GLOBAL void MasterClsPriWtHandle(uint16_t dest_id);
MAIN_GLOBAL void MasterClsPriHandle(uint16_t destid);
MAIN_GLOBAL void MasterSpcCmdHandle(uint16_t dest_id, uint8_t Para);
MAIN_GLOBAL void DestIdListHandle(uint16_t dest_id);
MAIN_GLOBAL void MicInforProcess(void);
MAIN_GLOBAL void ResultSendBuffInit(uint8_t MeetMode, uint16_t dest_id, uint8_t Res);
MAIN_GLOBAL void SignInHandle(uint16_t dest_id, uint8_t Para);
MAIN_GLOBAL void VoteHandle(uint16_t dest_id, uint8_t ver2);
MAIN_GLOBAL void SetIdHandle(uint16_t dest_id, uint8_t ver2);
MAIN_GLOBAL void SetSlaveStaticPrg(uint16_t DestId, uint8_t CmdL);
MAIN_GLOBAL void SlaveFrameProcess(void);
MAIN_GLOBAL void SlaveIcCardMsgProcess(uint16_t *pRxData);
MAIN_GLOBAL void BuildOnliIdListPrg(void);
MAIN_GLOBAL void PwrOffHandle(void);
MAIN_GLOBAL void HostScanOnliIdPrg(void);
MAIN_GLOBAL void HostScanPrg(void);
MAIN_GLOBAL void HostSendCmdPrg(void);
MAIN_GLOBAL void ResetMicInfoPrg(void);
MAIN_GLOBAL void ResetApplyMicInfoPrg(void);
MAIN_GLOBAL void ReturnVoteResult(void);
MAIN_GLOBAL void ReturnSignResult(void);
MAIN_GLOBAL void ReturnVoteNamedId(void);
MAIN_GLOBAL void ReturnPcResultPrg(void);
MAIN_GLOBAL void SysModeSwitch(uint8_t ver);
MAIN_GLOBAL void ScanOnliIdInit(uint8_t Mode);
MAIN_GLOBAL uint8_t MicOnChannleNumber(uint16_t DestId);
MAIN_GLOBAL uint8_t MicCloseChannleNumber(uint16_t DestId);
MAIN_GLOBAL void HostScan(void);
MAIN_GLOBAL uint8_t ReadWriteFPGA2Port(uint8_t Port, uint8_t RD, uint8_t Data);
MAIN_GLOBAL void InterrupChannleUpLan(void);
MAIN_GLOBAL  void InterpreterOutNumLoad(uint16_t dest_id,uint8_t Cmmd);
MAIN_GLOBAL uint8_t CheckInterOutNum(uint8_t OutNum);
MAIN_GLOBAL uint8_t CheckInterRoomNum(uint8_t RoomNum);
MAIN_GLOBAL void InterUpdata(uint16 dest_id,uint8 ver2);
MAIN_GLOBAL void DefaultInit(void);
MAIN_GLOBAL void PcHasPara(void);


#endif

/*********************************************************************************************************
** End Of File
*********************************************************************************************************/
