#ifndef __DEFINE_H__
#define __DEFINE_H__

//#define INIT_DEBUG

//-------------------------------------------------------------------------------
///////////////////////////系统参数设置
//系统时钟
#define		FOSC_44_4M						///200M/4.5=44.444M
//#define		FOSC_57_1M						///200M/3.5=57.142M
//#define		FOSC_80_0M						///200M/2.5=80.000M


#define		LAN_CN					0		// 菜单语言是 0:中文 1:英文 2:繁体
#define		LAN_EN					1
#define		LAN_FN					2


//#define		LANGUAGE				LAN_CN
#define		LANGUAGE				LAN_EN
//#define		LANGUAGE				LAN_FT

// 打印信息，打开看门狗
#define		DEBUG_DOG			0

#define     BackupHostID        0xFA1		//备份主机ID
//网络设置
//#define		NetHardWareRxBuf	8,8,8,8,8,8,8,8		
//#define		NetHardWareTxBuf	8,8,8,8,8,8,8,8	
//#define		NetIPVal			192,168,10,100
//#define		NetSubnetVal		255,255,255,0
//#define		NetGateWayVal		192,168,10,1
//#define		NetMacVal			0x00,0x08,0xDC,0x01,0x11,0x33

//串口波特率
//#define 	Uart0BaudRate   	9600		//9600//串口0的波特率,调试用
//#define 	Uart1BaudRate   	9600		//115200//串口1的波特率,控制摄像头专用

//-------------------------------------------------------------------------------
///////////////////////////外部设备地址定义

//#define 	LcdDispPort			(*(volatile unsigned short *)0xa00003FE) ///0x3FE中W5300没用
//#define 	FPGA_PORT 			(*(volatile unsigned short*)0x60000002)//与实物地址要偏移一位,A0没用到.不能用奇数
//#define 	FPGA_PORT_HOST   	(volatile unsigned short *)0x60000000
#define		CHECK_MIC			0x102		// 通知FPGA测试MIC输入状态地址		
#define		CHECK_COMM			0x103		// 通知FPGA通信分析地址

#define		WIFIDEBUG			0xFFFF		//controlwifi屏蔽位
//A10 为1选择前面板FPGA
//A9 Key读写	低电平有效
//A8 LED读写	低电平有效
//A7 LCD命令选择
//A6 LCD片选 低电平有效


#define		AGCKey			0x80
#define		ANCKey			0x40
#define		AFCKey			0x20

#define		Write			0x00
#define		Read			0x01
/*主菜单索引*/
#define		mode			0x00
#define		active			0x01
#define		volume			0x02
#define		eq				0x03
#define		zone			0x04
#define		manage			0x05
#define		net				0x06
#define		trans			0x07
#define		camera			0x08
#define		lang			0x09
#define		misc			0x0A
#define		defaultpara		0x0B
#define		version			0x0C
/*左右键修改类型*/
#define		NoneType		0x00
#define		ModeType		0x01
#define		ActiveType		0x02
#define		LineinType		0x03
#define		LineoutType		0x04
#define		UnitoutType		0x05
#define		TrebleType		0x06
#define		BassType		0x07
#define		SpeakerType		0x08
#define		EqEffectType	0x09
#define		Band1Type		0x0A
#define		Band2Type		0x0B
#define		Band3Type		0x0C
#define		Band4Type		0x0D
#define		Band5Type		0x0E
#define		Band6Type		0x0F
#define		Band7Type		0x10
#define		Zone1IDType		0x11
#define		Zone2IDType		0x12
#define		Zone3IDType		0x13
#define		Zone4IDType		0x14
#define		Zone1GainType	0x15
#define		Zone2GainType	0x16
#define		Zone3GainType	0x17
#define		Zone4GainType	0x18
#define		ZoneSWType		0x19
#define		ScanType		0x1A
#define		IpType			0x1B
#define		MacType			0x1C
#define		ChannelsumType	0x1D
#define		ChannellangType	0x1E
#define		ChannelautoType	0x1F
#define		ChannelLockType	0x20
#define		RoomSumType		0x21
#define		RoomSetType		0x22
#define		camProtlType	0x23
#define		camsetType		0x24
#define		cammapnumType	0x25		/*球机序号*/
#define		cammapaddrType	0x26
#define		cammaparreyType	0x27
#define		langType		0x28
#define		wifiirType		0x29
#define		defaultType		0x2A
#define		remoteType		0x2B	
/*ChooseNum所对应的参数位置*/
/*---------volume----------*/
#define		linein			0x00
#define		lineout			0x01
#define		unit			0x02
#define		treble			0x03
#define		bass			0x04
#define		remotein		0x05
#define		speaker			0x06
/*-----------EQ----------*/
#define		effet			0x00
#define		custom			0x01
#define		band1			0x00
#define		band2			0x01
#define		band3			0x02
#define		band4			0x03
#define		band5			0x04
#define		band6			0x05
#define		band7			0x06
/*-----------ZONE---------*/
#define		speaker1		0x00
#define		speaker2		0x01
#define		speaker3		0x02
#define		speaker4		0x03
#define		zonesw			0x04
#define		remoteout		0x05
#define		endid			0x00
#define		gain01			0x01
#define		gain02			0x02
#define		gain03			0x03
#define		gain04			0x04
#define		zoonswth		0x00
#define		unitanc			0x01
#define		zoondsp			0x02
#define		zoondsp2		0x03
/*---------NETWORK--------*/
#define		aip				0x00
#define		amac			0x01
#define		ip1				0x00
#define		ip2				0x01
#define		ip3				0x02
#define		ip4				0x03
#define		ipok			0x04
#define		mac1			0x00
#define		mac2			0x01
#define		mac3			0x02
#define		mac4			0x03
#define		mac5			0x04
#define		mac6			0x05
#define		macok			0x06
/*------------TRANS-----------*/
#define		chnlsum			0x00
#define		chnllang		0x01
#define		chnlauto		0x02
#define		chnllock		0x03
#define		chnlroom		0x04
#define		roomsum			0x00
#define		roomset			0x01
/*-----------CAMERA-----------*/
#define		camProtocol		0x00
#define		cammap			0x01
#define		camset			0x02
#define		camnum			0x00
#define		camaddr			0x01
#define		cammatrix		0x02
/*------------MISC------------*/
#define		dsp				0x00
#define		ir				0x01
#define		delaytime		0x02
#define		savepower		0x03
#define		compat			0x04
#define		autho			0x05
#define		alc				0x06
#define		light			0x07
#define		scan			0x08
//-------------------------------------------------------------------------------
///////////////////////////FPGA本地寄存器定义
#define			REG_MAP_BASE			0x60000000	

