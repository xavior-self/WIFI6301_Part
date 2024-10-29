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
{														/* 当下一级菜单标志为TRUE时，参数为下级菜单入口参数 */
	void (* currentWindow)(void);						/* 当前窗口显示函数      */
	uint8_t	 enterFlag;									/* 进入当前菜单标志	 	 */
	uint8_t  *currentPara;								/* 当前菜单所用参数      */
	uint8_t	 paraRange;									/* 参数值范围			 */
	uint8_t  modifyType;								/* 左右键修改类型		 */
}floorWindow;
typedef struct 
{														/* 当下一级菜单标志为TRUE时，参数为下级菜单入口参数 */
	void (* currentWindow)(void);						/* 当前窗口显示函数      */
	uint8_t	 nextFlag;									/* 是否有下一级菜单标志  */
	uint8_t *currentPara;								/* 当前菜单所用参数      */
	uint8_t	 paraRange;									/* 参数值范围			 */
	uint8_t  modifyType;								/* 左右键修改类型		 */
	uint8_t	 enterFlag;									/* 进入当前菜单标志	 	 */
	floorWindow * nextPage;								/* 下级菜单入口指针		 */
}sonWindow;
typedef struct 
{														/* 当下一级菜单标志为TRUE时，参数为下级菜单入口参数 */
	void (* currentWindow)(void);						/* 当前窗口显示函数      */
	uint8_t	 nextFlag;									/* 是否有下一级菜单标志  */
	uint8_t *currentPara;								/* 当前菜单所用参数      */
	uint8_t	 paraRange;									/* 参数值范围			 */
	uint8_t  modifyType;								/* 左右键修改类型		 */
	uint8_t	 enterFlag;									/* 进入当前菜单标志	     */
	sonWindow * nextPage;								/* 下级菜单入口指针		 */
}window;
MAIN_GLOBAL window MenuWindow[13];						/* 十三个主菜单			 */

MAIN_GLOBAL	sonWindow VolumeMenu[7]; 					/* VOLUME菜单有6个子菜单 */

MAIN_GLOBAL	sonWindow EQMenu[2]; 						/* EQ菜单有2个子菜单	 */
MAIN_GLOBAL	floorWindow SoundCustomMenu[7]; 			/* 自定义7段均衡器		 */

MAIN_GLOBAL	sonWindow ZoneMenu[6];						/* zone第一级有5个菜单	 */
MAIN_GLOBAL	floorWindow zone1Page[5];					/* 每个zone有两个选项	 */
MAIN_GLOBAL	floorWindow zone2Page[5];
MAIN_GLOBAL	floorWindow zone3Page[5];
MAIN_GLOBAL	floorWindow zone4Page[5];
MAIN_GLOBAL	floorWindow zoneSw[4];
MAIN_GLOBAL	sonWindow NetMenu[2];						/* 网络设置				 */
MAIN_GLOBAL	floorWindow ipPage[5];
MAIN_GLOBAL	floorWindow macPage[7];
MAIN_GLOBAL	sonWindow TransMenu[5];						/* 翻译间设置			 */
MAIN_GLOBAL	floorWindow roomPage[2];
MAIN_GLOBAL	sonWindow CameraMenu[3];					/* 摄像球设置			 */
MAIN_GLOBAL	floorWindow mapPage[3];
MAIN_GLOBAL	sonWindow MiscMenu[9];						/* 杂类 WIFI CONTROL	 */

