
#include "stdafx.h"
#include "ByteOrder.h"


#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif	//_WIN32


//�G���f�B�A���𒲂ׂ�\����
typedef struct tagMULTIVALUE
{
	union tagval
	{
		char	cVal[4];
		int		nVal;
	} val;
} MULTIVALUE;


#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack(0)
#endif	//_WIN32


/// <summary>
/// ���݂̊��̃G���f�B�A�����擾����
/// </summary>
/// <returns>�G���f�B�A��(ENDIAN_LITTLE:���g���G���f�B�A��, ENDIAN_BIG:�r�b�O�G���f�B�A���j</returns>
int GetCurrentByteOrder(void)
{
	static bool		bCheckByteOrder = false;
	static int		nByteOrder = ENDIAN_LITTLE;
	MULTIVALUE		mv;


	if( true != bCheckByteOrder )
	{
		mv.val.nVal = 1;
		
		nByteOrder = (1 == mv.val.cVal[0]) ? ENDIAN_LITTLE : ENDIAN_BIG;

		bCheckByteOrder = true;
	}

	return	nByteOrder;
}