//读写寄存器


//ByPass 标志
#define			REG_ByPass				0x380//	(REG_MAP_BASE+(0x380<<1))
//language1+language2
#define			REG_Lan1Lan2			0x381//	(REG_MAP_BASE+(0x381<<1))
//language3+language4
#define			REG_Lan3Lan4			0x382//	(REG_MAP_BASE+(0x382<<1))
//language5+language6
#define			REG_Lan5Lan6			0x383//	(REG_MAP_BASE+(0x383<<1))
//language7+language8
#define			REG_Lan7Lan8			0x384//	(REG_MAP_BASE+(0x384<<1))
//language9+language10
#define			REG_Lan9Lan10			0x385//	(REG_MAP_BASE+(0x385<<1))
//language11+language12
#define			REG_Lan11Lan12			0x386//(REG_MAP_BASE+(0x386<<1))
//language13+language14
#define			REG_Lan13Lan14			0x387//(REG_MAP_BASE+(0x387<<1))
//language15+MCU自定义8位
#define			REG_Lan15Default		0x388//(REG_MAP_BASE+(0x388<<1))
//实时下发已签到人数
#define			REG_SignNum				0x389//	(REG_MAP_BASE+(0x389<<1))
//voteNumber表决人数
#define			REG_VotePeople			0x38a//(REG_MAP_BASE+(0x38a<<1))
//voteResultAA，++表决结果
#define			REG_VoteRes1			0x38b//	(REG_MAP_BASE+(0x38b<<1))
//voteResultA，+表决结果
#define			REG_VoteRes2			0x38c//	(REG_MAP_BASE+(0x38c<<1))
//voteResult0，0表决结果
#define			REG_VoteRes3			0x38d//	(REG_MAP_BASE+(0x38d<<1))
//voteResultS，-表决结果
#define			REG_VoteRes4			0x38e//	(REG_MAP_BASE+(0x38e<<1))
//voteResultSS，--表决结果
#define			REG_VoteRes5			0x38f//	(REG_MAP_BASE+(0x38f<<1))
//switchLan，0x190中英繁,扬声器开关，自动0通道开关，代表机上电LED亮2s开关；
#define			REG_Other2				0x390//(REG_MAP_BASE+(0x390<<1))