MAIN_GLOBAL uint8_t RefreshFlag;						/* 界面刷新标志			 */
MAIN_GLOBAL uint8_t BeInFlag;
MAIN_GLOBAL uint8_t	EnterFlag;							/* 进入最外层设置菜单标志*/
MAIN_GLOBAL uint8_t	CurrentPage;						/* 当前菜单索引			 */
MAIN_GLOBAL uint8_t ChooseNum;							/* 选项位置选择			 */
MAIN_GLOBAL uint8_t ParaIndex;							/* 参数位置选择			 */
MAIN_GLOBAL uint8_t NumDsp;
//-----------------------------------------------------------------------------------
MAIN_GLOBAL unsigned long TheSysClock;					// 系统时钟变量
static volatile unsigned short *pEpiFpgaPort;			// 总线指针
MAIN_GLOBAL uint8_t 	SysFaultAnalyse;					// 芯片故障分析
MAIN_GLOBAL uint8_t 	PowerUpState;						// 上电状态 1:上电 2:扫描完毕
MAIN_GLOBAL	uint8_t	MicNum;								// 发给PC话筒数量 0.1.2.3.4
MAIN_GLOBAL uint16_t  CurrID;
MAIN_GLOBAL	uint8_t   CmdModeFlag;						// 0:DIG   1:WIFI   
MAIN_GLOBAL	uint8_t   ModeSwitch;                         //PC切换主机
MAIN_GLOBAL	uint8_t   ModeSwDelay;                        //主机切换延时
MAIN_GLOBAL	uint8_t   VersionDis;                         //版本显示计数
MAIN_GLOBAL	uint8_t   VerDelay;                           //版本切换延时
MAIN_GLOBAL	uint8_t	BackupInit;							//备份主机启动相关变量初始化
MAIN_GLOBAL	uint8_t   HostOnline;                         //00 主机不在线 01 主机在线  02主机掉线 
MAIN_GLOBAL	uint8_t   HostFlag;
MAIN_GLOBAL	uint8_t   HostSta;                            //主机状态  00：备份主机状态  01：主机状态
MAIN_GLOBAL	uint8_t   HostTemp;
MAIN_GLOBAL	uint16_t	ScanID;                             //轮询ID号
MAIN_GLOBAL	uint8_t   ScanOnlineId;						//扫描在线ID
MAIN_GLOBAL	uint8_t   HostStart;							//备份主机启动初始化  TRUE：初始化完成  FALSE：正在初始化
MAIN_GLOBAL	uint8_t	HostCheckDelay;                     //主机在线检查
MAIN_GLOBAL	uint8_t   AlarmCount;                         //消防警报计数
MAIN_GLOBAL	uint8_t   StaDelay;
MAIN_GLOBAL	uint16_t	Mic_Buff[12];						//Mic信息更新缓冲
MAIN_GLOBAL	uint8_t	MicUpdata;                          //Mic更新标志位
MAIN_GLOBAL	uint8_t	MicWaitUpdata;						//Mic等待列表更新
MAIN_GLOBAL	uint8_t	KeyLockStatus;						//按键锁定标志
MAIN_GLOBAL uint8_t   IDRepeat;                          //ID冲突标志
MAIN_GLOBAL uint16_t  Repeat_ID;						   //重复ID
MAIN_GLOBAL uint8_t	KeyOpr;  
MAIN_GLOBAL	uint8_t	VoteRtnFlag; 					   //发送结果标志
MAIN_GLOBAL uint8_t	SetIDDelay;	
MAIN_GLOBAL uint16_t  PowerOnDelay;                   //主机上电延时
MAIN_GLOBAL uint8_t   PowerOnFlag;
MAIN_GLOBAL uint8_t   PowerFlag;                      //上电完成标志
//MAIN_GLOBAL uint8_t   WaitUpdata;                     //上电更新等待列表
MAIN_GLOBAL uint8_t   BackupHostOnline;
MAIN_GLOBAL uint8_t	MicStaUpdata;
MAIN_GLOBAL uint8_t   OnlineDelay;
MAIN_GLOBAL uint8_t   CheckHostDelay;
MAIN_GLOBAL uint16_t	InterPara;                     //议员机相关信息处理
MAIN_GLOBAL uint16_t  InterVal;
MAIN_GLOBAL uint8_t   ModeStaDelay;
MAIN_GLOBAL uint8_t   ModeSta;
MAIN_GLOBAL uint8_t   StaCount;
MAIN_GLOBAL uint16_t	Setdestid;  
MAIN_GLOBAL	uint16_t  SignNumber;                         //签到人数
MAIN_GLOBAL	uint16_t  SetIDNumber;                        //下发的ID号
MAIN_GLOBAL	uint16_t  RegValue;							//获取寄存器值
MAIN_GLOBAL	uint8_t   SendResult;                         //下发投票结果
MAIN_GLOBAL	uint8_t   ParaInit;                           //备份主机启动变量初始化
MAIN_GLOBAL	uint8_t	BlinkFlag;                          //闪烁标志位
//MAIN_GLOBAL uint8_t	Mic_Wait_Flag;                      //Mic等待列表更新标志
MAIN_GLOBAL uint16_t	Mic_Wait_Buff[12];					//Mic等待列表发送缓冲
MAIN_GLOBAL uint8_t   Mic_WaitTx_Updata;
MAIN_GLOBAL uint8_t   BackupStart;
MAIN_GLOBAL uint8_t	BidFlag;                          //投票签到话筒权限标志
MAIN_GLOBAL uint8_t	TypeID;
MAIN_GLOBAL uint8_t	InterRoomSet;
MAIN_GLOBAL uint8_t	InterRoomFlag;
MAIN_GLOBAL uint8_t	RangeBit;  							//0:没有光标  1：个位  2：十位  3：百位  4千位
MAIN_GLOBAL char	Idtemp[6];
MAIN_GLOBAL uint8_t	Scanflag;
typedef struct 
{
	uint16_t	tos;		//上限  开风扇
	uint16_t	thyst;	//下限  关风扇
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
//	uint16_t	SwitchZone;	/* bit(4)  1:扬声器分区功能开  0:扬声器分区功能关  bit(3--0)DSP声道   0:分区1  1:分区2  2:分区3  3:分区4 */
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
			unsigned currentID	:15;/* 当前ID */
			unsigned setIDflag	:1;	/* 1:开始编ID   0:结束编ID */
		}wifiID;
	}wifiID;
	union
	{
		uint16_t controlwifi;
		struct
		{
			unsigned debugstart	:1;/* 1:开始调试  0:结束调试*/
			unsigned clearflag	:1;/* 置位清零调试结果 */
			unsigned modeflag	:1;/* 1:省电模式  0:正常模式*/
			unsigned wifisign	:1;/* 1:开始签到  0:结束签到*/
			unsigned wifivote	:1;/* 1:开始表决  0:结束表决  2:查看表决结果*/
			unsigned votemode	:2;/* 00:三键表决 01:5键选举1 2 3 4 5   10:5键评级-- - 0 + ++*/
			unsigned votefirst	:1;/* 1:表决首次有效  0:表决最后一次有效*/
			unsigned votesigned	:1;/* 0:表决无需签到  1:表决需要签到*/
			unsigned updatwifi	:1;/* 1:开始更新KEY和SSID  0:结束更新*/
			unsigned rfflag		:1;/* 1:RF开始配对, 0:RF结束配对*/
			unsigned rebootflag :1;/* 更新KEY和SSID后置一个下降沿使WIFI单元重启*/
			unsigned timedelay	:4;/* 0:10ms 1:15ms 2:18ms 3:20ms 4:25ms 5:30ms  控制延时时间，默认为18ms*/
//			unsigned defaul		:1;
		}wififlag;
	}wififlag;
}WifiInfor_R;
MAIN_GLOBAL	WifiInfor_R WifiInfor;
MAIN_GLOBAL	uint8_t RFDelayFlag;/* 0:3.1s低电平延时  1:15.1s高电平延时*/
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
	uint8_t SpkNum;				// 最大发言人数	
	uint8_t MicMode;				// 0:FIFO 	1:Normal	2:Apply
	uint8_t MicWaitNum;			// 申请开话筒人数
	uint8_t MicOnMasterNum;		// 正在发言主席机人数,控制查询发言列表
	uint8_t MicOnSlaveNum;		// 正在发言代表机人数	
	uint8_t MicOnInterNum;		// 正在发言译员机人数
	uint8_t MicChannelNum;		// MIC通道为6（WIFI限制），MIC通道为8(原DIG限制)
	uint8_t MicFreeOnNum;		// FREE_MIC_ON_NUM

	uint8_t MeetSta;				// 会议状态
	uint8_t VoteType;				// 选举类型
	uint16_t PollId;        		// 当前轮询的id
	uint16_t CurrSetId; 			// 当前SET ID 
	uint16_t SetStartIdNum;		// 设置开始ID
	
	uint8_t LineInVol;			// 线路输入音量
	uint8_t IR_ChanVol;			// 红外通道音量
	uint8_t BassVol;				// 低音音量
	uint8_t TrebleVol;			// 高音音量
	uint8_t LinOutVol;			// 线路输出音量
	uint8_t UintOutVol;			// 单元输出音量
	uint8_t	LinkOutVol;
	uint8_t	Band1;					//EQ设置七个频段1---7
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

	uint8_t InterChanTotalNum;	// 翻译通道总数
	uint8_t CurrentLanNum;		// 当前显示的语言号
	uint8_t CurrentLanVal;		// 当前显示的语言值
	uint8_t InterRoomNum;			// 翻译间数		

	uint8_t Languge;
	uint8_t BackUpFlag;			// 热备份开关标志
	uint8_t Sound_AFNGC1;
	uint8_t Sound_AFNGC2;

	uint8_t IPAddValue[4];		// IP地址
	uint8_t SubMaskValue[4];		// 子网掩码
	uint8_t GatewayValue[4];		// 默认网关
	
	uint8_t ChannelLan[32];     	// 每个通道对应的语种信息0-178..原音不变

	uint8_t DefaultValue;			// 默认

	
} MainInfoStruct;
MAIN_GLOBAL	MainInfoStruct SysInfo;

