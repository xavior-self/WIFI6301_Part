#ifndef __DEFINE_H__
#define __DEFINE_H__

//#define INIT_DEBUG

//-------------------------------------------------------------------------------
///////////////////////////ϵͳ��������
//ϵͳʱ��
#define		FOSC_44_4M						///200M/4.5=44.444M
//#define		FOSC_57_1M						///200M/3.5=57.142M
//#define		FOSC_80_0M						///200M/2.5=80.000M


#define		LAN_CN					0		// �˵������� 0:���� 1:Ӣ�� 2:����
#define		LAN_EN					1
#define		LAN_FN					2


//#define		LANGUAGE				LAN_CN
#define		LANGUAGE				LAN_EN
//#define		LANGUAGE				LAN_FT

// ��ӡ��Ϣ���򿪿��Ź�
#define		DEBUG_DOG			0

#define     BackupHostID        0xFA1		//��������ID
//��������
//#define		NetHardWareRxBuf	8,8,8,8,8,8,8,8		
//#define		NetHardWareTxBuf	8,8,8,8,8,8,8,8	
//#define		NetIPVal			192,168,10,100
//#define		NetSubnetVal		255,255,255,0
//#define		NetGateWayVal		192,168,10,1
//#define		NetMacVal			0x00,0x08,0xDC,0x01,0x11,0x33

//���ڲ�����
//#define 	Uart0BaudRate   	9600		//9600//����0�Ĳ�����,������
//#define 	Uart1BaudRate   	9600		//115200//����1�Ĳ�����,��������ͷר��

//-------------------------------------------------------------------------------
///////////////////////////�ⲿ�豸��ַ����

//#define 	LcdDispPort			(*(volatile unsigned short *)0xa00003FE) ///0x3FE��W5300û��
//#define 	FPGA_PORT 			(*(volatile unsigned short*)0x60000002)//��ʵ���ַҪƫ��һλ,A0û�õ�.����������
//#define 	FPGA_PORT_HOST   	(volatile unsigned short *)0x60000000
#define		CHECK_MIC			0x102		// ֪ͨFPGA����MIC����״̬��ַ		
#define		CHECK_COMM			0x103		// ֪ͨFPGAͨ�ŷ�����ַ

#define		WIFIDEBUG			0xFFFF		//controlwifi����λ
//A10 Ϊ1ѡ��ǰ���FPGA
//A9 Key��д	�͵�ƽ��Ч
//A8 LED��д	�͵�ƽ��Ч
//A7 LCD����ѡ��
//A6 LCDƬѡ �͵�ƽ��Ч


#define		AGCKey			0x80
#define		ANCKey			0x40
#define		AFCKey			0x20

#define		Write			0x00
#define		Read			0x01
/*���˵�����*/
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
/*���Ҽ��޸�����*/
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
#define		cammapnumType	0x25		/*������*/
#define		cammapaddrType	0x26
#define		cammaparreyType	0x27
#define		langType		0x28
#define		wifiirType		0x29
#define		defaultType		0x2A
#define		remoteType		0x2B	
/*ChooseNum����Ӧ�Ĳ���λ��*/
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
///////////////////////////FPGA���ؼĴ�������
#define			REG_MAP_BASE			0x60000000	

//��д�Ĵ���


//ByPass ��־
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
//language15+MCU�Զ���8λ
#define			REG_Lan15Default		0x388//(REG_MAP_BASE+(0x388<<1))
//ʵʱ�·���ǩ������
#define			REG_SignNum				0x389//	(REG_MAP_BASE+(0x389<<1))
//voteNumber�������
#define			REG_VotePeople			0x38a//(REG_MAP_BASE+(0x38a<<1))
//voteResultAA��++������
#define			REG_VoteRes1			0x38b//	(REG_MAP_BASE+(0x38b<<1))
//voteResultA��+������
#define			REG_VoteRes2			0x38c//	(REG_MAP_BASE+(0x38c<<1))
//voteResult0��0������
#define			REG_VoteRes3			0x38d//	(REG_MAP_BASE+(0x38d<<1))
//voteResultS��-������
#define			REG_VoteRes4			0x38e//	(REG_MAP_BASE+(0x38e<<1))
//voteResultSS��--������
#define			REG_VoteRes5			0x38f//	(REG_MAP_BASE+(0x38f<<1))
//switchLan��0x190��Ӣ��,���������أ��Զ�0ͨ�����أ�������ϵ�LED��2s���أ�
#define			REG_Other2				0x390//(REG_MAP_BASE+(0x390<<1))