//用于上电扫描完标志
#define			REG_Timer				0x391//(REG_MAP_BASE+(0x391<<1))
//实时下发总签到总人数
#define			REG_SignMaxNum			0x392//(REG_MAP_BASE+(0x392<<1))
//译员机通道开关
#define			REG_Interpreter			0x393//(REG_MAP_BASE+(0x393<<1))
/*
B15：		0。扬声器打开。1。扬声器关闭。
B14-B13：	00。中文语言。01。英文。10。繁体。
B12-B11：	提示音。01：定时表决倒计时。10：定时发言倒计时。
B10-B8：	最大话筒数量

B7:        	PC在线
B6:			打开茶水服务
B5:			IC卡上传服务
B4:			显示职位
B3：        显示姓名
//B7-B3		11111: PC在线。
B2-B0		正在发言人数

*/
#define			REG_Default1			0x394//(REG_MAP_BASE+(0x394<<1))
//MCU自定义16位 定时表决时间
#define			REG_Default2			0x395//(REG_MAP_BASE+(0x395<<1))

// 主席机本机发起签到
#define			REG_MySignStart			0x396//(REG_MAP_BASE+(0x396<<1))

// 主席机本机发起表决
/*
D15：		1。已表决。
D14：		1。查看表决结果。
D13	-D12：		
D11-D0：	发起表决的ID号。

*/
#define			REG_MyVoteStart			0x397//(REG_MAP_BASE+(0x397<<1))

//MCU自定义寄存器 定义代表机的相关状态
/*
            
      是否需要IC卡标志
      投票禁止标志
      签到禁止标志
B15  1：锁定译员通道号  0：解锁译员通道号
//B14    1:扬声器默认开	 0：扬声器关
B13    1：投票签到模式禁止开话筒  0：允许开话筒
B12    发送结果标志
B11-B9  最大发言人数   000：1个    111:8人
B8-B7   0x:00    FIFO		// 先进先出模式		
        0x01    NORMAL		// 固定模式					
		0x02    FREE		// 自由模式			
		0x03    APPLY		// 申请模式			

B6-B3   下发的ID号
B2-B0   模式 ：01 会议模式  02 投票模式  03 签到模式   04 设置ID模式

*/
#define			REG_Default3			0x398//(REG_MAP_BASE+(0x398<<1))



#define         REG_Default4			0x399//(REG_MAP_BASE+(0x399<<1))
//bit0 ---bit1	 00不移动 01左移 02右移  bit4 0:5201 1:5201A
#define         REG_Default5			0x39A//(REG_MAP_BASE+(0x39A<<1))

#define         REG_Default6			0x39B//(REG_MAP_BASE+(0x39B<<1))

#define					REG_Lan16Lan17		0x39C//(REG_MAP_BASE+(0x39C<<1))

#define					REG_Lan18Lan19		0x39D//(REG_MAP_BASE+(0x39D<<1))

#define					REG_Lan20Lan21		0x39E//(REG_MAP_BASE+(0x39E<<1))

#define					REG_Lan22Lan23		0x39F//(REG_MAP_BASE+(0x39F<<1))

#define					REG_Lan24Lan25		0x3A0//(REG_MAP_BASE+(0x3A0<<1))

#define					REG_Lan26Lan27		0x3A1//(REG_MAP_BASE+(0x3A1<<1))

#define					REG_Lan28Lan29		0x3A2//(REG_MAP_BASE+(0x3A2<<1))

#define					REG_Lan30Lan31		0x3A3//(REG_MAP_BASE+(0x3A3<<1))

#define					REG_Wait0ID			0x3D0//APPLY模式申请列表
#define					REG_Wait1ID			0x3D1
#define					REG_Wait2ID			0x3D2
#define					REG_Wait3ID			0x3D3
#define					REG_Wait4ID			0x3D4
#define					REG_Wait5ID			0x3D5
#define					REG_Channel0ID		0x3B0//32通道ID寄存器，当开关话筒时，把当前ID的通道号给FPGA
#define					REG_Channel1ID		0x3B1//话筒一共8个通道
#define					REG_Channel2ID		0x3B2
#define					REG_Channel3ID		0x3B3
#define					REG_Channel4ID		0x3B4
#define					REG_Channel5ID		0x3B5
#define					REG_Channel6ID		0x3B6
#define					REG_Channel7ID		0x3B7
#define					REG_IR_Track		0x3B8

#define					REG_DebugResult1	0x3B9//wifi调试结果寄存器，1号话筒和0号话筒（只读寄存器）
#define					REG_DebugResult2	0x3BA//wifi调试结果寄存器，3号话筒和2号话筒（只读寄存器）
#define					REG_DebugResult3	0x3BB//wifi调试结果寄存器，5号话筒和4号话筒（只读寄存器）
#define					REG_WifiSetID		0x3BC//bit15: 1/0:开始编ID/结束编ID     bit 14-0:编ID过程中当前下发ID号
#define					REG_WifiControl		0x3BD//wifi控制寄存器
#define					REG_WifiDelay		0x3BE//wifi控制延时时间