MAIN_GLOBAL union
{
	uint16_t	RegTrackBackVal;
	struct
	{
		unsigned BackID		:12;	//摄像跟踪过程中，当一个话筒关闭时，发送前一个预置位的ID
		unsigned TrackSw	:1;		//WIFI单元红外跟踪LED开关使能
		unsigned Nc			:3;		//预留
	}RegTrackBack;
}RegTrackBack;

MAIN_GLOBAL union
{
	uint16_t RegOther2Val;

	struct
	{		
		unsigned HostMode	:4;// 热备份 0101:主机	1010:从机
		unsigned Default	:1;// 
		unsigned HostOnline	:1;// 有台主机在线
		unsigned AutoZero	:1;// 自动0通道 0: 关闭 1: 打开
		unsigned Speaker 	:1;// 扬声器 1: 打开  0:关闭 没用
		unsigned Language   :2;// 菜单显示语言0: 中文 1: 英文 2: 繁体
		unsigned ChangeHost	:2;// 10时表示要求切换成主机，01表示要求切换成从机
		unsigned SyaTick	:4;// 主从机节拍
	}RegOther2;
}RegOther2;

MAIN_GLOBAL union
{
	uint16_t REG_IRLightVal;
	struct
	{		
		unsigned IRLight	:4;// 红外灯亮度调节
		unsigned other		:12;//没用
	}RegIR;
}RegIR;
/*
B15：		0。扬声器打开。1。扬声器关闭。
B14-B13：	00。中文语言。01。英文。10。繁体。
B12-B11：	提示音。01：定时表决倒计时。10：定时发言倒计时。
B10-B8：	最大话筒数量

B7-B3		
B2-B0		正在发言人数

*/
MAIN_GLOBAL union
{
	uint16_t REG_Default1Val;
	struct
	{
		unsigned MicOnNum	:3;
		unsigned PC_Online	:5;		// PC在線
		unsigned MaxSpkNum	:3;
		unsigned Default 	:2;		// 提示音
		unsigned Language 	:2;		
		unsigned Speaker	:1;		// 0。扬声器打开。1。扬声器关闭。
	}U_REG_Default1;
	
}U_REG_Default1;

