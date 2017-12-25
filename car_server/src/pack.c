/*************************************************
* �ļ�����pack.c     
* ˵����ͨ��Э�����ݴ���Ͳ��
*       ��㸺����װ�õ������������Э������
*       ����ͨ�Ų�
*       ������������У��(�����)��ȷ������ 
*       ���,������ݰ���֡ͷβ����ַ�����ȣ�У���
* �汾��V1.0
* ���ߣ�AWE
* �޸����ڣ�2017/06/06
**************************************************/
#include "pack.h"
#include "packCmd_port.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>



/*************************************************
* ������: checkout
* ˵��������У��
* ���������data            ���ݻ���
*           size            ���ݳ���
* ���������ԭ����+У��������ֽ���
**************************************************/
static int checkout(unsigned char *data, int size)
{

	return 1;
}





/*************************************************
* ������: commData_unpack
* ˵�������ݲ�������÷�������
* ���������data            Դ���ݻ���
*           size            ���ݳ���
*        //   data_analy      ����ʽ  
* ����������Ƿ������Ч����
*************************************************/
int commData_unpack(const unsigned char *data, int size)
{
	const unsigned char *p = data;
	int len = 0;
	int frame_num = 0;
	int i = 0;
	while (1)
	{
		if (size < FREAM_MIN_LEN)         //�����С����
			return frame_num;

		if (*p++ == FREAM_HEAD)
		{	
			len = ((*p)<<8) + *(p+1);
			#ifdef PACK_PRINT
				printf("pack len is %d\n", len);
			#endif
			if (len <= (size-2)) //���ݳ��ȺϷ��ж�   FREAM_HEAD+FREAM_TAIL
			{
			#ifdef DATA_CHECK
				if ((*(p+len) == FREAM_TAIL) && checkout(*(p+2), len-2))//�����жϺ�У��
			#else
				if (*(p+len) == FREAM_TAIL)//�����ж�
			#endif
				{
					cmdData_analysis((p+2), len-2);
					frame_num++;
					size -= (len+1);  //len+FREAM_TAIL
				}
			}
			size -= 3;      //FREAM_HEAD+len
			len = 0;
		}
		else
			size--;

		if (0 == size)      
			return frame_num;
	}

}



/*----------------------------------------------------------------------------*/
/*************************************************
* ������: generate_check
* ˵�������ø���У��
* ���������data            ���ݻ���
*           size            ���ݳ���
* ���������ԭ����+У��������ֽ���
**************************************************/
static int generate_check(unsigned char *data, int size)
{


	return 1;
}


/*************************************************
* ������: commData_pack
* ˵�������ݴ������
*       �ⲿ��֤���������㹻����
* ���������data            �������ݻ���
*           size            ���ݳ���,������ʶ
* ���������ԭ����+У��������ֽ���
**************************************************/
int commData_pack(unsigned char *data, int size)
{
	int i = 0;
	//�����ƶ�3���ֽ�(����,֡ͷ)
	for (i = size - 1; i >= 0; i--)
	{
		data[size+3] = data[size];
	}
	data[0] = 0X7F;
	data[1] = size+2 / 256;
	data[2] = size+2 % 256;
	data[size+3] = 0x65;
	return 1;

}
