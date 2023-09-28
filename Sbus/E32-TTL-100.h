/**
 * E32-TTL-100 Transceiver Interface
 *
 * @author Bob Chen (bob-0505@gotmail.com)
 * @date 1 November 2017
 * https://github.com/Bob0505/E32-TTL-100
 */
#ifndef E32-TTL-100_H_
#define E32-TTL-100_H_

#define MSG_PRINT_DEBUG 1
#define SYSTEM_ID 0x4753
#define TIME_WAIT_AUX 30

#define PKT_LEN_BEACON 10

#define TIME_OUT_CNT  100
#define MAX_TX_SIZE   58

#define DEVICE_A_ADDR_H 0xFF
#define DEVICE_A_ADDR_L 0xFF
#define DEVICE_B_ADDR_H 0x05
#define DEVICE_B_ADDR_L 0x02

#define DEVICE_BROADCAST_ADDR_H 0xFF
#define DEVICE_BROADCAST_ADDR_L 0xFF

typedef enum {
  RET_SUCCESS = 0,
  RET_ERROR_UNKNOWN,	/* something shouldn't happened */
  RET_NOT_SUPPORT,
  RET_NOT_IMPLEMENT,
  RET_NOT_INITIAL,
  RET_INVALID_PARAM,
  RET_DATA_SIZE_NOT_MATCH,
  RET_BUF_TOO_SMALL,
  RET_TIMEOUT,
  RET_HW_ERROR,
  RET_SLAVES_NOT_EXIST,
  RET_SLAVES_EXIST,
} RET_STATUS;

enum BAUD_RATE_TYPE
{
  BAUD_RATE_9600 = 9600,
  BAUD_RATE_100000 = 100000,
  BAUD_RATE_115200 = 115200
};

enum SERIAL_TYPE
{
  SERIAL_PHY_0 = 0,
  SERIAL_PHY_1,
  SERIAL_PHY_2,
  SERIAL_PHY_3,
  SERIAL_SOFT_0,
  SERIAL_SOFT_1,
  SERIAL_SOFT_2,
  SERIAL_SOFT_3
};

enum MODE_TYPE
{
  MODE_0_NORMAL = 0,
  MODE_1_WAKE_UP,
  MODE_2_POWER_SAVIN,
  MODE_3_SLEEP,
  MODE_INIT = 0xFF
};

enum SYS_ROLE
{
  SYS_ROLE_MASTER = 0x00,
  SYS_ROLE_SLAVE  = 0x01
};

//SPED+
enum SLEEP_MODE_CMD_TYPE
{
  W_CFG_PWR_DWN_SAVE = 0xC0,
  R_CFG              = 0xC1,
  W_CFG_PWR_DWN_LOSE = 0xC2,
  R_MODULE_VERSION   = 0xC3,
  W_RESET_MODULE     = 0xC4
};

enum GEOSAT_SWARM_CMD_TYPE
{
  GEOSAT_SWARM_CMD_SAVECFG                  = 0xC0,
  GEOSAT_SWARM_CMD_READCFG                  = 0xC1,
  GEOSAT_SWARM_CMD_LOSECFG                  = 0xC2,
  GEOSAT_SWARM_CMD_READ_VER                 = 0xC3,
  GEOSAT_SWARM_CMD_RESET_MODULE             = 0xC4,
  GEOSAT_SWARM_CMD_CONNECT                  = 0xC5,
  GEOSAT_SWARM_CMD_ROLE                     = 0xC6,
  GEOSAT_SWARM_CMD_NODESCAN_START           = 0xC7,
  GEOSAT_SWARM_CMD_NODESCAN_STOP            = 0xC8,
  GEOSAT_SWARM_CMD_GET_SLAVES_CNT           = 0xC9,
  GEOSAT_SWARM_CMD_SAVE_SLAVES_TO_EEPROM    = 0xCA,
  GEOSAT_SWARM_CMD_CLEAR_SLAVES_FROM_EEPROM = 0xCB,
  GEOSAT_SWARM_CMD_CTRL_TARGET              = 0xCD
};

enum GEOSAT_SWARM_PACKET_TYPE
{
  GEOSAT_SWARM_PACKET_BEACON          = 0x01,
  GEOSAT_SWARM_PACKET_BEACON_RESP     = 0x02,
  GEOSAT_SWARM_PACKET_BEACON_VERIFY   = 0x03,
  GEOSAT_SWARM_PACKET_CMDCTRL_UGV     = 0x04,
  GEOSAT_SWARM_PACKET_CMDCTRL_UAV     = 0x05
};

enum GEOSAT_SWARM_ADDR_LEN
{
  GEOSAT_SWARM_ADDR_SYSTEM_START  = 0,
  GEOSAT_SWARM_ADDR_SLAVE_START   = 100
};