#define					REG_Part1Gain12		0x3C0//分区1     扬声器1和2增益
#define					REG_Part1Gain34		0x3C1//分区1     扬声器3和4增益
#define					REG_Part1End		0x3C2//分区1     结束ID号
#define					REG_Part2Gain12		0x3C3//分区2     扬声器1和2增益
#define					REG_Part2Gain34		0x3C4//分区2     扬声器3和4增益
#define					REG_Part2End		0x3C5//分区2     结束ID号
#define					REG_Part3Gain12		0x3C6//分区3     扬声器1和2增益
#define					REG_Part3Gain34		0x3C7//分区3     扬声器3和4增益
#define					REG_Part3End		0x3C8//分区3     结束ID号
#define					REG_Part4Gain12		0x3C9//分区4     扬声器1和2增益
#define					REG_Part4Gain34		0x3CA//分区4     扬声器3和4增益
#define					REG_Part4End		0x3CB//分区4     结束ID号
#define					REG_SwitchZone		0x3CC//	/* bit(4)  1:扬声器分区功能开  0:扬声器分区功能关  bit(3--0)   0:分区1  1:分区2  2:分区3  3:分区4 */
#define					REG_Volume			0x3CD//线出音量	单元音量 低8位      扬声器音量  高8位
#define					REG_LineIn			0x3CE//线入音量	使用低8位
#define					REG_IRLight			0x3CF//红外光亮度调节
//---45个寄存器用于WIFI签到和表决结果列表
#define					REG_StateWifi		0x3D6
/*  | 						   |			   |   共计45个
	|						   |			   |
#define					REG_StateWifi		0x402*/

///////////////////////////变量类型替换

//对于8位MCU来说
//uchar 	1个字节
//ushort	2个字节
//uint		2个字节
//ulong		4个字节

//对于32位MCU来说
//uchar 	1个字节
//ushort	2个字节
//uint		4个字节
//ulong		4个字节
typedef char int8;                        /**< The 8-bit signed data type. */
typedef volatile char vint8;              /**< The volatile 8-bit signed data type. */
typedef unsigned char uint8;              /**< The 8-bit unsigned data type. */
typedef volatile unsigned char vuint8;    /**< The volatile 8-bit unsigned data type. */
typedef short int16;                      /**< The 16-bit signed data type. */
typedef volatile short vint16;            /**< The volatile 16-bit signed data type. */
typedef unsigned short uint16;            /**< The 16-bit unsigned data type. */
typedef volatile unsigned short vuint16;  /**< The volatile 16-bit unsigned data type. */                                           
typedef long int32;                       /**< The 32-bit signed data type. */                                           
typedef volatile long vint32;             /**< The volatile 32-bit signed data type. */                                          
typedef unsigned long uint32;             /**< The 32-bit unsigned data type. */                                          
typedef volatile unsigned long vuint32;   /**< The volatile 32-bit unsigned data type. */


typedef uint8 SOCKET;					// 驱动W5300专用

#ifndef 	FALSE
#define 	FALSE   			0
#endif
#ifndef 	TRUE
#define 	TRUE				1
#endif
#define     NULL  				0 			// 空指针

//-------------------------------------------------------------------------------

#define SYS_MAX_DELEG_NUM		4000//1001//		// 系统中最大的代表机/主席机数量 4000
#define	MAX_DELEG_NUM			4000//1001//		// 只是主席机和代表机
#define	MAX_EXMAC_NUM			0//10

#define	MAX_MIC_ON_NUM			8		// 最大发言人数
#define	MAX_MIC_WAIT_NUM		8		// 最大等待人数
#define	FREE_MIC_ON_NUM			6		// FREE模式下，最大可开启的话筒数量
#define	MIC_INFOR_BUF_SIZE		10		// 存放MIC信息操作的帧长
#define	CEMARE_SERIES			8		// 摄像跟踪回退级数
#define	VOTE_SENT_BUF_SIZE		60		// 实时发送表决数据给PC的缓冲区深度

//单元最大128字节 128/5=25帧.还有5帧给返回的命令用
#define	EPI_SCAN_TX_MAX_SIZE	35		// 主机扫描最大同时发送帧数(0-46),代表机一次最多接收15帧广播码
#define	TX_Frame_MAX_SIZE		80		// 48主机命令最大同时发送256/5=51帧数(0-51),不能到51
//#define	ExHOST_RX_BUF_SIZE		4		// 主机接收扩展主机缓冲区的大小为5帧
//#define	PC_RX_BUF_SIZE			4		// 主机接收PC下发的信息帧数
//#define	PC_FRAME_LEN			5		// PC下发的信息帧长度
//#define	PC_FRAME_SIZE			100		// PC下发的信息缓冲大小


