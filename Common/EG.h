
#ifndef	__EG_H__
#define	__EG_H__


#ifdef _WIN32
#else
#include "CoreFoundation/CoreFoundation.h"
#define		GUID    CFUUIDBytes
#endif


#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif	//_WIN32


//最大参加プレイヤー数
#define	MAX_PLAYER				2

//ユーザIDの最大バイト数
#define	MAX_USERID				256


//ユーザID
typedef struct tagUSERID
{
	char	cUserID[MAX_USERID];					//ユーザID
	GUID	guid;									//一意なID
} USERID;


//ユーザ情報
typedef struct tagPLAYERINFO
{
	int		nCount;									//ユーザの人数
	USERID	UserID[MAX_PLAYER];						//ユーザの情報
} PLAYERINFO;


//領地の状態(占領地は占領したコマの順番が入力される)
enum
{
	TERRITORY_BLANK		= 0,						//未占領地
	TERRITORY_FRIEND,
	TERRITORY_ENEMY,
};

//領地情報
typedef struct tagSQUAREMAP
{
	unsigned char	cTerritory;						//占領地情報
} SQUAREMAP;


//コマの種類
#define	PIECETYPE_COMMON		0					//一般のコマ
#define	PIECETYPE_WRECKER		1					//解体屋のコマ
#define	PIECETYPE_HYPNOTIST		2					//催眠術師のコマ


//睡眠情報
#define	DEF_SLEEP_COUNT			3					//標準睡眠ターン数


//コマ情報
typedef struct tagPIECE
{
	int				nX;								//X座標
	int				nY;								//Y座標
	int				nOrder;							//動作順番
	int				nType;							//コマの種類
	int				nSleep;							//残り睡眠ターン数
} PIECE;


//プレイヤーの最大コマ数
#define	MAX_PIECE				4

//プレイヤーの最大複数マス移動(ブート)回数
#define	MAX_BOOT				3


//プレイヤーのコマ情報
typedef struct tagPLAYERPIECE
{
	unsigned int	nBootCount;						//残り複数マス移動の回数
	PIECE			Piece[MAX_PIECE];				//コマ情報
} PLAYERPIECE;


//ゲームに参加できる人数
#define	MAX_GAMEPLAYER			2

//マップのサイズ
#define	MAP_WIDTH				31					//マップの横幅
#define	MAP_HEIGHT				31					//マップの縦幅

//ゲームの最大ターン数
#define	MAX_TURN				300	//1000

//移動にかけられる時間(ミリ秒)
#define	MAX_TIME_MOVE			5000


//ゲーム情報
typedef struct tagGAMEINFO
{
	int				nTurn;							//ターン数
	int				nOrder;							//動作順番
	SQUAREMAP		Map[MAP_HEIGHT][MAP_WIDTH];		//マップ情報
	PLAYERPIECE		Player[MAX_GAMEPLAYER];	//プレイヤーのコマ情報
} GAMEINFO;


//ゲーム情報
typedef struct tagGAMESTARTINFO
{
	int				nPlayerCount;					//プレイ人数
	GUID			PlayerGuid[MAX_GAMEPLAYER];		//プレイヤーのGUID
	GAMEINFO		GameInfo;						//Gameの初期データ
} GAMESTARTINFO;


//コマの最大移動マス数
#define	MAX_STEP				2

//移動方向

typedef enum {
	DIRECTION_NONE	= 0,
	DIRECTION_UP	= 1,
	DIRECTION_DOWN	= 2,
	DIRECTION_LEFT	= 4,
	DIRECTION_RIGHT	= 8,

	DIRECTION_UP_LEFT    = DIRECTION_UP   | DIRECTION_LEFT,
	DIRECTION_UP_RIGHT   = DIRECTION_UP   | DIRECTION_RIGHT,
	DIRECTION_DOWN_LEFT  = DIRECTION_DOWN | DIRECTION_LEFT,
	DIRECTION_DOWN_RIGHT = DIRECTION_DOWN | DIRECTION_RIGHT,

	DIRECTION_ALL	= DIRECTION_UP | DIRECTION_DOWN | DIRECTION_LEFT | DIRECTION_RIGHT,
} MOVE_DIRECTION;


//移動情報
typedef struct tagMOVEPIECEINFO
{
	int				nOrder;							//移動するコマの動作順番
	int				nStep;							//コマの移動幅(睡眠中は0を設定する)
	unsigned char	cDirection[MAX_STEP];			//移動方向
} MOVEPIECEINFO;


//移動結果の値
#define	MOVERESULT_OK					0			//成功
#define	MOVERESULT_ERR_TIMEOUT			1			//移動時間切れ
#define	MOVERESULT_ERR_FRIENDLYFIRE		2			//移動先に味方が居て移動できない
#define	MOVERESULT_ERR_ENEMYSLEEP		3			//移動先の敵が眠っている
#define	MOVERESULT_ERR_POSITION			4			//正しい移動が行われなかった（マップ外に移動、睡眠以外で移動しない）
#define	MOVERESULT_ERR_ORDER			5			//順番のコマ以外が指定された
#define	MOVERESULT_ERR_SLEEP			6			//眠っているコマで移動できない
#define	MOVERESULT_ERR_BOOT				7			//複数マス移動を行えない


//移動結果
typedef struct tagMOVEPIECERESULT
{
	int				nResult;						//結果
} MOVEPIECERESULT;


//占領地情報
typedef struct tagTERRITORYINFO
{
	unsigned int	nCount[MAX_GAMEPLAYER];			//占領地数
} TERRITORYINFO;


//終了結果の値
#define	ENDRESULT_OCCUPATION	0					//全マップが占領され終了
#define	ENDRESULT_TURNOVER		1					//ターン経過により終了
#define	ENDRESULT_MOVETIMEUP	2					//移動時間経過により終了


//終了情報
typedef struct tagENDINFO
{
	int				nResult;
	union tagInfo
	{
		int			nPlayerIndex;					//プレイヤーのインデックス
	} Info;
	TERRITORYINFO	TerritoryInfo;					//占領地数
} ENDINFO;


//ゲーム結果の値
#define	GAMERESULT_WIN			1					//勝利
#define	GAMERESULT_LOSE			-1					//敗北
#define	GAMERESULT_DRAW			0					//引き分け


//ゲーム結果
typedef struct tagGAMEENDINFO
{
	int				nWinner;						//-1:敗北 1:勝利 0:引き分け
	unsigned int	nMyTerritory;					//自分の占領地数
	unsigned int	nOtherTerritory;				//相手の占領地数
} GAMEENDINFO;


#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack(0)
#endif	//_WIN32


#endif	//__EG_H__


