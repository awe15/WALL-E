/*************************************************
* �ļ�����pack.h     
* ˵����ͨ��Э�����ݴ��
*       ��㸺����װ�õ������������Э������
*       ����ͨ�Ų�
*       ������ݰ���֡ͷβ����ַ�����ȣ�У���
* �汾��V1.0
* ���ߣ�AWE
* �޸����ڣ�2017/06/06
**************************************************/
#ifndef    PACK_H_
#define    PACK_H_


/*!  ������Ϣ  */
#define    PACK_PRINT


/*!--------------------Э���޸�----------------------*/
/*!  ֡ͷ����  */
//#define  DATA_CHECK        //����У��

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


/*!    �������    */
int commData_pack(unsigned char *data, int size);

/*!    �������    */
int commData_unpack(const unsigned char *data, int size);

#endif