//一次最发送40个汉字。40*2=80Byte 再加5个其它字节 最大85Byte
#define	SMS_TX_BUF_SIZE			256		

//----------------------------------------------------------------------
// 系统状态
#define MEET_STATE			0x01			// 会议模式
#define VOTE_STATE			0x02			// 表决模式
#define SIGN_IN_STATE		0x03			// 签到模式
#define SET_ID_STATE		0x04			// 设置ID模式
#define Version_STATE       0x52            //显示版本号
#define Alarm_STATE         0x53            //消防警报
// 表决方式	
#define	VOTE_MODE			0x00			// 投票
#define	SELECT_MODE			0x01			// 选举
#define	EVALUA_MODE			0x02			// 评级

// 话筒工作模式
#define FIFO				0x00			// 先进先出模式
#define NORMAL				0x01			// 固定模式
#define	FREE				0x02			// 自由模式
#define	APPLY				0x03			// 申请模式


#define	VOICE				0x04			// 语音激励模式		
#define	AUTOMODE			0x05			// 自动模式

// 主机在相应模式下的轮询码
#define	PollOnlineMeet		0x50			// 主机会议状态下的轮询码在线ID
#define PollMeet			0x51			// 会议  
#define PollSign       		0x52			// 签到 
#define PollVote       		0x53 			// 表决
#define PollSect       		0x57 			// 选举
#define PollVet        		0x5B			// 评级
#define PollSetId      		0x5F 			// 设置ID

// 各个模式下的命令高8位
#define	MeetCmdHead			0x00
#define SignCmdHead			0x10
#define	VoteCmdHead			0x20
#define	SetIdCmdHead		0x30
#define	OtherCmdHead		0x40
#define	LoadCmdHead			0x50
#define	InterLoadCmd		0x60			// 给译员机
#define CloseRoomCmd		0x70



// 代表机/主席机会议模式相关命令  
#define	PowerDown			0x02			// 关机
#define	PcMicOprOpen		0x03			// PC通知某ID话筒开
#define	PcMicOprClose		0x04			// PC通知某ID话筒关
#define DeMicOpen    		0x05			// 代表机申请开话筒
#define DeMicClose   		0x06   			// 代表机申请关话筒
#define MaMicOpen    		0x07    		// 主席机开话筒
#define MaMicClose   		0x08    		// 主席机关话筒 
#define	DeMicWait			0x0D			// 代表机等待
#define	DeMicDisWait		0x0E			// 代表机撤消等待
#define DeMicListFull		0x0F			// 等待/申请/FREE模式下固定20个MIC列表已满
#define MaPausePri       	0x09    		// 主席机优先权命令
#define MaDisPausePri      	0x0A    		// 撤销主席机优先权命令 
#define MaClsPri	    	0x0B    		// 主席机关闭所有代表机话筒优先权命令 
#define AllMicClose	    	0x0C			// 关闭所有单元话筒
#define	CloseSpk			0x10			// 关闭扬声器
#define	OpenSpk				0x11			// 开启扬声器
#define PrioriyMute			0x12			// 主席优先权模式为"全部静音"，
#define PrioriyClose		0x13			// 主席优先权模式为"全部关闭"，
#define	ChairAgree			0x15			// 主席机同意声请者开启
#define	ChairAbject			0x16  			// 主席机反对声请这开启
#define	InterruptUp			0x17			// 通知更新译员机通道语言
#define SMS_Ask				0x18			// 下发短信成功应答码
#define	PcMaPausePri		0x19			// PC通知某ID主席机优先权命令
#define PcMaDisPausePri     0x1A    		// PC通知某ID撤销主席机优先权命令 
#define PcMaClsPri	    	0x1B    		// PC通知某ID主席机关闭所有代表机话筒优先权命令 
#define	ApplyService		0x1C			// 代表/主席单元申请茶水服务请求命令
#define	CancleService		0x1D			// 代表/主席单元撤消茶水服务请求命令
#define CancleTea           0X50            
#define	InterMicOpen		0x1E			// 通知译员机开话筒 
#define	InterMicClose		0x1F			// 通道译员机关话筒

#define	CallOperator		0x22			// 代表/主席单元申请内部通话
#define	CanclOperator		0x23			// 代表/主席单元撤消内部通话

#define	ScanIDStart			0x24			// 开始扫描在线ID
#define	ScanIDStop			0x25			// 停止扫描在线ID

#define	CallChair			0x27			// 向主席机申请内部通话
#define CanclChair			0x28			// 向主席机撤销内部通话
#define	ChairAgreeCall		0x30			// 主席机同意内部通话
#define	CallChairFull		0x31			// 申请与主席机通话人数满

