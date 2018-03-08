/**
  ******************************************************************************
  * @file    appprotocol.h 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   
	*					 + (1) init
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPPROTOCOL_H
#define __APPPROTOCOL_H 
/* Includes ------------------------------------------------------------------*/
#include "zkrt.h"
/* Exported macro ------------------------------------------------------------*/

//version and model string
#define DEV_MODEL              "Q30TIR-H"
#define DEV_HW                 "010000"
#define DEV_SW                 "010000"
//
#define CTRL_INIT_ENABLE       1
/* Exported constants --------------------------------------------------------*/
//type num enum
typedef enum
{
	TN_None =0,
	TN_HEARTBEAT,
	TN_GETDEVINFO,
	TN_MAX
}TypeNumEnum;
//hb flag
#define TNHB_FLAG                     0xaabbccdd
#define THHB_FIXED_LEN                5
/* Exported types ------------------------------------------------------------*/
#pragma pack(1)
///////////////////////////////subdev to uav zkrt data struct
//////common command
typedef struct
{
	u8 type_num;
	u8 type_data[ZK_DATA_MAX_LEN-1];
}common_data_plst;
//heartbeat data
typedef struct
{
	u32 hb_flag;
	u8 hb_data[ZK_DATA_MAX_LEN-THHB_FIXED_LEN];
}common_hbd_plst;
//dev info data
typedef struct
{
	u8 status;
	u8 model[10];
	u8 hw_version[6];
	u8 sw_version[6];
}common_get_devinfo_plst;
//////normal command
typedef struct
{
	u16 yaw;
	u16 pitch;
	u8 reserved1[4];
	u16 photo;
	u16 record;
	u16 zoom;
	u8 reserved2[3];
	u8 color;
	u8 gimbal_mode;
	u8 daynight;
	u8 mirrorflip;
	u8 ir_record;
	u16 ir_zoom;
	u8 reserved3[5];
	u8 init_status;
}normal_plst_mti;
typedef struct
{
	u16 yaw;
	u16 pitch;
	u8 reserved1[8];
	u16 zoom;
	u8 reserved2[4];
	u8 gimbal_mode;
	u8 daynight;
	u8 reserved3[4];
	u8 gyroCalibration;
	u8 defog;
	u8 electricImageStabilizin;
	u8 reserved4[2];
	u8 init_status;
}normal_plst_q30tirh;
///////////////////////////////
#pragma pack() 
/* Exported functions ------------------------------------------------------- */
#endif /* __APPCAN_H */
/**
  * @}
  */ 

/**
  * @}
  */
	
/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

