
#ifndef __BYTEORDER_H__
#define __BYTEORDER_H__


//�G���f�B�A��
#define ENDIAN_LITTLE	0		//���g���G���f�B�A��
#define ENDIAN_BIG		1		//�r�b�O�G���f�B�A��


int GetCurrentByteOrder(void);


//�o�C�g�I�[�_�[�ϊ��}�N��
#define SWAP2BYTE( val )		((((val)&0x00FF)<<8) | (((val) & 0xFF00) >> 8))
#define SWAP4BYTE( val )		((SWAP2BYTE((val)&0x0000FFFF)<<16) | (SWAP2BYTE(((val)>>16)&0x0000FFFF)))

#define ENDIAN_SWAP2BYTE( endian, val )		((GetCurrentByteOrder()== endian)? val : SWAP2BYTE(val))
#define ENDIAN_SWAP4BYTE( endian, val )		((GetCurrentByteOrder()== endian)? val : SWAP4BYTE(val))


#endif