#define	CheckIDRepeat		0x35			// 检测到ID相同

#define	ApplyHelp			0x36			// 译员机申请帮助
#define	ApplyHelpAsk		0x37			// 译员机申请帮助得到应答
#define SetIcCardOnline		0x38			// 设置IC卡在线
#define	DisICCard			0x39			// 不需要IC卡
#define	EnICCard			0x40			// 需要IC卡

#define CardOn 				0x42			// 有卡插上

#define	MIC_NumberOpen		0x10			// 打开话筒号0-7


// 代表机/主席机签到模式相关命令
#define SignStart       	0x01    		// 签到开始 
#define SignOk         		0x05   			// 从机签到命令 		协议上的命令为05，但是为了区别DAppMicOpen，特意加0x10
#define SignOver        	0x06    		// 签到结束
#define PreSignOver			0x16			// 预结束,此时所有的机器不能再签到了，只有主席机显示结果
#define PcRepSign	     	0x07    		// PC代替从机签到
#define EnSignFunc          0x08    		// 开放签到功能
#define DisSignFunc        	0x09    		// 禁用签到功能
#define	MSignStart			0x0E			// 主席机请求发起签到
#define	MSignOver			0x0F			// 主席机请求结束签到

// 代表机/主席机投票模式相关命令
// M4102/4C4丝印：
// 1		2		3		4		5	
// --		-		0		+		++
// Start	YES		Abs		No		Stop/NC
#define VoteStart      		0x10    		// 投票开始 
#define VoteOver       		0x29    		// 投票结束
#define	MVoteStart			0x2E			// 主席机请求发起表决
#define	MVoteOver			0x2F			// 主席机请求停止表决
#define	MVotePause			0x30			// 主席机请求暂停表决
#define	EnVoteFunc			0x38			// 使能表决功能
#define	DisVoteFunc			0x39			// 禁用表决功能

// 表决
#define YES            		0x25   		 	// 赞成 
#define ABS            		0x26    		// 弃权 
#define NO             		0x27    		// 反对

// 评级
#define	AA_SER				0x45			// --
#define	A_SER				0x46			// -
#define	O_SER				0x47			// 0
#define	F_SER				0x48			// +
#define	FF_SER				0x49			// ++

// 选举
#define	PerFive				0x65			// Person 1
#define	PerFour				0x66			// Person 2
#define	PerThree			0x67			// Person 3
#define	PerTwo				0x68			// Person 4
#define	PerOne				0x69			// Person 5

// 设置id 
#define EnterId        		0x80    		// 进入编ID广播
#define AckId          		0x85   	 		// 确认为本机ID
#define	ScanId				0x86			// 通知扩展主机重新搜索在线ID

//关闭相应id的机器 
#define DisWork        		0xB1    		// 禁止机器工作 
#define EnWork         		0xB2   		 	// 使能机器工作

//禁止相应id的话筒功能 
#define DisMic        		0xB3    		// 禁止机器话筒功能 
#define EnMic         		0xB4   		 	// 使能机器话筒功能

//禁止相应id的表决功能 
#define DisVote        		0xB5    		// 禁止机器表决功能 
#define EnVote         		0xB6   		 	// 使能机器表决功能

//禁止相应id的表决功能 
#define DisSign        		0xB7    		// 禁止机器表决功能 
#define EnSign         		0xB8   		 	// 使能机器表决功能

//#define MICON          		0x01 
//#define MICOFF         		0xFE
#define VYES           		0x10 
#define VABS           		0x20 
#define VNO            		0x30 
#define VNC            		0x8F 
//#define SIGNOK         		0x08 
//#define SNC            		0xF7 
#define ONLINE         		0x04  			// 在线单元
#define DelegUnit           0x02			// 代表机
#define	ChairUnit			0x01			// 主席机
#define	InterUnit			0x08			// 译员机

// 广播地址
#define all_broad_addr    		4050			// 广播地址,对全局进行广播
#define extend_broad_addr 		4081			// 广播地址,对扩展主机进行广播
#define slave_broad_addr  		4082			// 广播地址，对连接其下的代表机进行广播
#define value_broad_addr1 		4051   			// 广播地址，用于下载表决的第1项结果 『赞成，++，1』
#define value_broad_addr2 		4052   			// 广播地址，用于下载表决的第2项结果 『弃权，+，2』
#define value_broad_addr3 		4053   			// 广播地址，用于下载表决的第1项结果 『反对，0，3』
#define value_broad_addr4 		4054   			// 广播地址，用于下载表决的第1项结果 『-，4』
#define value_broad_addr5 		4055   			// 广播地址，用于下载表决的第1项结果 『--，5』