enum UART_FORMAT_TYPE
{
  UART_FORMAT_8N1 = 0x00,  /*no   parity bit one stop*/
  UART_FORMAT_8O1 = 0x01,  /*odd  parity bit one stop*/
  UART_FORMAT_8E1 = 0x02   /*even parity bitone stop*/
};

enum UART_BPS_TYPE
{
  UART_BPS_1200 = 0x00,
  UART_BPS_9600 = 0x03,
  UART_BPS_115200 = 0x07
};

enum AIR_BPS_TYPE
{
  AIR_BPS_300   = 0x00,
  AIR_BPS_2400  = 0x02,
  AIR_BPS_19200 = 0x05
};
//SPED-

//410~441MHz : 410M + CHAN*1M
enum AIR_CHAN_TYPE
{
  AIR_CHAN_410M = 0x00,
  AIR_CHAN_433M = 0x17,
  AIR_CHAN_441M = 0x1F
};

//OPTION+
#define TRSM_TT_MODE		0x00	/*Transparent Transmission*/
#define TRSM_FP_MODE		0x01	/*Fixed-point transmission mode*/

#define OD_DRIVE_MODE		0x00
#define PP_DRIVE_MODE		0x01

enum WEAK_UP_TIME_TYPE
{
  WEAK_UP_TIME_250  = 0x00,
  WEAK_UP_TIME_1000 = 0x03,
  WEAK_UP_TIME_2000 = 0x07
};

#define DISABLE_FEC			0x00
#define ENABLE_FEC			0x01

//Transmit power
enum TSMT_PWR_TYPE
{
  TSMT_PWR_20DB = 0x00,
  TSMT_PWR_17DB = 0x01,
  TSMT_PWR_14DB = 0x02,
  TSMT_PWR_10DB = 0x03
};
//OPTION-

#pragma pack(push, 1)
struct SPEDstruct {
  uint8_t air_bps : 3; //bit 0-2
  uint8_t uart_bps: 3; //bit 3-5
  uint8_t uart_fmt: 2; //bit 6-7
};

struct PktCommonStruct {
  uint8_t cmd;
  uint8_t nl;
};

struct PktSysRoleStruct {
  uint8_t cmd;
  uint8_t sysRole;
  uint8_t nl;
};

struct OPTIONstruct {
  uint8_t tsmt_pwr    : 2; //bit 0-1
  uint8_t enFWC       : 1; //bit 2
  uint8_t wakeup_time : 3; //bit 3-5
  uint8_t drive_mode  : 1; //bit 6
  uint8_t trsm_mode   : 1; //bit 7
};

struct CFGstruct {
  uint8_t HEAD;
  uint8_t ADDH;
  uint8_t ADDL;
  struct SPEDstruct   SPED_bits;
  uint8_t CHAN;
  struct OPTIONstruct OPTION_bits;
};

struct PktReadCfgStruct {
  struct CFGstruct rCfg;
  uint8_t nl;
};

struct MVerstruct {
  uint8_t HEAD;
  uint8_t Model;
  uint8_t Version;
  uint8_t features;
};

struct LoraPacketstruct {
  uint16_t  prefixSendID;
  uint8_t   prefixSendChl;  
  uint16_t  sysID;
  uint8_t   sysRole;
  uint16_t  srcID;
  uint16_t  dstID;
  uint8_t   srcChl;
  uint8_t   dstChl;   
  uint8_t   pktType;
  uint8_t   payloadlen;
  uint8_t   payload[45];
  uint16_t  crc;
};

struct MasterInfostruct {
  uint8_t   stat;
  uint8_t   idH;
  uint8_t   idL;
  uint8_t   chl;  
};

struct SlaveInfostruct {
  uint8_t   chk;  
  uint16_t  id;
  uint8_t   chl;
};

struct SysInfostruct {
  uint8_t   sysRole;  
  uint8_t   reserver[99];
};

struct SlaveListstruct {
  uint8_t cnt;
  struct SlaveInfostruct slaveList[15];
};

#if 0
struct CtrlTargetFromHost {
  uint16_t  sendID;
  uint8_t   sendChl;
  uint8_t   targetAct;
  uint8_t   targetSpeed;
};
#else
struct CtrlTargetFromHost {
  uint16_t  sendID;
  uint8_t   sendChl;
  uint8_t   targetType;
  uint8_t   data[16];
};

struct UgvCtrlPkt {
  uint8_t   UgvAct;
  uint8_t   UgvSpeed;
};

struct UavCtrlPkt {
  uint8_t   UavAct;
};

struct SysRecvCmdStatus {
  uint8_t cmd;
  boolean flagOne;
  boolean flagTwo;  
};
#endif

#pragma pack(pop)
#endif /* E32-TTL-100_H_ */