//�����ϵ�ɨ�����־
#define			REG_Timer				0x391//(REG_MAP_BASE+(0x391<<1))
//ʵʱ�·���ǩ��������
#define			REG_SignMaxNum			0x392//(REG_MAP_BASE+(0x392<<1))
//��Ա��ͨ������
#define			REG_Interpreter			0x393//(REG_MAP_BASE+(0x393<<1))
/*
B15��		0���������򿪡�1���������رա�
B14-B13��	00���������ԡ�01��Ӣ�ġ�10�����塣
B12-B11��	��ʾ����01����ʱ�������ʱ��10����ʱ���Ե���ʱ��
B10-B8��	���Ͳ����

B7:        	PC����
B6:			�򿪲�ˮ����
B5:			IC���ϴ�����
B4:			��ʾְλ
B3��        ��ʾ����
//B7-B3		11111: PC���ߡ�
B2-B0		���ڷ�������

*/
#define			REG_Default1			0x394//(REG_MAP_BASE+(0x394<<1))
//MCU�Զ���16λ ��ʱ���ʱ��
#define			REG_Default2			0x395//(REG_MAP_BASE+(0x395<<1))

// ��ϯ����������ǩ��
#define			REG_MySignStart			0x396//(REG_MAP_BASE+(0x396<<1))

// ��ϯ������������
/*
D15��		1���ѱ����
D14��		1���鿴��������
D13	-D12��		
D11-D0��	��������ID�š�

*/
#define			REG_MyVoteStart			0x397//(REG_MAP_BASE+(0x397<<1))

//MCU�Զ���Ĵ��� �������������״̬
/*
            
      �Ƿ���ҪIC����־
      ͶƱ��ֹ��־
      ǩ����ֹ��־
B15  1��������Աͨ����  0��������Աͨ����
//B14    1:������Ĭ�Ͽ�	 0����������
B13    1��ͶƱǩ��ģʽ��ֹ����Ͳ  0��������Ͳ
B12    ���ͽ����־
B11-B9  ���������   000��1��    111:8��
B8-B7   0x:00    FIFO		// �Ƚ��ȳ�ģʽ		
        0x01    NORMAL		// �̶�ģʽ					
		0x02    FREE		// ����ģʽ			
		0x03    APPLY		// ����ģʽ			

B6-B3   �·���ID��
B2-B0   ģʽ ��01 ����ģʽ  02 ͶƱģʽ  03 ǩ��ģʽ   04 ����IDģʽ

*/
#define			REG_Default3			0x398//(REG_MAP_BASE+(0x398<<1))



#define         REG_Default4			0x399//(REG_MAP_BASE+(0x399<<1))
//bit0 ---bit1	 00���ƶ� 01���� 02����  bit4 0:5201 1:5201A
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

#define					REG_Wait0ID			0x3D0//APPLYģʽ�����б�
#define					REG_Wait1ID			0x3D1
#define					REG_Wait2ID			0x3D2
#define					REG_Wait3ID			0x3D3
#define					REG_Wait4ID			0x3D4
#define					REG_Wait5ID			0x3D5
#define					REG_Channel0ID		0x3B0//32ͨ��ID�Ĵ����������ػ�Ͳʱ���ѵ�ǰID��ͨ���Ÿ�FPGA
#define					REG_Channel1ID		0x3B1//��Ͳһ��8��ͨ��
#define					REG_Channel2ID		0x3B2
#define					REG_Channel3ID		0x3B3
#define					REG_Channel4ID		0x3B4
#define					REG_Channel5ID		0x3B5
#define					REG_Channel6ID		0x3B6
#define					REG_Channel7ID		0x3B7
#define					REG_IR_Track		0x3B8