// 主席机发起表决标志
MAIN_GLOBAL union
{
	uint16_t	REG_MyVoteStartVal;
	struct
	{
		unsigned Id			:12;	// ID号
		unsigned Default 	:2;		// 没用
		unsigned LookRes	:1;		// 查看表决结果		
		unsigned Ask		:1;		// 表决过了		
	}U_REG_MyVoteStart;
	
}U_REG_MyVoteStart;

//-----------------------------------------------------------------------
// 主机发送给代表机的信息帧
typedef struct 
{
	uint16_t FpgaDest;			//目标地址
	//ushort Byte;				//发送的字节数
	uint16_t Type_AddrL;			//数据类型和目标地址低8位
	uint16_t Para_CmdL;			//命令高4位&地址高4位,命令低8位
	//ushort CRC;				//校验码0xFCFC
}EpiTxFrameStruct;
MAIN_GLOBAL EpiTxFrameStruct EpiTxFrameBuff[TX_Frame_MAX_SIZE+1];
MAIN_GLOBAL	uint8_t TxWriteCount;
MAIN_GLOBAL	uint8_t TxReadCount;	
MAIN_GLOBAL uint8_t EpiTxFrameCount;		//控制速度,发送不过来.//主机要发送的帧数,普通使命最多同时发送16帧(4*16=64),短信息最多120个字节.
// 根据在线ID数确定最大扫描一帧。当在线ID数大于等于35时就扫描35帧。
MAIN_GLOBAL uint8_t EpiTxFrameMax_Scan;		// 代表机最大接收15帧，但每次只能上发一帧。所以只能接收一帧数据处理
MAIN_GLOBAL	EpiTxFrameStruct WifiTxBuff[RX_Frame_MAX_SIZE];
MAIN_GLOBAL	uint8_t WifiTxWrite;
MAIN_GLOBAL	uint8_t WifiTxRead;
MAIN_GLOBAL	uint16_t WifiPollID;
//-----------------------------------------------------------------------
// 主机接收代表机信息帧
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
	//uint8_t	Camera_Num;		//选择球机NUmber
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