#define	SignVal_Broad_Addr1		4057			// 用于下载相应模式下应到的总人数
#define	SignVal_Broad_Addr2		4058			// 用于下载相应模式下实际到的总人数

#define	ApplayEn_Broad_Addr		4059			// 代表机申请开话筒
#define	ApplayDis_Broad_Addr	4060			// 代表机撤消申请开话筒


// 主机接收扩展主机信息帧状态
#define	STATUS_FRAME_ADDR		0			// 地址段
#define	STATUS_FRAME_DLEN		1			// 数据长度段
#define	STATUS_FRAME_DBUF		2			// 数据段
#define	STATUS_FRAME_DTYPE		3			// 数据类型段
#define	STATUS_FRAME_CRC		4			// 校验段

// 扩展主机数据段的类型编码
#define	ExHOST_DTYPE_MIC		0x01		// 话筒数据
#define	ExHOST_DTYPE_SIGN		0x02		// 签到
#define	ExHOST_DTYPE_VOTE		0x03		// 表决ID及结果
#define	ExHOST_DTYPE_XUANJU		0x04		// 无记名表决各项总数
#define	ExHOST_DTYPE_IDEL		0x0E		// 空闲码

#define SAMSUNG			0x01
#define	SONY				0x02
#define PELCO_D			0x03
#define PELCO_P_L		0x05		//4800
#define PELCO_P_H		0x06		//9600
#define	CUSTOM			0x04		//9600

#define	Camera_Zoom	0x09		//Zoom 信息存放位置
#define Camera_Addr	13		//球机地址


//	24C256中的地址分配
//0x0000----0x00FF地址区间作为主机EEPROM存储数据使用 地址0x0100----0x8000区间作为摄像跟踪使用
//0x0000---0x00FF地址分配如下 未用的作为备份扩展使用
#define SIGN_BYTES				0x00			//0--3写入Jony

#define SPEAKER_NUM_ADDR	0x05
#define MIC_MODE_ADDR			0x06
#define MEET_STATE_ADDR		0x07
#define	AFNGC1_Add				0x08
#define	AFNGC2_Add				0x09
#define	Track_Add				0x0A
#define	Sound_UNIT_Add		0X0B
#define	AUTO_ZERO_ADDR		0x0C
#define	CameraID					0x0D		//0x11 samsung	0x22 sony	0x33 pelco-d	0x44 pelco-p/4800	0x55 pelco-p/9600
#define	ALCSwitch			0x0E

#define	LINEIN_ADDR				0x10			// 线路输入音量
#define	BASS_ADDR					0x11			// 低音音量值存放地址
#define	TREBLE_ADDR				0x12			// 高音音量值存放地址
#define	VOLUME_ADDR				0x13			// 扬声器总音量值存放地址
#define	UnitVol_ADDR			0x1D			// 单元机总音量存放地址
#define	LinkVol_ADDR			0x1E			// 远程视频会议存放地址
#define	SPK_ADDR					0x14			// 上电开关单元扬声器
#define LANG_ADDR					0x15			// 界面语言值存放地址
#define	IR_CHAN_ADDR			0x16			// 24路输出通道地址
#define ChanneleTotalAdd	0x17
#define LanguageTypeAdd		0x18
#define TranslateRoomAdd	0x19

#define	Speaker_ADDR			0x1A			//单元扬声器音量存放地址
#define	Display_ADDR			0x1B			//桌牌显示
#define	Move_ADDR				0x1C			//VFD移动效果
#define	Delay_ADDR				0x1F			//控制WIFI延时时间			

/*扬声器分区变量存放地址*/
#define	End1_ADDR				0x20  /*分区1*/
#define	FirstGain1_ADDR			0x22
#define	FirstGain2_ADDR			0x23
#define	FirstGain3_ADDR			0x24
#define	FirstGain4_ADDR			0x25
#define	End2_ADDR				0x26  /*分区2*/
#define	SecGain1_ADDR			0x28
#define	SecGain2_ADDR			0x29
#define	SecGain3_ADDR			0x2A
#define	SecGain4_ADDR			0x2B
#define	End3_ADDR				0x2C  /*分区3*/
#define	ThirdGain1_ADDR			0x2E
#define	ThirdGain2_ADDR			0x2F
#define	ThirdGain3_ADDR			0x50
#define	ThirdGain4_ADDR			0x51
#define	End4_ADDR				0x52  /*分区4*/
#define	FourthGain1_ADDR		0x54
#define	FourthGain2_ADDR		0x55
#define	FourthGain3_ADDR		0x56
#define	FourthGain4_ADDR		0x57
#define	FanZone_ADDR			0x58  /* bit(4)  1:扬声器分区功能开  0:扬声器分区功能关  bit(3--0)   0:分区1  1:分区2  2:分区3  3:分区4 */
/************************/