#define					REG_DebugResult1	0x3B9//wifi���Խ���Ĵ�����1�Ż�Ͳ��0�Ż�Ͳ��ֻ���Ĵ�����
#define					REG_DebugResult2	0x3BA//wifi���Խ���Ĵ�����3�Ż�Ͳ��2�Ż�Ͳ��ֻ���Ĵ�����
#define					REG_DebugResult3	0x3BB//wifi���Խ���Ĵ�����5�Ż�Ͳ��4�Ż�Ͳ��ֻ���Ĵ�����
#define					REG_WifiSetID		0x3BC//bit15: 1/0:��ʼ��ID/������ID     bit 14-0:��ID�����е�ǰ�·�ID��
#define					REG_WifiControl		0x3BD//wifi���ƼĴ���
#define					REG_WifiDelay		0x3BE//wifi������ʱʱ��

#define					REG_Part1Gain12		0x3C0//����1     ������1��2����
#define					REG_Part1Gain34		0x3C1//����1     ������3��4����
#define					REG_Part1End		0x3C2//����1     ����ID��
#define					REG_Part2Gain12		0x3C3//����2     ������1��2����
#define					REG_Part2Gain34		0x3C4//����2     ������3��4����
#define					REG_Part2End		0x3C5//����2     ����ID��
#define					REG_Part3Gain12		0x3C6//����3     ������1��2����
#define					REG_Part3Gain34		0x3C7//����3     ������3��4����
#define					REG_Part3End		0x3C8//����3     ����ID��
#define					REG_Part4Gain12		0x3C9//����4     ������1��2����
#define					REG_Part4Gain34		0x3CA//����4     ������3��4����
#define					REG_Part4End		0x3CB//����4     ����ID��
#define					REG_SwitchZone		0x3CC//	/* bit(4)  1:�������������ܿ�  0:�������������ܹ�  bit(3--0)   0:����1  1:����2  2:����3  3:����4 */
#define					REG_Volume			0x3CD//�߳�����	��Ԫ���� ��8λ      ����������  ��8λ
#define					REG_LineIn			0x3CE//��������	ʹ�õ�8λ
#define					REG_IRLight			0x3CF//��������ȵ���
//---45���Ĵ�������WIFIǩ���ͱ������б�
#define					REG_StateWifi		0x3D6
/*  | 						   |			   |   ����45��
	|						   |			   |
#define					REG_StateWifi		0x402*/

///////////////////////////���������滻

//����8λMCU��˵
//uchar 	1���ֽ�
//ushort	2���ֽ�
//uint		2���ֽ�
//ulong		4���ֽ�

//����32λMCU��˵
//uchar 	1���ֽ�
//ushort	2���ֽ�
//uint		4���ֽ�
//ulong		4���ֽ�
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


typedef uint8 SOCKET;					// ����W5300ר��

#ifndef 	FALSE
#define 	FALSE   			0
#endif
#ifndef 	TRUE
#define 	TRUE				1
#endif
#define     NULL  				0 			// ��ָ��

//-------------------------------------------------------------------------------

#define SYS_MAX_DELEG_NUM		4000//1001//		// ϵͳ�����Ĵ����/��ϯ������ 4000
#define	MAX_DELEG_NUM			4000//1001//		// ֻ����ϯ���ʹ����
#define	MAX_EXMAC_NUM			0//10

#define	MAX_MIC_ON_NUM			8		// ���������
#define	MAX_MIC_WAIT_NUM		8		// ���ȴ�����
#define	FREE_MIC_ON_NUM			6		// FREEģʽ�£����ɿ����Ļ�Ͳ����
#define	MIC_INFOR_BUF_SIZE		10		// ���MIC��Ϣ������֡��
#define	CEMARE_SERIES			8		// ������ٻ��˼���
#define	VOTE_SENT_BUF_SIZE		60		// ʵʱ���ͱ�����ݸ�PC�Ļ��������

//��Ԫ���128�ֽ� 128/5=25֡.����5֡�����ص�������
#define	EPI_SCAN_TX_MAX_SIZE	35		// ����ɨ�����ͬʱ����֡��(0-46),�����һ��������15֡�㲥��
#define	TX_Frame_MAX_SIZE		80		// 48�����������ͬʱ����256/5=51֡��(0-51),���ܵ�51
//#define	ExHOST_RX_BUF_SIZE		4		// ����������չ�����������Ĵ�СΪ5֡
//#define	PC_RX_BUF_SIZE			4		// ��������PC�·�����Ϣ֡��
//#define	PC_FRAME_LEN			5		// PC�·�����Ϣ֡����
//#define	PC_FRAME_SIZE			100		// PC�·�����Ϣ�����С


//һ�����40�����֡�40*2=80Byte �ټ�5�������ֽ� ���85Byte
#define	SMS_TX_BUF_SIZE			256		

//----------------------------------------------------------------------
// ϵͳ״̬
#define MEET_STATE			0x01			// ����ģʽ
#define VOTE_STATE			0x02			// ���ģʽ
#define SIGN_IN_STATE		0x03			// ǩ��ģʽ
#define SET_ID_STATE		0x04			// ����IDģʽ
#define Version_STATE       0x52            //��ʾ�汾��
#define Alarm_STATE         0x53            //��������
// �����ʽ	
#define	VOTE_MODE			0x00			// ͶƱ
#define	SELECT_MODE			0x01			// ѡ��
#define	EVALUA_MODE			0x02			// ����

// ��Ͳ����ģʽ
#define FIFO				0x00			// �Ƚ��ȳ�ģʽ
#define NORMAL				0x01			// �̶�ģʽ
#define	FREE				0x02			// ����ģʽ
#define	APPLY				0x03			// ����ģʽ


#define	VOICE				0x04			// ��������ģʽ		
#define	AUTOMODE			0x05			// �Զ�ģʽ

// ��������Ӧģʽ�µ���ѯ��
#define	PollOnlineMeet		0x50			// ��������״̬�µ���ѯ������ID
#define PollMeet			0x51			// ����  
#define PollSign       		0x52			// ǩ�� 
#define PollVote       		0x53 			// ���
#define PollSect       		0x57 			// ѡ��
#define PollVet        		0x5B			// ����
#define PollSetId      		0x5F 			// ����ID

// ����ģʽ�µ������8λ
#define	MeetCmdHead			0x00
#define SignCmdHead			0x10
#define	VoteCmdHead			0x20
#define	SetIdCmdHead		0x30
#define	OtherCmdHead		0x40
#define	LoadCmdHead			0x50
#define	InterLoadCmd		0x60			// ����Ա��
#define CloseRoomCmd		0x70