//MAIN_GLOBAL unsigned short EPI_RX_Buff[EPI_RX_BUFF_SIZE];	//接收缓存

//-----------------------------------------------------------------------
// 主机发送信息缓冲区及计数器
MAIN_GLOBAL	uint16_t  SMS_Tx_Buff[SMS_TX_BUF_SIZE];	
MAIN_GLOBAL uint8_t 	SMS_TxSizeCount;				// 本次短信息多少字节数
MAIN_GLOBAL uint8_t	SMS_Tx_State;					// 0:没有数据发送 1:有短信息发送 2:短信发送完 3:
MAIN_GLOBAL unsigned char * SMS_WR_Point;
MAIN_GLOBAL unsigned char * SMS_RE_Point;
MAIN_GLOBAL unsigned char * AddrPoint;
MAIN_GLOBAL unsigned char * FramePoint;
MAIN_GLOBAL unsigned char FrameCount;

//MAIN_GLOBAL uint8_t 	SMS_Tx_DelayCount;				// 短信数据如果有二十个字节以上，发送后要等待10ms往总线上送数据，要不然覆盖。
//-----------------------------------------------------------------------
// 单元上传IC卡里数据
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
// 会议模式下相关变量定义
MAIN_GLOBAL	uint16_t 	FreeMicOnList[FREE_MIC_ON_NUM];				// 自由开启模式下，存放MIC的ID列表
MAIN_GLOBAL	uint16_t 	SlaverMicOnList[MAX_MIC_ON_NUM];			// 正在发言列表,代表机
MAIN_GLOBAL	uint16_t 	MasterMicOnList[MAX_MIC_ON_NUM];			// 正在发言列表,主席机
MAIN_GLOBAL	uint16_t 	InterMicOnList[INTER_MAX_SPK_NUM];			// 正在发言列表,译员机.查询当前ID是不是在发言人列表里
MAIN_GLOBAL	uint16_t 	WaitMicList[MAX_MIC_WAIT_NUM];				// 发言等待列表
MAIN_GLOBAL	uint16_t 	UnitMicNumList[MAX_MIC_ON_NUM];				// 正在打开的话筒号码。(0-7),存的是ID号
MAIN_GLOBAL uint8_t  	InterMicOutList[INTER_MAX_SPK_NUM];  		// 译员机输出通道号(1-15)
MAIN_GLOBAL	uint16_t  OnliSlaverId[MAX_DELEG_NUM]; 				// 在线的代表机的ID
MAIN_GLOBAL	uint16_t  OnlineNum;     								// 所有在线单元
MAIN_GLOBAL	uint16_t  OnlineChairNum;     						// 在线的主席机数
MAIN_GLOBAL	uint16_t  OnlineSlaveNum;     						// 在线的代表机数
MAIN_GLOBAL	uint8_t   OnlineWifiNum;
MAIN_GLOBAL	uint16_t	OnlineInterNum;								// 在线的译员机
MAIN_GLOBAL uint16_t	Mic_Wait_Buff[12];				          	//Mic等待列表发送缓冲

MAIN_GLOBAL uint16_t	PauseID;						
MAIN_GLOBAL uint8_t	CheckRepeatIdFlag;	
MAIN_GLOBAL uint8_t   PauseOnList;						
//MAIN_GLOBAL	uint16_t  OnliSlaverIdBuff[MAX_DELEG_NUM]; 			// 在线的代表机的ID,按ID号存在相应位置

