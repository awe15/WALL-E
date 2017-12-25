/*************************************************
* 文件名：pack.h     
* 说明：通信协议数据打包
*       这层负责组装好的数据做最后打包协议数据
*       传向通信层
*       打包内容包括帧头尾，地址，长度，校验等
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#ifndef    PACK_H_
#define    PACK_H_


/*!  调试信息  */
#define    PACK_PRINT


/*!--------------------协议修改----------------------*/
/*!  帧头定义  */
//#define  DATA_CHECK        //数据校验

#define    FREAM_HEAD             0x7f
#define    FREAM_TAIL             0x65
#define    BOARD_ADDRESS          0x01

#define    FREAM_HEAD_BIT         0x00
#define    BOARD_ADDRESS_BIT      0x01
#define    FREAM_LEN_H_BIT        0x01
#define    FREAM_LEN_L_BIT        0x02
#define    PACKAGE_BIT            0x04
#define    CNT_TYPE_BIT           0x05

#define    FREAM_MIN_LEN            5
/*!---------------------------------------------------*/


/*!    打包函数    */
int commData_pack(unsigned char *data, int size);

/*!    拆包函数    */
int commData_unpack(const unsigned char *data, int size);

#endif