// �����/��ϯ������ģʽ�������  
#define	PowerDown			0x02			// �ػ�
#define	PcMicOprOpen		0x03			// PC֪ͨĳID��Ͳ��
#define	PcMicOprClose		0x04			// PC֪ͨĳID��Ͳ��
#define DeMicOpen    		0x05			// ��������뿪��Ͳ
#define DeMicClose   		0x06   			// ���������ػ�Ͳ
#define MaMicOpen    		0x07    		// ��ϯ������Ͳ
#define MaMicClose   		0x08    		// ��ϯ���ػ�Ͳ 
#define	DeMicWait			0x0D			// ������ȴ�
#define	DeMicDisWait		0x0E			// ����������ȴ�
#define DeMicListFull		0x0F			// �ȴ�/����/FREEģʽ�¹̶�20��MIC�б�����
#define MaPausePri       	0x09    		// ��ϯ������Ȩ����
#define MaDisPausePri      	0x0A    		// ������ϯ������Ȩ���� 
#define MaClsPri	    	0x0B    		// ��ϯ���ر����д������Ͳ����Ȩ���� 
#define AllMicClose	    	0x0C			// �ر����е�Ԫ��Ͳ
#define	CloseSpk			0x10			// �ر�������
#define	OpenSpk				0x11			// ����������
#define PrioriyMute			0x12			// ��ϯ����ȨģʽΪ"ȫ������"��
#define PrioriyClose		0x13			// ��ϯ����ȨģʽΪ"ȫ���ر�"��
#define	ChairAgree			0x15			// ��ϯ��ͬ�������߿���
#define	ChairAbject			0x16  			// ��ϯ�����������⿪��
#define	InterruptUp			0x17			// ֪ͨ������Ա��ͨ������
#define SMS_Ask				0x18			// �·����ųɹ�Ӧ����
#define	PcMaPausePri		0x19			// PC֪ͨĳID��ϯ������Ȩ����
#define PcMaDisPausePri     0x1A    		// PC֪ͨĳID������ϯ������Ȩ���� 
#define PcMaClsPri	    	0x1B    		// PC֪ͨĳID��ϯ���ر����д������Ͳ����Ȩ���� 
#define	ApplyService		0x1C			// ����/��ϯ��Ԫ�����ˮ������������
#define	CancleService		0x1D			// ����/��ϯ��Ԫ������ˮ������������
#define CancleTea           0X50            
#define	InterMicOpen		0x1E			// ֪ͨ��Ա������Ͳ 
#define	InterMicClose		0x1F			// ͨ����Ա���ػ�Ͳ

#define	CallOperator		0x22			// ����/��ϯ��Ԫ�����ڲ�ͨ��
#define	CanclOperator		0x23			// ����/��ϯ��Ԫ�����ڲ�ͨ��

#define	ScanIDStart			0x24			// ��ʼɨ������ID
#define	ScanIDStop			0x25			// ֹͣɨ������ID

#define	CallChair			0x27			// ����ϯ�������ڲ�ͨ��
#define CanclChair			0x28			// ����ϯ�������ڲ�ͨ��
#define	ChairAgreeCall		0x30			// ��ϯ��ͬ���ڲ�ͨ��
#define	CallChairFull		0x31			// ��������ϯ��ͨ��������

#define	CheckIDRepeat		0x35			// ��⵽ID��ͬ

#define	ApplyHelp			0x36			// ��Ա���������
#define	ApplyHelpAsk		0x37			// ��Ա����������õ�Ӧ��
#define SetIcCardOnline		0x38			// ����IC������
#define	DisICCard			0x39			// ����ҪIC��
#define	EnICCard			0x40			// ��ҪIC��

#define CardOn 				0x42			// �п�����

#define	MIC_NumberOpen		0x10			// �򿪻�Ͳ��0-7


// �����/��ϯ��ǩ��ģʽ�������
#define SignStart       	0x01    		// ǩ����ʼ 
#define SignOk         		0x05   			// �ӻ�ǩ������ 		Э���ϵ�����Ϊ05������Ϊ������DAppMicOpen�������0x10
#define SignOver        	0x06    		// ǩ������
#define PreSignOver			0x16			// Ԥ����,��ʱ���еĻ���������ǩ���ˣ�ֻ����ϯ����ʾ���
#define PcRepSign	     	0x07    		// PC����ӻ�ǩ��
#define EnSignFunc          0x08    		// ����ǩ������
#define DisSignFunc        	0x09    		// ����ǩ������
#define	MSignStart			0x0E			// ��ϯ��������ǩ��
#define	MSignOver			0x0F			// ��ϯ���������ǩ��

// �����/��ϯ��ͶƱģʽ�������
// M4102/4C4˿ӡ��
// 1		2		3		4		5	
// --		-		0		+		++
// Start	YES		Abs		No		Stop/NC
#define VoteStart      		0x10    		// ͶƱ��ʼ 
#define VoteOver       		0x29    		// ͶƱ����
#define	MVoteStart			0x2E			// ��ϯ����������
#define	MVoteOver			0x2F			// ��ϯ������ֹͣ���
#define	MVotePause			0x30			// ��ϯ��������ͣ���
#define	EnVoteFunc			0x38			// ʹ�ܱ������
#define	DisVoteFunc			0x39			// ���ñ������

// ���
#define YES            		0x25   		 	// �޳� 
#define ABS            		0x26    		// ��Ȩ 
#define NO             		0x27    		// ����

// ����
#define	AA_SER				0x45			// --
#define	A_SER				0x46			// -
#define	O_SER				0x47			// 0
#define	F_SER				0x48			// +
#define	FF_SER				0x49			// ++

// ѡ��
#define	PerFive				0x65			// Person 1
#define	PerFour				0x66			// Person 2
#define	PerThree			0x67			// Person 3
#define	PerTwo				0x68			// Person 4
#define	PerOne				0x69			// Person 5

// ����id 
#define EnterId        		0x80    		// �����ID�㲥
#define AckId          		0x85   	 		// ȷ��Ϊ����ID
#define	ScanId				0x86			// ֪ͨ��չ����������������ID

//�ر���Ӧid�Ļ��� 
#define DisWork        		0xB1    		// ��ֹ�������� 
#define EnWork         		0xB2   		 	// ʹ�ܻ�������

//��ֹ��Ӧid�Ļ�Ͳ���� 
#define DisMic        		0xB3    		// ��ֹ������Ͳ���� 
#define EnMic         		0xB4   		 	// ʹ�ܻ�����Ͳ����

//��ֹ��Ӧid�ı������ 
#define DisVote        		0xB5    		// ��ֹ����������� 
#define EnVote         		0xB6   		 	// ʹ�ܻ����������

//��ֹ��Ӧid�ı������ 
#define DisSign        		0xB7    		// ��ֹ����������� 
#define EnSign         		0xB8   		 	// ʹ�ܻ����������

//#define MICON          		0x01 
//#define MICOFF         		0xFE
#define VYES           		0x10 
#define VABS           		0x20 
#define VNO            		0x30 
#define VNC            		0x8F 
//#define SIGNOK         		0x08 
//#define SNC            		0xF7 
#define ONLINE         		0x04  			// ���ߵ�Ԫ
#define DelegUnit           0x02			// �����
#define	ChairUnit			0x01			// ��ϯ��
#define	InterUnit			0x08			// ��Ա��

// �㲥��ַ
#define all_broad_addr    		4050			// �㲥��ַ,��ȫ�ֽ��й㲥
#define extend_broad_addr 		4081			// �㲥��ַ,����չ�������й㲥
#define slave_broad_addr  		4082			// �㲥��ַ�����������µĴ�������й㲥
#define value_broad_addr1 		4051   			// �㲥��ַ���������ر���ĵ�1���� ���޳ɣ�++��1��
#define value_broad_addr2 		4052   			// �㲥��ַ���������ر���ĵ�2���� ����Ȩ��+��2��
#define value_broad_addr3 		4053   			// �㲥��ַ���������ر���ĵ�1���� �����ԣ�0��3��
#define value_broad_addr4 		4054   			// �㲥��ַ���������ر���ĵ�1���� ��-��4��
#define value_broad_addr5 		4055   			// �㲥��ַ���������ر���ĵ�1���� ��--��5��

#define	SignVal_Broad_Addr1		4057			// ����������Ӧģʽ��Ӧ����������
#define	SignVal_Broad_Addr2		4058			// ����������Ӧģʽ��ʵ�ʵ���������

#define	ApplayEn_Broad_Addr		4059			// ��������뿪��Ͳ
#define	ApplayDis_Broad_Addr	4060			// ������������뿪��Ͳ


// ����������չ������Ϣ֡״̬
#define	STATUS_FRAME_ADDR		0			// ��ַ��
#define	STATUS_FRAME_DLEN		1			// ���ݳ��ȶ�
#define	STATUS_FRAME_DBUF		2			// ���ݶ�
#define	STATUS_FRAME_DTYPE		3			// �������Ͷ�
#define	STATUS_FRAME_CRC		4			// У���

// ��չ�������ݶε����ͱ���
#define	ExHOST_DTYPE_MIC		0x01		// ��Ͳ����
#define	ExHOST_DTYPE_SIGN		0x02		// ǩ��
#define	ExHOST_DTYPE_VOTE		0x03		// ���ID�����
#define	ExHOST_DTYPE_XUANJU		0x04		// �޼��������������
#define	ExHOST_DTYPE_IDEL		0x0E		// ������

#define SAMSUNG			0x01
#define	SONY				0x02
#define PELCO_D			0x03
#define PELCO_P_L		0x05		//4800
#define PELCO_P_H		0x06		//9600
#define	CUSTOM			0x04		//9600

#define	Camera_Zoom	0x09		//Zoom ��Ϣ���λ��
#define Camera_Addr	13		//�����ַ


//	24C256�еĵ�ַ����
//0x0000----0x00FF��ַ������Ϊ����EEPROM�洢����ʹ�� ��ַ0x0100----0x8000������Ϊ�������ʹ��
//0x0000---0x00FF��ַ�������� δ�õ���Ϊ������չʹ��
#define SIGN_BYTES				0x00			//0--3д��Jony

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

#define	LINEIN_ADDR				0x10			// ��·��������
#define	BASS_ADDR					0x11			// ��������ֵ��ŵ�ַ
#define	TREBLE_ADDR				0x12			// ��������ֵ��ŵ�ַ
#define	VOLUME_ADDR				0x13			// ������������ֵ��ŵ�ַ
#define	UnitVol_ADDR			0x1D			// ��Ԫ����������ŵ�ַ
#define	LinkVol_ADDR			0x1E			// Զ����Ƶ�����ŵ�ַ
#define	SPK_ADDR					0x14			// �ϵ翪�ص�Ԫ������
#define LANG_ADDR					0x15			// ��������ֵ��ŵ�ַ
#define	IR_CHAN_ADDR			0x16			// 24·���ͨ����ַ
#define ChanneleTotalAdd	0x17
#define LanguageTypeAdd		0x18
#define TranslateRoomAdd	0x19

#define	Speaker_ADDR			0x1A			//��Ԫ������������ŵ�ַ
#define	Display_ADDR			0x1B			//������ʾ
#define	Move_ADDR				0x1C			//VFD�ƶ�Ч��
#define	Delay_ADDR				0x1F			//����WIFI��ʱʱ��			

/*����������������ŵ�ַ*/
#define	End1_ADDR				0x20  /*����1*/
#define	FirstGain1_ADDR			0x22
#define	FirstGain2_ADDR			0x23
#define	FirstGain3_ADDR			0x24
#define	FirstGain4_ADDR			0x25
#define	End2_ADDR				0x26  /*����2*/
#define	SecGain1_ADDR			0x28
#define	SecGain2_ADDR			0x29
#define	SecGain3_ADDR			0x2A
#define	SecGain4_ADDR			0x2B
#define	End3_ADDR				0x2C  /*����3*/
#define	ThirdGain1_ADDR			0x2E
#define	ThirdGain2_ADDR			0x2F
#define	ThirdGain3_ADDR			0x50
#define	ThirdGain4_ADDR			0x51
#define	End4_ADDR				0x52  /*����4*/
#define	FourthGain1_ADDR		0x54
#define	FourthGain2_ADDR		0x55
#define	FourthGain3_ADDR		0x56
#define	FourthGain4_ADDR		0x57
#define	FanZone_ADDR			0x58  /* bit(4)  1:�������������ܿ�  0:�������������ܹ�  bit(3--0)   0:����1  1:����2  2:����3  3:����4 */
/************************/

#define	NetIpAdd					0x30
#define	NetSubMaskAdd			0x34
#define	NetGateWayAdd			0x38
#define MACAddr						0x3C			//0x3C----0x40
#define MACAddr6					0x4C      //MACAddr[5] 