//MAIN_GLOBAL	uint8_t	UnitType[MAX_DELEG_NUM];                   //机器类型码扫描缓存

								
MAIN_GLOBAL	uint16_t   DelegInfoTable[MAX_DELEG_NUM];				// 代表机状态表
//   D15 D14        D13~D8      D7    D6 D5 D4   D3     D2       D1      D0 
//  机器组合个数    机器类型码    NC     Vote      NC   Online   Dis/En    M/S  

#define			CALL_CHAIR_ON_NUM		1
MAIN_GLOBAL	uint16_t 	CallChairOnList[CALL_CHAIR_ON_NUM];			// 申请与主席机内部通话列表
MAIN_GLOBAL	uint8_t  OnlineCallChairNum;     						// 在线申请与主席机内部通话数

// 扫描下相关变量
MAIN_GLOBAL uint8_t 	StartScanOnID;							// 扫描在线单元标志 0: 没有扫描 1: 进入扫描 2: 开始扫描
MAIN_GLOBAL uint16_t 	ScanOnIDCount;							
MAIN_GLOBAL	uint8_t   PollTargetFlag;			  					// 轮询目标标志位 0：代表机 1：扩展主机
MAIN_GLOBAL	uint8_t   ChkOnlineIdFlag;		    				// 查询在线代表机标志位
MAIN_GLOBAL	uint8_t  	CheckOnliIdCnt;								// 查询在线单元循环次数
MAIN_GLOBAL	uint8_t	ScanEnd;                                   //扫描结束
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


//延时20ms后再处理代表机上发的数据
typedef struct
{
	uint8_t State;		// 0:没数据处理 1.开始延时 2:收集扫描下数据 3:可以处理数据 4:收集数据完毕
	uint8_t DelayCount;	// 每计一下10ms
}ScanDataStruct;
MAIN_GLOBAL ScanDataStruct ScanData;


// MIC操作信息帧定义
typedef struct  
{ 
    uint8_t OprMod;              	 	// MIC操作方式(开/关)
    uint16_t  Oprid;                    // 需要MIC操作的ID
} MicOprFrameStruct;

MAIN_GLOBAL	MicOprFrameStruct   MicOprInforBuff[MIC_INFOR_BUF_SIZE];
MAIN_GLOBAL	uint8_t	 	MicWrPnt;								// MIC信息写指针
MAIN_GLOBAL	uint8_t	 	MicRdPnt;								// MIC信息读指针
//MAIN_GLOBAL	uint8_t		MicInforOkFlag;	

// 表决
MAIN_GLOBAL	uint8_t	VoteLookres;
MAIN_GLOBAL	uint8_t	VoteFuncFlag;								// 表决功能标志
MAIN_GLOBAL	uint8_t	gbit_start_vote;							// 表决模式启动标志
MAIN_GLOBAL	uint8_t 	VoteResTypeFlag;      						// 主、客观投票标志位
MAIN_GLOBAL	uint8_t	VoteSignedFlag;      						// 需要签到完才能投票标志位
MAIN_GLOBAL	uint8_t	VoteResRtnOk;								// 表决结果返回OK标志
MAIN_GLOBAL	uint8_t	QuitVoteFlag;								// 退出表决标志
MAIN_GLOBAL	uint8_t   VoteResRtnStart;      						// 发送结果标志位
MAIN_GLOBAL	uint8_t   VoteResRtnEnable;  							// 
MAIN_GLOBAL	uint16_t  VoteResRtnCnt;				      			// 发送结果记时变量
MAIN_GLOBAL	uint8_t 	VoteMode;
MAIN_GLOBAL	uint16_t  VoteResult[5];								// 10 台扩展主机 + 1台主机 的表决结果
MAIN_GLOBAL	uint16_t  VoteResultFlag;								// 扩展主机返回表决各项总结果标志

MAIN_GLOBAL	uint16_t	VoteTimer;									// 定时发表决时间
MAIN_GLOBAL uint8_t	VoteTimeState;								// 定时表决状态 0: 关闭定时表决。1：使能定时表决。2: 开始定时表决。	
MAIN_GLOBAL uint8_t 	Vote_10ms;									// 10ms计数，用来刷新显示时间