#define	NetIpAdd					0x30
#define	NetSubMaskAdd			0x34
#define	NetGateWayAdd			0x38
#define MACAddr						0x3C			//0x3C----0x40
#define MACAddr6					0x4C      //MACAddr[5] 

//默认音效3种，用户也可以采用自定义音效	Band1_Addr---Band7_Addr 存入用户设置的音效参数
#define	Band1_Addr				0x41
#define	Band2_Addr				0x42
#define	Band3_Addr				0x43
#define	Band4_Addr				0x44
#define	Band5_Addr				0x45
#define	Band6_Addr				0x46
#define	Band7_Addr				0x47
#define	Affect_addr				0x48			//0x01：音效1	0x01：音效2	0x01：音效3  0x07:原音 当采用自定义均衡音效时写入0xFF

#define TOS_ADDR			0x49			//风扇转动温度设置控制
#define THYST_ADDR			0x4A
#define	IRLIGHT_ADDR		0x4B

#define	INTER_LANG_ADDR		0x60			//0x60---0x80 译员机通道语言存放地址，共32个

#define TYPE_ADDR					0x90 			// 机器类型
#define	SCAN_ADDR					0x91			// 单元ID冲突检测开关

#define	CHAN_ADDR					0X100     //0x100 --- 0x10F
#define CAME_ADDR					0X110			//0x110 --- 0x11F

//0x0100----0x8000地址区间作为摄像跟踪使用
//0x100---0x180存入预设位的ID，上电读取预留0x200---0x3000用于扩展
//信息存入格式如下 :第一个位置用于全景存储	0---7  pan/title position   8---11  zoom data
//128个预设位 对应地址 0x300  --- 0xB00
#define	PrePos_ADDR				0x300		

//samsung	0x2000--0x2300	第一个位置用于存储预设ID的个数，第二个位置用于存储全景位置，每个位置占用2个字节
//下面的PELCO协议的位置存储与SAMSUNG相同	算法实现如下：
//0x2000为全景位置，0x2001---0x207F共127个预设位置，每个位置存放的是摄像球的ID。占用一个字节的地址空间
#define	Samsung_ADDR			0x2000
//PELCO_D	0x2300--0x2600
#define	Pelco_D_ADDR			0x2300
//PELCO_P_L	0x2600--0x2900
#define	Pelco_L_ADDR			0x2600
//PELCO_P_H	0x2900--0x2c00
#define	Pelco_H_ADDR			0x2900

//KEY和SSID保存位置
#define	Key_ADDR				0x4000//00--5F   64个字节
#define	Ssid_ADDR				0x4060//60--7F   32个字节
#define	MicChannel_ADDR			0x4080//6:MIC通道最大为6（WIFI）  8:MIC通道最大为8（DIG）
#define	SpareWIFI_ADDR			0x4081//1:省电模式  0:正常模式
///位为1表示有问题
#define		Sys_FlashEEprom		0x01
#define		Sys_NandFlash		0x02
#define		Sys_FPGA			0x04
#define		Sys_RTC8306			0x08

#define		INTER_MAX_SPK_NUM	33// 译员机最大发言人数.从1开始

#define ActuallyLangNum  178  //实际应用语言数

#define RX_Frame_MAX_SIZE	40
// 接收单元上发IC卡里的数据, 每个IC卡里包含数据40个字节。一次最多支持3个IC卡里数据
#define		CARD_RX_BUFF_SIZE		60

///1.接收普通指令是3个16位，128/4=32帧
///2.接收短信息是31个16位，128/31=4帧
//GLOBAL_MAIN uchar *pEPI_RxFrame;
//GLOBAL_MAIN uchar *pEPI_RxFrameProcess;
//GLOBAL_MAIN uchar EPI_RxFrameBuff[EPI_RX_Frame_SIZE];


#define EPI_RX_BUFF_SIZE	128								//EPI接收缓存

//TCP/IP接收
#define	NET_RX_BUFF_SIZE	500
#define	NET_RX_RELAY_TIMER	5			// 5*10ms

#define		AT24C08_Write_Addr		0xAE
#define		AT24C08_Read_Addr		0xAF

#define		ChanneleTotalMax	32
#define		TranslateRoomMax	32
#define		LanguageTypeMax		178
#define		MaxVolume		10
#define		MaxBass			24
#define		MaxTreble		24
#define		MaxBand			40
#define		MaxSelect		7
#define		MaxCamera		4

#endif