//Ĭ����Ч3�֣��û�Ҳ���Բ����Զ�����Ч	Band1_Addr---Band7_Addr �����û����õ���Ч����
#define	Band1_Addr				0x41
#define	Band2_Addr				0x42
#define	Band3_Addr				0x43
#define	Band4_Addr				0x44
#define	Band5_Addr				0x45
#define	Band6_Addr				0x46
#define	Band7_Addr				0x47
#define	Affect_addr				0x48			//0x01����Ч1	0x01����Ч2	0x01����Ч3  0x07:ԭ�� �������Զ��������Чʱд��0xFF

#define TOS_ADDR			0x49			//����ת���¶����ÿ���
#define THYST_ADDR			0x4A
#define	IRLIGHT_ADDR		0x4B

#define	INTER_LANG_ADDR		0x60			//0x60---0x80 ��Ա��ͨ�����Դ�ŵ�ַ����32��

#define TYPE_ADDR					0x90 			// ��������
#define	SCAN_ADDR					0x91			// ��ԪID��ͻ��⿪��

#define	CHAN_ADDR					0X100     //0x100 --- 0x10F
#define CAME_ADDR					0X110			//0x110 --- 0x11F

//0x0100----0x8000��ַ������Ϊ�������ʹ��
//0x100---0x180����Ԥ��λ��ID���ϵ��ȡԤ��0x200---0x3000������չ
//��Ϣ�����ʽ���� :��һ��λ������ȫ���洢	0---7  pan/title position   8---11  zoom data
//128��Ԥ��λ ��Ӧ��ַ 0x300  --- 0xB00
#define	PrePos_ADDR				0x300		

//samsung	0x2000--0x2300	��һ��λ�����ڴ洢Ԥ��ID�ĸ������ڶ���λ�����ڴ洢ȫ��λ�ã�ÿ��λ��ռ��2���ֽ�
//�����PELCOЭ���λ�ô洢��SAMSUNG��ͬ	�㷨ʵ�����£�
//0x2000Ϊȫ��λ�ã�0x2001---0x207F��127��Ԥ��λ�ã�ÿ��λ�ô�ŵ����������ID��ռ��һ���ֽڵĵ�ַ�ռ�
#define	Samsung_ADDR			0x2000
//PELCO_D	0x2300--0x2600
#define	Pelco_D_ADDR			0x2300
//PELCO_P_L	0x2600--0x2900
#define	Pelco_L_ADDR			0x2600
//PELCO_P_H	0x2900--0x2c00
#define	Pelco_H_ADDR			0x2900

//KEY��SSID����λ��
#define	Key_ADDR				0x4000//00--5F   64���ֽ�
#define	Ssid_ADDR				0x4060//60--7F   32���ֽ�
#define	MicChannel_ADDR			0x4080//6:MICͨ�����Ϊ6��WIFI��  8:MICͨ�����Ϊ8��DIG��
#define	SpareWIFI_ADDR			0x4081//1:ʡ��ģʽ  0:����ģʽ
///λΪ1��ʾ������
#define		Sys_FlashEEprom		0x01
#define		Sys_NandFlash		0x02
#define		Sys_FPGA			0x04
#define		Sys_RTC8306			0x08

#define		INTER_MAX_SPK_NUM	33// ��Ա�����������.��1��ʼ

#define ActuallyLangNum  178  //ʵ��Ӧ��������

#define RX_Frame_MAX_SIZE	40
// ���յ�Ԫ�Ϸ�IC���������, ÿ��IC�����������40���ֽڡ�һ�����֧��3��IC��������
#define		CARD_RX_BUFF_SIZE		60

///1.������ָͨ����3��16λ��128/4=32֡
///2.���ն���Ϣ��31��16λ��128/31=4֡
//GLOBAL_MAIN uchar *pEPI_RxFrame;
//GLOBAL_MAIN uchar *pEPI_RxFrameProcess;
//GLOBAL_MAIN uchar EPI_RxFrameBuff[EPI_RX_Frame_SIZE];


#define EPI_RX_BUFF_SIZE	128								//EPI���ջ���

//TCP/IP����
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