// 签到
MAIN_GLOBAL	uint8_t   SignFuncFlag;								// 签到功能标志变量
MAIN_GLOBAL	uint8_t   SignStartFlag;								// 签到开始标志
MAIN_GLOBAL	uint16_t  SignedNum;									// 已签到人数
MAIN_GLOBAL	uint16_t  SignMaxNum;									// 最大签到人数,10台扩展主机 + 1台主机 * 125
//MAIN_GLOBAL	uchar	  SignFullFlag;							// 签到满标志
MAIN_GLOBAL uint16_t  MasterSigneId;								// 主席机发起签到的ID号

MAIN_GLOBAL	uint16_t  SignVoteIdBuf[VOTE_SENT_BUF_SIZE];  		// 已经签到/表决的ID(发送给PC机的缓冲区)
MAIN_GLOBAL	uint16_t   VoteResultBuf[VOTE_SENT_BUF_SIZE];			// 已经表决的ID的结果
// 每台ID已发送签到或表决结果标志位。每个变量8个位，代表连续的8个ID标志
MAIN_GLOBAL	uint16_t  SignVoteSendList[SYS_MAX_DELEG_NUM/8+1]; // 已经发送签到结果列表,已经发送签到结果列表
MAIN_GLOBAL	uint8_t   VoteWrPnt;									// 发送给PC的写指针
MAIN_GLOBAL	uint8_t   VoteRdPnt;									// 发送给PC的读指针
MAIN_GLOBAL	uint8_t	VoteSendPcFlag;								// 发送数据给PC标志
MAIN_GLOBAL	uint8_t	StopScanFlag;								// 发送缓冲区满后，禁止扫描
MAIN_GLOBAL unsigned char DonwLoader;
MAIN_GLOBAL	uint8_t	SignExitTimeStart;							// 签到时候退出，但还有数据没发完不能马上进入会议
//------------------------------------------------
//TCP/IP数据
MAIN_GLOBAL uint16_t 	NetRxBuff[NET_RX_BUFF_SIZE];				// 
MAIN_GLOBAL uint16_t 	*NetRxPoint;									// 接收帧头
MAIN_GLOBAL uint16_t 	*NetRxProcess;									// 接收处理进程
MAIN_GLOBAL uint16_t 	*NetRxTail;										// 接收指令处理进程
MAIN_GLOBAL uint8_t 	NetRxRelay;										// 在这时间内处理完,不然就会丢掉部分数据
MAIN_GLOBAL unsigned char	*NetRxPoint1;
MAIN_GLOBAL unsigned char 	*NetRxProcess1;
MAIN_GLOBAL	unsigned char tempNUM;
MAIN_GLOBAL unsigned char 	*Ptobuff;
MAIN_GLOBAL unsigned char RxTime;
MAIN_GLOBAL unsigned char RxTimeDelay;
MAIN_GLOBAL unsigned char PCLoad;
MAIN_GLOBAL unsigned char PCLoadDelay;
MAIN_GLOBAL uint16_t 	NetRxBuff_Size;								//此次数据大小,增加PC格式判断,否则更新图片会出现图片数据也有0xFCFC的情况
MAIN_GLOBAL uint16_t 	NetRxBuff_Type;								//此次数据类型
MAIN_GLOBAL uint16_t 	NetRxBuff_Type_DWIN;					//此次数据为DWIN数据
MAIN_GLOBAL uint16_t 	NetRxBuff_Size_i;							//此次数据接收次数

// 主机和PC连接标志
MAIN_GLOBAL	uint8_t	bConnectedPcFlag;							// 标志位备份
MAIN_GLOBAL	uint8_t   ConnectedPcFlag;							// 启动在线检测PC
MAIN_GLOBAL uint16_t	PcConnectCnt;								// 连接计数器
MAIN_GLOBAL	uint16_t	PcConnectCntBuf;							// 连接计数器缓冲区
MAIN_GLOBAL uint8_t  	DisconnectPc;

//按键信息相关变量
MAIN_GLOBAL	uint8_t   ChairKeyMaskSts;			// 主席机是否按下了代表机话筒的屏蔽键 0：没有，1：有
MAIN_GLOBAL	uint8_t	SetIdKeyPressed;
MAIN_GLOBAL	uint8_t   QuitDelay;                  //退出编ID延时
MAIN_GLOBAL	uint8_t   QuitFlag;                   //退出编ID标志
MAIN_GLOBAL	uint8_t  	SetIdFlag;
MAIN_GLOBAL uint8_t 	SetIdKeyCount;				//长按计数
MAIN_GLOBAL	uint8_t	KeyOprOkFlag;
//MAIN_GLOBAL	uint8_t   LcdReturnFlag;
//MAIN_GLOBAL	uint16_t  LcdReturnDelay;
MAIN_GLOBAL	uint16_t	KeyValue;
MAIN_GLOBAL	uint8_t	KeyIndex;
MAIN_GLOBAL	uint8_t	sKeyIndex;
MAIN_GLOBAL	uint8_t	KeyTypeFlag;				// 按键类型，0x00：短按，0xCC：长按		
MAIN_GLOBAL	uint8_t	KeyPushPopFlg;				// 按键按下/弹起标志 1：按下，0：弹起
MAIN_GLOBAL	uint8_t 	KeyDelay10Ms;
//MAIN_GLOBAL	uint8_t	KeyLockStatus;				// 按键锁定状态
// 制作字库
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

MAIN_GLOBAL uint8_t 	NetSetBuff[4];				// 设置网络信息临时用
MAIN_GLOBAL uint8_t 	Buff1K[1024];				//
MAIN_GLOBAL uint8_t 	NetSetBuff[4];				// 设置网络信息临时用
MAIN_GLOBAL	uint8_t 	MACAddrBuff[6];
#define InterMicBuffSize		5
// 建立译员机开话筒列表，防止上发来的房间和输出通道号不一起
typedef struct
{
	uint16_t OpenId;				// 译员机申请开话筒的ID
	uint8_t Room;					// 译员机所在的房间
}InterMicStruct;

MAIN_GLOBAL uint8_t CurrentCount;		// 
MAIN_GLOBAL InterMicStruct InterMicBuff[InterMicBuffSize];// 暂存翻译间号
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

MAIN_GLOBAL uint8_t	SysTickCount;			// 系统节拍，做与主从机通讯用。
MAIN_GLOBAL uint8_t	SysTickValue;			// 主机更新码
MAIN_GLOBAL uint8_t	ChangToHostCount;		// 切换成主机模式计数.当为5S时就自动切换成主机

#define		ByPass_3108_Bit				0x0001
#define		ByPass_AFC_Bit				0x0002
#define		ByPass_ANC_Bit				0x0004
#define		ByPass_AGC_Bit				0x0008
#define		ByPass2_AFC_Bit				0x0040
#define		ByPass2_ANC_Bit				0x0080
#define		ByPass2_AGC_Bit				0x0100


MAIN_GLOBAL uint16_t	ByPassValue;			// Bit0=1 启用3108。Bit1=1 启用音效
MAIN_GLOBAL uint16_t	Startup_3108;     //用于上电时判断断电前是否开启音效的标志
// 摄像跟踪
//MAIN_GLOBAL	uint8_t   SBUF2;
MAIN_GLOBAL	uint8_t   cmd_control[15];
MAIN_GLOBAL	uint8_t		Sony_Zoom[9];
//MAIN_GLOBAL	uchar   cmd_control[11] = {0xa0,0x10,0x10,0x00,0x07,0x00,0xff,0xff,0xff,0xaf,0xdb};
//MAIN_GLOBAL	uchar code camera_code[11] 	= {0xa0,0x10,0x10,0x00,0x07,0x00,0xff,0xff,0xff,0xaf,0xdb};
//MAIN_GLOBAL	uchar code result_code[11] 	= {0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0xff,0xff,0xff,0xff,0xff};

// 串口2变量
//MAIN_GLOBAL	uint8_t  	uart2_index;
//MAIN_GLOBAL	uint8_t  	uart2_cnt;
//MAIN_GLOBAL	uint8_t   SoftUartBusy;

// 上一个申请发言的id
MAIN_GLOBAL	uint16_t  CameraList[CEMARE_SERIES];
MAIN_GLOBAL	uint8_t 	CameraPnt;
MAIN_GLOBAL	uint8_t   ChairPosition;
MAIN_GLOBAL	uint8_t   debugstart_Count;		//控制发送短消息给WIFI6302K

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
